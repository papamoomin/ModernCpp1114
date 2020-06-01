[뒤로](https://github.com/papamoomin/Study#a-tour-of-c)  

# 목차
[13. 자원 관리에는 객체를 이용하라.](#Item13)<br>
[14. 자원 관리 클래스의 복사 동작을 확실히 알자.](#Item14)<br>


<br><br><br>

# Chapter 3. 자원 관리

- 자원 : 사용 후 시스템에 반환해야 하는 모든 것.



<a name="Item13"></a>
## 13. C++이 사용자 모르게 만들어 호출하는 함수에 주의하자.

```cpp
class Investment
{
	private:	Investment*	create();	//파생된 클래스의 객체를 얻는 팩토리 함수 용도
	public:		void		use();		//팩토리 함수를 호출 후 사용하고, 해제하는 용도
};

void Investment::use()
{
	Investment* node = create();

	//사용

	delete node;
}
```

이 코드에서는 ```use```함수가 객체 삭제에 실패할 가능성이 많다. 도중에 ```return```이 이나 ```goto``` 등을 만난다거나, 예외가 발생한다거나 할 가능성 말이다. 그런 일이 벌어진다면 메모리 릭이 발생하게 된다.

이런 일이 없도록 하기 위해 존재하는 것이 있다. 현재 선언된 블록이 끝나면 자원이 해제되게 만들어주는 스마트 포인터(smart pointer)가.

```cpp
void Investment::use()
{
	std::auto_ptr<Investment> node(create());

	//사용
}	//여기서 auto_ptr의 소멸자를 통해 삭제됨.
```

여기서 우리는 자원 관리에 객체를 사용하는 방법의 두 가지 특징을 알 수 있다.
1. 자원을 얻고, 자원 관리 객체에게 넘겨라. - 자원 획득 즉 초기화 (Resource Acquisition Is Initialization : RAII)
<br> : 바로 초기화하지 않고 대입을 하는 경우도 있지만, 자원 관리 객체에 넘겨주는 것은 필요하다.

2. 자원 관리 객체의 소멸자에서 자원을 확실하게 해제시켜라.
<br> : 소멸자는 객체가 소멸할 때 자동으로 불리므로 반드시 해제가 이루어진다.

<br>

알아둘 점은 ```auto_ptr```은 가리키는 대상에 대해 자동으로 ```delete```를 부르기 때문에, 한 객체에 둘 이상의 ```auto_ptr```이 가리키게 되면 한 자원이 두 번 삭제되어 문제가 발생할 수 있을 것이다. 그래서 그걸 방지하기 위해 ```auto_ptr```은 객체를 복사할 시 원본 객체를 ```null```로 만드는 특성을 갖는다. 한 객체만이 유일한 소유권(ownership)을 갖는다는 얘기다.

```cpp
int main()
{
	std::auto_ptr<Investment> a(create());	// a는 create 함수를 통해 반환된 객체를 소유

	std::auto_ptr<Investment> b(a);			// b는 위 객체를 가지고, a는 null이 됨.

	a = b;									// a는 위 객체를 가지고, b는 null이 됨.

	return 0;
}
```

참고로 ```auto_ptr```은 위와 같은 특성 때문에 STL 컨테이너 등에는 쓰이지 못한다.

그리하여 그 대안으로 나온 것이 참조 카운팅 방식 스마트 포인터(reference-counting smart pointer : RCSP)다. RCSP는 특정 자원을 참조하는 객체의 수를 기억하다, 수가 0이 되면 자원을 자동으로 삭제하는 방식의 스마트 포인터이다. 가비지 콜렉션의 동작과 비슷하나, 참조 상태가 고리를 이루는 경우(두 객체가 서로를 가리키고 있는 경우)가 되면 없어지지 않는 문제가 있다.

TR1(2007년에 제정된 modern C++)의 ```shared_ptr```이 대표적인 RCSP이다.

```cpp
void Investment::use()
{
	std::tr1::shared_ptr<Investment> a(create());// a는 create 함수를 통해 반환된 객체 보유

	std::tr1::shared_ptr<Investment> b(a);		//b는 a와 같은 객체를 가리킨다.

	a = b;										//a는 b와 같은 객체를 가리킨다.
}	//여기서 a와 b는 소멸되며, 가리키는 객체도 함께 사라진다.
```

그리고 추가로 주의해야 할 점은, ```auto_ptr```이든 ```shared_ptr```이든 소멸자 내부에서 ```delete```를 사용한다. ```delete[]```가 아니기 때문에 배열을 가리키게 하면 문제가 될 수 있다는 얘기다. 이는 동적 배열에도 마찬가지다.

```cpp
int main()
{
	std::auto_ptr<std::string> a(new std::string[10]);	//잘못된 delete를 초래하는 코드

	std::tr1::shared_ptr<int> b(new int[10]);			//잘못된 delete를 초래하는 코드

	return 0;
}
```

이런 동적 배열을 처리하기 위해서는 ```boost```의 ```boost::scoped_array```나 ```boost::shared_array```를 찾아봐야 하며, 그게 아니라면 ```vector```나 ```string```을 이용하자.

<br>

기억해야 할 점은, 자원 해제를 직접 하다가는 실수가 생길 수 있다는 것이다. 위에 설명한 것도 이런 실수를 줄이기 위한 도구이며, 이로도 해결되지 않는 것은 직접 자원 관리 클래스를 만들어야 한다는 점이다.

그리고 반환 타입이 포인터인 함수를 불렀을 때는, 호출한 쪽에서 delete해야 한다는 점 역시 잊지 말자.

<br>

정리하자면, 자원 누출을 막기 위해 자원을 획득 후 소멸자에서 해제하는 RAII 객체를 애용하는 것이 좋다.



<br><br><br>


<a name="Item14"></a>
## 14. 자원 관리 클래스의 복사 동작을 확실히 알자.
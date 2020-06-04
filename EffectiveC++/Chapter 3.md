[뒤로](https://github.com/papamoomin/Study#a-tour-of-c)  

# 목차
[13. 자원 관리에는 객체를 이용하라.](#Item13)<br>
[14. 자원 관리 클래스의 복사 동작을 확실히 알자.](#Item14)<br>
[15. 자원 관리 클래스에서 관리하는 자원은 외부에서 접근이 가능하도록 하라.](#Item15)<br>


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

그리고 반환 타입이 포인터인 함수를 불렀을 때는, 호출한 쪽에서 ```delete```해야 한다는 점 역시 잊지 말자.

<br>

정리하자면, 자원 누출을 막기 위해 자원을 획득 후 소멸자에서 해제하는 RAII 객체를 애용하는 것이 좋다.



<br><br><br>


<a name="Item14"></a>
## 14. 자원 관리 클래스의 복사 동작을 확실히 알자.

힙에 생기지 않는 자원은 스마트 포인터로 처리하기엔 적합하지 않다.

```cpp
void lock(Mutex *m);		//뮤텍스에 잠금을 거는 함수
void unlock(Mutex *m);		//뮤텍스의 잠금을 푸는 함수

class Lock
{
public:		explicit Lock(Mutex *m);
public:		~Lock();

private:	Mutex* _m;
};

Lock::Lock(Mutex *m)
: _m(m)
{
	lock(_m);
}

Lock::~Lock(Mutex *m)
{
	unlock(_m);
}

int main()
{
	Mutex m;

	{
		Lock m1(&m);	//뮤텍스에 잠금을 건다.

		//필요한 작업 수행
	}	//뮤텍스에 걸린 잠금 해제

	{
		Lock m1(&m);	//뮤텍스에 잠금을 건다.
		Lock m2(m1);	//m1을 m2로 복사한다.
	}
	return 0;
}
```

여기서 ```Lock m2(m1);``` 코드에서 무슨 일이 벌어져야 할까요? 대부분은 다음 선택지 중 하나에 속할 것입니다.

```cpp
class Lock : private Uncopyable
{
	...
};
```
1. 복사를 금지한다.
<br> : 복사 함수를 ```private```으로 하여 복사를 막는다.

<br>

```cpp
void lock(Mutex *m);		//뮤텍스에 잠금을 거는 함수
void unlock(Mutex *m);		//뮤텍스의 잠금을 푸는 함수

class Lock
{
public:		explicit Lock(Mutex *m);

private:	std::tr1::shared_ptr<Mutex> _m;	//shared_ptr을 사용
};

Lock::Lock(Mutex *m)
: _m(m, unlock)		//가리킬 포인터로 m을, 삭제자로 unlock 함수를 넣는다.
{
	lock(_m.get());
}
```
2. 관리하는 자원에 참조 카운팅을 한다.
<br> : 자원을 다 쓰기 전까지는 삭제하지 않아야 할 때가 있는 법이다. 이럴 때는 복사 대신 카운트를 증가시키는 작업을 해야 한다. 이는 위 예제에 사용한 ```shared_ptr```이 사용하고 있는 방법이기도 하다. 위 방법에서 기억해야 할 점은, 이제 ```Lock``` 클래스가 소멸자를 선언할 필요가 없다는 것이다. 

<br>

3. 관리하는 자원을 정말 복사한다.
<br> : 자원 관리 객체를 복사하면 그 객체가 둘러싼 자원까지 깊은 복사(deep copy)를 해버리는 방법. 객체가 힙 메모리를 물고 있을 때, 그 객체를 복사하면 포인터와 포인터가 가리키는 새로운 힙 메모리를 가지게 되는 동작을 예로 들 수 있다.

<br>

4. 관리 중인 자원의 소유권을 옮긴다.
<br> : ```auto_ptr```과 같은 방식의 동작이라고 볼 수 있다.

<br>

정리하자면, RAII 객체의 복사는 객체가 관리하는 자원의 복사 문제를 안고 있으므로, 어떤 식으로 복사하냐를 고려하여야 한다. 일반적으로는 복사를 금지하거나, 참조 카운팅을 하기도 하지만, 위에 추가로 언급한 다른 방법들을 이용할 수도 있다.



<br><br><br>


<a name="Item15"></a>
## 15. 자원 관리 클래스에서 관리하는 자원은 외부에서 접근이 가능하도록 하라.

RAII 클래스의 객체(```shared_ptr```이나 ```auto_ptr``` 등)를 사용할 때에는, 그 객체의 실제 자원에 접근해야 할 일이 생긴다. 이 때는 실제 자원으로 변환할 방법이 필요한데, 명시적 변환(explicit conversion)과 암시적 변환(implicit conversion)이 있다.

```shared_ptr```이나 ```auto_ptr```은 명시적 변환을 위한 ```get```함수를 제공한다.

```cpp
class seed
{
	...
};

int makeNum(const seed* s);

int main()
{
	std::tr1::shared_ptr<seed> pSeed();
	int myNum = makeNum(pSeed.get())
	return 0;
}
```

그리고 암시적 변환이 가능한 ```operator->```, ```operator*``` 연산자 역시 제공하고 있다.

```cpp
class seed
{
public:		bool isSetSeed();
};

int main()
{
	std::tr1::shared_ptr<seed> pSeed();
	bool isSet1 = pSeed->isSetSeed();
	bool isSet2 = (*pSeed).isSetSeed();
	return 0;
}
```

이런 식으로 암시적 변환 역시 가능하다.

<br>

```cpp
FontHandle getFont();
void releaseFont(FontHandle f);
void changeFontSize(FontHandle f, int size);
class Font
{
public:		explicit Font(FontHandle f);
public:		~Font();
public:		FontHandle	get() const;
public:		operator FontHandle() const;
private:	FontHandle 	_f;
};

Font::Font(FontHandle f)
: _f(f)	//값에 의한 전달
{
	__noop;
}

Font::~Font()
{
	releaseFont(_f);
}

Font::get() const
{
	return _f;	//명시적 변환 함수
}

Font::operator FontHandle() const
{
	return _f;	//암시적 변환 함수
}

int main()
{
	Font f1(getFont());
	int fontSize = 10;
	changeFontSize(f1, fontSize);	//Font에서 FontHandle로 암시적 변환을 수행.

	Font f2(getFont());
	FontHandle fh = f2;				//Font 객체 복사를 의도했으나 f1이 FontHandle로 복사
	return 0;
}
```

이렇게 되면 ```Font``` 객체 ```f2```가 관리중인 ```FontHandle```이 ```fh```를 통해서도 사용이 가능해진다. ```f2```가 소멸될 때 ```fh```는 소멸된 객체에 매달린 셈이 되기 때문.

RAII 클래스에게 명시적/암시적 변환을 허용하는 것은 이렇듯 용도와 사용 환경에 따라 달라진다. 결과적으로 잘 설계한 클래스라면 올바르게 쓰기는 편하고, 안 좋게 쓰기는 어렵게 짜야 한다.

<br>

RAII 클래스는 캡슐화에 위배되지만, 애초에 데이터 은닉을 위한 클래스가 아니기 때문에 상관은 없다. ```shared_ptr```은 엄격한 캡슐화와 느슨한 캡슐화를 모두 제공하고 있는데, 참조 카운팅 매커니즘에 필요한 것은 모두 캡슐화하고 있지만, 자신이 관리하는 포인터에는 쉽게 접근할 수 있게 한다는 것이 그 예이다.

<br>

정리하자면, RAII 클래스를 만들 때에는 그 클래스가 관리하는 자원에 접근해야 하는 기존 API들을 고려하여 자원을 얻을 통로를 열어두어야 한다. 그리고 자원 접근은 명시적/암시적 변환이 있는데, 안정성은 명시적 변환이, 편의성은 암시적 변환이 우수하다.


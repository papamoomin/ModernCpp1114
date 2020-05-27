[뒤로](https://github.com/papamoomin/Study#a-tour-of-c)  

# 목차
[5. C++이 사용자 모르게 만들어 호출하는 함수에 주의하자.](#Item5)<br>
[6. 컴파일러가 만든 함수가 필요가 없다면 사용을 막아버려라.](#Item6)<br>
[7. 다형성을 가진 기본 클래스는 반드시 가상 소멸자를 선언하라.](#Item7)<br>
[8. 예외가 소멸자를 벗어나지 못하게 하라.](#Item8)<br>
[9. 객체 생성, 소멸 중에는 가상 함수를 호출하지 말라.](#Item9)<br>
[10. 대입 연산자는 *this의 참조자를 반환하게 하라.](#Item10)<br>


<br><br><br>

# Chapter 2. 생성자, 소멸자, 대입 연산자

- 생성자 : 새로운 객체를 메모리에 만들 때 필요한 과정을 제어하고 객체의 초기화를 맡는 함수
- 소멸자 : 객체를 없앰과 동시에 객체가 메모리에서 적절히 사라질 수 있도록 제어하는 함수
- 대입 연산자 : 기존의 객체에 다른 객체의 값을 줄 때 사용하는 함수

<a name="Item5"></a>
## 5. C++이 사용자 모르게 만들어 호출하는 함수에 주의하자.

빈 클래스처럼 아무 것도 선언하지 않은 것처럼 보여도 컴파일러가 필요할 때 알아서 선언해주는 함수들이 있다.  
기본형 함수들인 복사 생성자, 복사 대입 연산자, 소멸자 함수들인데, 이들은 모두 ```public```이며 ```inline```이다.

이 말인즉슨,

```cpp
class Empty{};
```

라고 작성하였더라도 실질적으로는

```cpp
class Empty
{
public:	Empty() {};									//기본 생성자
public:	Empty(const Empty& rhs) {};					//복사 생성자
public:	~Empty() {};								//소멸자
public:	Empty&	operator=(const Empty& rhs) {};		//복사 대입 연산자
};
```

이런 코드인 셈이나 다름이 없다는 의미이다.

그리고 이런 코드들이 나타나는 조건은 다음과 같다.
```cpp
Empty x;	//기본 생성자와 소멸자
Empty y(x);	//복사 생성자
x = y;		//복사 대입 연산자
```

<br>

기본 생성자와 소멸자는 컴파일러에게 기본 클래스 및 비정적 데이터 멤버들의 생성자와 소멸자를 호출할 수 있는 자리를 마련해준다. 그리고 이 때 소멸자는 부모 클래스의 소멸자가 가상 소멸자가 아니라면 비가상 소멸자로 만들어진다는 점을 명심하자.

그리고 만약 생성자를 명시적으로 선언하였다면 기본 생성자는 만들어지지 않음을 기억하여야 한다.

<br>

복사 생성자와 복사 대입 연산자는 원본 객체의 비정적 데이터를 사본 객체로 복사해주는 역할을 한다.

```cpp
class A
{
public:		A(const char* name, const int num);

private:	std::string _name;
private:	int	_num;
};

int main()
{
	A x("messi", 10);
	A y(x);				//여기서 복사 생성자가 호출된다.

	return 0;
}
```

여기서 ```string```은 자체 복사 생성자를 가지고 있으므로, ```x```의 ```_name```은 ```string```의 복사 생성자의 인자가 된다. 그리고 ```_num```의 경우는 기본 타입이므로 비트를 그대로 복사하는 식으로 진행될 것이다.

<br>

컴파일러가 만드는 복사 대입 연산자는 적법해야(legal)하고 이치에 맞아야(reasonable) 한다. 그렇지 않다면 컴파일러는 ```operator=```의 자동생성을 거부해버린다.

```cpp
class A
{
public: 	A(std::string& name, const int num);	//name이 참조자가 되어 
													//상수타입의 name을 넣지 못한다.

private:	std::string&	_name;	//참조자
private:	const int		_num;	//상수
};

int main()
{
	std::string a("aaa");
	std::string b("bbb");

	A x(a, 1);
	A y(b, 2);
	x = y;		//대입 연산. 컴파일 에러.
	return 0;
}
```
```x```, ```y```는 모두 참조자이며, 이런 경우 참조자 자체가 바뀌어야 할지, 아니면 ```y```의 ```_name```을 참조해야 할지가 애매해진다. 이런 경우에 컴파일러는 컴파일 에러를 출력. 

참조자나 상수 객체를 데이터 멤버로 가지려면 직접 복사 대입 연산자를 만들어야 함을 주의하라.

그리고 ```private```으로 복사 대입 연산자를 선언하는 경우에는 암시적 복사 생성자를 가질 수 없으며, 파생 클래스라 할지라도 호출할 권한이 없어진다는 점도 명심하여야 할 것.

<br>

정리하자면 컴파일러는 경우에 따라 클래스에 대해 기본 생성자, 복사 생성자, 복사 대입 연산자, 소멸자를 암시적으로 만들 수 있다.


<br><br><br>


<a name="Item6"></a>
## 6. 컴파일러가 만든 함수가 필요가 없다면 사용을 막아버려라.

컴파일러는 5번 항목에서 말했듯이, 만들지 않아도 알아서 함수를 만들어버리곤 한다. 그런데 만약 복사를 막고 싶다면 어떻게 해야할까?

컴파일러가 만드는 함수는 ```public```으로 선언되므로, 명시적으로 ```private```에 해당 함수들을 선언해주면 해결된다. 즉, 복사의 경우는 외부에서의 호출이 필요하므로 ```private```으로 처리하여 외부 호출을 막아버릴 수 있다.

하지만 만약 ```friend```를 이용하여 ```private```을 호출해버린다면? 그걸 막기 위해 ```private```으로 선언 후 구현하지 않는 방식을 사용한다. ```iostream``` 라이브러리의 ```ios_base```, ```basic_ios```, ```sentry``` 등에도 사용되는 기법이다.

이를 적용하면 다음과 같은 코드가 된다.

```cpp
class object 
{
	public:		object();

	private:	object(const object&);				//선언만 해둔다.
	private:	object& operator=(const object&);	//선언만 해둔다.
};

int main()
{
	object a;
	object b(a);	//복사 불가
	object b = a;	//복사 불가
	return 0;
}
```
이렇게 한다면 컴파일 에러가 날 것이고, 멥버 함수나 프렌드 함수 내에서 할 경우 링크 에러가 난다.

그리고 링크 에러가 발생하는 시점을 컴파일 시점으로 옮길 수 있는데ㅡ에러는 미리 탐지하는 것이 좋으므로 권장하는 바라고 함ㅡ, 복사 생성자와 복사 대입 연산자를 ```private```으로 하되, 이걸 별도의 기본 클래스에 넣은 뒤 원래 사용하려던 클래스에서 파생을 시키는 것이다. 그렇게 하면 별도의 기본 클래스는 복사 방지만 맡게 된다.

```cpp
class noncopyable
{
protected:	noncopyable() {};		//파생 클래스에게 생성 허용
protected:	~noncopyable() {};		//파생 클래스에게 소멸 허용

private:	noncopyable(const noncopyable&);			//복사 방지
private:	noncopyable& operator=(const noncopyable&);	//복사 방지
};

class object : private noncopyable
{
	//복사 생성자와 복사 대입 연산자는 선언되지 않는다.
};
```

컴파일러가 생성한 복사 함수는 기본 클래스의 대응 버전을 호출하게 되어있으나, 기본 클래스에서 비공개로 되어있기에 호출할 수가 없다.

그리고 여기 ```noncopyable```의 구현에 대해 기술하겠다.
1. ```noncopyable```로부터의 상속은 public이 아니어도 된다.
2. ```noncopyable```의 소멸자는 가상 소멸자가 아니어도 된다.
3. ```noncopyable```은 공백 기본 클래스 최적화 기법을 사용할 수도 있으나, 기본 클래스인 탓에 다중 상속이 된다면 의도한 기법대로 동작하지 않을 가능성이 있다.
4. 부스트 라이브러리에 해당 항목과 같은 클래스가 있는데, 사용해도 된다. (이름은 ```noncopyable``` 그대로이다.)

<br>

정리하자면 컴파일러에서 자동으로 선언해주는 함수를 막으려면 대응되는 함수를 ```private```으로 선언 후 구현을 하지 말거나, ```noncopyable```과 같은 기본 클래스를 이용하라.


<br><br><br>


<a name="Item7"></a>
## 7. 다형성을 가진 기본 클래스는 반드시 가상 소멸자를 선언하라.

```cpp
class foodMaker
{
	public:		foodMaker();
	public:		~foodMaker();
};

class koreanFoodMaker : public foodMaker {};
class AmericanFoodMaker : public foodMaker {};
```

파생 클래스를 사용하는 사용자는 정보에 접근하기 위해 기본 클래스의 포인터를 반환하는 함수를 이용하곤 한다. (여기서 파생 클래스 객체에 대한 기본 클래스 포인터를 반환하는 함수를 팩토리 함수라고 부른다.)

```cpp
foodMaker*	getFoodMaker();	//파생 클래스에서 동적으로 할당된 객체의 포인터 반환 용도
```

팩토리 함수의 규약대로라면 ```getFoodMaker```로 반환되는 객체는 힙 메모리에 있으므로 메모리 릭을 막기 위해 delete 해줘야 한다.

```cpp
foorMaker* hungry = getFoodMaker();

delete hungry;
```

허나 객체 삭제를 사용자에게만 전가시키는 것은 에러 발생 확률을 높이는 것과 다르지 않다.

여기서 문제는 ```getFoodMaker``` 함수가 반환하는 포인터는 파생 클래스를 통해 삭제되며, 기본 클래스인 foodMaker의 소멸자가 비가상 소멸자이기에 파생 클래스가 소멸되지 않게 된다는 점이다.

```getFoodMaker```를 통해 얻은 ```koreanFoodMaker``` 객체를 예로 들어보자. 이 객체가 기본 클래스 포인터를 통해 삭제될 때, 파생 클래스의 소멸자는 불리지 않는다. 즉, 기본 클래스에 있지 않은, 파생 클래스만 가진 부분은 소멸되지 않고 남아있게 되는 것이다. 이를 부분 소멸(partially destroyed) 객체라고 부른다.

이를 막는 방법은 간단하다. 기본 클래스의 소멸자는 모두 가상 소멸자로 두어라.

```cpp
class foodMaker
{
	public:		foodMaker();
	public:		virtual ~foodMaker();
};

class koreanFoodMaker : public foodMaker {};
class AmericanFoodMaker : public foodMaker {};
```

이렇게 한다면 이제 객체 소멸 시 파생 클래스만 가진 부분도 모두 잘 소멸된다.

기본 클래스라면, 그리고 가상 함수를 가진 함수라면 가상 소멸자를 가져야 한다.

<br>

여기서 반대로, 파생 클래스를 가질 생각이 없는 클래스의 소멸자를 가상 소멸자로 두는 것은 좋지 않다.

가상 함수를 가진 클래스는 프로그램 실행 중 주어진 객체에 대해 어떤 가상 함수를 호출할 지를 결정하기 위한 정보를 포인터의 형태로 저장하는 자료구조를 가진다. 여기서 해당 포인터를 ```vptr```(virtual table pointer = 가상 함수 테이블 포인터)라고 부르며, 포인터가 저장된 배열을 ```vtbl```(virtual table = 가상 함수 테이블)이라고 부른다.

즉, 파생 클래스를 가질 필요가 없는데 ```virtual``` 함수를 가진다면 해당 클래스를 가진 객체의 크기가 커진다는 것이다. 거기다가 이식성, 다른 언어와의 호환성이 사라진다. 다른 언어와 데이터 구조를 맞췄다고 하더라도 ```vptr```은 어찌 할 방도가 없는 까닭이다.

<br>

그리고 가상 함수가 없음에 불구하고 비가상 소멸자로 문제가 발생할 수도 있다.

```std::string```은 가상 함수가 없지만, 가끔가다 ```std::string```을 기본 클래스로 하여 파생 클래스를 만드는 미친 행각이 목격되곤 한다. 이런 경우 파생 클래스의 포인터를 기본 클래스의 포인터로 변환한 후 ```delete```가 되는 순간 파생 클래스의 소멸자가 호출되지 않는 문제가 생긴다.

어떤 클래스든 상속을 받으려면 그 클래스가 가상 소멸자를 가지고 있는지부터 확인하자.

<br>

순수 가상 함수를 이용해 순수 가상 소멸자를 두는 것도 좋은 방법이다.

순수 가상 함수는 해당 클래스를 추상 클래스(abstract class)ㅡ자체 타입으로는 객체를 생성할 수 없는 클래스ㅡ로 만들어 준다.

여기서 주의할 점이 있다. 소멸자가 동작하는 순서는 가장 말단의 파생 클래스부터 호출하여 거슬러 올라가는 방식인데, 컴파일러는 기본 클래스의 소멸자를 호출하기 위해 파생 클래스의 소멸자를 사용할 것이므로 순수 가상 소멸자의 정의를 만들어두지 않으면 링크 에러가 발생한다.

```cpp
class A
{
public:	virtual ~A() = 0;
};

A::~A()
{
	//순수 가상 소멸자의 정의
}
```

이렇듯 기본 클래스의 손에 가상 소멸자를 두는 규칙은 다형성을 가진 기본 클래스에 한정된다. 기본 클래스의 인터페이스를 통해 파생 클래스의 조작을 허용하도록 설계한 클래스에 한하여만 적용된다는 것이다.

모든 클래스가 다형성을 갖도록 설계되진 않으므로, 가상 소멸자는 다형성의 유무를 판단하여 사용하도록 하여야 한다.

<br>

정리하자면, 다형성을 가진 기본 클래스라면. 즉, 가상 함수를 하나라도 가지고 있다면 가상 소멸자를 불러줘야 한다. 그리고 이런 경우가 아니라면 가상 소멸자를 절대 부르지 말아야 한다.


<br><br><br>

<a name="Item8"></a>
## 8. 예외가 소멸자를 벗어나지 못하게 하라.

```cpp
class A
{
public:		~A() {};	//여기서 예외가 발생한다 가정.
};

int main()
{
	std::vector<A> aVec;
	return 0;
}	//aVec은 여기서 소멸
```

벡터 ```aVec```이 소멸될 때 가지고 있는 A들을 소멸시켜야 하는데 중간에 예외가 발생했다고 가정하자. 그런데 예외가 하나가 아니라면? 프로그램이 꼬일 염려가 있다. 이건 벡터가 아니라 다른 표준 라이브러리 컨테이너나 배열의 경우에도 마찬가지이다. 이렇듯 프로그램이 꼬이는 원인은 예외가 발생해도 방치하는 소멸자이다.

```cpp
class connection
{
public:		connection() {};
public:		~connection() {};

public:		static connection	create();
public:		void				close();// 연결을 해제하며, 실패할 시 예외를 던져야하는 함수
};

class connectionManager
{
public: 	connectionManager(connection& connect) {};
public:		~connectionManager() { connect.close(); };

private:	connection 	_connect;
};

int main()
{
	connectionManager cm(connection::create());
	return 0;
}			//블록이 끝나며 connectionManager의 소멸자가 불려야하는 부분
```

여기서 close 호출만 잘 된다면 문제가 없다.

하지만 예외가 발생한다면? ```connectionManager```의 소멸자는 예외를 전파할 것이며, 소멸자에서 밖으로 예외를 내보내는 일이 된다.

<br>

이 문제를 회피할 방법은 세 가지이다.

```cpp
connectionManager::~connectionManager()
{
	try
	{
		connection.close();
	}
	catch(...)
	{
		//log작성
		std::abort();
	}
}
```
1. 예외가 발생 시 프로그램을 바로 종료(```abort```를 호출하는 경우가 대부분임).
<br>: 객체 소멸 중 에러가 발생하고 프로그램이 지속되기 힘든 상황이라면, 예외가 예기치 못한 동작을 일으킬 가능성이 있다면 괜찮은 선택지이다.

<br>

```cpp
connectionManager::~connectionManager()
{
	try
	{
		connection.close();
	}
	catch(...)
	{
		//log작성
	}
}
```
2. 예외가 호출된 곳에서 예외 자체를 삼켜버림.
<br>: 대부분의 경우엔 좋지 않다. 무엇이 문제인지를 알려주는 중요한 정보가 묻히기 때문. 하지만 프로그램 종료나 예상치 못한 동작 대신 이렇게 처리해버리는게 괜찮을 때도 있다. 단, 이렇게 진행하려면 예외를 먹은 후에도 프로그램이 신뢰성 있게 실행을 이어나가도록 신경써야한다.

<br>

```cpp
class connectionManager
{
public: 	connectionManager(connection& connect) {};
public:		~connectionManager() { connect.close(); };
public:		void		close();

private:	connection 	_connect;
private:	bool		_isClose;
};

connectionManager::close()
{
	_connect.close();
	isClose = true;
}

connectionManager::~connectionManager()
{
	if(false == isClose)
	{
		try
		{
			connection.close();	//연결이 닫히지 않았으면 여기서라도 시도해보자.
		}
		catch(...)
		{
			//위 두 가지 방법 중 하나를 처리
		}
	}
}
```
3. ```close``` 호출의 책임을 ```connectionManager```의 소멸자가 아닌, ```connectionManager```의 사용자에게 떠넘김.
<br>: 어떤 동작이 예외를 일으킬 가능성이 있고, 예외를 처리해야 할 가능성이 있다면 소멸자가 아닌 다른 함수에서 비롯되어야 한다는 것이 이 방식의 포인트이다. 
<br> 이 방식은 사용자가 호출할 ``` close ``` 함수를 두었으며, 이는 사용자가 에러를 처리할 기회를 주는 셈.

<br>

정리해보면, 소멸자에서 예외가 빠져나가선 안된다. 만약 그럴 가능성이 있다면 소멸자가 삼키거나 프로그램을 끝내야 한다. 그리고 예외에 대해 사용자가 반응해야 할 필요가 있는 경우, 예외가 발생한 함수는 소멸자가 아닌 함수여야 한다.


<br><br><br>

<a name="Item9"></a>
## 9. 객체 생성, 소멸 중에는 가상 함수를 호출하지 말라.

```cpp
class bankAccount
{
	public:		backAccount();
	public:		~backAccount();
	public:		virtual void log() const = 0;
};

bankAccount::bankAccount()
{
	log();			//사실 이 부분은 링크 에러를 유발함.
}

class deposit : public bankAccount
{
	public:		virtual void log() const;
};

class withdraw : public bankAccount
{
	public:		virtual void log() const;
};

int main()
{
	deposit d;
	return 0;
}
```

여기서 ```deposit d;``` 부분에서 ```deposit```의 생성자가 호출된다. 하지만 그 이전에 기본 클래스인 ```bankaccount```의 생성자가 호출되는데, 여기서 가상 함수인 ```log```가 호출된다.

바로 여기서 문제가 발생한다. 기본 클래스의 생성자가 호출되는 동안에는 가상 함수가 파생 클래스 쪽으로 내려가지 않는다. 그 대신, 객체 스스로가 기본 클래스인 것ㅊ처럼 동작한다. 즉, 이 상황에서 호출되는 ```log```는 ```deposit```의 ```log```가 아닌 ```bankAccount```의 ```log```라는 사실이다.

어째서일까? 이유는 기본 클래스의 생성자가 먼저 호출되는 까닭이다. 기본 클래스 생성자가 호출되는 동안 파생 클래스는 아직 초기화가 되지 않은 상태이기 때문에.

<br>

여기서 하나 더 알아둬야 할 점이 있는데, 파생 클래스 객체의 기본 클래스 부분이 생성되는 동안은 해당 객체의 타입이 기본 클래스로 처리된다. 호출되는 가상 함수는 물론이거니와, 런타임 정보를 사용하는 언어 요소(```dynamic_cast```나 ```typeid``` 등)등 모든 것이 기본 클래스 타입의 객체로 취급된다는 말이다. 위의 예제를 들면 ```deposit``` 객체의 기본 클래스 부분이 초기화되는 동안은 해당 객체는 ```deposit```이 아닌 ```bankAccount```가 되는 것이다. 이는 안전을 위해 어쩔 수 없는 선택이다. 초기화되지 않은 것을 건드릴 수는 없으니 말이다.

<br>

객체가 소멸될 때도 방향은 역순이지만 이유는 같다.

파생 클래스의 소멸자가 호출되고 나면 파생 클래스의 데이터 멤버는 정의되지 않은 게 된다. 기본 클래스의 소멸자로 진입하면 이제 기본 클래스의 객체와 같게 처리가 된다는 것이다.

<br>

위에서 생성자에 ```log```를 호출하는 것은 링크 에러를 유발하는데, 순수 가상 함수인 탓에 구현 부를 찾을 수 없기 때문이다.

하지만 비슷한 원리인데 에러가 발생하지 않는 경우도 있다. 다음 예제를 보자.

```cpp
class bankAccount
{
	public:		backAccount();
	public:		~backAccount();
	public:		virtual void log() const = 0;

	private:	void init();
};

bankAccount::bankAccount()
{
	init();	//생성자에서 비가상 함수를 호출한다.
}

bankAccount::init()
{
	log();	//근데 그 비가상 함수에서 가상 함수를 호출한다.
}
```

이런 경우는 컴파일도 링크도 잘 된다. 하지만 실행을 한다면? 순수 가상 함수를 실행한 셈이기에 대부분 ```abort```처리가 될 것이다. 강제로 종료가 된다는 말이다.

그런데 여기서 만약 ```log```가 순수 가상 함수가 아니라고 가정해보자. 그럼 이제 에러도 발생하지 않고, 파생 클래스의 ```log```를 호출하고 싶었는데 기본 클래스의 ```log```가 호출되는 등 의도치 않은 일들이 벌어질 것이다.

이를 방지할 방법은 생성자와 소멸자에서 가상 함수를 호출하는 것을 철저히 막는 방법 뿐이다.

또 다른 방법으로는 ```log```함수를 ```bankAccount``` 클래스의 비가상 멤버 함수로 바꾸고, 파생 클래스의 생성자가 로그에 대한 정보를 ```bankAccount``` 클래스의 생성자로 넘겨주는 식으로 처리할 수 있을 것이다.

```cpp
class bankAccount
{
	public:		explicit	bankAccount(const std::string& info);
	public:		void		log(const std::string& info) const;		//비가상 함수
};

bankAccount::bankAccount(const std::string& info)
{
	log(info);
}

class deposit : public bankAccount
{
	public:		deposit( params );

	private:	static std::string	infoHelper( params );
};

deposit::deposit( params )
: bankAccount(infoHelper( params ))
{
	__noop;
}
```

이렇게 필요한 초기화 정보를 파생 클래스에서 기본 클래스의 생성자로 보내어 역으로 정보를 채울 수 있다.

여기서 infoHelper라는 정적 함수는 기본 클래스의 생성자로 넘길 값을 만들어주기 위한 도우미 함수인데, 기본 클래스에서 멤버 초기화 리스트가 여기저기 얽혀 있는 경우 이런 방식이 유용하게 적용된다.

미초기화된 데이터 멤버는 정의되지 않은 상태에 있음에 유의하자. 생성자나 소멸자에서 가상 함수를 호출하여도 파생 클래스로 내려가지 않음을 명심하여야 한다.


<br><br><br>


<a name="Item10"></a>
## 10. 대입 연산자는 *this의 참조자를 반환하게 하라.

```cpp
int main()
{
	int x, y, z;
	x = y = z = 10;
	return 0;
}
```

대입 연산은 사슬처럼 엮일 수 있으며, 우측 연관(right-associative) 연산이라는 특성을 가지고 있다.

이는 위 코드를 풀면 다음과 같은 모양을 가진다는 뜻이다.

```cpp
int main()
{
	int x, y, z;
	x = (y = (z = 15));
	return 0;
}
```

15는 z에 가장 먼저 대입되고, 갱신된 z가 y에, 갱신된 y가 x에 순차적으로 대입된다.

이렇듯 대입 연산이 사슬처럼 엮이게 하기 위해서는 대입 연산자가 좌변 인자의 참조자를 반환하도록 하여야 한다. 이는 관례나 다름 없는데, 클래스를 만들 때 대입 연산자를 만들게 된다면 이 관례를 지키는 쪽을 추천한다.


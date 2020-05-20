[뒤로](https://github.com/papamoomin/Study#a-tour-of-c)  

# 목차
[1. C++은 언어들의 연합체이다.](#Item1)<br>
[2. #define보다는 const, enum, inline.](#Item2)<br>
[3. const를 애용하자.](#Item3)<br>
[4. 객체는 사용하기 전에 꼭 초기화하자.](#Item4)

<br><br><br>

# Chapter 1. C++에 왔으면 C++의 법을 따라라

<a name="Item1"></a>
## 1. C++은 언어들의 연합체이다.

C++은 <b>다중패러다임 프로그래밍 언어(multiparadigm programming language)</b>이다. 절차적(procedural) 프로그래밍을 베이스로, 객체 지향(objectoriented), 함수식(functional), 일반화(generic), 메타프로그래밍(metaprogramming) 개념까지 지원한다.

그러므로 c++은 단일 언어라기보다 여러 언어들의 연합체로 보는 것이 좋다. 

<br>

### <span style="color:rgb(170,170,255)">- c++의 하위 언어</span>
- <b>C</b> : c++은 c를 기본으로 한다. c에서 블록, 문장, 선행 처리자, 기본제공 데이터타입, 배열, 포인터 등의 많은 것을 가지고 왔다.

- <b>객체 지향 개념 C++</b> : 클래스를 사용하는 C에 관한 모든 것. 클래스, 캡슐화, 상속, 다형성, 가상 함수(동적 바인딩) 등이다.

- <b>템플릿 C++</b> : C++의 일반화 프로그래밍 부분. 오늘날 이 템플릿이 C++에 끼치는 영향은 전방위적이기 때문에 <b>템플릿 메타프로그래밍(template metaprogramming : TMP)</b>이라는 프로그래밍 패러다임이 파생되었다. 

- <b>STL</b> : STL은 Standard Template Library로, 컨테이너와 반복자, 알고리즘과 함수 객체들이 엮여 돌아간다.

<br>

C++은 이 하위 언어들로 구성되어 있다. 그리고 C++을 사용한 효과적인 프로그래밍 규칙은 C++을 구성하는 어느 하위 언어를 주로 사용하느냐에 따라 달라질 수 있으니, 필요한 부분에 맞게 전략적으로 규칙을 적용시켜야 한다.


<br><br><br>

<a name="Item2"></a>
## 2. #define보다는 const, enum, inline.

### <span style="color:rgb(170,170,255)">- 가급적이면 선행 처리자보다 컴파일러를 더 가까이 하는 것이 좋다.</span>

#define 매크로를 사용할 때, 우리의 눈에는 기호식 이름(symbolic name)이 보이지만, 컴파일러는 기호식 이름을 지우고 상수로 바꿔버린다. 그래서 만약 해당 매크로를 통해 에러가 난다면 혼동의 여지가 있다. 이는 기호식 디버거(symbolic debugger)나 기호 테이블에도 마찬가지로 발생할 수 있는 문제이다.

이런 문제는 매크로 대신 const 상수를 사용하면 해결된다. const 상수는 컴파일러에서도 인식 가능하며 기호 테이블에도 들어간다.

그리고 #define을 썼을 때에는 선행 처리자에 의해 사용 횟수만큼 상수의 사본이 들어가지만, const 상수를 이용시에는 사본은 하나만 생기기 때문에 컴파일 후의 최종 코드 크기가 더 작게 나올 수 있다.

<br>

### <span style="color:rgb(170,170,255)">- #define을 상수로 교체할 때는 두 가지 경우를 주의한다.</span>

- 상수 포인터(constant pointer)를 정의하는 경우 : 상수 정의는 헤더에 적는 것이 상례이므로 포인터를 const로 선언해주어야 하고, 포인터가 가리키는 대상도 const로 선언해야 한다. <br>
```cpp
const char* const waterName = "tamsaasoo";
```

- 클래스의 멤버로 상수를 정의하는 경우 : 상수의 유효범위를 클래스로 한정하는 경우, 상수를 멤버로 정의한다. 단, 상수의 사본을 하나로 유지시키고 싶다면 정적 멤버(static member)로 만든다. 
```cpp
static const int bottleCount = 3; //정의가 아닌 선언(declaration)임에 유의하자.
```

#define은 클래스 상수로 사용할 수 없다. #define은 유효범위를 정할 수 없고, #undef를 만나거나 컴파일이 끝날 때까지는 유효하기 때문.

<br>

### <span style="color:rgb(170,170,255)">- 나열자 둔갑술 (enum hack)</span>

구식 컴파일러를 위한 기법으로, 나열자(enumerator) 타입의 값은 int로도 쓸 수 있다는 것을 활용한 기술.

```cpp
class testClass
{
    private:    enum { none = 0 };  //none을 기호식 이름으로 만든다.
    private:    int values[none];
};
```

이 방식은 const보다는 #define에 가깝다. enum의 주소를 취할 수 없기 때문. 이 방식의 enum은 #define처럼 쓸데없는 메모리 할당을 저지르지 않는다.

그리고 많은 코드에서 이 기법을 사용하고 있다. 이 기법은 템플릿 메타프로그래밍의 핵심 기법인 까닭이다.

<br>

### <span style="color:rgb(170,170,255)">- #define의 오용 사례 - 매크로 함수</span>

```cpp
#define CALL_MAX(a, b) f((a) > (b) ? (a) : (b))

int x = 5, y = 0;

CALL_MAX(++x, y);       //x가 2번 증가한다.
CALL_MAX(++x, y + 10);  //x가 1번 증가한다.
```

우선 define에 사용하는 인수들과 수식 전체는 괄호로 싸지 않으면 연산자 우선순위에 의해 의도와 다른 값을 보일 수 있음에 주의하자.

c++에서는 기존 매크로의 효율을 유지하며 정규 함수의 동작방식과 타입에 대한 안정성도 모두 보장하는 <b>인라인 함수에 대한 템플릿</b>이 있다.

```cpp
template<typename T>    // T가 뭔지 알 수 없으니 매개변수로 상수 객체에 대한 참조자 사용.
inline void callMax(const T& a, const T& b)
{
    f(a > b ? a : b);
}
```

이 함수는 템플릿 함수기 때문에 위 매크로 함수를 대체할 수 있다. 

<br>

단순한 상수를 사용할 때에는 #define보다 const 객체와 enum을 우선적으로 쓰도록 하자.

그리고 함수처럼 쓰이는 매크로는 #define을 통한 매크로 함수보다 인라인 함수를 사용하도록 하자.


<br><br><br>

<a name="Item3"></a>
## 3. const를 애용하자.

### <span style="color:rgb(170,170,255)">- const</span>

const는 객체에 대한 외부 변경을 불가능하게 만들어준다.

클래스 바깥에서는 전역 변수나 네임스페이스 범위 내의 상수를 선언하는데에 사용하고, 특정 범위 내에서 static으로 선언한 객체에도 사용할 수 있으며 클래스 내부에서는 멤버를 상수로 선언할 수 있다.

포인터의 경우에는 포인터 자체를 상수로 하기도 하고 포인터가 가리키는 데이터를 상수로 할 수 있으며, 이 두 기능을 동시에 지정할 수도 있다.

```cpp
char sayHi[] = "Hi";
char* a = sayHi;                //비상수 포인터, 비상수 데이터
const char* a = sayHi;          //비상수 포인터, 상수 데이터
char* const a = sayHi;          //상수 포인터, 비상수 데이터
const char* const a = sayHi;    //상수 포인터, 비상수 데이터
char const *a = sayHi;          //비상수 포인터, 상수 데이터 => 포인터과 데이터를 가르는 것은 오직 *의 위치의 어느 쪽에 있느냐이다.
```

*을 기점으로 왼쪽에 const가 오면 포인터가 가리키는 대상이 상수이다.<br>
그리고 *을 기점으로 오른쪽에 const가 오면 포인터 자체가 상수이다.

<br>

### <span style="color:rgb(170,170,255)">- STL 반복자(iterator)</span>

STL 반복자는 포인터를 본떠 만들었기 때문에, 동작 원리가 T* 포인터와 흡사하다.

어떤 반복자를 const로 선언하는 것은 포인터를 상수로 선언하는 것(== T* const 포인터)와 같다.

반복자는 자신이 가리키는 대상이 아닌 것을 가리킬 수 없지만, 가리키는 대상 자체를 변경할 수는 있다.

변경이 불가능한 객체를 가리키는 반복자가 필요하다면 const_iterator를 사용하면 된다.

```cpp
std::vector<int> v;

const std::vector<int>::iterator it = v.begin();	//it는 T* const처럼 동작
*it = 10;											//it가 가리키는 대상 변경
++it;												//it는 상수라 변경 불가
//////////////////////
std::vector<int>::const_iterator cIt = v.begin();
++cIt;												//cIt를 변경하는 것이니 문제 없음
*cIt = 10;											//*cIt가 상수여서 변경 불가
```

<br>

const는 함수 반환 값이나 각 매개변수, 멤버 함수 앞이나 함수 전체에 대해서도 붙을 수 있다.

함수 반환 값의 상수화는 안정성이나 효율을 유지하며 에러 상황을 줄일 수 있다. 예를 들면 == 대신 =를 입력했을 때의 실수 등을 방지한다.

<br>

### <span style="color:rgb(170,170,255)">- 상수 멤버 함수</span>

멤버 함수에 붙는 const는 해당 멤버 함수가 상수 객체에 대해 호출되는 함수임을 알려주는 역할을 한다.

이 함수가 중요한 이유는 두 가지이다.

- 클래스의 인터페이스를 이해하기 용이하다 : 함수가 해당 클래스로 만든 객체를 변경할 수 있는지에 대한 유무를 사용자가 알고 있어야 한다.

- 키워드를 통해 상수 객체를 사용하게 한다 : 코드의 효율을 위해 몹시 중요한 부분. c++의 실행 성능을 높이는 핵심 기법 중 하나가 <b>상수 객체에 대한 참조자가 객체를 전달하는 것</b>이며, 이는 const 멤버 함수가 필요하기 때문이다.

const 키워드 유무만 차이나는 멤버 함수의 경우 오버로딩이 가능하다. c++의 중요한 성질이니 기억해둘 것.
```cpp
class text
{
	public:	const char& operator[](size_t pos) const {return text[pos];}	//상수 객체에 대한 operator[]
	public:	char& operator[](size_t pos) {return text[pos];}				//비상수 객체에 대한 operator[]
};

~~

	text t("hi");
	cout<<t[0]<<endl;		//text::operator[]의 비상수 멤버 호출
	
	const text ct("hi");
	cout<<ct[0]<<endl;		//text::operator[]의 상수 멤버 호출

~~

void print(const text& ct)
{
	cout<<ct[0]<<endl;		//text::operator[]의 상수 멤버 호출
}

~~

	cout<<t[0]<<endl;		//비상수 text 객체를 읽는다.
	t[0] = 'x';				//비상수 text 객체를 쓴다.
	cout<<ct[0]<<endl;		//상수 text 객체를 읽는다.
	ct[0] = 'x';			//컴파일 에러. 상수 text 객체는 쓸 수 없다.
```

이와 같은 내용들을 실제로 사용하는 경우는 상수 객체에 대한 포인터나 상수 객체에 대한 참조자로 객체가 전달될 때이다.

<br>

```cpp
ct[0] = 'x';
```
에서의 에러는 operator[]의 리턴 타입때문에 생긴 에러로, operator[] 호출 자체는 잘못되지 않았다. const char& 타입에 대해 연산을 시도했기 때문에 생긴 것.

그리고 operator[]의 비상수 멤버가 char의 레퍼런스를 반환한다는 점으로, char 하나만 쓰면 안된다는 점에 유의하여야 한다. 만약 그냥 char을 반환시켰다면
```cpp
t[0] = 'x';
```
에서 컴파일 에러가 발생하였을 것이다. 기본제공 타입을 반환하는 함수의 반환 값을 수정할 수 없기 때문.

<br>

### <span style="color:rgb(170,170,255)"> - 상수성</span>

어떤 멤버 함수가 const 멤버라는 것은 두 가지의 의미로 나뉜다.

그 두 가지는 비트수준 상수성(bitwise constness. 또는 physical constness-물리적 상수성 으로도 칭해짐)과 논리적 상수성(logical constness)이다.

1. 비트수준 상수성 : 멤버 함수가 그 객체의 어떤 데이터 멤버도 건드리지 않아야 const임을 인정한다. 이는 c++에서 정의하고 있는 상수성으로, 컴파일러는 데이터에 대한 대입 연산이 수행되었는지만 판단한다. 그렇기에 헛점이 존재하기도 한다.
```cpp
class cText
{
	//부적절하지만 비트수준 상수성이 있어서 허용되는 operator[]의 선언
	public:		char& operator[](size_t pos) const {return pText[pos];}
	private:	char *pText;
};
```
이 코드에서 operator[] 함수는 상수 멤버 함수로 선언되었지만, 해당 객체의 내부 데이터에 대한 참조자를 반환한다. 하지만 pText는 건드리지 않으니 컴파일러가 이를 막을 이유도 없다. 

하지만 이렇게 했을 때 다음과 같은 코드를 짠다면 상수성이 무너진다.
```cpp
const cText ct("Hi");	//상수 객체 선언
char *pct = &ct[0];		//ct의 내부 데이터에 대한 포인터 획득
*pct = 'X';				//ct의 값 변경
cout<<pct<<endl;		//Xi 출력
```
이런 사태를 막기 위해 나온 개념이 논리적 상수성이다.

2. 논리적 상수성 : 상수 멤버 함수여도 객체의 일부 비트를 건들 수는 있되, 사용자 측에서는 알지 못하게 한다면 const임을 인정한다.
```cpp
class cText
{
	public:		size_t 	length() const;

	private:	size_t 	tLength;		//텍스트 길이
	private:	char 	*pText;
	private:	bool	isLengthValid;	//텍스트 길이가 유효한가
};

size_t cText::length() const
{
	if(false == isLengthValid)
	{
		tLength = strlen(pText);
		isLengthValid = true;
		//에러. 상수 멤버 함수 내에서 tLength, isLengthValid 대입 불가.
	}

	return tLength;
}
```
length의 구현은 비트수준 상수성을 따르지 않는다. tLength와 isLengthValid가 바뀔 수 있기 때문. 그렇다면 논리적 상수성을 이용하려면 어떻게 해야하느냐? 바로 mutable을 사용하면 된다.

mutable은 비정적 데이터 멤버의 비트수준 상수성을 풀어줄 수 있다.
```cpp
class cText
{
	public:		size_t 			length() const;

	private:	size_t 			tLength;
	private:	mutable char 	*pText;
	private:	mutable bool	isLengthValid;
};

size_t cText::length() const
{
	if(false == isLengthValid)
	{
		tLength = strlen(pText);
		isLengthValid = true;
	}

	return tLength;
}
```
mutable이 들어간 데이터 멤버는 상수 멤버 함수 안을 포함한 그 어떤 순간에도 수정이 가능하다.

<br>

### <span style="color:rgb(170,170,255)">- 상수/비상수 멤버에서 코드 중복 피하기</span>

mutable은 비트 수준 상수성을 해결하는 괜찮은 방법이지만, const에 대한 문제를 모두 해결할 수는 없다. 경계 검사, 접근 데이터 로깅, 자료 무결성 검증 등을 비상수/상수버전 함수에 중복되어 넣게되는 일이 생기기 때문.
```cpp
const char& operator[](size_t pos) const
{
	//경계검사
	//접근데이터로깅
	//자료무결성검증
	return text[pos];
}

char& operator[](size_t pos)
{
	//경계검사
	//접근데이터로깅
	//자료무결성검증
	return text[pos];
}
```
이런 경우를 해결하기 위해서는 캐스팅을 하는 것이 좋다. 물론 캐스팅 자체는 그렇게 추천할만한 것이 아님을 기억하라. 

허나, 이렇게 하는 작업이 같으나 const 유무가 갈리는 경우에는 비상수 operator[]가 상수 operator[]를 호출하도록 하면 좋다.

```cpp
const char& operator[](size_t pos) const
{
	//경계검사
	//접근데이터로깅
	//자료무결성검증
	return text[pos];
}

char& operatr[](size_t pos)
{
	return const_cast<char&>( static_cast<const text&>(*this)[pos] );
}
```
이 코드처럼 operator[]의 반환타입에 캐스팅을 적용해 const를 떼어내고, *this의 타입에 const를 붙이는 식으로 operator[]의 상수 버전을 호출하는 것이다.

캐스팅이 두 번 되어 있는데, 이는 비상수 operator[]에서 자기 자신을 호출하여 무한 재귀에 빠지지 않도록 상수 operator[]를 호출하겠다고 알리기 위함이다.<br>
이를 위해 *this를 타입 캐스팅하여 const를 붙인 셈이고, 그리고 상수 operator[]의 반환 값에서 다시 const를 떼어 반환하여야 하므로 총 두 번의 캐스팅을 한 것이다.

const를 붙이는 캐스팅은 안전한 타입 변환(비상수->상수)이므로 static_cast만 써도 되지만, const를 제거하는 캐스팅은 const_cast뿐임을 기억하라.

그리고 반대로 상수 버전이 비상수 버전을 호출하는 것은 좋지 않다. 상수는 논리적인 상태를 바꾸지 않겠다고 지정되어 있으나, 비상수는 그렇지 않으므로 문제가 생길 여지가 충분하기 때문이다.

<br>

const는 컴파일러가 사용자의 에러를 잡는 데에 도움을 준다. const는 객체, 매개변수, 리턴 타입, 멤버 함수 등에 붙을 수 있다.

컴파일러는 비트수준 상수성을 지켜야 하지만, 프로그래머는 논리적 상수성을 지켜야 한다.

상수/비상수 멤버 함수가 똑같이 구현되어 있을 때에는, 비상수 버전이 상수 버전을 호출하게 하여 중복을 피할 수 있다.

<br><br><br>

<a name="Item4"></a>
## 4. 객체는 사용하기 전에 꼭 초기화하자.

### <span style="color:rgb(170,170,255)">- 초기화 리스트</span>

c++의 객체/변수 초기화는 언제 초기화가 보장되는지에 대한 규칙이 명확하게 정해져있다.

허나 가장 좋은 방법은 정해져있다. 객체는 사용하기 전에 초기화하자.

특히 중요한 것은 생성자에서 그 객체의 모든 것을 초기화해주는 것이다.

다만 여기서 기억해야할 것은, 대입(assignment)과 초기화(initialization)는 차이가 있다는 점이다.

```cpp
myData::myData(const string& name, const string& address)
{
	myName = name;
	myAddress = address;
	dataIndex = 0;
}
```
이것은 초기화가 아닌 대입이다.<br>
c++ 규칙에서는 어떤 객체든 생성자의 본문이 실행되기 이전에 초기화가 되어야 한다.<br>
하지만 이 생성자에서는 초기화가 지나간 후 본문에 진입하여 대입되고 있는 것에 불과하다.

```cpp
myData::myData(const string& name, const string& address)
: myName(name)
, myAddress(address)
, dataIndex(0)		//기본제공 타입도 초기화 리스트에 넣어주자.
{
	__noop;
}
```
이제 생성자 본문에 진입하기 전에 초기화되며, 본문에는 아무 것도 들어있지 않다.

데이터 멤버에 사용자가 원하는 값을 주고 시작하는 것이므로 이전의 코드보다 더 효율적이기도 하다. 이전 코드는 기본 생성자로 초기화를 거친 후 다시 한 번 대입을 해주는 까닭이다.

그리고 일부 상수나 참조자는 대입 자체가 불가능하기 때문에 초기화 리스트에 필수적으로 넣어줘야 한다. 사실상 그냥 다 초기화 리스트에 넣는 것을 추천하는 바이다.

<br>

### <span style="color:rgb(170,170,255)">- 초기화 순서</span>

객체를 구성하는 데이터는 초기화 순서가 다음과 같다.

1. 기본 클래스는 파생 클래스보다 먼저 초기화된다.
2. 클래스 데이터 멤버는 선언된 순서대로 초기화된다. <br>
초기화 리스트에 넣은 순이 아닌, 선언된 순서대로임을 기억하자. <br>
물론, 초기화리스트와 선언 순서를 맞춰두는 것을 권장한다.

하지만 여기서 문제가 되는 것이 정적 객체이다.

<br>

정적 객체(static object)는 생성된 시점부터 프로그램이 끝날 때, 즉 main()함수의 실행이 끝날 때 소멸자가 호출되는 객체이다. 즉, 스택과 힙 기반의 객체가 아니라는 의미라고도 할 수 있겠다.

정적 객체의 범주에 들어가는 것은 다음과 같다.

1. 전역 객체
2. 네임스페이스 유효범위 내의 객체
3. 클래스/함수/파일 유효범위 안에서 static으로 선언된 객체

그리고 이들 중 함수 안의 객체를 지역 정적 객체(local static object)라고 하고, 나머지를 비지역 정적 객체(non-local static object)라고 한다.

이런 정적 객체의 초기화 순서는 개별 번역 단위에서 정해진다.

여기서 번역 단위(translation unit)란 컴파일을 통해 한 목적 파일(object file)을 만드는 소스 코드를 뜻한다.

번역은 소스 코드를 기계어로 옮기는 것인데, 
```
소스 파일 하나 + include된 파일들 = 하나의 번역 단위
```
라고 볼 수 있다.

<br>

여기서 문제는, 별도로 컴파일된 두 소스파일이 각각 비지역 정적 객체를 가지고 있는 경우이다.

한 번역 단위의 정적 객체가 초기화되며 다른 번역 단위의 비지역 정적 객체가 사용된다면? 

별개의 번역 단위에 정의된 비지역 정적 객체 사이에 상대적인 초기화 순서가 정해져있지 않기 때문에 문제가 발생할 수 있다. 다음 예제를 보자.

```cpp
class system							//라이브러리의 클래스
{
	public: size_t diskCount() const;	//멤버함수
};

extern system s;						//사용자가 쓸 객체
```

```cpp
class directory
{
	public:	directory(param);
};

directory::directory(param)
{
	size_t count = s.diskCount();		//system의 객체 사용
}
```

```cpp
directory temp(params);					//임시 파일을 담는 디렉토리 선언
```

위는 정적 객체의 초기화 순서로 인해 문제가 발생할 수 있는 상황이다.

s와 temp는 소스 파일도 다르기 때문에 다른 번역 단위에 정의된 비지역 정적 객체들이다. temp를 사용하기 전에 s를 초기화하는 것은 이런 상황에서 불가능하다.

이를 해결하기 위해서는 비지역 정적 객체를 하나씩 맡는 함수를 준비하고, 그 안에 각 객체를 넣는 것이다. 함수 속에서 이들을 정적 객체로 선언한 후, 함수에서 이들에 대한 참조자를 반환한다.

즉, 비지역 정적 객체를 지역 정적 객체로 바꾼다는 것이다. (싱글톤 패턴을 사용하는 방식과 동일하다.)

지역 정적 객체는 함수 호출 중 그 객체의 정의에 최초로 접근 시 초기화되도록 되어있다. 이런 식으로 정적 객체에 대한 참조자를 반환할 것이라면, 참조자가 반드시 초기화된 객체를 참조하도록 하여야 하며 '지역 객체 참조자 반환 함수'가 호출되지 않을 시에는 객체의 생성/소멸 비용도 생기지 않게 막아야 한다.

```cpp
class system
{
	public: size_t diskCount() const;
};

system& s()					//extern으로 뒀던 s를 이 함수로 대체한다.
{
	static system staticS;	//지역 정적 객체를 정의, 초기화한다.
	return staticS;			//지역 정적 객체의 레퍼런스를 반환한다.
}
```

```cpp
class directory
{
	public:	directory(param);
};

directory::directory(param)
{
	size_t count = s.diskCount();		//system의 객체 사용
}
```

```cpp
directory& temp()						//temp를 이 함수로 대체한다.
{
	static directory staticDirectory;	//지역 정적 객체를 정의, 초기화한다.
	return staticDirectory;				//지역 정적 객체의 레퍼런스를 반환한다.
}
```

이런 방식을 이용한다면 정적 객체 자체를 직접 쓰지 않고 참조자를 반환하는 함수를 통해 해결할 수 있다.

이런 기법을 이용한 참조자 반환 함수는 복잡하게 구현될 일이 없다. 물론 단일스레드에서의 이야기겠지만.

다중스레드 시스템에서는 비상수 정적 객체 자체가 시한폭탄과 같다는 것에 유의하자. 이런 경우엔 다중스레드로 돌입하기 전에 참조자 반환 함수를 전부 수동으로 호출하여 초기화에 대한 경쟁 상태(race condition)을 방지할 수 있다.

그리고 위와 같은 방식은 객체들의 초기화 순서를 제대로 맞춰둔다는 전제조건 하에 가능한 이야기이다. 만약 객체 B가 초기화되기 전에 객체 A가 초기화되어야 하는데, A가 B에 의존하도록 되어있다면 문제가 된다.

<br>

어떤 객체가 초기화되기 전에 그 객체를 사용하지 않도록 하기 위해서는 세 가지를 기억해야 한다.

1. 멤버가 아닌 기본제공 타입 객체는 직접 초기화하라. 경우에 따라 되기도, 안되기도 하기 때문.
2. 객체의 모든 부분에 대해 멤버 초기화 리스트를 사용하라. 그리고 초기화 리스트의 순서는 멤버가 선언된 순서와 똑같이 나열해주자.
3. 별개의 번역 단위에 있는 비지역 정적 객체들의 초기화 순서에 대해 고려하자. 비지역 정적 객체를 지역 정적 객체로 바꿔주어라.



#include <iostream>
#include <functional>
using namespace std;

/* 
함수포인터 : 함수의 메모리 주소를 저장하기 위한 변수를 선언할 수 있다.
전역함수의 경우 함수명이 곧 함수의 메모리 주소이다.
함수포인터 선언방법 : 반환타입 (*포인터변수명)(인자타입);의 형태로 구성
*/

int Sum(int a, int b) {
	return a + b;
}

int OutSum(int a, int b) {
	cout << "a - b" << endl;
	return a - b;
}

void Output() {
	cout << "Output Function" << endl;
}

float TestFunc(float a) {
	cout << a << endl;
	return a;
}

class CHanzo {
public:
	CHanzo() {

	}
	~CHanzo() {

	}
public:
	int m_iTest;
	void Output() {
		// this->는 생략 가능하다.
		cout << this->m_iTest << endl;
	}
};

typedef struct _tagPoint {
	int x;
	int y;

	_tagPoint() : x(0), y(0) {

	}
	_tagPoint(int _x, int _y) : x(_x), y(_y) {

	}
	_tagPoint(const _tagPoint& pt) {
		// 얕은 복사를 한다. 이렇게 해줄 경우 this는 자기자신의 포인터이고
		// *을 붙여서 자기자신을 역참조하여 모든 데이터를 목사하게 한다.
		*this = pt;
	}

	_tagPoint operator +(const _tagPoint& pt) {
		_tagPoint result;
		result.x = x + pt.x;
		result.y = y + pt.y;
		return result;
	}
	_tagPoint operator +(int a) {
		_tagPoint result;
		result.x = x + a;
		result.y = y + a;
		return result;
	}

}POINT, *PPOINT;


int main() {
	int (*pFunc)(int, int) = Sum;
	cout << pFunc(10, 20) << endl;

	pFunc = OutSum;
	OutSum(10, 20);

	//pFunc = Output (X)
	void (*pFunc1)() = Output;
	pFunc1();

	CHanzo hanzo1, hanzo2;
	hanzo1.m_iTest = 100;
	hanzo2.m_iTest = 200;

	// this 포인터 : 클래스 안에서 this를 사용할 경우 해당 객체의
	// 메모리 주소가 된다. 즉 자기자신의 포인터이다.
	// 멤버함수를 호출할 때 this를 호출자로 세팅한다.
	hanzo1.Output();
	hanzo2.Output();

	// 멤버함수포인터 선언시 포인터변수 이름 앞에 클래스명::을 붙여준다.
	void(CHanzo::*pFunc2)() = &CHanzo::Output;

	(hanzo1.*pFunc2)();

	function<void()> func1;
	function<void()> func2;

	/*
	function 기능은 C++11부터 지원을 해주는 기능이다. 이 기능은 함수포인터를
	전역, 멤버 가리지 않고 쉽게 주소를 저장해서 호출해줄 수 있도록 만들어주는
	기능이다.
	선언방법 : function<반환타입(인자타입)> 변수명; 의 형태로 선언한다.
	*/

	cout << "===============Funtional===================" << endl;
	// bind 함수로 함수를 묶어준다.
	func1 = bind(Output);
	func2 = bind(&CHanzo::Output, &hanzo2);
	
	func1();
	func2();

	function<int(int, int)> func3;
	function<float(float)> func4;

	// 함수에 인자가 있을 경우 placeholders를 이용해서 인자의 순서를 정의할 수 있다.
	// 아래처럼 2,1로 넣어주면 인자가 서로 바뀌게 된다.
	func3 = bind(OutSum, placeholders::_2, placeholders::_1);

	// 위에서 2,1로 바꿔줬기 때문에 b=10,a=20이다.
	func3(20, 10);

	func4 = bind(TestFunc, placeholders::_1);
	func4(3.14f);
	cout << "=======================Point=========================" << endl;
	POINT pt1(10, 20), pt2(30, 40), pt3;

	// POINT 구조체는 +연산자가 operator로 재정이되어있다.
	// 그래서 +연산이 가능하게 되고 pt1의 +연산자 함수를 호출해주는
	// 개념이다. pt2를 인자로 넘겨준다.
	pt3 = pt1 + pt2;
	cout << "x : " << pt3.x << " y : " << pt3.y << endl;

	// 오퍼레이터도 오버로딩이 가능하다.
	pt3 = pt1 + 1000;
	cout << "x : " << pt3.x << " y : " << pt3.y << endl;
	return 0;
}
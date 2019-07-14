#include <iostream>
using namespace std;

void test1(){
	int i = 1;
	// 隐式类型转换
	double d = i;
	printf("%d, %.2f\n", i, d);
	int* p = &i;
	// 显示的强制类型转换
	int address = (int)p;
	printf("%x, %x\n", p, address);
}

void test2(){
	double d = 12.34;
	int a = static_cast<int>(d);
	cout << a << endl;
}

typedef void(*FUNC)();
int DoSomething(int i){
	cout << "DoSomething" << endl;
	return 0;
}
void test3(){
	// reinterpret_cast可以编译器以FUNC的定义方式去看待DoSomething函数
	// 所以非常的BUG，下面转换函数指针的代码是不可移植的，所以不建议这样用
	// C++不保证所有的函数指针都被一样的使用，所以这样用有时会产生不确定的结果
	FUNC f = reinterpret_cast<FUNC>(DoSomething);
	f();
}

void test4(){
	volatile const int a = 2;
	int* p = const_cast<int*>(&a);
	*p = 3;
	cout << a << endl;
	cout << *p << endl;
}

class A
{
public:
	virtual void f(){}
};
class B : public A
{};
void fun(A* pa){
	// dynamic_cast会先检查是否能转换成功，能成功则转换，不能则返回0
	B* pb1 = static_cast<B*>(pa);
	B* pb2 = dynamic_cast<B*>(pa);
	cout << "pb1:" << pb1 << endl;
	cout << "pb2:" << pb2 << endl;
}
void test5(){
	A a;
	B b;
	fun(&a);
	fun(&b);
}

class C
{
public:
	explicit C(int a){ cout << "C(int a)" << endl;}
	C(const C& a){ cout << "C(const C& a)" << endl;}
private:
	int _a;
};
void test6(){
	C a1(1);
	// 隐式转换-> A tmp(1); A a2(tmp);
	C a2(a1);
}


int main(){
	//test1();
	//test2();
	//test3();
	//test4();
	//test5();
	test6();
	system("pause");
	return 0;
}
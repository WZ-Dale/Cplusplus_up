#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <array>

using namespace std;

//int main()
//{
//	int a[] = { 1, 2, 3, 4 };
//	vector<int> v1 = { 1, 2, 3, 4 };
//	vector<int> v2{ 1, 2, 3, 4 };
//	list<int> l1 = { 1, 2, 3, 4 };
//	map<string, string> dict = { { "sort", "排序" }, { "map", "映射" } };
//	int* p = new int[5]{1,2,3,4,5};
//
//	for (auto e : a)
//	{
//		cout << e << " ";
//	}
//	cout << endl;
//
//	for (auto e : v1)
//	{
//		cout << e << " ";
//	}
//	cout << endl;
//
//	for (auto e : v2)
//	{
//		cout << e << " ";
//	}
//	cout << endl;
//
//	for (const auto& e : dict)
//	{
//		cout << e.first << ":" << e.second << endl;
//	}
//
//
//	return 0;
//}
// C++98
class A
{
public:
	//explicit A(int a)
	A(int a)
		:_a(a)
	{}
private:
	int _a;
};

// C++11
class Point
{
public:
	//explicit Point(int x = 0, int y = 0) : _x(x), _y(y)
	Point(int x = 0, int y = 0) : _x(x), _y(y)
	{}
private:
	int _x;
	int _y;
};

template<class T>
class Array
{
public:
	Array(int* a, size_t n)
	{
		_a = new T[n];
		_n = n;
		for (size_t i = 0; i < n; ++i)
		{
			_a[i] = a[i];
		}
	}

	Array(const initializer_list<T>& lt)
	{
		_a = new T[lt.size()];
		_n = lt.size();
		size_t i = 0;
		for (auto& e : lt)
		{
			_a[i++] = e;
		}
	}
private:
	T* _a;
	size_t _n;
};

//int main()
//{
//	//A aa1(1);
//	//A aa2 = 2;
//
//	//Point p1(1, 2);
//	//Point p2 = {3,4};
//
//	/*int a1[] = { 1, 2, 3, 4 };
//	Array<int> aa1(a1, sizeof(a1)/sizeof(int));
//	Array<int> aa2 = { 1, 2, 3, 4 };*/
//
//	int a = 10;
//	decltype(a) b = a;
//
//	//array<int, 1024*1024*10> aaa;
//
//	vector<int> v;
//	v.resize(1024 * 1024 * 10);
//
//	return 0;
//}

//class C
//{
//public:
//	/*C(int i)
//		:_i(i)
//		, _d(0.0)
//		, _ch('0')
//	{}
//
//	C(char ch)
//		:_i(0)
//		, _d(0.0)
//		, _ch(ch)
//	{}
//
//	C(double d)
//		:_i(0)
//		, _d(d)
//		, _ch('0')
//	{}*/
//
//	// 委派构造
//	C()  
//		:_i(0)
//		, _d(0.0)
//		, _ch('0')
//	{}
//
//	C(int i)
//		:C()
//	{
//		_i = i;
//	}
//
//	C(char ch)
//		:C()
//	{
//		_ch = ch;
//	}
//
//	C(double d)
//		:C()
//	{
//		_d = d;
//	}
//
//private:
//	int _i;
//	char _ch;
//	double _d;
//};

//class C
//{
//public:
//	C(int i)
//		:_ch('a')
//	{
//		_i = i;
//	}
//
//	C(char ch)
//	{
//		_ch = ch;
//	}
//
//	C(double d)
//	{
//		_d = d;
//	}
//
//private:
//	int _i = 0;
//	char _ch = '0';
//	double _d = 0.0;  // 声明  + 缺省值 
//};

//class D
//{
//public:
//	D() = default;
//	D(const D& d);
//private:
//	int _d1 = 1;
//	int _d2 = 2;
//};
//
//
//int main()
//{
//	//C cc(1);
//	D dd;
//
//	return 0;
//}

// 提高C++有些部分的效率
/*int main()
{
	// 什么是左值，什么是右值？
	int a = 10;
	int b = a;

	// 结论分析：赋值符号左边的是左值，赋值符号右边的不一定是右值
	// 右值：常量，表达式的返回值(临时变量)

	// 左值引用/右值引用
	int& left_ref1 = a;
	int&& right_ref1 = 10;

	// 左值引用能否引用右值？no ——》 const 左值引用 ok
	const int& left_ref2 = 10;
	// 右值引用能否引用左值？no ——》  move(左值) ok
	int&& right_ref2 = move(a);

	return 0;
}*/

// 深拷贝String
class String
{
public:
	String(char* str = "")
	{
		_str = new char[strlen(str) + 1];
		strcpy(_str, str);
	}

	// C++11 右值  纯右值  +  将亡值
	// 移动构造 + 移动赋值

	// String s1(tmp) -> tmp将亡值
	String(String&& s)
	{
		cout << "String(String&& s)" << endl;
		_str = s._str;
		s._str = nullptr;
	}

	// s1 = tmp -> tmp将亡值
	String& operator=(String&& s)
	{
		cout << "String(String&& s)" << endl;
		swap(_str, s._str);
		return *this;
	}

	// 拷贝构造 + 赋值
	String(const String& s)
		: _str(new char[strlen(s._str) + 1])
	{
		cout << "String(const String& s)" << endl;
		strcpy(_str, s._str);
	}
	String& operator=(const String& s)
	{
		cout << "String& operator=(const String& s)" << endl;

		if (this != &s)
		{
			char* pTemp = new char[strlen(s._str) + 1];
			strcpy(pTemp, s._str);
			delete[] _str;
			_str = pTemp;
		}
		return *this;
	}

	~String()
	{
		if (_str)
			delete[] _str;
	}

	// s1 += s2;
	String& operator+= (const String& s)
	{
		//this->Append(s._str);
		return *this;
	}

	// s1 + s2
	String operator+ (const String& s)
	{
		String ret(*this);
		//ret.Append(s._str);
		return ret;
	}

private:
	char* _str;
};

//int main()
//{
//	// http://www.cplusplus.com/reference/array/
//	// http://www.cplusplus.com/reference/vector/
//	//String header = "http://";
//	//String domain = "www.cplusplus.com";
//	//String url_array = header + domain + "reference/array/";
//	//String url_vector = header + domain + "reference/vector/";
//
//	String header = "http://";
//	String domain = "www.cplusplus.com";
//	String url_array = header;
//	url_array += domain;
//
//	cout << "------------------" << endl;
//	String url_vector;
//	url_vector = header + domain;
//
//	string str("hello1111111111111111111111111111");
//	string&& r_ref = move(str);
//	string copy = move(str);
//
//	vector<string> v;
//	v.push_back(copy);
//	v.push_back(move(copy));
//
//	string name = "peter";
//	move(name);
//	v.push_back(move(name));			  
//	v.push_back(string("peter")); // 移动拷贝
//	v.push_back("peter");		  // 移动拷贝
// 
//	return 0;
//}

// 左值引用 ： 1.引用传参   2.引用传返回值
// 右值引用 ： 1.移动构造和移动赋值 跟拷贝构造和赋值 构成重载 移动构造和移动赋值几乎没有消耗
// 			   2.传值返回拷贝时，就会自动掉移动构造和移动赋值
// 			   3.类似于容器的插入数据接口，提供右值引用的接口，减少对象拷贝。

void f1(String && s)
{
	String copy(std::forward<String>(s));
}

int main()
{
	//String str("hello");
	//f1(move(str));
	//String copy(move(str));

	//vector<string> v;
	//string name = "peter";
	//v.push_back(move(name));

	vector<pair<string, string>> v = { { "sort", "排序" }, { "sort", "排序" } };
	v.push_back(make_pair("sort", "排序"));
	pair<string, string> val = { "sort", "排序" };
	v.push_back(val);

	v.emplace_back(val); // 左值
	v.emplace_back(make_pair("sort", "排序")); // 将亡值
	v.emplace_back("sort", "排序"); // 
	//v.push_back("sort", "排序"); // 

	map<string, string> m;
	m.insert(make_pair("sort", "排序"));
	m.emplace("sort", "排序"); // 


	return 0;
}
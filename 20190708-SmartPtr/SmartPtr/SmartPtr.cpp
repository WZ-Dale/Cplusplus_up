#include <iostream>
#include <memory>
#include <thread>
#include <mutex>
using namespace std;

// 最基础的智能指针
template<class T>
class SmartPtr
{
public:
	SmartPtr(T* ptr = nullptr) : _ptr(ptr){}
	~SmartPtr(){
		if (ptr){
			delete(ptr);
		}
	}
	T& operator*(){
		return *_ptr;
	}
	T* operator->(){
		return _ptr;
	}
private:
	T* _ptr;
};

// 定义一个日期类
class Date
{
public:
	Date(){ cout << "Date()" << endl; }
	~Date(){ cout << "~Date()" << endl; }
	int _year = 0;
	int _month = 0;
	int _day = 0;
};

// auto_ptr的实现原理是:管理权转移
template<class T>
class AutoPtr
{
public:
	AutoPtr(T* ptr = NULL) : _ptr(ptr){}
	~AutoPtr(){
		if (_ptr){
			delete _ptr;
		}
	}
	// 一旦发生拷贝，就将ap中资源转移到当前对象中，然后另ap与其所管理资源断开联系，
	// 这样就解决了一块空间被多个对象使用而造成程序奔溃问题
	AutoPtr(AutoPtr<T>& ap) : _ptr(ap._ptr){
		ap._ptr = NULL;
	}
	AutoPtr<T>& operator=(AutoPtr<T>& ap){
		// 检测是否为自己给自己赋值
		if (this != &ap){
			// 释放当前对象中资源
			if (_ptr){
				delete _ptr;
			}
			// 转移ap中资源到当前对象中
			_ptr = ap._ptr;
			ap._ptr = NULL;
		}
		return *this;
	}
	T& operator*(){
		return *_ptr;
	}
	T* operator->(){
		return _ptr;
	}
private:
	T* _ptr;
};
void test1(){
	AutoPtr<Date> ap(new Date);
	// 现在再从实现原理层来分析会发现，这里拷贝后把ap对象的指针赋空了，导致ap对象悬空
	// 通过ap对象访问资源时就会出现问题。
	//AutoPtr<Date> copy(ap);
	ap->_year = 2018;
}

// unique_ptr的实现原理: 简单粗暴的防拷贝
template<class T>
class UniquePtr
{
public:
	UniquePtr(T * ptr = nullptr) : _ptr(ptr){}
	~UniquePtr(){
		if (_ptr){
			delete _ptr;
		}
	}
	T& operator*() { return *_ptr; }
	T* operator->() { return _ptr; }
private:
	// C++98防拷贝的方式：只声明不实现+声明成私有
	//UniquePtr(UniquePtr<T> const &);
	//UniquePtr & operator=(UniquePtr<T> const &);
	// C++11防拷贝的方式：delete
	UniquePtr(UniquePtr<T> const &) = delete;
	UniquePtr & operator=(UniquePtr<T> const &) = delete;
private:
	T * _ptr;
};
void test2(){
	UniquePtr<Date> ap(new Date);
	//UniquePtr<Date> copy(ap);
	ap->_year = 2018;
}
// shared_ptr的实现原理: 引用计数
template <class T>
class SharedPtr
{
public:
	SharedPtr(T* ptr = nullptr)
		: _ptr(ptr)
		, _pRefCount(new int(1))
		, _pMutex(new mutex){}
	~SharedPtr() { Release(); }
	SharedPtr(const SharedPtr<T>& sp)
		: _ptr(sp._ptr)
		, _pRefCount(sp._pRefCount)
		, _pMutex(sp._pMutex){
		AddRefCount();
	}
	// sp1 = sp2
	SharedPtr<T>& operator=(const SharedPtr<T>& sp){
		//if (this != &sp)
		if (_ptr != sp._ptr){
			// 释放管理的旧资源
			Release();
			// 共享管理新对象的资源，并增加引用计数
			_ptr = sp._ptr;
			_pRefCount = sp._pRefCount;
			_pMutex = sp._pMutex;
			AddRefCount();
		}
		return *this;
	}
	T& operator*() { return *_ptr; }
	T* operator->() { return _ptr; }
	int UseCount() { 
		cout << *_ptr << endl;
		return *_pRefCount;
	}
	T* Get() { return _ptr; }
	void AddRefCount(){
		// 加锁或者使用加1的原子操作
		_pMutex->lock();
		++(*_pRefCount);
		_pMutex->unlock();
	}
	int Get_Count(){ return *_pRefCount;}
private:
	void Release(){
		bool deleteflag = false;
		// 引用计数减1，如果减到0，则释放资源
		_pMutex->lock();
		if (--(*_pRefCount) == 0){
			delete _ptr;
			delete _pRefCount;
			deleteflag = true;
		}
		_pMutex->unlock();
		if (deleteflag == true)
			delete _pMutex;
	}
private:
	int* _pRefCount; // 引用计数
	T* _ptr; // 指向管理资源的指针 
	mutex* _pMutex; // 互斥锁
};
void test3(){
	SharedPtr<int> sp1(new int(10));
	cout << sp1.UseCount() << endl;// 10 1
	SharedPtr<int> sp2(sp1);
	*sp2 = 20;
	cout << sp1.UseCount() << endl;// 20 2
	cout << sp2.UseCount() << endl;// 20 2
	SharedPtr<int> sp3(new int(10));
	sp2 = sp3;
	cout << sp1.UseCount() << endl;// 20 1
	cout << sp2.UseCount() << endl;// 10 1
	cout << sp3.UseCount() << endl;// 10 2
	sp1 = sp3;
	cout << sp1.UseCount() << endl;// 10 3
	cout << sp2.UseCount() << endl;// 10 3
	cout << sp3.UseCount() << endl;// 10 3
}
// shared_ptr管理的资源(对象)不是线程安全的,因此对管理的资源进行操作时也需要加锁.
mutex mtx;// 定义一个全局锁
void SharePtrFunc(SharedPtr<Date>& sp, size_t n){
	cout << sp.Get() << endl;
	for (size_t i = 0; i < n; ++i){
		// 这里智能指针拷贝会++计数，智能指针析构会--计数，这里是线程安全的。
		SharedPtr<Date> copy(sp);
		// 这里智能指针访问管理的资源，不是线程安全的。所以我们看看这些值两个线程++了2n次，但是最终看到的结果，并一定是加了2n
		mtx.lock();// 加锁保证管理对象的线程安全
		copy->_year++;
		copy->_month++;
		copy->_day++;
		mtx.unlock();
	}
}
void test4(){
	SharedPtr<Date> p(new Date);
	cout << p.Get() << endl;
	const size_t n = 100000;
	thread t1(SharePtrFunc, p, n);
	thread t2(SharePtrFunc, p, n);
	t1.join();
	t2.join();
	cout << p.Get_Count() << endl;
	cout << p->_year << endl;
	cout << p->_month << endl;
	cout << p->_day << endl;
}
// shared_ptr的循环引用问题
struct ListNode
{
	int _data;
	//shared_ptr<ListNode> _prev;
	//shared_ptr<ListNode> _next;
	// 可以使用弱指针来解决循环引用问题
	weak_ptr<ListNode> _prev;
	weak_ptr<ListNode> _next;
	~ListNode(){ cout << "~ListNode()" << endl; }
};
void test5(){
	shared_ptr<ListNode> node1(new ListNode);
	shared_ptr<ListNode> node2(new ListNode);
	cout << node1.use_count() << endl;
	cout << node2.use_count() << endl;
	// weak_ptr不增加引用计数
	node1->_next = node2;
	node2->_prev = node1;
	cout << node1.use_count() << endl;
	cout << node2.use_count() << endl;
}
// 如果不是new出来的对象如何通过智能指针管理呢？
// 其实shared_ptr设计了一个删除器来解决这个问题
// 仿函数的删除器
template<class T>
struct FreeFunc {
	void operator()(T* ptr) {
		cout << "free:" << ptr << endl;
		free(ptr);
	}
};
template<class T>
struct DeleteArrayFunc {
	void operator()(T* ptr)	{
		cout << "delete[]" << ptr << endl;
		delete[] ptr;
	}
};
void test6()
{
	FreeFunc<int> freeFunc;
	shared_ptr<int> sp1((int*)malloc(4), freeFunc);
	DeleteArrayFunc<int> deleteArrayFunc;
	shared_ptr<int> sp2((int*)malloc(4), deleteArrayFunc);
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

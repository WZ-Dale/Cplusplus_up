#include <iostream>
#include <memory>
#include <thread>
#include <mutex>
using namespace std;

// �����������ָ��
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

// ����һ��������
class Date
{
public:
	Date(){ cout << "Date()" << endl; }
	~Date(){ cout << "~Date()" << endl; }
	int _year;
	int _month;
	int _day;
};

// auto_ptr��ʵ��ԭ����:����Ȩת��
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
	// һ�������������ͽ�ap����Դת�Ƶ���ǰ�����У�Ȼ����ap������������Դ�Ͽ���ϵ��
	// �����ͽ����һ��ռ䱻�������ʹ�ö���ɳ���������
	AutoPtr(AutoPtr<T>& ap) : _ptr(ap._ptr){
		ap._ptr = NULL;
	}
	AutoPtr<T>& operator=(AutoPtr<T>& ap){
		// ����Ƿ�Ϊ�Լ����Լ���ֵ
		if (this != &ap){
			// �ͷŵ�ǰ��������Դ
			if (_ptr){
				delete _ptr;
			}
			// ת��ap����Դ����ǰ������
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
	// �����ٴ�ʵ��ԭ����������ᷢ�֣����������ap�����ָ�븳���ˣ�����ap��������
	// ͨ��ap���������Դʱ�ͻ�������⡣
	//AutoPtr<Date> copy(ap);
	ap->_year = 2018;
}

// unique_ptr��ʵ��ԭ��: �򵥴ֱ��ķ�����
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
	// C++98�������ķ�ʽ��ֻ������ʵ��+������˽��
	//UniquePtr(UniquePtr<T> const &);
	//UniquePtr & operator=(UniquePtr<T> const &);
	// C++11�������ķ�ʽ��delete
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
// shared_ptr��ʵ��ԭ��: ���ü���
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
			// �ͷŹ���ľ���Դ
			Release();
			// ��������¶������Դ�����������ü���
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
		// ��������ʹ�ü�1��ԭ�Ӳ���
		_pMutex->lock();
		++(*_pRefCount);
		_pMutex->unlock();
	}
private:
	void Release(){
		bool deleteflag = false;
		// ���ü�����1���������0�����ͷ���Դ
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
	int* _pRefCount; // ���ü���
	T* _ptr; // ָ�������Դ��ָ�� 
	mutex* _pMutex; // ������
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

int main(){
	//test1();
	//test2();
	//test3();
	system("pause");
	return 0;
}

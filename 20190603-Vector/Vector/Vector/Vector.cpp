#include <iostream>
#include <assert.h>
using namespace std;

#if 0
#include <vector>
void Test1()
{
	std::vector<int> v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	v.push_back(4);

	//auto ret = std::find(v.begin(), v.end(), 3);
	//v.erase(ret); // ret失效

	// 非法
	//++ret;
	//cout << *ret << endl;
	cout << v.capacity() << endl;
	auto ret = std::find(v.begin(), v.end(), 2);
	v.insert(ret, 20);
	cout << v.capacity() << endl;
	//	cout << *ret << endl;
}
void Test2()
{
	std::vector<int> v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(2);
	v.push_back(2);
	v.push_back(3);
	v.push_back(4);

	auto it = v.begin();
	while (it != v.end()){
		if (*it % 2 == 0){
			it = v.erase(it);
		}
		else		{
			++it;
		}
	}

	for (auto e : v){
		cout << e << endl;
	}
}
#endif

// Vector的模拟实现
template<class T>
class Vector{
	public:
		typedef T* iterator;
		typedef const T* const_iterator;

		Vector() :_start(nullptr), _finish(nullptr), _endofstorage(nullptr){
		}
		//Vector(const Vector<T>& v)
		//{
		//	_start = new T[v.Capacity()];
		//	memcpy(_start, v._start, v.Size()*sizeof(T));
		//	_finish = _start + v.Size();
		//	_endofstorage = _start + v.Capacity();
		//}
		Vector(const Vector<T>& v) :_start(nullptr), _finish(nullptr), _endofstorage(nullptr){
			//Reserve(v.Capacity());
			Reserve(v.Size());
			for (size_t i = 0; i < v.Size(); ++i)
			{
				this->PushBack(v[i]);
			}
		}
		Vector<T>& operator=(Vector<T> v){
			this->Swap(v);
			return *this;
		} 
		void Swap(Vector<T>& v){
			swap(_start, v._start);
			swap(_finish, v._finish);
			swap(_endofstorage, v._endofstorage);
		}
		~Vector(){
			if (_start){
				delete[] _start;
				_start = _finish = _endofstorage = nullptr;
			}
		}
		iterator begin(){
			return _start;
		}
		iterator end(){
			return _finish;
		}
		const_iterator begin() const{
			return _start;
		}
		const_iterator end() const{
			return _finish;
		}
		void PushBack(const T& x){
			if (_finish == _endofstorage){
				size_t newcapacity = Capacity() == 0 ? 4 : Capacity() * 2;
				Reserve(newcapacity);
			}
			*_finish = x;
			++_finish;
		}
		void PopBack(){
			assert(_finish > _start);
			--_finish;
		}
		// void Insert(iterator& pos, const T& x)
		void Insert(iterator pos, const T& x){
			assert(pos < _finish);
			if (_finish == _endofstorage){
				size_t n = pos - _start;
				size_t newcapacity = Capacity() == 0 ? 4 : Capacity() * 2;
				Reserve(newcapacity);
				pos = _start + n;
			}
			iterator end = _finish - 1;
			while (end >= pos){
				*(end + 1) = *end;
				--end;
			}
			*pos = x;
			++_finish;
			//pos = pos + 1;
		}
		iterator Erase(iterator pos){
			assert(pos < _finish);
			iterator it = pos;
			while (it < _finish - 1){
				*it = *(it + 1);
				++it;
			}
			--_finish;
			return pos;
		}
		void Reserve(size_t n){
			if (n > Capacity()){
				size_t size = Size();
				// 开新空间
				T* newarray = new T[n];
				if (_start){
					memcpy(newarray, _start, sizeof(T)*Size());
				}
				// 释放旧空间
				delete[] _start;
				// 赋值
				_start = newarray;
				_finish = _start + size;
				_endofstorage = _start + n;
			}
		}
		void Resize(size_t n, const T& val = T()){
			if (n <= Size()){
				_finish = _start + n;
			}
			else{
				Reserve(n);
				while (_finish != _start + n){
					*_finish = val;
					++_finish;
				}
			}
		}
		T& operator[](size_t pos){
			assert(pos < Size());
			return _start[pos];
		}
		const T& operator[](size_t pos) const{
			assert(pos < Size());
			return _start[pos];
		}
		size_t Size() const{
			return _finish - _start;
		}
		size_t Capacity() const{
			return _endofstorage - _start;
		}
	private:
		iterator _start;
		iterator _finish;
		iterator _endofstorage;
};

void TestVector1(){
	Vector<int> v;
	v.PushBack(1);
	v.PushBack(2);
	v.PushBack(3);
	v.PushBack(4);
	v.PushBack(5);
	// operator[]遍历
	for (size_t i = 0; i < v.Size(); ++i){
		cout << v[i] << " ";
	}
	cout << endl;
	cout << v.Capacity() << endl;
	// 用迭代器遍历
	Vector<int>::iterator it = v.begin();
	while (it != v.end()){
		cout << *it << " ";
		++it;
	}
	cout << endl;
	// 范围for遍历
	for (auto e : v){
		cout << e << " ";
	}
	cout << endl;
}
void TestVector2(){
	Vector<int> v;
	v.PushBack(1);
	v.PushBack(2);
	v.PushBack(3);
	v.PushBack(4);
	v.PushBack(5);
	v.PushBack(6);
	v.PushBack(7);

	Vector<int>::iterator it = v.begin();
	v.Insert(it, 0);
	Vector<int>::iterator ret = std::find(v.begin(), v.end(), 3);
	if (ret != v.end()){
		cout << *ret << endl;
		v.Insert(ret, 30);
		//cout << *ret << endl;
	}
	ret = std::find(v.begin(), v.end(), 3);
	v.Erase(ret);

	for (auto e : v){
		cout << e << " ";
	}
	cout << endl;
}
void TestVector3()
{
	Vector<int> v;
	v.PushBack(1);
	v.PushBack(2);
	v.PushBack(2);
	v.PushBack(3);
	v.PushBack(4);
	v.PushBack(5);
	v.PushBack(6);
	v.PushBack(7);

	auto it = v.begin();
	while (it != v.end()){
		if (*it % 2 == 0){
			it = v.Erase(it);
		}
		else{
			++it;
		}
	}

	for (auto e : v){
		cout << e << " ";
	}
	cout << endl;
}
void TestVector4(){
	Vector<int> v;
	v.PushBack(1);
	v.PushBack(2);
	v.PushBack(2);
	v.PushBack(3);
	v.PushBack(4);
	v.PushBack(5);
	v.PushBack(6);
	v.PushBack(7);
	for (auto e : v){
		cout << e << " ";
	}
	cout << endl;

	Vector<int> copy(v);
	for (auto e : copy){
		cout << e << " ";
	}
	cout << endl;

	Vector<int> v2;
	v2.PushBack(10);
	v2.PushBack(10);
	v = v2;
	for (auto e : v){
		cout << e << " ";
	}
	cout << endl;
}
#include <string>
void TestVector5(){
	Vector<std::string> v;
	v.PushBack("1111111111111111");
	v.PushBack("1111111111111111");
	v.PushBack("1111111111111111");
	v.PushBack("1111111111111111");
	v.PushBack("1111111111111111");
	for (auto e : v){
		cout << e << endl;
	}
	cout << endl;
}

int main()
{
	//Test1();
	//Test2();
	//TestVector1();
	//TestVector2();
	//TestVector3();
	//TestVector4();
	TestVector5();
	system("pause");
	return 0;
}

#include <iostream>
using namespace std;

#if 0
// ʵ��һ�������String(��Ҫ�漰��ǳ����) -> ��ͳд��
class String{
public:
	// ����
	String(char* str = "") :_str(new char[strlen(str) + 1]){
		strcpy(_str, str);
	}
	// ��������
	String(const String& s) :_str(new char[strlen(s._str) + 1]){
		strcpy(_str, s._str);
	}
	// ��ֵ
	String& operator= (const String& s){
		if (this != &s){
			delete[] _str;
			_str = new char[strlen(s._str) + 1];
			strcpy(_str, s._str);
		}
		return *this;
	}
	// ����
	~String(){
		if (_str){
			delete[] _str;
			_str = nullptr;
		}
	}
	char& operator[](size_t index){
		return _str[index];
	}
	const char* c_str(){
		return _str;
	}
private:
	char* _str;
};

void Test(){
	String s1;
	String s2("hello");
	s2[0] = 'x';
	cout << s1.c_str() << endl;
	cout << s2.c_str() << endl;

	String s3(s2);
	s3[0] = 'h';
	cout << s3.c_str() << endl;

	s1 = s3;
	cout << s1.c_str() << endl;
	s1 = s1;
	cout << s1.c_str() << endl;
}
#endif

#if 0
// ʵ��һ�������String(��Ҫ�漰��ǳ����) -> �ִ�д��
class String{
public:
	// ����
	String(char* str = "") :_str(new char[strlen(str) + 1]){
		strcpy(_str, str);
	}
	// �������� -> �ִ�д��
	String(const String& s) :_str(nullptr){
		String tmp(s._str);
		swap(_str, tmp._str);
	}
	//// ��ֵ -> �ִ�д��
	//String& operator= (const String& s){
	//	if (this != &s){
	//		String tmp(s);//��������
	//		// String tmp(s._str);// �� ����
	//		swap(_str, tmp._str);
	//	}
	//	return *this;
	//}
	// ��ֵ -> ������ִ�д��
	String& operator= (String s){
		swap(_str, s._str);
		return *this;
	}
	// �ִ�д����Ҫ�������ͷ�,�����Լ�ȥ��,�վ��������������ȥ��

	// ����
	~String(){
		if (_str){
			delete[] _str;
			_str = nullptr;
		}
	}
	char& operator[](size_t index){
		return _str[index];
	}
	const char* c_str(){
		return _str;
	}
private:
	char* _str;
};

void Test(){
	String s1("hello");
	String s2(s1);
	cout << s1.c_str() << endl;
	cout << s2.c_str() << endl;

	String s3("world");
	s1 = s3;
	cout << s1.c_str() << endl;
}
#endif

#if 1
#include <assert.h>
// ʵ��һ�������String(��Ҫ�漰��ǳ����) -> �ִ�д��_up
class String{
public:
	// ������
	typedef char* iterator; // ��ͨ������
	typedef const char* const_iterator; // const������
	iterator begin(){
		return _str;
	}
	iterator end(){
		return _str + _size;
	}
	const_iterator begin() const{
		return _str;
	}
	const_iterator end() const{
		return _str + _size;
	}
	// ����
	//String(char* str = ""){
	//	_size = _capacity = strlen(str);
	//	_str = new char[_capacity + 1];
	//	strcpy(_str, str);
	//}
	// ����_up
	String(char* str = "") :_str(nullptr), _capacity(0){
		_size = strlen(str);
		Reserve(_size);
		strcpy(_str, str);
	}
	// ��������
	String(const String& s) :_str(nullptr), _size(0), _capacity(0){
		String tmp(s._str);
		Swap(tmp);	// �� tmp.Swap(*this);	//ֻҪ�����Ϳ���
	}
	// ��ֵ
	String& operator= (String s){
		Swap(s);
		return *this;
	}
	void Swap(String& s)
	{
		swap(_str, s._str);
		swap(_size, s._size);
		swap(_capacity, s._capacity);
	}
	// ����
	~String(){
		if (_str){
			delete[] _str;
			_str = nullptr;
			_size = _capacity = 0;
		}
	}
	//// �����ַ�
	//void PushBack(char ch)	{
	//	if(_size == _capacity){
	//		Reserve(_capacity * 2);
	//	}
	//	_str[_size] = ch;
	//	++_size;
	//	_str[_size] = '\0';
	//}
	// �����ַ�_up
	void PushBack(char ch)	{
		Insert(_size, ch);
	}
	//// �����ַ���
	//void Append(const char* str){
	//	size_t len = strlen(str);
	//	if(_size + len > _capacity){
	//		Reserve(_size + len);
	//	}
	//	strcpy(_str + _size, str);
	//	_size += len;
	//}
	// �����ַ���_up
	void Append(const char* str){
		Insert(_size, str);
	}
	// ��posλ�ò����ַ�
	void Insert(size_t pos, char ch){
		assert(pos <= _size);
		if (_size == _capacity){
			Reserve(_capacity * 2);
		}
		for (int i = _size; i >= (int)pos; --i){
			_str[i + 1] = _str[i];
		}
		_str[pos] = ch;
		_size++;
	}
	// ��posλ�ò����ַ���
	void Insert(size_t pos, const char* str){
		size_t len = strlen(str);
		if (len + _size > _capacity){
			Reserve(len + _size);
		}
		/*
		size_t end = _size + len;
		while (end > pos+len-1)
		{
			_str[end] = _str[end-len];
			--end;
		}
		*/
		int end = _size;
		while (end >= (int)pos){
			_str[end + len] = _str[end];
			--end;
		}
		// �˴�����strcpy��Ὣ'/0'Ҳ������,�Ḳ�Ǻ�����ַ�
		strncpy(_str + pos, str, len);
		_size += len;
	}
	// ��posλ��ɾ��
	void Erase(size_t pos = 0, size_t len = npos){
		if (pos + len >= _size){
			_size = pos;
			_str[_size] = '\0';
		}
		else{
			size_t start = pos + len;
			strcpy(_str + pos, _str + start);
			_size -= len;
		}
	}
	// ��posλ����������ַ�
	size_t Find(char ch, size_t pos = 0){
		for (size_t i = pos; i < _size; ++i){
			if (_str[i] == ch){
				return i;
			}
		}
		return npos;
	}
	// ��posλ����������ַ���
	size_t Find(const char* str, size_t pos = 0){
		const char* ret = strstr(_str + pos, str);
		if (ret == nullptr){
			return npos;
		}
		else{
			return ret - _str;
		}
	}
	bool operator<(const String& s)const{
		size_t i = 0;
		size_t j = 0;
		while (i < _size && j < s._size){
			if (_str[i] < s[j]){
				return true;
			}
			else if (_str[i] > s[j]){
				return false;
			}
			else{
				++i;
				++j;
			}
		}
		// "hello" "hello"  false
		// "hello!" "hello" false
		// "hello" "hello!" true
		if (j < s._size){
			return true;
		}
		else{
			return false;
		}
	}
	bool operator<=(const String& s)const{
		return *this < s || *this == s;
	}
	bool operator>(const String& s)const{
		return !(*this <= s);
	}
	bool operator>=(const String& s)const{
		return !(*this < s);
	}
	bool operator==(const String& s)const{
		size_t i = 0;
		size_t j = 0;
		while (i < _size && j < s._size){
			if (_str[i] != s[j]){
				return false;
			}
			else{
				++i;
				++j;
			}
		}
		if (i == _size && j == s._size){
			return true;
		}
		else{
			return false;
		}
	}
	bool operator!=(const String& s)const{
		return !(*this == s);
	}

	String& operator+=(char ch){
		PushBack(ch);
		return *this;
	}
	String& operator+=(const char* str){
		Append(str);
		return *this;
	}
	String operator+(char ch) const{
		String tmp(*this);
		tmp.PushBack(ch);
		return tmp;
	}
	String& operator+(const char* str){
		String tmp(*this);
		tmp.Append(str);
		return tmp;
	}
	char& operator[](size_t pos){
		assert(pos < _size);
		return _str[pos];
	}
	const char& operator[](size_t pos) const{
		assert(pos < _size);
		return _str[pos];
	}
	const char* c_str() const{
		return _str;
	}
	size_t Size() const{
		return _size;
	}
	size_t Capacity() const{
		return _capacity;
	}
	// ����
	void Reserve(size_t n){
		if (n == 0 || n > _capacity){
			//size_nt newsize = (n + 7) & ( ~7);
			size_t newsize = n;
			if (n % 8 != 0){
				newsize = (n / 8 + 1) * 8;
			}
			else{
				newsize = n + 8;
			}
			char* newstr = new char[newsize];
			if (_str){
				strcpy(newstr, _str);
			}
			_str = newstr;
			_capacity = newsize - 1;
		}
	}
	void Resize(size_t n, char ch = '\0'){
		if (n <= _size){
			_size = n;
			_str[_size] = '\0';
		}
		else{
			Reserve(n);
			for (size_t i = _size; i < n; ++i){
				_str[i] = ch;
				++_size;
			}
			_str[_size] = '\0';
		}
	}
private:
	char* _str;
	size_t _size;
	size_t _capacity;

	static size_t npos;
};

size_t String::npos = -1;

void Test1(){
	String s1("hello");
	String s2(s1);
	cout << s1.c_str() << endl;
	cout << s2.c_str() << endl;

	String s3("world");
	s1 = s3;
	cout << s1.c_str() << endl;
}
void Test2()
{
	// operator[]
	String s1("hello");
	for (size_t i = 0; i < s1.Size(); ++i){
		cout << s1[i] << " ";
	}
	cout << endl;
	// ��ͨ������
	String::iterator it1 = s1.begin();
	while (it1 != s1.end()){
		cout << *it1 << " ";
		++it1;
	}
	cout << endl;
	// const������
	const String s2("world");
	String::const_iterator it2 = s2.begin();
	while (it2 != s2.end()){
		//*it2 = 'x';
		cout << *it2 << " ";
		++it2;
	}
	cout << endl;
	cout << s2.c_str() << endl;
	// ��Χfor����
	for (auto& e : s1){
		e = 'y';
		cout << e << " ";
	}
	cout << endl;
	cout << s1.c_str() << endl;
}
void Test3(){
	String s1("hello");
	s1 += ' ';
	s1 += "world";
	cout << s1.c_str() << endl;
	cout << s1.Size() << endl;
	cout << s1.Capacity() << endl;

	s1.Resize(5);
	cout << s1.c_str() << endl;
	cout << s1.Size() << endl;
	cout << s1.Capacity() << endl;

	s1.Resize(20, 'x');
	cout << s1.c_str() << endl;
	cout << s1.Size() << endl;
	cout << s1.Capacity() << endl;
}
void Test4()
{
	String s1("hello!!!!!!");
	s1.Insert(5, " world world world");
	cout << s1.Size() << endl;
	cout << s1.Capacity() << endl;
	cout << s1.c_str() << endl;

	s1.Insert(0, "hello ");
	cout << s1.c_str() << endl;

	s1.Erase(0, 6);
	cout << s1.c_str() << endl;
	s1.Erase(5, 1000);
	cout << s1.c_str() << endl;
	s1.Erase(2, 2);
	cout << s1.c_str() << endl;
}
void Test5()
{
	String url("http://www.cplusplus.com//reference/cstring/strstr/?kw=strstr");
	cout << url.Find("www") << endl;
	cout << url.Find("//", 7) << endl;

	String s1("hello");
	String s2("hello!");
	String s3("hello");
	cout << (s1 < s2) << endl;
	cout << (s1 < s3) << endl;
	cout << (s1 == s2) << endl;
	cout << (s1 == s3) << endl;
}
#endif

int main(){
	//Test1();
	//Test2();
	//Test3();
	//String s;
	//cout << s.Capacity() << endl;
	//Test4();
	Test5();
	system("pause");
	return 0;
}

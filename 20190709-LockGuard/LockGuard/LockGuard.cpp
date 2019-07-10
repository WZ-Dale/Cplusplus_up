#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

// C++11�Ŀ���Ҳ��һ��lock_guard(������)�������LockGuard��д��Ϊ��ѧϰ��ԭ��
template<class Mutex>
class LockGuard
{
public:
	LockGuard(Mutex& mtx) :_mutex(mtx) {
		_mutex.lock();
	}
	~LockGuard() {
		_mutex.unlock();
	}
	// ��������ᵼ��һ��������������,�����Ҫ�ѿ�������͸�ֵ����
	LockGuard(const LockGuard<Mutex>&) = delete;
	LockGuard<Mutex>& operator=(const LockGuard<Mutex>&) = delete;
private:
	// ע���������ʹ�����ã��������ľͲ���һ������������
	Mutex& _mutex;
};
mutex mtx;
static int n = 0;
void Func() {
	LockGuard<mutex> lock(mtx);
	for (size_t i = 0; i < 1000000; ++i) {
		//LockGuard<mutex> lock(mtx);
		++n;
	}
}

int main() {
	int begin = clock();
	thread t1(Func);
	thread t2(Func);
	t1.join();
	t2.join();
	int end = clock();
	cout << n << endl;
	cout << "cost time:" << end - begin << endl;
	system("pause");
	return 0;
}

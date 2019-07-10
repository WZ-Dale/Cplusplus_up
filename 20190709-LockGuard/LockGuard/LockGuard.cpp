#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

// C++11的库中也有一个lock_guard(守卫锁)，下面的LockGuard自写是为了学习其原理
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
	// 拷贝构造会导致一个锁被解锁两次,因此需要把拷贝构造和赋值禁掉
	LockGuard(const LockGuard<Mutex>&) = delete;
	LockGuard<Mutex>& operator=(const LockGuard<Mutex>&) = delete;
private:
	// 注意这里必须使用引用，否则锁的就不是一个互斥量对象
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

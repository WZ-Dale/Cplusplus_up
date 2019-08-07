#pragma once

#include <iostream>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <time.h>

using namespace std;

void test_unordered()
{
	unordered_set<int> us;
	us.insert(3);
	us.insert(2);
	us.insert(5);
	us.insert(2);
	us.insert(4);

	//unordered_set<int>::iterator uit = us.begin();
	auto uit = us.begin();
	while (uit != us.end()){
		cout << *uit << " ";
		++uit;
	}
	cout << endl;

	unordered_map<string, string> dict;
	dict["left"];
	dict["left"] = "左边";
	dict["left"] = "剩余";
}


// 测性能要用release版本，有优化
// debug版本内联不起作用，为了调试，会加入一些调试信息
// release相对会快一点
// 两个版本的优化程度不同
void test_map_set_op()
{
	const int n = 1000000;
	vector<int> v1;
	vector<int> v2;
	v1.reserve(n);
	v2.reserve(n);

	srand(time(0));
	for (int i = 0; i < n; ++i){
		v1.push_back(rand());
		v2.push_back(rand());
	}
	// 测试 set 和 unordered_set 对比插入时间
	{
		size_t begin1 = clock();
		set<int> s1;
		for (int i = 0; i < n; ++i){
			s1.insert(v1[i]);
		}
		size_t end1 = clock();

		size_t begin2 = clock();
		unordered_set<int> s2;
		for (int i = 0; i < n; ++i){
			s2.insert(v1[i]);
		}
		size_t end2 = clock();

		cout << "set insert:" << end1 - begin1 << endl;
		cout << "unordered_set insert:" << end2 - begin2 << endl;
	}
	// 测试 set 和 unordered_set 对比查找时间
	{
		size_t begin1 = clock();
		set<int> s1;
		for (int i = 0; i < n; ++i){
			s1.find(v2[i]);
		}
		size_t end1 = clock();

		size_t begin2 = clock();
		unordered_set<int> s2;
		for (int i = 0; i < n; ++i){
			s2.find(v2[i]);
		}
		size_t end2 = clock();

		cout << "set find:" << end1 - begin1 << endl;
		cout << "unordered_set find:" << end2 - begin2 << endl;
	}
	// 测试 set 和 unordered_set 对比删除时间
	{
		size_t begin1 = clock();
		set<int> s1;
		for (int i = 0; i < n; ++i){
			s1.erase(v2[i]);
		}
		size_t end1 = clock();

		size_t begin2 = clock();
		unordered_set<int> s2;
		for (int i = 0; i < n; ++i){
			s2.erase(v2[i]);
		}
		size_t end2 = clock();

		cout << "set erase:" << end1 - begin1 << endl;
		cout << "unordered_set erase:" << end2 - begin2 << endl;
	}
}

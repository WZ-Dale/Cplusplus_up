#pragma once
#include <vector>

namespace closehash
{
	enum State
	{
		EXITS,
		EMPTY,
		DELETE,
	};

	template<class T>
	struct HashStruct
	{
		T _v;			// 
		State _state;
	};

	template<class K, class T>
	class HashTable
	{
		typedef HashStruct<T> HashStruct;
	public:
		bool Insert(const T& v)
		{
			// 1.除0
			// 2.数据映射关系打乱
			if (_table.size() == 0 ||
				10 * _num / _table.size() >= 7)
			{
				size_t newsize = _table.size() == 0 ? 10 : _table.size() * 2;
				/*	vector<HashStruct> newtable;
					newtable.resize(newsize);
					for (size_t i = 0; i < _table; i++)
					{
					if (_table[i]._state == EXITS)
					{

					}

					}*/
				HashTable<K, T> newht;
				newht._table.resize(newsize);
				for (size_t i = 0; i < newht._table.size(); ++i)
				{
					newht._table[i]._state = EMPTY;
				}

				for (size_t i = 0; i < _table.size(); ++i)
				{
					if (_table[i]._state == EXITS)
					{
						newht.Insert(_table[i]._v);
					}
				}

				_table.swap(newht._table);
			}

			//size_t index = v % _table.size();
			//// 线性探测
			//while (_table[index]._state == EXITS)
			//{
			//	if (_table[index]._v == v)
			//	{
			//		return false;
			//	}

			//	++index;
			//	if (index == _table.size())
			//		index = 0;
			//}

			// 二次探测
			size_t start = v % _table.size();
			size_t index = start;
			size_t i = 1;
			while (_table[index]._state == EXITS)
			{
				if (_table[index]._v == v)
				{
					return false;
				}

				index = start + i*i;
				index %= _table.size();
				++i;
			}

			_table[index]._v = v;
			_table[index]._state = EXITS;
			_num++;

			return true;
		}

		HashStruct* Find(const K& k)
		{
			if (_table.size() == 0)
				return nullptr;

			size_t index = k % _table.size();
			while (_table[index]._state != EMPTY)
			{
				if (_table[index]._v == k && _table[index]._state == EXITS)
				{
					return &_table[index];
				}

				++index;
				if (index == _table.size())
				{
					index = 0;
				}
			}

			return nullptr;
		}

		bool Erase(const K& k)
		{
			HashStruct* ret = Find(k);
			if (ret == nullptr)
			{
				return false;
			}
			else
			{
				ret->_state = DELETE;
				--_num;
				return true;
			}
		}

	private:
		//HashStruct* _table;
		//size_t _size;
		//size_t _capacity;
		vector<HashStruct> _table;
		size_t _num = 0;	// 映射存储的数据个数
	};

	void TestHashTable()
	{
		//HashTable<int, int> s;
		//s.Insert(5);
		//s.Insert(3);
		//s.Insert(2);
		//s.Insert(12);
		//s.Insert(22);
		//s.Insert(9);
		//s.Insert(8);
		//s.Insert(7);

		HashTable<int, int> s;
		s.Insert(2);
		s.Insert(12);
		s.Insert(22);
		s.Insert(32);
	}
}

namespace hashbucket
{
	template<class T>
	struct HashNode
	{
		HashNode<T>* _next;
		T _v;
	};

	template<class T>
	struct __HTIterator
	{
		typedef HashNode<T> Node;
		Node* _node;
	};

	template<class K, class T, class KeyOfValue>
	class HashTable
	{
		typedef HashNode<T> Node;
	public:
		bool Insert(const T& v)
		{
			// 增容 load factor == 1
			if (_table.size() == _num)
			{
				size_t newsize = _table.size() == 0 ? 10 : _table.size() * 2;
				vector<Node*> newtable;
				newtable.resize(newsize);

				// 挪动旧表数据到新表计算新的位置
				for (size_t i = 0; i < _table.size(); ++i)
				{
					Node* cur = _table[i];
					while (cur)
					{
						Node* next = cur->_next;

						size_t index = HashFunc(kov(cur->_v), newtable.size());
						cur->_next = newtable[index];
						newtable[index] = cur;

						cur = next;
					}

					_table[i] = nullptr;
				}

				newtable.swap(_table);
			}

			KeyOfValue kov;
			size_t index = HashFunc(kov(v), _table.size());
			Node* cur = _table[index];
			while (cur)
			{
				if (kov(cur->_v) == kov(v))
				{
					return false;
				}

				cur = cur->_next;
			}

			// 头插
			Node* newnode = new Node(v);
			newnode->_next = _table[index];
			_table[index] = newnode;
			++_num;

			return true;
		}

		Node* Find(const K& k);
		bool Erase(const K& k);

		size_t HashFunc(const K& k, size_t n)
		{
			return k % n;
		}

	private:
		vector<Node*> _table;
		size_t _num = 0;
	};
}

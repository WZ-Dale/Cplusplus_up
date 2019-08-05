#pragma once
#include <iostream>
#include <assert.h>

using namespace std;

enum Colour
{
	RED,
	BLACK,
};

template<class K, class V>
struct RBtreeNode
{
	RBtreeNode<K, V>* _left;
	RBtreeNode<K, V>* _right;
	RBtreeNode<K, V>* _parent;

	pair<K, V> _kv;
	Colour	   _col;

	RBtreeNode(const pair<K, V>& kv)
		: _kv(kv)
		, _left(nullptr)
		, _right(nullptr)
		, _parent(nullptr)
		, _col(RED)	// ��ʼ��Ϊ�죬���ᵼ�ºڽ�����������
	{}
};

template<class K, class V>
class RBTree
{
	typedef RBtreeNode<K, V> Node;	// �ڵ���������ΪNode
public:
	bool Insert(const pair<K, V>& kv)
	{
		// ����ڵ�
		if (_root == nullptr){	// ������ڵ�Ϊ�գ���ֱ�Ӳ��뵽���ڵ�
			_root = new Node(kv);
			_root->_col = BLACK;	// ���ڵ��Ǻ�ɫ��
			return true;
		}

		Node* parent = nullptr;
		Node* cur = _root;
		while (cur){	// ������ڵ㲻Ϊ�գ�����ҿ��Բ���ĺ���λ��
			if (cur->_kv.first < kv.first){
				parent = cur;
				cur = cur->_right;
			}
			else if (cur->_kv.first > kv.first){
				parent = cur;
				cur = cur->_left;
			}
			else{	// ��ȵĻ�������ʧ��
				return false;
			}
		}
		// ���ҵ��ĺ���λ�ò���
		cur = new Node(kv);// �˴���Ҫ�½��ڵ�
		cur->_col = RED;
		if (parent->_kv.first < kv.first){	// �ж�����߻����ұ߲���
			parent->_right = cur;
			cur->_parent = parent;
		}
		else{
			parent->_left = cur;
			cur->_parent = parent;
		}
		// ����֮�󣬺�����������Ƿ��쵽�ƻ��������ƻ�����Ҫ������ƽ�⣨�ӽ�ƽ�⣩
		while (parent && parent->_col == RED){	// �����ڵ���ɫΪ�ڣ���û��Ҫ���ϸ�����
			Node* grandfather = parent->_parent;
			if (parent == grandfather->_left){
				Node* uncle = grandfather->_right;
				if (uncle && uncle->_col == RED){	// 1.��ǰΪ�죬��Ϊ�죬���游��Ȼ�Ǻڣ������������Ϊ��		�����ں죩
					parent->_col = BLACK;
					uncle->_col = BLACK;
					grandfather->_col = RED;

					cur = grandfather;	//	���������ж�
					parent = cur->_parent;
				}
				else{	// 2.��ǰ���������������ڻ�Ϊ��		�����ںڣ�
					if (cur == parent->_right){	// ���3��������Ҫ����תΪ���2��Ȼ�������2ͳһ����
						RotateL(parent);
						swap(parent, cur);	// ��ת����Ҫ�ı�cur��parent��ָ�򣬲ź����2����
					}
					// ���2
					RotateR(grandfather);
					parent->_col = BLACK;
					grandfather->_col = RED;

					break;
				}
			}
			else{	// �ԳƵ���һ��
				Node* uncle = grandfather->_left;
				if (uncle && uncle->_col == RED)
				{
					parent->_col = BLACK;
					uncle->_col = BLACK;
					grandfather->_col = RED;

					cur = grandfather;
					parent = cur->_parent;
				}
				else
				{
					if (cur == parent->_left)
					{
						RotateR(parent);
						swap(parent, cur);
					}

					RotateL(grandfather);
					parent->_col = BLACK;
					grandfather->_col = RED;
					break;
				}
			}
		}

		_root->_col = BLACK;

		return true;
	}

	void RotateL(Node* parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;

		parent->_right = subRL;
		if (subRL){
			subRL->_parent = parent;
		}
		subR->_left = parent;
		Node* ppNode = parent->_parent;
		parent->_parent = subR;

		if (parent == _root){	// ������ڵ��Ǹ��ڵ�
			_root = subR;
			_root->_parent = nullptr;
		}
		else{	// ������ǣ�������游��ָ��
			if (ppNode->_left == parent){
				ppNode->_left = subR;
			}
			else{
				ppNode->_right = subR;
			}
			subR->_parent = ppNode;
		}
	}

	void RotateR(Node* parent)
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;

		parent->_left = subLR;
		if (subLR){
			subLR->_parent = parent;
		}
		subL->_right = parent;

		Node* ppNode = parent->_parent;
		parent->_parent = subL;

		if (ppNode == nullptr){
			_root = subL;
			_root->_parent = nullptr;
		}
		else{
			if (ppNode->_left == parent){
				ppNode->_left = subL;
			}
			else{
				ppNode->_right = subL;
			}
			subL->_parent = ppNode;
		}
	}

	bool IsValidRBTree()
	{
		Node* pRoot = _root;
		// ����Ҳ�Ǻ����
		if (nullptr == pRoot){
			return true;
		}
		// �����ڵ��Ƿ��������
		if (pRoot->_col != BLACK){
			cout << "Υ����������ʶ������ڵ����Ϊ��ɫ" << endl;
			return false;
		}
		// ��ȡ����һ��·���к�ɫ�ڵ�ĸ���
		size_t blackCount = 0;
		Node* pCur = pRoot;
		while (pCur){
			if (pCur->_col == BLACK){
				blackCount++;
			}
			pCur = pCur->_left;
		}
		// ����Ƿ��������������ʣ�k������¼·���к�ɫ�ڵ�ĸ���
		size_t k = 0;
		return _IsValidRBTree(pRoot, k, blackCount);
	}

	bool _IsValidRBTree(Node* pRoot, size_t k, const size_t blackCount)
	{
		//�ߵ�null֮���ж�k��black�Ƿ����
		if (nullptr == pRoot){
			if (k != blackCount){
				cout << "Υ�������ģ�ÿ��·���к�ɫ�ڵ�ĸ���������ͬ" << endl;
				return false;
			}
			return true;
		}
		// ͳ�ƺ�ɫ�ڵ�ĸ���
		if (pRoot->_col == BLACK){
			k++;
		}
		// ��⵱ǰ�ڵ�����˫���Ƿ�Ϊ��ɫ
		Node* pParent = pRoot->_parent;
		if (pParent && pParent->_col == RED && pRoot->_col == RED){
			cout << "Υ����������û������һ��ĺ�ɫ�ڵ�" << endl;
			return false;
		}
		return _IsValidRBTree(pRoot->_left, k, blackCount) && _IsValidRBTree(pRoot->_right, k, blackCount);
	}

	void InOrder()
	{
		_InOrder(_root);
		cout << endl;
	}

	void _InOrder(Node* root)
	{
		if (root == nullptr){
			return;
		}
		_InOrder(root->_left);
		cout << root->_kv.first << " ";
		_InOrder(root->_right);
	}
private:
	Node* _root = nullptr;
};

void TestRBtree()
{
	RBTree<int, int> t;
	int a[] = { 4, 2, 6, 1, 3, 5, 9, 7, 8, 0, 0, 3 };
	//int a[] = { 16, 3, 7, 11, 9, 26, 18, 14, 15 };
	//int a[] = { 4, 2, 6, 1, 3, 5, 15, 7, 16, 14 };
	for (auto e : a){
		t.Insert(make_pair(e, e));
	}

	t.InOrder();
	cout << t.IsValidRBTree() << endl;
}

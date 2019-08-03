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
		, _col(RED)	// 初始化为红，不会导致黑结点数量不相等
	{}
};

template<class K, class V>
class RBTree
{
	typedef RBtreeNode<K, V> Node;	// 节点类重命名为Node
public:
	bool Insert(const pair<K, V>& kv)
	{
		// 插入节点
		if (_root == nullptr){	// 如果根节点为空，则直接插入到根节点
			_root = new Node(kv);
			_root->_col = BLACK;	// 根节点是黑色的
			return true;
		}

		Node* parent = nullptr;
		Node* cur = _root;
		while (cur){	// 如果根节点不为空，则查找可以插入的合适位置
			if (cur->_kv.first < kv.first){
				parent = cur;
				cur = cur->_right;
			}
			else if (cur->_kv.first > kv.first){
				parent = cur;
				cur = cur->_left;
			}
			else{	// 相等的话，插入失败
				return false;
			}
		}
		// 在找到的合适位置插入
		cur = new Node(kv);// 此处需要新建节点
		cur->_col = RED;
		if (parent->_kv.first < kv.first){	// 判断在左边还是右边插入
			parent->_right = cur;
			cur->_parent = parent;
		}
		else{
			parent->_left = cur;
			cur->_parent = parent;
		}
		// 插入之后，红黑树的性质是否造到破坏，若被破坏则需要将树调平衡（接近平衡）
		while (parent && parent->_col == RED){	// 若父节点颜色为黑，就没必要向上更新了
			Node* grandfather = parent->_parent;
			if (parent == grandfather->_left){
				Node* uncle = grandfather->_right;
				if (uncle && uncle->_col == RED){	// 1.当前为红，父为红，（祖父必然是黑），叔叔存在且为红		（红红黑红）
					parent->_col = BLACK;
					uncle->_col = BLACK;
					grandfather->_col = RED;

					cur = grandfather;	//	继续向上判断
					parent = cur->_parent;
				}
				else{	// 2.当前结点的叔叔若不存在或为黑		（红红黑黑）
					if (cur == parent->_right){	// 情况3，折线需要先旋转为情况2，然后与情况2统一处理
						RotateL(parent);
						swap(parent, cur);	// 旋转后需要改变cur和parent的指向，才和情况2适用
					}
					// 情况2
					RotateR(grandfather);
					parent->_col = BLACK;
					grandfather->_col = RED;

					break;
				}
			}
			else{	// 对称的另一边
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

		if (parent == _root){	// 如果父节点是根节点
			_root = subR;
			_root->_parent = nullptr;
		}
		else{	// 如果不是，则更改祖父的指向
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

private:
	Node* _root = nullptr;
};

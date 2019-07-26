#pragma once
#include <iostream>

using namespace std;

// 搜索树
template<class K, class V>
struct BSTreeNode
{
	pair<K, V> _kv;
	BSTreeNode<K, V>* _left;
	BSTreeNode<K, V>* _right;

	BSTreeNode(const pair<K, V>& kv)
		:_left(nullptr)
		, _right(nullptr)
		, _kv(kv)
	{}
};

template<class K, class V>
class BSTree
{
	typedef BSTreeNode<K, V> Node;
public:
	BSTree()
		:_root(nullptr)
	{}

	bool Insert(const pair<K, V>& kv){
		if (_root == nullptr){
			_root = new Node(kv);
			return true;
		}

		Node* parent = nullptr;
		Node* cur = _root;
		while (cur){	// 找合适的插入位置
			if (cur->_kv.first > kv.first){
				parent = cur;
				cur = cur->_left;
			}
			else if (cur->_kv.first < kv.first){
				parent = cur;
				cur = cur->_right;
			}
			else{
				return false;
			}
		}

		cur = new Node(kv);	// 找的合适的插入位置后进行插入
		if (parent->_kv.first < kv.first){
			parent->_right = cur;
		}
		else{
			parent->_left = cur;
		}
		return true;
	}

	Node* Find(const K& key){
		Node* cur = _root;
		while (cur){
			if (cur->_kv.first < key){
				cur = cur->_right;
			}
			else if (cur->_kv.first > key){
				cur = cur->_left;
			}
			else{
				return cur;
			}
		}
		return nullptr;
	}

	bool Remove(const K& key){
		Node* parent = nullptr;
		Node* cur = _root;
		while (cur){
			if (cur->_kv.first < key){	// 找到要删除的节点
				parent = cur;
				cur = cur->_right;
			}
			else if (cur->_kv.first > key){
				parent = cur;
				cur = cur->_left;
			}
			else{
				// 1.只有0-1个孩纸
				// 2.有两个孩纸
				Node* del = cur;	// 标记删除位置
				if (cur->_left == nullptr){	// 1.左孩子为空,要删除的结点只有右孩子结点
					if (parent == nullptr){	// 若要删根节点,则让其右孩子为根
						_root = cur->_right;
					}
					else{	// 如果不是删除根节点,使被删除节点的父结点指向被删除节点的右孩子结点
						if (cur == parent->_left){
							parent->_left = cur->_right;
						}
						else{
							parent->_right = cur->_right;
						}
					}
				}
				else if (cur->_right == nullptr){	// 1.右孩子为空,要删除的结点只有左孩子结点
					if (parent == nullptr){	// 若要删根节点,则让其左孩子为根
						_root = cur->_left;
					}
					else{	// 如果不是删除根节点,使被删除节点的父结点指向被删除节点的左孩子结点
						if (cur == parent->_left){
							parent->_left = cur->_left;
						}
						else{
							parent->_right = cur->_left;
						}
					}
				}
				else{	// 2.要删除的结点有左、右孩子结点
					Node* rpParent = cur;
					Node* replace = cur->_right;
					while (replace->_left){	// 在它的右子树中寻找中序下的第一个结点(关键码最小)，用它的值填补到被删除节点中，再来处理该结点的删除问题
						rpParent = replace;
						replace = replace->_left;
					}

					cur->_kv = replace->_kv;	// 取代要删的结点
					del = replace;
					if (rpParent->_left == replace){	// 如果rp有左子树
						rpParent->_left = replace->_right;	// 把re的右子树往上提
					}
					else{	// 如果rp没有左子树
						rpParent->_right = replace->_right;
					}
				}

				delete del;	// 释放要删除的结点空间

				return true;
			}
		}

		return false;
	}

	void InOrder(){
		_InOrder(_root);
		cout << endl;
	}

	void _InOrder(Node* root){
		if (root == nullptr){
			return;
		}

		_InOrder(root->_left);	// 中序遍历
		cout << root->_kv.first << " ";
		_InOrder(root->_right);
	}

private:
	Node* _root;
};

void TestBSTree(){
	BSTree<int, int> tree;
	tree.Insert(make_pair(3, 1));
	tree.Insert(make_pair(1, 1));
	tree.Insert(make_pair(4, 1));
	tree.Insert(make_pair(6, 1));
	tree.Insert(make_pair(5, 1));
	tree.Insert(make_pair(2, 1));
	tree.InOrder();
	//cout << tree.Find(5)->_kv.first << " ";

	tree.Remove(1);
	tree.Remove(3);
	tree.Remove(2);
	tree.Remove(6);
	tree.Remove(4);
	tree.Remove(1);
	tree.Remove(10);
	tree.InOrder();

}

#pragma once
#include <iostream>

using namespace std;

// ������
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
		while (cur){	// �Һ��ʵĲ���λ��
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

		cur = new Node(kv);	// �ҵĺ��ʵĲ���λ�ú���в���
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
			if (cur->_kv.first < key){	// �ҵ�Ҫɾ���Ľڵ�
				parent = cur;
				cur = cur->_right;
			}
			else if (cur->_kv.first > key){
				parent = cur;
				cur = cur->_left;
			}
			else{
				// 1.ֻ��0-1����ֽ
				// 2.��������ֽ
				Node* del = cur;	// ���ɾ��λ��
				if (cur->_left == nullptr){	// 1.����Ϊ��,Ҫɾ���Ľ��ֻ���Һ��ӽ��
					if (parent == nullptr){	// ��Ҫɾ���ڵ�,�������Һ���Ϊ��
						_root = cur->_right;
					}
					else{	// �������ɾ�����ڵ�,ʹ��ɾ���ڵ�ĸ����ָ��ɾ���ڵ���Һ��ӽ��
						if (cur == parent->_left){
							parent->_left = cur->_right;
						}
						else{
							parent->_right = cur->_right;
						}
					}
				}
				else if (cur->_right == nullptr){	// 1.�Һ���Ϊ��,Ҫɾ���Ľ��ֻ�����ӽ��
					if (parent == nullptr){	// ��Ҫɾ���ڵ�,����������Ϊ��
						_root = cur->_left;
					}
					else{	// �������ɾ�����ڵ�,ʹ��ɾ���ڵ�ĸ����ָ��ɾ���ڵ�����ӽ��
						if (cur == parent->_left){
							parent->_left = cur->_left;
						}
						else{
							parent->_right = cur->_left;
						}
					}
				}
				else{	// 2.Ҫɾ���Ľ�������Һ��ӽ��
					Node* rpParent = cur;
					Node* replace = cur->_right;
					while (replace->_left){	// ��������������Ѱ�������µĵ�һ�����(�ؼ�����С)��������ֵ�����ɾ���ڵ��У���������ý���ɾ������
						rpParent = replace;
						replace = replace->_left;
					}

					cur->_kv = replace->_kv;	// ȡ��Ҫɾ�Ľ��
					del = replace;
					if (rpParent->_left == replace){	// ���rp��������
						rpParent->_left = replace->_right;	// ��re��������������
					}
					else{	// ���rpû��������
						rpParent->_right = replace->_right;
					}
				}

				delete del;	// �ͷ�Ҫɾ���Ľ��ռ�

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

		_InOrder(root->_left);	// �������
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

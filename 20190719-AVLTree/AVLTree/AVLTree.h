#pragma once
#include <iostream>
#include <assert.h>

using namespace std;

template<class K, class V>
struct AVLTreeNode
{
	pair<K, V> _kv;
	AVLTreeNode<K, V>* _left;
	AVLTreeNode<K, V>* _right;
	AVLTreeNode<K, V>* _parent;

	int _bf; // balance factor	（平衡因子）

	AVLTreeNode(const pair<K, V>& kv)	// 构造函数
		: _kv(kv)
		, _left(nullptr)
		, _right(nullptr)
		, _parent(nullptr)
		, _bf(0)
	{}
};

template<class K, class V>
class AVLTree
{
	typedef AVLTreeNode<K, V> Node;	// 节点类重命名为Node
public:
	bool Insert(const pair<K, V>& kv){	// 插入节点
		if (_root == nullptr){	// 如果根节点为空，则直接插入到根节点
			_root = new Node(kv);
			_root->_bf = 0;	// 根节点的平衡因子设为0
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
		cur = new Node(kv);	// 此处需要新建节点
		if (parent->_kv.first < kv.first){	// 判断在左边还是右边插入
			parent->_right = cur;
			cur->_parent = parent;
		}
		else{
			parent->_left = cur;
			cur->_parent = parent;
		}


		// 插入之后，需要将树调平衡
		while (parent){
			// 1.更新平衡因子
			if (cur == parent->_right){	// 如果插入在右，父节点平衡因子++
				parent->_bf++;
			}
			else{	// 如果插入在左，父节点平衡因子--
				parent->_bf--;
			}
			// 2.根据平衡因子，做出判断，看是否需要调平衡
			if (parent->_bf == 0){	// 更新后平衡因子绝对值为0，说明高度不变，更新完成
				break;
			}
			else if (abs(parent->_bf) == 1){	// 高度变了，需继续向上更新
				cur = parent;
				parent = parent->_parent;	// 将cur和parent的指向向上挪动
			}// 若挪到最后parent指向空(说明cur指向了根节点)则会退出循环，若挪动指向的过程中平衡因子更新到2则会执行下面的旋转
			else if (abs(parent->_bf) == 2){	// 更新后平衡因子绝对值为2，说明不平衡了，立即旋转
				if (parent->_bf == 2){
					if (cur->_bf == 1){		// 父为2，右子为1，需左旋
						RotateL(parent);
					}
					else if (cur->_bf == -1){		// 右子为-1，需右左旋
						RotateRL(parent);
					}
				}
				else if (parent->_bf == -2){
					if (cur->_bf == -1){	// 父为-2，左子为-1，需右旋
						RotateR(parent);
					}
					else if (cur->_bf == 1){		// 右子为-1，需左右旋
						RotateLR(parent);
					}
				}
				break;	// 旋完则会达到平衡
			}
			else{
				assert(false);
			}
		}
		return true;
	}

	void RotateL(Node* parent){
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
		parent->_bf = subR->_bf = 0;
	}

	void RotateR(Node* parent){
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
		parent->_bf = subL->_bf = 0;
	}

	void RotateLR(Node* parent){
		RotateL(parent->_left);
		RotateR(parent);
	}

	void RotateRL(Node* parent){
		Node* subR = parent->_right;
		Node* subRL = subR->_left;
		int bf = subRL->_bf;

		RotateR(parent->_right);
		RotateL(parent);

		if (bf == 0){
			parent->_bf = subRL->_bf = subR->_bf = 0;
		}
		else if (bf == 1){
			subR->_bf = 0;
			parent->_bf = -1;
			subRL->_bf = 0;
		}
		else if (bf == -1){
			parent->_bf = 0;
			subR->_bf = 1;
			subRL->_bf = 0;
		}
	}

	void InOrder(){
		_InOrder(_root);
		cout << endl;
	}

	void _InOrder(Node* root){
		if (root == nullptr){
			return;
		}
		_InOrder(root->_left);	// 中序
		cout << root->_kv.first << "[" << root->_kv.second << "]" << " ";
		_InOrder(root->_right);
	}

	// 验证是否平衡
	int _Height(Node* root){
		if (root == nullptr){
			return 0;
		}

		int leftheight = _Height(root->_left);
		int rightheight = _Height(root->_right);

		return leftheight > rightheight ? leftheight + 1 : rightheight + 1;
	}

	bool IsBalance(){
		return _IsBalance(_root);
	}

	bool _IsBalance(Node* root){
		if (root == nullptr){
			return true;
		}
		int leftheight = _Height(root->_left);
		int rightheight = _Height(root->_right);
		if (rightheight - leftheight != root->_bf){
			cout << root->_kv.first << "平衡因子异常" << endl;
			return false;
		}
		return abs(leftheight - rightheight) < 2 && _IsBalance(root->_left) && _IsBalance(root->_right);
	}

private:
	Node* _root = nullptr;
};

void TestAVLTree(){
	AVLTree<int, int> t;
	//int a[] = { 16, 3, 7, 11, 9, 26, 18, 14, 15 };
	int a[] = { 4, 2, 6, 1, 3, 5, 15, 7, 16, 14, };
	for (auto e : a){
		t.Insert(make_pair(e, e));
	}
	t.InOrder();
	cout << t.IsBalance() << endl;
}

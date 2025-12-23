#include "binnode.h"
#include"Queue.h"
#define IsRoot(x) (!((x).parent))
#define IsLChild(x) (!IsRoot(x) && ((x).parent->lc == &(x)))
#define IsRChild(x) (!IsRoot(x) && ((x).parent->rc == &(x)))
#define FromParentTo(x) (IsRoot(x) ? _root : (IsLChild(x) ? (x).parent->lc : (x).parent->rc))
using namespace std;
template<typename T> class BinTree {
protected:
	int _size;// 规模
	BinNodePosi(T) _root;// 根节点
	virtual int updateHeight(BinNodePosi(T) x);// 更新节点x的高度
	void updateHeightAbove(BinNodePosi(T) x);// 更新节点x及其祖先的高度
public:
	BinTree() : _size(0), _root(NULL) {}// 构造函数
	~BinTree() { if (0 < _size) remove(_root); }// 析构函数
	int size() const { return _size; }// 规模
	BinNodePosi(T) root() const { return _root; }// 根节点
	BinNodePosi(T) insertAsRoot(T const& e);// 插入根节点
	BinNodePosi(T) insertAsLC(BinNodePosi(T) x, T const& e);// 插入左孩子
	BinNodePosi(T) insertAsRC(BinNodePosi(T) x, T const& e);// 插入右孩子
	int remove(BinNodePosi(T) x);// 删除以位置x处节点为根的子树，返回该子树原先的规模
	BinTree<T>* secede(BinNodePosi(T) x);// 将子树x从当前树中摘除，并将其转换为一棵独立子树
	template <typename VST> void travLevel(VST& visit) { if (_root) _root->travLevel(visit); }// 层次遍历
	template <typename VST> void travPre(VST& visit) { if (_root) _root->travPre(visit); }// 先序遍历
	template <typename VST> void travIn(VST& visit) { if (_root) _root->travIn(visit); }// 中序遍历
	template <typename VST> void travPost(VST& visit) { if (_root) _root->travPost(visit); }// 后序遍历
	bool operator<(BinTree<T> const& t) { return _root && t._root && lt(_root, t._root); }// 比较器
	bool operator==(BinTree<T> const& t) { return _root && t._root && (_root == t._root); }// 判等器
	BinNodePosi(T) attachAsLC(BinNodePosi(T) x, BinTree<T>*& S);// 左子树接入
	BinNodePosi(T) attachAsRC(BinNodePosi(T) x, BinTree<T>*& S);// 右子树接入
};
template <typename T> int BinTree<T>::updateHeight(BinNodePosi(T) x) {
	return x->height = 1 + max(stature(x->lc), stature(x->rc));
}
template <typename T> void BinTree<T>::updateHeightAbove(BinNodePosi(T) x) {
	while (x) {
		updateHeight(x);
		x = x->parent;
	}
}
//节点插入
template <typename T> BinNodePosi(T) BinTree<T>::insertAsRoot(T const& e) {
	_size = 1;
	return _root = new BinNode<T>(e);
}// 插入根节点
template <typename T> BinNodePosi(T) BinTree<T>::insertAsLC(BinNodePosi(T) x, T const& e) {
	_size++;
	x->insertAsLC(e);
	updateHeightAbove(x);
	return x->lc;
}// 插入左孩子
template <typename T> BinNodePosi(T) BinTree<T>::insertAsRC(BinNodePosi(T) x, T const& e) {
	_size++;
	x->insertAsRC(e);
	updateHeightAbove(x);
	return x->rc;
}// 插入右孩子
//子树接入
template <typename T> BinNodePosi(T) BinTree<T>::attachAsLC(BinNodePosi(T) x, BinTree<T>* &S) {
	if (x->lc = S->_root) x->lc->parent = x;
	_size += S->_size;
	updateHeightAbove(x);
	S->_root = nullptr;
	S->_size = 0;
	delete S;
	S = NULL;
	return x;
}// 左子树接入
template <typename T> BinNodePosi(T) BinTree<T>::attachAsRC(BinNodePosi(T) x, BinTree<T>* &S) {
	if (x->rc = S->_root) x->rc->parent = x;
	_size += S->_size;
	updateHeightAbove(x);
	S->_root = nullptr;
	S->_size = 0;
	delete S;
	S = NULL;
	return x;
}// 右子树接入
//子树删除
template <typename T> int BinTree<T>::remove(BinNodePosi(T) x) {
	FromParentTo(*x) = NULL;
	updateHeightAbove(x->parent);
	int n = removeAt(x);
	_size -= n;
	return n;
}
template <typename T> 
static int removeAt(BinNodePosi(T) x) {
	if (!x) return 0;
	int n = 1 + removeAt(x->lc) + removeAt(x->rc);
	delete x;
	return n;
}
template <typename T> BinTree<T>* BinTree<T>::secede(BinNodePosi(T) x) {
	assert(x);
	BinTree<T>* S = new BinTree<T>;
	if (x == _root) {
		_root = NULL;
	} else {
		if (IsLChild(*x)) {
			x->parent->lc = NULL;
		} else {
			x->parent->rc = NULL;
		}
		updateHeightAbove(x->parent);
	}
	x->parent = NULL;
	S->_root = x;
	S->_size = x->size();
	_size -= S->_size;
	return S;
}// 将子树x从当前树中摘除，并将其转换为一棵独立子树
//先序遍历
template <typename T, typename VST>
void travPre_R(BinNodePosi(T) x, VST& visit) {
	if (!x) return;
	visit(x->data);
	travPre_R(x->lc, visit);
	travPre_R(x->rc, visit);
}
//中序遍历
template <typename T, typename VST>
void travIn_R(BinNodePosi(T) x, VST& visit) {
	if (!x) return;
	travIn_R(x->lc, visit);
	visit(x->data);
	travIn_R(x->rc, visit);
}
//后序遍历
template <typename T, typename VST>
void travPost_R(BinNodePosi(T) x, VST& visit) {
	if (!x) return;
	travPost_R(x->lc, visit);
	travPost_R(x->rc, visit);
	visit(x->data);
}
//层次遍历
template <typename T, typename VST>
void travLevel(BinNodePosi(T) x, VST& visit) {
	Queue<BinNodePosi(T)> Q;
	Q.enqueue(x);
	while (!Q.empty()) {
		BinNodePosi(T) x = Q.dequeue();
		visit(x->data);
		if (x->lc) Q.enqueue(x->lc);
		if (x->rc) Q.enqueue(x->rc);
	}
}


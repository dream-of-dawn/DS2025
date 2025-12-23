#include<iostream>
#define BinNodePosi(T) BinNode<T>*
#define stature(p) ((p) ? (p)->height : -1)
typedef enum { RB_RED, RB_BLACK } RBColor;
template <typename T> struct BinNode {
	T data;
	BinNodePosi(T) parent; BinNodePosi(T) lc; BinNodePosi(T) rc;
	int height;
	int npl; // null path length, (左式堆，也可用height替代)
	RBColor color; // （颜色，红黑树）
	BinNode(T e, BinNodePosi(T) p = NULL, BinNodePosi(T) lc = NULL, BinNodePosi(T) rc = NULL,
			int h = 0, int l = 1, RBColor c = RB_RED)
		: data(e), parent(p), lc(lc), rc(rc), height(h), npl(l), color(c) {}
	int size(); // 统计当前节点后代总数，即以该节点为根的子树节点总数
	BinNodePosi(T) insertAsLC(T const&); // 作为当前节点的左孩子插入新节点
	BinNodePosi(T) insertAsRC(T const&); // 作为当前节点的右孩子插入新节点
	BinNodePosi(T) succ(); // 取消当前节点的直接后继节点
	template <typename VST> void travLevel(VST&); // 子树层次遍历
	template <typename VST> void travPre(VST&); // 子树先序遍历
	template <typename VST> void travIn(VST&); // 子树中序遍历
	template <typename VST> void travPost(VST&); // 子树后序遍历
	//比较器、判等器
	bool operator<(BinNode const& bn) { return data < bn.data; }
	bool operator==(BinNode const& bn) { return data == bn.data; }
};
template <typename T> BinNodePosi(T) BinNode<T>::insertAsLC(T const& e) {
	return lc = new BinNode(e, this);
}// 作为当前节点的左孩子插入新节点
template <typename T> BinNodePosi(T) BinNode<T>::insertAsRC(T const& e) {
	return rc = new BinNode(e, this);
}// 作为当前节点的右孩子插入新节点
template <typename T> BinNodePosi(T) BinNode<T>::succ() {
	BinNodePosi(T) s = this;
	if (rc) {
		s = rc;
		while (s->lc) s = s->lc;
	} else {
		while (IsRChild(*s)) s = s->parent;
		s = s->parent;
	}
	return s;
} // 取消当前节点的直接后继节点
template <typename T> int BinNode<T>::size() {
	int s = 1;
	if (lc) s += lc->size();
	if (rc) s += rc->size();
	return s;
} // 统计当前节点后代总数，即以该节点为根的子树节点总数
template <typename T>
void release(T* ptr) {
	if (ptr) {
		delete ptr;
	}
}

// 对于非指针类型，什么也不做（因为可能是栈对象）
template <typename T>
void release(T& obj) {
	// 对于非指针的栈对象，不需要特殊处理
	// 让它们自然析构即可
}

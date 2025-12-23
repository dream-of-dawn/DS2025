#include <cstddef>
typedef int Rank; //秩
#define ListNodePosi(T) ListNode<T>* //列表节点位置
template <typename T> struct ListNode { //列表节点模板类
	T data; //数值
	ListNodePosi(T) pred; //前驱
	ListNodePosi(T) succ; //后继
//构造函数
	ListNode() {} //针对header和trailer的构造
	ListNode(T e, ListNodePosi(T) p = NULL, ListNodePosi(T) s = NULL)
		: data(e), pred(p), succ(s) {} //默认构造器
	ListNodePosi(T) insertAsPred(T const& e); //插入作为当前节点的前驱
	ListNodePosi(T)  insertAsSucc(T const& e); //插入作为当前节点的后继
};

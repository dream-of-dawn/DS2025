#include <cstddef>
typedef int Rank; //��
#define ListNodePosi(T) ListNode<T>* //�б�ڵ�λ��
template <typename T> struct ListNode { //�б�ڵ�ģ����
	T data; //��ֵ
	ListNodePosi(T) pred; //ǰ��
	ListNodePosi(T) succ; //���
//���캯��
	ListNode() {} //���header��trailer�Ĺ���
	ListNode(T e, ListNodePosi(T) p = NULL, ListNodePosi(T) s = NULL)
		: data(e), pred(p), succ(s) {} //Ĭ�Ϲ�����
	ListNodePosi(T) insertAsPred(T const& e); //������Ϊ��ǰ�ڵ��ǰ��
	ListNodePosi(T)  insertAsSucc(T const& e); //������Ϊ��ǰ�ڵ�ĺ��
};

#include "listNode.h"
#include <iostream>
template <typename T> class List { //�б�ģ����
private:
	int _size; ListNodePosi(T) header; ListNodePosi(T) trailer; //��ģ��ͷ�ڱ���β�ڱ�
protected:
	void init(); //�б���ʱ�ĳ�ʼ��
	int clear(); //������нڵ�
	void copyNodes(ListNodePosi(T) p, int n); //�����б�����λ��p���n��
	void merge(ListNodePosi(T)& p, int n, List<T>& L, ListNodePosi(T) q, int m); //�鲢
	void mergeSort(ListNodePosi(T)& p, int n); //�Դ�p��ʼ������n���ڵ�鲢����
	void selectionSort(ListNodePosi(T) p, int n); //�Դ�p��ʼ������n���ڵ�ѡ������
	void insertionSort(ListNodePosi(T) p, int n); //�Դ�p��ʼ������n���ڵ��������
public:
	List() { init(); } //Ĭ�Ϲ��캯��
	List(List<T> const& L) { copyNodes(L.first(), L._size); } //���ƹ��캯��
	List(List<T> const& L, Rank r, int n) { copyNodes(L[r], n); }//�����б�L���Ե�r�����n��
	List(ListNodePosi(T) p, int n) { copyNodes(p, n); } //�����б�����λ��p���n��
	//��������
	~List();
	//ֻ�����ʽӿ�
	int size() const { return _size; } //��ģ
	bool empty() const { return _size <= 0; } //�п�
	ListNodePosi(T) first() const { return header->succ; } //�׽ڵ�λ��
	ListNodePosi(T) last() const { return trailer->pred; } //ĩ�ڵ�λ��
	bool valid(ListNodePosi(T) p) //�ж�λ��p�Ƿ����Ϸ�
	{
		return p && (trailer != p) && (header != p);
	}
	int disordered() const; //�ж��Ƿ�������
	T& operator[](Rank r) const; //�����±�����������ȷ��ʽڵ㣨Ч�ʵͣ�
	ListNodePosi(T) find(T const& e) const //�����б����
	{
		return find(e, _size, trailer);
	}
	ListNodePosi(T) find(T const& e, int n, ListNodePosi(T) p) const; //�����б��������
	ListNodePosi(T) search(T const& e) const //�����б����
	{
		return search(e, _size, trailer);
	}
	ListNodePosi(T) search(T const& e, int n, ListNodePosi(T) p) const; //�����б��������
	ListNodePosi(T) selectMax(ListNodePosi(T) p, int n); //��p�����n-1���ڵ���ѡ�������
	ListNodePosi(T) selectMax() { return selectMax(header->succ, _size); } //���������
	//��д���ʽӿ�
	ListNodePosi(T) insertAsFirst(T const& e); //��e�����׽ڵ����
	ListNodePosi(T) insertAsLast(T const& e); //��e����ĩ�ڵ����
	ListNodePosi(T) insertA(ListNodePosi(T) p, T const& e); //��e����p�ĺ�̲���
	ListNodePosi(T) insertB(ListNodePosi(T) p, T const& e); //��e����p��ǰ������
	T remove(ListNodePosi(T) p); //ɾ���Ϸ�λ��p���Ľڵ㣬��������ֵ
	void merge(List<T>& L) { merge(first(), _size, L, L.first(), L._size); } //ȫ�б�鲢
	void sort(ListNodePosi(T) p, int n); //�б���������
	void sort() { sort(first(), _size); } //�б���������
	int deduplicate(); //����ȥ��
	int uniquify(); //����ȥ��
	void reverse(); //ǰ����
	//����
	void traverse(void(*)(T&)); //����������ʵʩvisit������ʹ�ú���ָ�룬ֻ����ֲ��޸ģ�
	template<typename VST> void traverse(VST&); //����������ʵʩvisit������ʹ�ú������󣬿�ȫ���޸ģ�
};
//�б�ڵ���
template<typename T> int List<T>::clear() {
	int oldSize = _size; //��¼ԭ��ģ
	while (0 < _size) remove(header->succ); //����ɾ���׽ڵ㣬ֱ���б��Ϊֹ
	return oldSize; //����ɾ���ڵ�����
};
//�б��ʼ��
template<typename T> void List<T>::init() {
	header = new ListNode<T>; //����ͷ��β�ڱ��ڵ�
	trailer = new ListNode<T>;
	header->pred = NULL; header->succ = trailer; //ͷ�ڱ��ڵ������ϵ
	trailer->pred = header; trailer->succ = NULL; //β�ڱ��ڵ������ϵ
	_size = 0; //��¼��ģ
};
//��������
template<typename T> List<T>::~List() {
	clear(); //����б�
	delete header; //�ͷ�ͷ��β�ڱ��ڵ�
	delete trailer;
};
//�����±������
template<typename T> T& List<T>::operator[](Rank r) const {
	ListNodePosi(T) p = first(); //���׽ڵ����
	while (0 < r--) p = p->succ; //˳����r���ڵ�
	return p->data; //���ظýڵ������
};
//�����б��������
template<typename T> ListNodePosi(T) List<T>::find(T const& e, int n, ListNodePosi(T) p) const {
	while (0 < n--) { //�Ժ���ǰ����ڵ����
		if (e == (p = p->pred)->data) return p; //�������򷵻ؽڵ�λ��
	}
	return NULL; //���򷵻�NULL
};
//�б�ڵ����ӿ�
template<typename T> ListNodePosi(T) List<T>::insertAsFirst(T const& e) {
	_size++; return header->insertAsSucc(e); //������Ϊ�׽ڵ�
};
template<typename T> ListNodePosi(T) List<T>::insertAsLast(T const& e) {
	_size++; return trailer->insertAsPred(e); //������Ϊĩ�ڵ�
};
template<typename T> ListNodePosi(T) List<T>::insertA(ListNodePosi(T) p, T const& e) {
	_size++; return p->insertAsSucc(e); //������Ϊp�ĺ��
};
template<typename T> ListNodePosi(T) List<T>::insertB(ListNodePosi(T) p, T const& e) {
	_size++; return p->insertAsPred(e); //������Ϊp��ǰ��
};
//insertAspred()�㷨
template<typename T> ListNodePosi(T) ListNode<T>::insertAsPred(T const& e) {
	ListNodePosi(T) x = new ListNode(e, pred, this); //�����½ڵ�
	pred->succ = x; pred = x; //���ô�����ϵ
	return x; //�����½ڵ�λ��
};
//insertAsSucc()�㷨
template<typename T> ListNodePosi(T) ListNode<T>::insertAsSucc(T const& e) {
	ListNodePosi(T) x = new ListNode(e, this, succ); //�����½ڵ�
	succ->pred = x; succ = x; //���ô�����ϵ
	return x; //�����½ڵ�λ��
};
//ɾ���ڵ�
template<typename T> T List<T>::remove(ListNodePosi(T) p) {
	T e = p->data; //���ݴ�ɾ���ڵ������
	p->pred->succ = p->succ; p->succ->pred = p->pred; //����ǰ���ͺ�̽ڵ��ָ��
	delete p; _size--; //�ͷŽڵ㣬���¹�ģ
	return e; //���ر�ɾ���ڵ������
};
//��������Ψһ��
template<typename T> int List<T>::deduplicate() {
	if (_size < 2) return 0; //ƽ�����
	int oldSize = _size; //��¼ԭ��ģ
	ListNodePosi(T) p = first(); ListNodePosi(T) q; Rank r = 1; //pΪ�������׽ڵ㣬qΪ���̣�rΪp����
	while (trailer != (q = p->succ)) //��p��δ�ִ�ĩ�ڵ�ʱ
		if (find(q->data, r, p) < 0) { p = q; r++; } //����p֮ǰ�Ҳ�����q��ͬ�ߣ���ת����һ�ڵ�
		else remove(q); //����ɾ���ڵ�q��ע�⣺q�漴ת����һ�ڵ㣩
	return oldSize - _size; //����ɾ���ڵ�����
};
//��������Ψһ��
template<typename T> int List<T>::uniquify() {
	if (_size < 2) return 0; //ƽ�����
	int oldSize = _size; //��¼ԭ��ģ
	ListNodePosi(T) p = first(); ListNodePosi(T) q; //pΪ�������׽ڵ㣬qΪ����
	while (trailer != (q = p->succ)) //��p��δ�ִ�ĩ�ڵ�ʱ
		if (p->data != q->data) p = q; //�����죬��ת����һ�ڵ�
		else remove(q); //����ɾ�����ߣ�ע�⣺q�漴ת����һ�ڵ�
	return oldSize - _size; //����ɾ���ڵ�����
};
//����
template<typename T> void List<T>::traverse(void(*visit)(T&)) {
	for (ListNodePosi(T) p = header->succ; p != trailer; p = p->succ)
		visit(p->data);
};
template<typename T> template<typename VST> void List<T>::traverse(VST& visit) {
	for (ListNodePosi(T) p = header->succ; p != trailer; p = p->succ)
		visit(p->data);
};
//�����б���ҽӿ�search()
template<typename T> ListNodePosi(T) List<T>::search(T const& e, int n, ListNodePosi(T) p) const {
	while (0 <= n--) {
		if (((p = p->pred)->data) <= e)break;
	}
	return p;
};
//�����췽��
template<typename T> void List<T>::sort(ListNodePosi(T) p, int n) {
	switch (rand() % 3) {
	case 1: insertionSort(p, n); break; //��������
	case 2: selectionSort(p, n); break; //ѡ������
	default: mergeSort(p, n); break; //�鲢����
	}
};
//��������
template<typename T> void List<T>::insertionSort(ListNodePosi(T) p, int n) {
	for (int r = 0; r < n; r++) {
		insertA(search(p->data, r, p), p->data);
		p = p->succ;
		remove(p->pred);
	}
};
//ѡ������
template<typename T> void List<T>::selectionSort(ListNodePosi(T) p, int n) {
	ListNodePosi(T) head = p->pred; ListNodePosi(T) tail = p;
	for (int i = 0; i < n; i++) tail = tail->succ;
	while (1 < n--) {
		insertB(tail, remove(selectMax(head->succ, n)));
		tail = tail->pred;
	}
};
//ѡȡ���ڵ�
template<typename T> ListNodePosi(T) List<T>::selectMax(ListNodePosi(T) p, int n) {
	ListNodePosi(T) max = p;
	for (ListNodePosi(T) cur = p; 1 < n; n--) {
		if ((cur = cur->succ)->data >= max->data) max = cur;
	}
	return max;
};
//�鲢�㷨
template<typename T>
void List<T>::merge(ListNodePosi(T)& p, int n, List<T>& L, ListNodePosi(T) q, int m) {
	ListNodePosi(T) pp = p->pred; //p��ǰ��
	while (0 < m) //�ں����б���δ�������֮ǰ
		if ((0 < n) && (p->data <= q->data)) { //��ǰ���б���δ��������ҵ�ǰԪ�ؽ�С
			if (q == (p = p->succ)) break; //��ת����һǰ���б�Ԫ��
			n--;
		}
		else { //����
			insertB(p, L.remove((q = q->succ)->pred)); //�������б�ǰԪ������ǰ���б���p֮ǰ
			m--;
		}
	p = pp->succ; //���ع鲢�����б����ʼλ��
};
//�����б�Ĺ鲢����
template<typename T>
void List<T>::mergeSort(ListNodePosi(T)& p, int n) {
	if (n < 2) return; //���ڵ���Ȼ���򣬷���...
	int m = n >> 1; //���е�Ϊ��
	ListNodePosi(T) q = p; //��p����
	for (int i = 0; i < m; i++) q = q->succ; //��λ���е�
	mergeSort(p, m); mergeSort(q, n - m); //��ǰ�����б�ֱ�����
	merge(p, m, *this, q, n - m); //�鲢
};


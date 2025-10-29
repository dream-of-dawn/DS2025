#include "listNode.h"
#include <iostream>
template <typename T> class List { //列表模板类
private:
	int _size; ListNodePosi(T) header; ListNodePosi(T) trailer; //规模、头哨兵、尾哨兵
protected:
	void init(); //列表创建时的初始化
	int clear(); //清除所有节点
	void copyNodes(ListNodePosi(T) p, int n); //复制列表中自位置p起的n项
	void merge(ListNodePosi(T)& p, int n, List<T>& L, ListNodePosi(T) q, int m); //归并
	void mergeSort(ListNodePosi(T)& p, int n); //对从p开始连续的n个节点归并排序
	void selectionSort(ListNodePosi(T) p, int n); //对从p开始连续的n个节点选择排序
	void insertionSort(ListNodePosi(T) p, int n); //对从p开始连续的n个节点插入排序
public:
	List() { init(); } //默认构造函数
	List(List<T> const& L) { copyNodes(L.first(), L._size); } //复制构造函数
	List(List<T> const& L, Rank r, int n) { copyNodes(L[r], n); }//复制列表L中自第r项起的n项
	List(ListNodePosi(T) p, int n) { copyNodes(p, n); } //复制列表中自位置p起的n项
	//析构函数
	~List();
	//只读访问接口
	int size() const { return _size; } //规模
	bool empty() const { return _size <= 0; } //判空
	ListNodePosi(T) first() const { return header->succ; } //首节点位置
	ListNodePosi(T) last() const { return trailer->pred; } //末节点位置
	bool valid(ListNodePosi(T) p) //判断位置p是否对外合法
	{
		return p && (trailer != p) && (header != p);
	}
	int disordered() const; //判断是否已排序
	T& operator[](Rank r) const; //重载下标操作符，按秩访问节点（效率低）
	ListNodePosi(T) find(T const& e) const //无序列表查找
	{
		return find(e, _size, trailer);
	}
	ListNodePosi(T) find(T const& e, int n, ListNodePosi(T) p) const; //无序列表区间查找
	ListNodePosi(T) search(T const& e) const //有序列表查找
	{
		return search(e, _size, trailer);
	}
	ListNodePosi(T) search(T const& e, int n, ListNodePosi(T) p) const; //有序列表区间查找
	ListNodePosi(T) selectMax(ListNodePosi(T) p, int n); //在p及其后n-1个节点中选出最大者
	ListNodePosi(T) selectMax() { return selectMax(header->succ, _size); } //整体最大者
	//可写访问接口
	ListNodePosi(T) insertAsFirst(T const& e); //将e当作首节点插入
	ListNodePosi(T) insertAsLast(T const& e); //将e当作末节点插入
	ListNodePosi(T) insertA(ListNodePosi(T) p, T const& e); //将e当作p的后继插入
	ListNodePosi(T) insertB(ListNodePosi(T) p, T const& e); //将e当作p的前驱插入
	T remove(ListNodePosi(T) p); //删除合法位置p处的节点，返回其数值
	void merge(List<T>& L) { merge(first(), _size, L, L.first(), L._size); } //全列表归并
	void sort(ListNodePosi(T) p, int n); //列表区间排序
	void sort() { sort(first(), _size); } //列表整体排序
	int deduplicate(); //无序去重
	int uniquify(); //有序去重
	void reverse(); //前后倒置
	//遍历
	void traverse(void(*)(T&)); //遍历，依次实施visit操作（使用函数指针，只读或局部修改）
	template<typename VST> void traverse(VST&); //遍历，依次实施visit操作（使用函数对象，可全局修改）
};
//列表节点数
template<typename T> int List<T>::clear() {
	int oldSize = _size; //记录原规模
	while (0 < _size) remove(header->succ); //反复删除首节点，直至列表空为止
	return oldSize; //返回删除节点总数
};
//列表初始化
template<typename T> void List<T>::init() {
	header = new ListNode<T>; //创建头、尾哨兵节点
	trailer = new ListNode<T>;
	header->pred = NULL; header->succ = trailer; //头哨兵节点从属关系
	trailer->pred = header; trailer->succ = NULL; //尾哨兵节点从属关系
	_size = 0; //记录规模
};
//析构函数
template<typename T> List<T>::~List() {
	clear(); //清空列表
	delete header; //释放头、尾哨兵节点
	delete trailer;
};
//重置下标操作符
template<typename T> T& List<T>::operator[](Rank r) const {
	ListNodePosi(T) p = first(); //从首节点出发
	while (0 < r--) p = p->succ; //顺数第r个节点
	return p->data; //返回该节点的数据
};
//无序列表区间查找
template<typename T> ListNodePosi(T) List<T>::find(T const& e, int n, ListNodePosi(T) p) const {
	while (0 < n--) { //自后向前，逐节点查找
		if (e == (p = p->pred)->data) return p; //若命中则返回节点位置
	}
	return NULL; //否则返回NULL
};
//列表节点插入接口
template<typename T> ListNodePosi(T) List<T>::insertAsFirst(T const& e) {
	_size++; return header->insertAsSucc(e); //插入作为首节点
};
template<typename T> ListNodePosi(T) List<T>::insertAsLast(T const& e) {
	_size++; return trailer->insertAsPred(e); //插入作为末节点
};
template<typename T> ListNodePosi(T) List<T>::insertA(ListNodePosi(T) p, T const& e) {
	_size++; return p->insertAsSucc(e); //插入作为p的后继
};
template<typename T> ListNodePosi(T) List<T>::insertB(ListNodePosi(T) p, T const& e) {
	_size++; return p->insertAsPred(e); //插入作为p的前驱
};
//insertAspred()算法
template<typename T> ListNodePosi(T) ListNode<T>::insertAsPred(T const& e) {
	ListNodePosi(T) x = new ListNode(e, pred, this); //创建新节点
	pred->succ = x; pred = x; //设置从属关系
	return x; //返回新节点位置
};
//insertAsSucc()算法
template<typename T> ListNodePosi(T) ListNode<T>::insertAsSucc(T const& e) {
	ListNodePosi(T) x = new ListNode(e, this, succ); //创建新节点
	succ->pred = x; succ = x; //设置从属关系
	return x; //返回新节点位置
};
//删除节点
template<typename T> T List<T>::remove(ListNodePosi(T) p) {
	T e = p->data; //备份待删除节点的数据
	p->pred->succ = p->succ; p->succ->pred = p->pred; //调整前驱和后继节点的指针
	delete p; _size--; //释放节点，更新规模
	return e; //返回被删除节点的数据
};
//无序序列唯一化
template<typename T> int List<T>::deduplicate() {
	if (_size < 2) return 0; //平凡情况
	int oldSize = _size; //记录原规模
	ListNodePosi(T) p = first(); ListNodePosi(T) q; Rank r = 1; //p为各区段首节点，q为其后继，r为p的秩
	while (trailer != (q = p->succ)) //在p尚未抵达末节点时
		if (find(q->data, r, p) < 0) { p = q; r++; } //若在p之前找不到与q相同者，则转向下一节点
		else remove(q); //否则删除节点q（注意：q随即转向下一节点）
	return oldSize - _size; //返回删除节点总数
};
//有序序列唯一化
template<typename T> int List<T>::uniquify() {
	if (_size < 2) return 0; //平凡情况
	int oldSize = _size; //记录原规模
	ListNodePosi(T) p = first(); ListNodePosi(T) q; //p为各区段首节点，q为其后继
	while (trailer != (q = p->succ)) //在p尚未抵达末节点时
		if (p->data != q->data) p = q; //若互异，则转向下一节点
		else remove(q); //否则删除后者（注意：q随即转向下一节点
	return oldSize - _size; //返回删除节点总数
};
//遍历
template<typename T> void List<T>::traverse(void(*visit)(T&)) {
	for (ListNodePosi(T) p = header->succ; p != trailer; p = p->succ)
		visit(p->data);
};
template<typename T> template<typename VST> void List<T>::traverse(VST& visit) {
	for (ListNodePosi(T) p = header->succ; p != trailer; p = p->succ)
		visit(p->data);
};
//有序列表查找接口search()
template<typename T> ListNodePosi(T) List<T>::search(T const& e, int n, ListNodePosi(T) p) const {
	while (0 <= n--) {
		if (((p = p->pred)->data) <= e)break;
	}
	return p;
};
//排序构造方法
template<typename T> void List<T>::sort(ListNodePosi(T) p, int n) {
	switch (rand() % 3) {
	case 1: insertionSort(p, n); break; //插入排序
	case 2: selectionSort(p, n); break; //选择排序
	default: mergeSort(p, n); break; //归并排序
	}
};
//插入排序
template<typename T> void List<T>::insertionSort(ListNodePosi(T) p, int n) {
	for (int r = 0; r < n; r++) {
		insertA(search(p->data, r, p), p->data);
		p = p->succ;
		remove(p->pred);
	}
};
//选择排序
template<typename T> void List<T>::selectionSort(ListNodePosi(T) p, int n) {
	ListNodePosi(T) head = p->pred; ListNodePosi(T) tail = p;
	for (int i = 0; i < n; i++) tail = tail->succ;
	while (1 < n--) {
		insertB(tail, remove(selectMax(head->succ, n)));
		tail = tail->pred;
	}
};
//选取最大节点
template<typename T> ListNodePosi(T) List<T>::selectMax(ListNodePosi(T) p, int n) {
	ListNodePosi(T) max = p;
	for (ListNodePosi(T) cur = p; 1 < n; n--) {
		if ((cur = cur->succ)->data >= max->data) max = cur;
	}
	return max;
};
//归并算法
template<typename T>
void List<T>::merge(ListNodePosi(T)& p, int n, List<T>& L, ListNodePosi(T) q, int m) {
	ListNodePosi(T) pp = p->pred; //p的前驱
	while (0 < m) //在后子列表尚未处理完毕之前
		if ((0 < n) && (p->data <= q->data)) { //若前子列表尚未处理完毕且当前元素较小
			if (q == (p = p->succ)) break; //则转向下一前子列表元素
			n--;
		}
		else { //否则
			insertB(p, L.remove((q = q->succ)->pred)); //将后子列表当前元素移至前子列表中p之前
			m--;
		}
	p = pp->succ; //返回归并后子列表的起始位置
};
//有序列表的归并排序
template<typename T>
void List<T>::mergeSort(ListNodePosi(T)& p, int n) {
	if (n < 2) return; //单节点自然有序，否则...
	int m = n >> 1; //以中点为界
	ListNodePosi(T) q = p; //从p出发
	for (int i = 0; i < m; i++) q = q->succ; //定位至中点
	mergeSort(p, m); mergeSort(q, n - m); //对前后子列表分别排序
	merge(p, m, *this, q, n - m); //归并
};



#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <algorithm> // 提供 swap 等标准算法
using namespace std;

// 向量（Vector）模板类：实现动态数组功能，支持元素的增删改查、排序、去重等操作
// 模板参数 T：向量中元素的类型
typedef int Rank; // 定义索引类型（本质为int，用于统一表示元素位置）
#define DEFAULT_CAPACITY 3 // 默认初始容量（最小容量）

template <typename T> class Vector { // 向量模板类
protected:
	Rank _size;      // 当前元素个数（向量规模）
	int _capacity;   // 当前可容纳的最大元素数（容量）
	T* _elem;        // 动态数组指针，存储元素数据

	// 从数组A的[lo, hi)区间复制元素到当前向量
	// 参数：A-源数组；lo-起始索引（包含）；hi-结束索引（不包含）
	void copyFrom(T const* A, Rank lo, Rank hi);

	// 扩容操作：当元素个数达到容量时，将容量翻倍以容纳更多元素
	void expand();

	// 缩容操作：当元素个数远小于容量时，将容量减半以节省内存
	void shrink();



	// 查找区间最大值：返回[lo, hi)区间中最大元素的索引
	Rank max(Rank lo, Rank hi);

	// 选择排序：对[lo, hi)区间元素进行选择排序（不稳定排序）
	void selectionSort(Rank lo, Rank hi);



	// 快速排序的划分操作（未实现）
	// Rank partition(Rank lo, Rank hi);
	// 快速排序（未实现）
	// void quickSort(Rank lo, Rank hi);
	// 堆排序（未实现）
	// void heapSort(Rank lo, Rank hi);

public:
	// 构造函数：初始化容量为c，规模为s，所有元素初始值为v
	// 参数：c-初始容量；s-初始元素个数；v-初始元素值（默认为0）
	// 注意：若s > c，仍按容量c分配空间（实际元素个数为s，可能触发后续扩容）
	Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0)
	{
		_elem = new T[_capacity = c]; // 分配容量为c的空间
		for (_size = 0; _size < s; _elem[_size++] = v); // 初始化s个元素为v
	}

	// 构造函数：从数组A的前n个元素构造向量（复制A[0, n)）
	Vector(T const* A, Rank n) { copyFrom(A, 0, n); }

	// 构造函数：从数组A的[lo, hi)区间构造向量
	Vector(T const* A, Rank lo, Rank hi) { copyFrom(A, lo, hi); }

	// 复制构造函数：从另一个向量V复制所有元素构造当前向量（复制V[0, V.size())）
	Vector(Vector<T> const& V) { copyFrom(V._elem, 0, V._size); }

	// 复制构造函数：从另一个向量V的[lo, hi)区间构造当前向量
	Vector(Vector<T> const& V, Rank lo, Rank hi) { copyFrom(V._elem, lo, hi); }

	// 析构函数：释放动态分配的元素数组内存
	~Vector() { delete[] _elem; }

	// 只读接口
	Rank size() const { return _size; } // 返回当前向量的元素个数（规模）
	bool empty() const { return !_size; } // 判断向量是否为空（规模为0）

	// 统计逆序相邻元素对数量：用于判断向量是否有序
	// 返回值：逆序对的数量（0表示向量已按非降序排列）
	int disordered() const;

	// 无序查找（全区间）：在整个向量中查找元素e
	// 返回值：找到则返回最后一次出现的索引；未找到返回-1（或小于0的值）
	Rank find(T const& e) const { return find(e, 0, _size); }

	// 无序查找（区间版）：在[lo, hi)区间内查找元素e（从右向左扫描）
	// 参数：e-目标元素；lo-起始索引（包含）；hi-结束索引（不包含）
	// 返回值：找到则返回最后一次出现的索引；未找到返回小于lo的值
	Rank find(T const& e, Rank lo, Rank hi) const;

	// 有序查找（全区间）：在整个有序向量中查找元素e（需先排序）
	// 返回值：找到则返回索引；未找到返回-1
	Rank search(T const& e) const
	{
		return (0 >= _size) ? -1 : search(e, 0, _size);
	}

	// 有序查找（区间版）：在有序区间[lo, hi)内二分查找元素e
	// 参数：e-目标元素；lo-起始索引（包含）；hi-结束索引（不包含）
	// 前提：区间[lo, hi)已按非降序排列
	Rank search(T const& e, Rank lo, Rank hi) const;

	// 重载下标运算符：支持通过索引访问元素（可读写）
	// 参数：r-元素索引（需满足0 <= r < _size，否则越界）
	T& operator[](Rank r);

	// 可写接口
	// 赋值运算符：将另一个向量V赋值给当前向量（深拷贝）
	Vector<T>& operator=(Vector<T> const&);

	// 删除单个元素：移除索引r处的元素，并返回被删除的元素
	// 参数：r-待删除元素的索引（需满足0 <= r < _size）
	T remove(Rank r);

	// 删除区间元素：移除[lo, hi)区间内的所有元素
	// 参数：lo-起始索引（包含）；hi-结束索引（不包含）
	// 返回值：实际删除的元素个数（hi - lo，若lo >= hi则返回0）
	int remove(Rank lo, Rank hi);

	// 插入元素：在索引r处插入元素e，返回插入位置r
	// 参数：r-插入位置（0 <= r <= _size，可在末尾插入）；e-待插入元素
	// 注意：插入前会检查容量，不足则扩容
	Rank insert(Rank r, T const& e);

	// 尾部插入：在向量末尾插入元素e（等价于insert(_size, e)）
	Rank insert(T const& e) { return insert(_size, e); }

	// 区间排序：对[lo, hi)区间元素进行排序（随机选择冒泡、选择或归并排序）
	void sort(Rank lo, Rank hi);

	// 全量排序：对整个向量元素进行排序（等价于sort(0, _size)）
	void sort() { sort(0, _size); }
	// 单次冒泡：从[lo, hi)区间左侧向右侧扫描，交换相邻逆序元素
	bool bubble(Rank lo, Rank hi);

	// 冒泡排序：对[lo, hi)区间元素进行冒泡排序（稳定排序）
	void bubbleSort(Rank lo, Rank hi);
	void bubbleSort() { bubbleSort(0, _size); }
	// 归并操作：将两个有序子区间[lo, mi)和[mi, hi)合并为一个有序区间[lo, hi)
	void merge(Rank lo, Rank mi, Rank hi);
	

	// 归并排序：对[lo, hi)区间元素进行归并排序（稳定排序，分治思想）
	void mergeSort(Rank lo, Rank hi);
	void mergeSort() { mergeSort(0, _size); }

	// 区间打乱：随机打乱[lo, hi)区间内的元素顺序
	void unsort(Rank lo, Rank hi);

	// 全量打乱：随机打乱整个向量的元素顺序（等价于unsort(0, _size)）
	void unsort() { unsort(0, _size); }

	// 无序向量去重：删除向量中所有重复元素（保留首次出现的元素）
	int deduplicate();

	// 有序向量去重：删除有序向量中所有重复元素（利用有序性高效去重）
	int uniquify();

	// 遍历：通过函数指针访问每个元素（可修改元素）
	// 参数：visit-函数指针，指向对元素的操作函数（参数为T&）
	void traverse(void (*visit)(T&));

	// 遍历：通过函数对象访问每个元素（可修改元素）
	// 参数：visit-函数对象，需重载operator()(T&)
	template <typename VST> void traverse(VST&);
	// 反转向量元素顺序（将[lo, hi)区间的元素逆序）
	void reverse(Rank lo, Rank hi);
	// 反转整个向量（默认反转[0, _size)区间）
	void reverse() { reverse(0, _size); }
};
// 反转[lo, hi)区间的元素
template <typename T> void Vector<T>::reverse(Rank lo, Rank hi) {
	if (hi - lo <= 1) return; // 区间长度≤1时，无需反转
	hi--; // 调整hi为区间最后一个元素的索引（原hi为开区间）
	while (lo < hi) {
		swap(_elem[lo++], _elem[hi--]); // 交换lo和hi位置的元素，然后向中间移动
	}
}
// 从数组A的[lo, hi)区间复制元素到当前向量
template<typename T> void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi) {
	_elem = new T[_capacity = 2 * (hi - lo)]; // 分配容量为区间长度2倍的空间（预留扩展余地）
	_size = 0; // 初始化规模为0
	while (lo < hi) // 逐个复制A[lo]到A[hi-1]
		_elem[_size++] = A[lo++];
}

// 赋值运算符实现：深拷贝另一个向量V的所有元素
template <typename T> Vector<T>& Vector<T>::operator=(Vector<T> const& V) {
	if (_elem) delete[] _elem; // 释放当前向量原有内存（避免内存泄漏）
	copyFrom(V._elem, 0, V._size); // 复制V的所有元素
	return *this; // 返回当前对象的引用（支持链式赋值）
}

// 扩容操作：当元素个数等于容量时，将容量翻倍
template <typename T> void Vector<T>::expand() {
	if (_size < _capacity) return; // 元素未填满容量，无需扩容
	if (_capacity < DEFAULT_CAPACITY) _capacity = DEFAULT_CAPACITY; // 保证容量不小于最小默认值
	T* oldElem = _elem; // 保存原有元素数组指针
	_elem = new T[_capacity <<= 1]; // 容量翻倍（左移1位等价于乘2），分配新空间
	for (int i = 0; i < _size; i++)
		_elem[i] = oldElem[i]; // 复制原有元素到新空间
	delete[] oldElem; // 释放原有空间
}

// 缩容操作：当元素个数小于容量的1/4时，将容量减半（节省内存）
template <typename T> void Vector<T>::shrink() {
	if (_capacity < (DEFAULT_CAPACITY << 1)) return; // 容量不小于最小容量的2倍时不缩容（避免频繁调整）
	if ((_size << 2) > _capacity) return; // 元素个数的4倍大于容量（使用率>25%），不缩容
	T* oldElm = _elem; // 保存原有元素数组指针
	_elem = new T[_capacity >>= 1]; // 容量减半（右移1位等价于除2），分配新空间
	for (int i = 0; i < _size; i++)
		_elem[i] = oldElm[i]; // 复制原有元素到新空间
	delete[] oldElm; // 释放原有空间
}

// 重载下标运算符：支持通过索引访问元素
template <typename T> T& Vector<T>::operator[](Rank r) {
	return _elem[r]; // 直接返回索引r处的元素引用（未检查越界，需调用者保证合法性）
}

// 随机置乱向量元素（全局辅助函数）
template <typename T> void permute(Vector<T>& V) {
	for (int i = V.size(); i > 0; i--)
		swap(V[i - 1], V[rand() % i]); // 从[0, i-1]中随机选一个元素与第i-1个元素交换
}

// 区间打乱：随机打乱[lo, hi)区间内的元素
template <typename T> void Vector<T>::unsort(Rank lo, Rank hi) {
	T* V = _elem + lo; // 指向区间起始位置（子数组首地址）
	for (Rank i = hi - lo; i > 0; i--)
		swap(V[i - 1], V[rand() % i]); // 对区间内元素进行随机交换
}

// 无序区间查找：从右向左查找[lo, hi)中首个等于e的元素
template<typename T> Rank Vector<T>::find(T const& e, Rank lo, Rank hi) const {
	while ((lo < hi--) && (e != _elem[hi])); // 从hi-1开始向左扫描，直到找到e或lo >= hi
	return hi; // 找到则返回索引，未找到则返回lo-1（小于lo）
}

// 插入元素：在位置r插入e，返回r
template <typename T> Rank Vector<T>::insert(Rank r, T const& e) {
	expand(); // 检查容量，不足则扩容
	for (int i = _size; i > r; i--) // 从末尾到r，元素依次后移一位（腾出位置）
		_elem[i] = _elem[i - 1];
	_elem[r] = e; // 插入新元素
	_size++; // 规模加1
	return r;
}

// 区间删除：删除[lo, hi)内的元素
template <typename T> int Vector<T>::remove(Rank lo, Rank hi) {
	if (lo == hi) return 0; // 空区间，删除0个元素
	// 将[hi, _size)区间的元素前移，覆盖[lo, hi)区间
	while (hi < _size) _elem[lo++] = _elem[hi++];
	_size = lo; // 更新规模（删除后最后一个元素的索引为lo-1）
	shrink(); // 检查是否需要缩容
	return hi - lo; // 返回删除的元素个数（原hi - lo）
}

// 单个元素删除：删除索引r处的元素，返回被删除元素
template <typename T> T Vector<T>::remove(Rank r) {
	T e = _elem[r]; // 保存被删除元素
	remove(r, r + 1); // 调用区间删除（删除[r, r+1)）
	return e;
}

// 无序向量去重：删除重复元素（保留首次出现的元素）
template <typename T> int Vector<T>::deduplicate() {
	int oldSize = _size; // 记录原始规模
	Rank i = 1; // 从第2个元素开始检查（索引0的元素必然唯一）
	while (i < _size) {
		// 查找当前元素在[0, i)中是否存在
		(find(_elem[i], 0, i) < 0) ? i++ : remove(i); // 不存在则继续，存在则删除当前元素
	}
	return oldSize - _size; // 返回删除的元素个数
}

// 函数指针版遍历：对每个元素执行visit操作
template<typename T> void Vector<T>::traverse(void(*visit)(T&)) {
	for (int i = 0; i < _size; i++)
		visit(_elem[i]); // 调用函数指针处理每个元素
}

// 函数对象版遍历：对每个元素执行visit操作（更灵活，支持带状态的操作）
template<typename T> template<typename VST> void Vector<T>::traverse(VST& visit) {
	for (int i = 0; i < _size; i++)
		visit(_elem[i]); // 调用函数对象的operator()处理每个元素
}

// 统计逆序相邻元素对数量：判断向量有序性
template<typename T> int Vector<T>::disordered() const {
	int n = 0; // 逆序对计数器
	for (int i = 1; i < _size; i++) {
		if (_elem[i - 1] > _elem[i]) // 前一个元素大于后一个元素，构成逆序对
			n++;
	}
	return n;
}

// 有序向量去重：利用有序性（相邻重复）高效去重
template<typename T> int Vector<T>::uniquify() {
	if (_size <= 1) return 0; // 元素个数<=1时，无重复元素
	Rank i = 0, j = 0; // i指向当前保留的最后一个不重复元素，j遍历向量
	while (++j < _size) {
		if (_elem[i] != _elem[j]) { // 找到不重复元素
			_elem[++i] = _elem[j]; // 前移到i的下一位
		}
	}
	_size = i + 1; // 更新规模（保留前i+1个不重复元素）
	shrink(); // 缩容以节省空间
	return (j - (i + 1)); // 返回删除的元素个数（原始j - 新规模）
}

// 有序区间查找：调用二分查找算法
template<typename T> Rank Vector<T>::search(T const& e, Rank lo, Rank hi) const {
	return binSearch(_elem, e, lo, hi); // 二分查找（默认使用经典二分）
}

// 经典二分查找算法：在有序数组A的[lo, hi)区间查找e
// 前提：A[lo, hi)已按非降序排列
template <typename T> static Rank binSearch(T* A, T const& e, Rank lo, Rank hi) {
	while (1 < hi - lo) { // 区间长度大于1时继续查找
		Rank mi = (lo + hi) >> 1; // 取中点（等价于(lo+hi)/2，避免溢出）
		(e < A[mi]) ? hi = mi : lo = mi; // 调整查找区间
	}
	return (e == A[lo]) ? lo : -1; // 区间长度为1时，判断是否匹配
}

// 区间排序：随机选择一种排序算法（冒泡、选择或归并）
template <typename T> void Vector<T>::sort(Rank lo, Rank hi) {
	switch (rand() % 3) {
		case 1: bubbleSort(lo, hi); break; // 冒泡排序
		case 2: selectionSort(lo, hi); break; // 选择排序
		default: mergeSort(lo, hi); break; // 归并排序（默认）
		// case 4: quickSort(lo, hi); break; // 快速排序（未实现）
		// default: heapSort(lo, hi); break; // 堆排序（未实现）
	}
}

// 冒泡排序实现：反复执行单次冒泡，直到区间有序
template<typename T> void Vector<T>::bubbleSort(Rank lo, Rank hi) {
	while (!bubble(lo, hi--)); // 每次冒泡后缩小区间上限，直到某次冒泡无交换（区间有序）
}

// 单次冒泡：从左到右扫描，交换相邻逆序元素
template<typename T> bool Vector<T>::bubble(Rank lo, Rank hi) {
	bool sorted = true; // 假设区间已有序
	while (++lo < hi) { // 从lo+1扫描到hi-1
		if (_elem[lo - 1] > _elem[lo]) { // 发现逆序对
			sorted = false; // 标记区间无序
			swap(_elem[lo - 1], _elem[lo]); // 交换相邻元素
		}
	}
	return sorted; // 返回本次扫描是否有序
}

// 归并排序实现：分治思想，递归排序子区间后合并
template<typename T> void Vector<T>::mergeSort(Rank lo, Rank hi) {
	if (hi - lo < 2) return; // 区间长度为1时已有序，直接返回
	Rank mi = (lo + hi) >> 1; // 取中点分割区间
	mergeSort(lo, mi); // 递归排序左半区间[lo, mi)
	mergeSort(mi, hi); // 递归排序右半区间[mi, hi)
	merge(lo, mi, hi); // 合并两个有序子区间
}

// 归并操作：将[lo, mi)和[mi, hi)合并为有序区间[lo, hi)
template<typename T> void Vector<T>::merge(Rank lo, Rank mi, Rank hi) {
	T* A = _elem + lo; // 指向合并后区间的起始位置（即原左区间起始）
	int lb = mi - lo; // 左区间长度
	T* B = new T[lb]; // 为左区间分配临时缓冲区（复制左区间元素）
	for (Rank i = 0; i < lb; B[i] = A[i++]); // 复制左区间元素到B
	int lc = hi - mi; // 右区间长度
	T* C = _elem + mi; // 右区间起始位置（原地使用，无需额外空间）
	// 合并B（左）和C（右）到A：i为A的索引，j为B的索引，k为C的索引
	for (Rank i = 0, j = 0, k = 0; (j < lb) || (k < lc);) {
		// 若B未耗尽且（C已耗尽或B[j] <= C[k]），则取B[j]
		if ((j < lb) && (!(k < lc) || (B[j] <= C[k]))) A[i++] = B[j++];
		else A[i++] = C[k++]; // 否则取C[k]
	}
	delete[] B; // 释放左区间缓冲区
}

// 选择排序实现：每次选择区间内最大元素放到区间末尾
template<typename T> void Vector<T>::selectionSort(Rank lo, Rank hi) {
	while (lo < --hi) { // 区间上限从hi-1逐步缩小到lo
		// 找到[lo, hi]区间的最大元素，与hi位置元素交换
		swap(_elem[max(lo, hi)], _elem[hi]);
	}
}

// 查找区间最大值：返回[lo, hi]区间中最大元素的索引
template<typename T> Rank Vector<T>::max(Rank lo, Rank hi) {
	Rank maxIdx = lo; // 初始假设lo位置为最大值
	while (++lo < hi) { // 从lo+1扫描到hi-1
		if (_elem[maxIdx] < _elem[lo]) { // 发现更大元素
			maxIdx = lo; // 更新最大值索引
		}
	}
	return maxIdx;
}


#include"Stack.h"
#include"Queue.h"
using namespace std;
typedef enum { UNDISCOVERED, DISCOVERED, VISITED }VStatus;
typedef enum { UNDETERMINED, TREE, CROSS, FORWARD, BACKWARD }EType;
template<typename Tv, typename Te>//定点类型、边类型
class Graph {//图Graph模板类
private:
	void reset() {//所有顶点、边的状态复位
		for (int i = 0; i < n; i++) {//所有顶点
			status(i) = UNDISCOVERED; dTime(i) = fTime(i) = -1; parent(i) = -1; priority(i) = INT_MAX;
			for (int j = 0; j < n; j++)//所有边
				if (exists(i, j)) type(i, j) = UNDETERMINED;
		}
	}
	void BFS(int, int&);//广度优先搜索算法
	void DFS(int, int&);//深度优先搜索算法
	void BCC(int, int&, Stack<int>&);//基于DFS的双连通分量分解算法
	bool TSort(int, int&, Stack<Tv>*);//基于DFS的拓扑排序算法
	template<typename PU> void PFS(int, PU);//优先级搜索框架
public:
	int n; //顶点数
	virtual int insert(Tv const&) = 0;//插入顶点
	virtual Tv remove(int) = 0;//删除顶点
	virtual Tv& vertex(int) = 0;//顶点数据
	virtual int inDegree(int) = 0;//顶点入度
	virtual int outDegree(int) = 0;//顶点出度
	virtual int firstNbr(int) = 0;//顶点的首个邻接顶点
	virtual int nextNbr(int, int) = 0;//顶点的下一个邻接顶点
	virtual VStatus& status(int) = 0;//顶点状态
	virtual int& dTime(int) = 0;//顶点时间标签dTime
	virtual int& fTime(int) = 0;//顶点时间标签fTime
	virtual int& parent(int) = 0;//顶点在遍历树中的父节点
	virtual int& priority(int) = 0;//顶点在遍历树中的优先级数
	int e; //边数
	virtual bool exists(int, int) = 0;//边是否存在
	virtual void insert(Te const&, int, int, int) = 0;//插入边
	virtual Te remove(int, int) = 0;//删除边
	virtual EType& type(int, int) = 0;//边类型
	virtual Te& edge(int, int) = 0;//边数据
	virtual int& weight(int, int) = 0;//边权重
	//算法
	void bfs(int);
	void dfs(int);
	void bcc(int);
	Stack<Tv>* tSort(int);
	void prim(int);
	void dijkstra(int);
	template<typename PU>void pfs(int, PU);
};
template<typename Tv>struct Vertex {//顶点模板类
	Tv data; int inDegree, outDegree; VStatus status; int dTime, fTime; int parent; int priority;
	Vertex(Tv const& d = (Tv)0) :data(d), inDegree(0), outDegree(0), status(UNDISCOVERED), dTime(-1), fTime(-1), parent(-1), priority(INT_MAX) {}
};
template<typename Te>struct Edge {//边模板类
	Te data; int weight; EType type;
	Edge(Te const& d, int w) :data(d), weight(w), type(UNDETERMINED) {}
};
template<typename Tv, typename Te>//顶点类型、边类型
class GraphMatrix :public Graph<Tv, Te> {//图的矩阵实现
private:
	Vector<Vertex<Tv>> V;//顶点集
	Vector<Vector<Edge<Te>*>> E;//边集
public:
	GraphMatrix() { this->n = this->e = 0; }
	~GraphMatrix() {//析构函数
		for (int j = 0; j < this->n; j++) {//释放所有边
			for (int i = 0; i < this->n; i++)
				delete E[j][i];
		}
	}
	//顶点的基本操作
	virtual Tv& vertex(int i) override { return V[i].data; }//顶点数据
	virtual int inDegree(int i) override { return V[i].inDegree; }//顶点入度
	virtual int outDegree(int i) override { return V[i].outDegree; }//顶点出度
	virtual int firstNbr(int i) override { return nextNbr(i, this->n); }//顶点的首个邻接顶点
	virtual int nextNbr(int i, int j) override {//顶点的下一个邻接顶点
		while ((-1 < j--) && (!this->exists(i, j)));
		return j;
	}
	virtual VStatus& status(int i) override { return V[i].status; }//顶点状态
	virtual int& dTime(int i) override { return V[i].dTime; }//顶点时间标签dTime
	virtual int& fTime(int i) override { return V[i].fTime; }//顶点时间标签fTime
	virtual int& parent(int i) override { return V[i].parent; }//顶点在遍历树中的父节点
	virtual int& priority(int i) override { return V[i].priority; }//顶点在遍历树中的优先级数
	//顶点的动态操作
	virtual int insert(Tv const& data) override {//插入顶点
		for (int j = 0; j < this->n; j++)//所有已有顶点
			E[j].insert(nullptr); //新增顶点前，所有行均需新增一列（初始化为空指针）
		this->n++;
		E.insert(Vector<Edge<Te>*>(this->n, this->n, (Edge<Te>*)nullptr)); //新增一行（初始化为空指针）
		return V.insert(Vertex<Tv>(data)); //新增顶点
	}
	virtual Tv remove(int i) override {//删除顶点
		for (int j = 0; j < this->n; j++) {//所有顶点
			if (this->exists(i, j)) {//删除出边
				delete E[i][j];
				V[j].inDegree--;
				this->e--;
			}
		}
		E.remove(i); //删除行
		this->n--;
		Tv vBak = vertex(i);
		V.remove(i); //删除顶点
		for (int j = 0; j < this->n; j++) {//所有顶点
			if (Edge<Te>* pE = E[j].remove(i)) {//删除入边
				delete pE;
				V[j].outDegree--;
			}
		}
		return vBak;
	}
	//边的确认操作
	virtual bool exists(int i, int j) override {//边是否存在
		return (0 <= i) && (i < this->n) && (0 <= j) && (j < this->n) && (E[i][j] != nullptr);
	}
	//边的基本操作
	virtual EType& type(int i, int j) override { return E[i][j]->type; }//边类型
	virtual Te& edge(int i, int j) override { return E[i][j]->data; }//边数据
	virtual int& weight(int i, int j) override { return E[i][j]->weight; }//边权重
	//边的动态操作
	virtual void insert(Te const& data, int w, int i, int j) override {//插入边
		if (this->exists(i, j)) return;//边已存在
		E[i][j] = new Edge<Te>(data, w);//创建新边
		this->e++;
		V[i].outDegree++;//更新顶点的出度和入度
		V[j].inDegree++;
	}
	virtual Te remove(int i, int j) override {//删除边
		Te eBak = edge(i, j);
		delete E[i][j];
		E[i][j] = nullptr;
		this->e--;
		V[i].outDegree--;//更新顶点的出度和入度
		V[j].inDegree--;
		return eBak;
	}
};
// 图的广度优先搜索算法
template<typename Tv, typename Te>
void Graph<Tv, Te>::bfs(int s) {
	reset(); // 重置所有顶点和边的状态
	int clock = 0; int v = s; // 初始化时间标签
	do 
		if (status(v) == UNDISCOVERED) // 对每个未发现的顶点
			BFS(v, clock); // 从该顶点出发进行BFS
	while (s != (v = (v + 1) % n));//遍历所有顶点
}
template<typename Tv, typename Te>
void Graph<Tv, Te>::BFS(int v, int& clock) {
	Queue<int> Q; // 辅助队列
	status(v) = DISCOVERED; // 标记顶点v为已发现
	Q.enqueue(v); // 顶点v入队
	while (!Q.empty()) {
		int v = Q.dequeue(); // 取出队首顶点v
		dTime(v) = ++clock; // 设置顶点v的时间标签dTime
		for (int u = firstNbr(v); -1 < u; u = nextNbr(v, u)) { // 枚举v的所有邻接顶点u
			if (status(u) == UNDISCOVERED) { // 对每个未发现的邻接顶点u
				status(u) = DISCOVERED; // 标记u为已发现
				Q.enqueue(u); // 顶点u入队
				type(v, u) = TREE; // 树边
				parent(u) = v; // 设置u的父节点为v
			}
			else {// 已发现或已访问的邻接顶点u
				type(v, u) = CROSS;
			}
		}
		status(v) = VISITED; // 标记顶点v为已访问
	}
}
// 图的深度优先搜索算法
template<typename Tv, typename Te>
void Graph<Tv, Te>::dfs(int s) {
	reset(); // 重置所有顶点和边的状态
	int clock = 0; int v = s; // 初始化时间标签
	do 
		if (status(v) == UNDISCOVERED) // 对每个未发现的顶点
			DFS(v, clock); // 从该顶点出发进行DFS
	while (s != (v = (v + 1) % n));//遍历所有顶点
}
template<typename Tv, typename Te>
void Graph<Tv, Te>::DFS(int v, int& clock) {
	dTime(v) = ++clock; // 设置顶点v的时间标签dTime
	status(v) = DISCOVERED; // 标记顶点v为已发现
	for (int u = firstNbr(v); -1 < u; u = nextNbr(v, u)) { // 枚举v的所有邻接顶点u
		switch (status(u)) {
		case UNDISCOVERED: // 对每个未发现的邻接顶点u
			type(v, u) = TREE; // 树边
			parent(u) = v; // 设置u的父节点为v
			DFS(u, clock); // 递归访问顶点u
			break;
		case DISCOVERED: // 对每个已发现的邻接顶点u
			type(v, u) = BACKWARD; // 后向边
			break;
		default: // 对每个已访问的邻接顶点u
			type(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS; // 前向边或横跨边
			break;
		}
	}
	status(v) = VISITED; // 标记顶点v为已访问
	fTime(v) = ++clock; // 设置顶点v的时间标签fTime
}
//图的括扑排序
template<typename Tv, typename Te>
Stack<Tv>* Graph<Tv, Te>::tSort(int s) {
	reset(); // 重置所有顶点和边的状态
	int clock = 0; int v = s; // 初始化时间标签
	Stack<Tv>* S = new Stack<Tv>; // 用于存储拓扑序列
	do 
		if (status(v) == UNDISCOVERED) // 对每个未发现的顶点
			TSort(v, clock, S); // 从该顶点出发进行拓扑排序
	while (s != (v = (v + 1) % n));//遍历所有顶点
	return S; // 返回拓扑序列
}
template<typename Tv, typename Te>
bool Graph<Tv, Te>::TSort(int v, int& clock, Stack<Tv>* S) {
	dTime(v) = ++clock; // 设置顶点v的时间标签dTime
	status(v) = DISCOVERED; // 标记顶点v为已发现
	for (int u = firstNbr(v); -1 < u; u = nextNbr(v, u)) { // 枚举v的所有邻接顶点u
		switch (status(u)) {
		case UNDISCOVERED: // 对每个未发现的邻接顶点u
			type(v, u) = TREE; // 树边
			parent(u) = v; // 设置u的父节点为v
			if (!TSort(u, clock, S)) return false; // 递归访问顶点u
			break;
		case DISCOVERED: // 对每个已发现的邻接顶点u
			type(v, u) = BACKWARD; // 后向边，说明有环，不能拓扑排序
			return false;
		default: // 对每个已访问的邻接顶点u
			type(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS; // 前向边或横跨边
			break;
		}
	}
	status(v) = VISITED; // 标记顶点v为已访问
	S->push(vertex(v)); // 将顶点v压入拓扑序列栈
	return true;
}
// 图的基于DFS的双连通分量分解算法
template<typename Tv, typename Te>
void Graph<Tv, Te>::bcc(int s) {
	reset(); // 重置所有顶点和边的状态
	int clock = 0; int v = s; // 初始化时间标签
	Stack<int> S; // 辅助栈
	do 
		if (status(v) == UNDISCOVERED) // 对每个未发现的顶点
			BCC(v, clock, S); // 从该顶点出发进行BCC
	while (s != (v = (v + 1) % n));//遍历所有顶点
}
#define hca(x) (fTime(x)) // 利用fTime存储hca值
template<typename Tv, typename Te>
void Graph<Tv, Te>::BCC(int v, int& clock, Stack<int>& S) {
	hca(v) = dTime(v) = ++clock; // 初始化hca和dTime
	status(v) = DISCOVERED; // 标记顶点v为已发现
	S.push(v); // 将顶点v压入辅助栈

	int children = 0; // 用于根节点判断

	for (int u = firstNbr(v); -1 < u; u = nextNbr(v, u)) { // 枚举v的所有邻接顶点u
		switch (status(u)) {
		case UNDISCOVERED: { // 对每个未发现的邻接顶点u
			type(v, u) = TREE; // 树边
			parent(u) = v; // 设置u的父节点为v
			children++; // 增加子节点计数

			BCC(u, clock, S); // 递归访问顶点u

			// 检查v是否是关节点
			if (parent(v) == -1 && children > 1) {
				// v是根节点且有多个子节点
				cout << "发现关节点(根节点): " << vertex(v) << endl;
			}
			else if (parent(v) != -1 && hca(u) >= dTime(v)) {
				// v不是根节点，但u无法通过后向边连接到v的祖先
				cout << "发现关节点: " << vertex(v) << endl;
			}
			if (hca(u) < dTime(v)) // 若u及其后代无法通过后向边回溯到v的祖先
				hca(v) = min(hca(v), hca(u)); // 则更新v的hca值
			else { // 否则，说明找到了一个双连通分量
				int w;
				do {
					w = S.pop();
					cout << vertex(w) << " ";
				} while (w != u); // 弹出直到u
				cout << vertex(v) << endl; // v是关节点，属于多个分量
				S.push(v); // 顶点v重新入栈（作为新分量的起点）
			}
			break;
		}
		case DISCOVERED: // 对每个已发现的邻接顶点u
			type(v, u) = BACKWARD; // 后向边
			if (parent(v) != u) // 排除直接父子关系的边
				hca(v) = min(hca(v), dTime(u)); // 更新v的hca值
			break;
		default: // 对每个已访问的邻接顶点u
			type(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS; // 前向边或横跨边
			break;
		}
	}
	status(v) = VISITED; // 标记顶点v为已访问
}
#undef hca
// 图的优先级搜索算法框架
template<typename Tv, typename Te> template<typename PU>
void Graph<Tv, Te>::pfs(int s,PU prioUpdater) {
	reset(); // 重置所有顶点和边的状态
	int v = s; // 初始化起始顶点
	do 
		if (status(v) == UNDISCOVERED) // 对每个未发现的顶点
			PFS(v, prioUpdater); // 从该顶点出发进行PFS
	while (s != (v = (v + 1) % n));//遍历所有顶点
}
template<typename Tv, typename Te> template<typename PU>
void Graph<Tv, Te>::PFS(int s, PU prioUpdater) {
	priority(s) = 0; // 起始顶点优先级设为0
	status(s) = VISITED; // 标记起始顶点为已访问
	parent(s) = -1; // 起始顶点无父节点
	while (1) {
		for (int w = firstNbr(s); -1 < w; w = nextNbr(s, w)) // 在所有顶点中选出优先级最小的顶点v
			prioUpdater(this, s, w); // 调用优先级更新器更新w的优先级
		for (int shortest = INT_MAX, w = 0; w < n; w++) // 在所有顶点中选出优先级最小的顶点v
			if (status(w) == UNDISCOVERED)
				if (shortest > priority(w)) {
					shortest = priority(w);
					s = w;
				}
		if (status(s) == VISITED) break; // 若所有顶点均已访问，则算法结束
		status(s) = VISITED; // 标记顶点s为已访问
		type(parent(s), s) = TREE; // 树边
	}
}
// Prim算法的优先级更新器
template<typename Tv, typename Te>struct PrimPU {
	virtual void operator()(Graph<Tv, Te>* G, int s, int w) {
		if (G->status(w) == UNDISCOVERED)  // 对每个未发现的邻接顶点w
			if (G->weight(s, w) < G->priority(w)) { // 若边(s, w)权重更小
				G->priority(w) = G->weight(s, w); // 则更新w的优先级为边权重
				G->parent(w) = s; // 更新w的父节点为s
			}
	}
};
//Prims算法
template<typename Tv, typename Te>
void Graph<Tv, Te>::prim(int s) {
	pfs(s, PrimPU<Tv, Te>()); // 调用优先级搜索框架，传入Prim算法的优先级更新器
}
// Dijkstra算法的优先级更新器
template<typename Tv, typename Te>struct DijkstraPU {
	virtual void operator()(Graph<Tv, Te>* G, int s, int w) {
		if (G->status(w) == UNDISCOVERED) // 对每个未发现的邻接顶点w
			if (G->priority(s) + G->weight(s, w) < G->priority(w)) { // 若路径更短
				G->priority(w) = G->priority(s) + G->weight(s, w); // 则更新w的优先级为路径长度
				G->parent(w) = s; // 更新w的父节点为s
		}
	}
};
//Dijkstra算法
template<typename Tv, typename Te>
void Graph<Tv, Te>::dijkstra(int s) {
	pfs(s, DijkstraPU<Tv, Te>()); // 调用优先级搜索框架，传入Dijkstra算法的优先级更新器
}

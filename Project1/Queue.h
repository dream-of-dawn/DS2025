#include"Listcpp.cpp"
template<typename T>class Queue : public List<T> { //队列模板类
	public:
		void enqueue(T const& e) { //入队操作
			this->insertAsLast(e); //将元素e作为末节点插入
		};
		T dequeue() { //出队操作
			return this->remove(this->first()); //删除首节点并返回其数据
		};
		T& front() { //访问队首元素
			return this->first()->data; //返回首节点的数据
		};
};
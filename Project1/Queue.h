#include"Listcpp.cpp"
template<typename T>class Queue : public List<T> { //����ģ����
	public:
		void enqueue(T const& e) { //��Ӳ���
			this->insertAsLast(e); //��Ԫ��e��Ϊĩ�ڵ����
		};
		T dequeue() { //���Ӳ���
			return this->remove(this->first()); //ɾ���׽ڵ㲢����������
		};
		T& front() { //���ʶ���Ԫ��
			return this->first()->data; //�����׽ڵ������
		};
};
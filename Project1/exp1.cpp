#include "Stack.h"
#include <cmath>
#include <ctime>
#include <iomanip>
#include <string>
using namespace std;
// �����ඨ��
class Complex {
private:
    double real;  
    double imag;  
public:
    Complex(double r = 0, double i = 0) : real(r), imag(i) {}
    double getReal() const { return real; }
    double getImag() const { return imag; }
    double magnitude() const {
        return sqrt(real * real + imag * imag);
    }
    bool operator==(const Complex& other) const {
        return (real == other.real) && (imag == other.imag);
    }
    bool operator!=(const Complex& other) const {
        return !(*this == other);
    }
    bool operator<(const Complex& other) const {
        if (magnitude() != other.magnitude()) {
            return magnitude() < other.magnitude();
        }
        return real < other.real;
    }
    bool operator>(const Complex& other) const {
        return other < *this;
    }
    bool operator<=(const Complex& other) const {
        if (magnitude() != other.magnitude()) {
            return magnitude() <= other.magnitude();
        }
        return real <= other.real;
    }

    friend ostream& operator<<(ostream& os, const Complex& c) {
        os << fixed << setprecision(2);
        os << "(" << c.real << ", " << c.imag << ")";
        os << " [ģ: " << c.magnitude() << "]";
        return os;
    }
};
// ��ʱ�����������㷨
void testSortingEfficiency(Vector<Complex>& vec, const string& caseName) {
    // ����ð������
    Vector<Complex> bubbleVec = vec;
    clock_t start = clock();
    bubbleVec.bubbleSort();
    clock_t end = clock();
    double bubbleTime = double(end - start) / CLOCKS_PER_SEC;
    // ���Թ鲢����
    Vector<Complex> mergeVec = vec;
    start = clock();
    mergeVec.mergeSort();
    end = clock();
    double mergeTime = double(end - start) / CLOCKS_PER_SEC;
    cout << caseName << "����Ч�ʣ�" << endl;
    cout << "  ð������ʱ��: " << fixed << setprecision(6) << bubbleTime << " ��" << endl;
    cout << "  �鲢����ʱ��: " << fixed << setprecision(6) << mergeTime << " ��" << endl;
    cout << "  �鲢�����ð������� " << fixed << setprecision(2) << (bubbleTime / mergeTime) << " ��" << endl << endl;
}
// �����������в���ģ����[m1, m2)������Ԫ��
Vector<Complex> findInRange(Vector<Complex>& sortedVec, double m1, double m2) {
    Vector<Complex> result;
    for (int i = 0; i < sortedVec.size(); ++i) {
        double mag = sortedVec[i].magnitude();
        if (mag >= m1 && mag < m2) {
            result.insert(sortedVec[i]);
        }
        else if (mag >= m2) {
            break;
        }
    }
    return result;
}
void printVector(Vector<Complex>& vec, const string& msg) {
    cout << msg << " (" << vec.size() << "��Ԫ��):" << endl;
    for (int i = 0; i < vec.size(); ++i) {
        cout << vec[i];
        if (i % 2 == 1) cout << endl; 
        else cout << "  ";
    }
    if (vec.size() % 2 == 1) cout << endl;
    cout << endl;
}


// ������״ͼ������������ĺ���
int largestRectangleArea(Vector<int>& heights) {
    Stack<int> st;  // ����ջ���洢����������ά���߶ȵ���������
    int max_area = 0;
    int n = heights.size();

    for (int i = 0; i < n; ++i) {
        // ����ǰ���Ӹ߶�С��ջ�����Ӹ߶�ʱ������ջ���������γɵ�������
        while (!st.empty() && heights[i] < heights[st.top()]) {
            int height = heights[st.top()];
            st.pop();
            // ������ȣ���߽�Ϊ��ջ������+1���ұ߽�Ϊ��ǰ����-1
            int width = st.empty() ? i : i - st.top() - 1;
            max_area = max(max_area, height * width);
        }
        st.push(i);
    }

    // ����ջ��ʣ������ӣ��Ҳ����޸��͵����ӣ�
    while (!st.empty()) {
        int height = heights[st.top()];
        st.pop();
        // ������ȣ���߽�Ϊ��ջ������+1���ұ߽�Ϊ����ĩβ
        int width = st.empty() ? n : n - st.top() - 1;
        max_area = max(max_area, height * width);
    }

    return max_area;
}

int main() {
    // ��1������������ֱ������������������������ظ��
    int vecSize = 20;
    double minVal = -10, maxVal = 10;
    Vector<Complex> complexVec;

    for (int i = 0; i < vecSize; ++i) {
        // 20%���������ظ������ǰһ��Ԫ�أ�
        if (i > 0 && rand() % 5 == 0) {
            complexVec.insert(complexVec[i - 1]);
        }
        else {
            // �������ʵ�����鲿
            double real = minVal + (rand() % 100) * (maxVal - minVal) / 100.0;
            double imag = minVal + (rand() % 100) * (maxVal - minVal) / 100.0;
            complexVec.insert(Complex(real, imag));  // �����¸���
        }
    }
    printVector(complexVec, "��ʼ�����������");
    // ��1���������������ĸ��ֲ���
    cout << "=== ���������������� ===" << endl;
    // �������Ҳ���
    complexVec.unsort();
    printVector(complexVec, "���Һ������");
    // ���Բ��Ҳ���
    if (!complexVec.empty()) {
        Complex target = complexVec[5];  // �Ե�5��Ԫ��ΪĿ��
        Rank found = complexVec.find(target);
        if (found != -1) {
            cout << "����Ԫ�� " << target << " �ɹ���λ��: " << found << endl;
        }
        else {
            cout << "����Ԫ�� " << target << " ʧ��" << endl;
        }
    }
    // ���Բ������
    Complex newComplex(100, 200);
    complexVec.insert(3, newComplex);  // ��λ��3������Ԫ��
    printVector(complexVec, "����Ԫ�غ������");
    // ����ɾ������
    if (complexVec.size() > 5) {
        Complex removed = complexVec.remove(5);  // ɾ��λ��5��Ԫ��
        cout << "ɾ����Ԫ��: " << removed << endl;
        printVector(complexVec, "ɾ��Ԫ�غ������");
    }
    // ����Ψһ������
    int removedCount = complexVec.deduplicate();
    cout << "Ψһ������ɾ���� " << removedCount << " ���ظ�Ԫ��" << endl;
    printVector(complexVec, "Ψһ���������");

    // ��2����������Ч��
    cout << "=== ��������Ч�� ===" << endl;
    // �������ֲ�ͬ״̬������
    Vector<Complex> orderedVec = complexVec;
    orderedVec.sort();  // ������������
    Vector<Complex> reversedVec = orderedVec;
    reversedVec.reverse();  // ������������
    // ��������Ч��
    testSortingEfficiency(complexVec, "�������");
    testSortingEfficiency(orderedVec, "��������");
    testSortingEfficiency(reversedVec, "��������");
    // ��3�������������
    cout << "=== ����������� ===" << endl;
    double m1 = 5.0;
    double m2 = 15.0;
    Vector<Complex> rangeVec = findInRange(orderedVec, m1, m2);
    printVector(rangeVec, "ģ��[" + to_string(m1) + ", " + to_string(m2) + ")�����ڵ�Ԫ��");
    cout << "======================�ڶ���==============" << endl;
    char expr[] = "3+4*2/(1-5)*2!"; 
    char rpn[200] = { 0 }; 
    float res = evaluate(expr);
    cout << "����ʽ��" << expr << endl;
    cout << "��������" << res << endl;
    cout << "=====================������===============" << endl;
    for (int i = 0; i < 10; ++i) {
        int length = 10;
        Vector<int> heights(length, length);
        for (int j = 0; j < length; ++j) {
            heights[j] = rand() % 105;
        }
        int maxArea = largestRectangleArea(heights);
        cout << maxArea << endl;
    }

    return 0;
}


#include "../Project1/Vector.h"
#include <cmath>
#include <ctime>
#include <iomanip>
#include <string>
#include "../Project1/Stack.h"
#include <cstdlib>
#include <iostream>
#include <algorithm>
using namespace std;
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
    char expr[] = "3+4*2/(1-5)!2"; // ʾ�����ʽ��ע�⣺!��һԪȡ���������ʾ��(1-5)��ȡ������
    char rpn[200] = { 0 }; // �洢�沨��ʽ
    float res = evaluate(expr, rpn);
    cout << "���ʽ��" << expr << endl;
    cout << "��������" << res << endl;
	cout << "=====================������===============" << endl;
    srand(time(0));  // ��ʼ����������ӣ�ȷ��ÿ���������ݲ�ͬ
    for (int i = 0; i < 10; ++i) {
        int length = 10;
        Vector<int> heights(length);
        for (int j = 0; j < length; ++j) {
            heights[j] = rand() % 105;
        }
        int maxArea = largestRectangleArea(heights);
        cout << maxArea << endl;
    }

    return 0;
}
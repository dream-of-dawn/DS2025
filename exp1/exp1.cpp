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

    // （1）在主函数中直接生成随机复数向量（包含重复项）
    int vecSize = 20;
    double minVal = -10, maxVal = 10;
    Vector<Complex> complexVec;

    for (int i = 0; i < vecSize; ++i) {
        // 20%概率生成重复项（复制前一个元素）
        if (i > 0 && rand() % 5 == 0) {
            complexVec.insert(complexVec[i - 1]);
        }
        else {
            // 生成随机实部和虚部
            double real = minVal + (rand() % 100) * (maxVal - minVal) / 100.0;
            double imag = minVal + (rand() % 100) * (maxVal - minVal) / 100.0;
            complexVec.insert(Complex(real, imag));  // 插入新复数
        }
    }
    printVector(complexVec, "初始随机复数向量");
    // （1）测试无序向量的各种操作
    cout << "=== 测试无序向量操作 ===" << endl;
    // 测试置乱操作
    complexVec.unsort();
    printVector(complexVec, "置乱后的向量");
    // 测试查找操作
    if (!complexVec.empty()) {
        Complex target = complexVec[5];  // 以第5个元素为目标
        Rank found = complexVec.find(target);
        if (found != -1) {
            cout << "查找元素 " << target << " 成功，位置: " << found << endl;
        }
        else {
            cout << "查找元素 " << target << " 失败" << endl;
        }
    }
    // 测试插入操作
    Complex newComplex(100, 200);
    complexVec.insert(3, newComplex);  // 在位置3插入新元素
    printVector(complexVec, "插入元素后的向量");
    // 测试删除操作
    if (complexVec.size() > 5) {
        Complex removed = complexVec.remove(5);  // 删除位置5的元素
        cout << "删除的元素: " << removed << endl;
        printVector(complexVec, "删除元素后的向量");
    }
    // 测试唯一化操作
    int removedCount = complexVec.deduplicate();
    cout << "唯一化操作删除了 " << removedCount << " 个重复元素" << endl;
    printVector(complexVec, "唯一化后的向量");

    // （2）测试排序效率
    cout << "=== 测试排序效率 ===" << endl;
    // 生成三种不同状态的向量
    Vector<Complex> orderedVec = complexVec;
    orderedVec.sort();  // 生成有序向量
    Vector<Complex> reversedVec = orderedVec;
    reversedVec.reverse();  // 生成逆序向量
    // 测试排序效率
    testSortingEfficiency(complexVec, "随机向量");
    testSortingEfficiency(orderedVec, "有序向量");
    testSortingEfficiency(reversedVec, "逆序向量");
    // （3）测试区间查找
    cout << "=== 测试区间查找 ===" << endl;
    double m1 = 5.0;
    double m2 = 15.0;
    Vector<Complex> rangeVec = findInRange(orderedVec, m1, m2);
    printVector(rangeVec, "模在[" + to_string(m1) + ", " + to_string(m2) + ")区间内的元素");
    cout << "======================第二题==============" << endl;
    char expr[] = "3+4*2/(1-5)!2"; // 示例表达式（注意：!是一元取反，这里表示“(1-5)的取反”）
    char rpn[200] = { 0 }; // 存储逆波兰式
    float res = evaluate(expr, rpn);
    cout << "表达式：" << expr << endl;
    cout << "计算结果：" << res << endl;
	cout << "=====================第三题===============" << endl;
    srand(time(0));  // 初始化随机数种子，确保每次运行数据不同
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
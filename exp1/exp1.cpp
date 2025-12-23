#include "Stack.h"
#include <cmath>
#include <ctime>
#include <iomanip>
#include <string>
using namespace std;

// 复数类定义
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
        os << " [模: " << c.magnitude() << "]";
        return os;
    }
};

// 测试排序效率
void testSortingEfficiency(Vector<Complex>& vec, const string& caseName) {
    // 冒泡排序
    Vector<Complex> bubbleVec = vec;
    clock_t start = clock();
    bubbleVec.bubbleSort();
    clock_t end = clock();
    double bubbleTime = double(end - start) / CLOCKS_PER_SEC;
    // 归并排序
    Vector<Complex> mergeVec = vec;
    start = clock();
    mergeVec.mergeSort();
    end = clock();
    double mergeTime = double(end - start) / CLOCKS_PER_SEC;
    cout << caseName << "排序算法性能对比" << endl;
    cout << "  冒泡排序耗时: " << fixed << setprecision(6) << bubbleTime << " 秒" << endl;
    cout << "  归并排序耗时: " << fixed << setprecision(6) << mergeTime << " 秒" << endl;
    cout << "  归并排序比冒泡排序快 " << fixed << setprecision(2) << (bubbleTime / mergeTime) << " 倍" << endl << endl;
}

// 在已排序的向量中查找模在[m1, m2)范围内的复数
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
    cout << msg << " (" << vec.size() << "个元素):" << endl;
    for (int i = 0; i < vec.size(); ++i) {
        cout << vec[i];
        if (i % 2 == 1) cout << endl; 
        else cout << "  ";
    }
    if (vec.size() % 2 == 1) cout << endl;
    cout << endl;
}

// 柱状图中最大矩形面积
int largestRectangleArea(Vector<int>& heights) {
    Stack<int> st;  // 存储索引的栈
    int max_area = 0;
    int n = heights.size();

    for (int i = 0; i < n; ++i) {
        // 当当前高度小于栈顶索引对应的高度时，计算栈顶元素的面积
        while (!st.empty() && heights[i] < heights[st.top()]) {
            int height = heights[st.top()];
            st.pop();
            // 宽度：如果栈为空，说明当前元素是左边最小的，宽度为i；否则为i - 栈顶索引 - 1
            int width = st.empty() ? i : i - st.top() - 1;
            max_area = max(max_area, height * width);
        }
        st.push(i);
    }

    // 处理栈中剩余的元素
    while (!st.empty()) {
        int height = heights[st.top()];
        st.pop();
        // 宽度：如果栈为空，说明这个高度可以延伸到末尾，宽度为n；否则为n - 栈顶索引 - 1
        int width = st.empty() ? n : n - st.top() - 1;
        max_area = max(max_area, height * width);
    }

    return max_area;
}

int main() {
    // 第一部分：创建和操作Vector<Complex>
    int vecSize = 20;
    double minVal = -10, maxVal = 10;
    Vector<Complex> complexVec;

    for (int i = 0; i < vecSize; ++i) {
        // 20%的概率重复上一个元素（模拟重复）
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
    printVector(complexVec, "初始生成的复数向量");

    // 第一部分：测试Vector的基本操作
    cout << "=== 测试Vector基本操作 ===" << endl;
    // 打乱顺序
    complexVec.unsort();
    printVector(complexVec, "打乱顺序后");
    // 查找元素
    if (!complexVec.empty()) {
        Complex target = complexVec[5];  // 假设查找第5个元素
        Rank found = complexVec.find(target);
        if (found != -1) {
            cout << "查找元素 " << target << " 成功，位置: " << found << endl;
        }
        else {
            cout << "查找元素 " << target << " 失败" << endl;
        }
    }
    // 插入元素
    Complex newComplex(100, 200);
    complexVec.insert(3, newComplex);  // 在位置3插入新元素
    printVector(complexVec, "在位置3插入新元素后");
    // 删除元素
    if (complexVec.size() > 5) {
        Complex removed = complexVec.remove(5);  // 删除位置5的元素
        cout << "删除的元素: " << removed << endl;
        printVector(complexVec, "删除位置5的元素后");
    }
    // 去重
    int removedCount = complexVec.deduplicate();
    cout << "去重操作共删除了 " << removedCount << " 个重复元素" << endl;
    printVector(complexVec, "去重后的向量");

    // 第二部分：排序算法性能测试
    cout << "=== 排序算法性能测试 ===" << endl;
    // 创建有序和逆序的向量用于测试
    Vector<Complex> orderedVec = complexVec;
    orderedVec.sort();  // 排序得到有序向量
    Vector<Complex> reversedVec = orderedVec;
    reversedVec.reverse();  // 反转得到逆序向量
    // 测试不同情况下的排序效率
    testSortingEfficiency(complexVec, "随机乱序");
    testSortingEfficiency(orderedVec, "完全有序");
    testSortingEfficiency(reversedVec, "完全逆序");

    // 第三部分：范围查找
    cout << "=== 范围查找测试 ===" << endl;
    double m1 = 5.0;
    double m2 = 15.0;
    Vector<Complex> rangeVec = findInRange(orderedVec, m1, m2);
    printVector(rangeVec, "模在[" + to_string(m1) + ", " + to_string(m2) + ")范围内的复数");

    cout << "======================表达式求值==============" << endl;
    char expr[] = "3+4*2/(1-5)*2!"; 
    char rpn[200] = { 0 }; 
    float res = evaluate(expr);
    cout << "表达式：" << expr << endl;
    cout << "计算结果：" << res << endl;
    cout << "=====================最大矩形面积测试===============" << endl;
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
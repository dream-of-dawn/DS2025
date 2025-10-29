#include "Vector.h"
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
// 计时并测试排序算法
void testSortingEfficiency(Vector<Complex>& vec, const string& caseName) {
    // 测试冒泡排序
    Vector<Complex> bubbleVec = vec;
    clock_t start = clock();
    bubbleVec.bubbleSort();
    clock_t end = clock();
    double bubbleTime = double(end - start) / CLOCKS_PER_SEC;
    // 测试归并排序
    Vector<Complex> mergeVec = vec;
    start = clock();
    mergeVec.mergeSort();
    end = clock();
    double mergeTime = double(end - start) / CLOCKS_PER_SEC;
    cout << caseName << "排序效率：" << endl;
    cout << "  冒泡排序时间: " << fixed << setprecision(6) << bubbleTime << " 秒" << endl;
    cout << "  归并排序时间: " << fixed << setprecision(6) << mergeTime << " 秒" << endl;
    cout << "  归并排序比冒泡排序快 " << fixed << setprecision(2) << (bubbleTime / mergeTime) << " 倍" << endl << endl;
}
// 在有序向量中查找模介于[m1, m2)的所有元素
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
    return 0;
}
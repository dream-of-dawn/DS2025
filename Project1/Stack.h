#include"Vector.h"
#include <iostream>
#include <cstring>
#include <cstdlib> // 用于atoi、atof
#include <cctype>  // 用于isdigit
#define N_OPTR 9 // 运算符种类数
using namespace std;

template <typename T> class Stack : public Vector<T> { //栈模板类
	public:
	Stack(int c = DEFAULT_CAPACITY, int s = 0, T v = 0) : Vector<T>(c, s, v) {} //构造函数
	Stack(T const* A, Rank n) : Vector<T>(A, n) {} //数组整体复制
	Stack(T const* A, Rank lo, Rank hi) : Vector<T>(A, lo, hi) {} //区间复制
	Stack(Vector<T> const& V) : Vector<T>(V) {} //向量整体复制
	Stack(Vector<T> const& V, Rank lo, Rank hi) : Vector<T>(V, lo, hi) {} //区间复制
	void push(T const& e) { insert(this->size(), e); } //入栈操作
	T pop() { return this->remove(this->size() - 1); } //出栈操作
	T top() const { return (*this)[this->size() - 1]; } //访问栈顶元素
};
//括号匹配算法（迭代版）
bool paren(const char exp[], int lo, int hi) {
	Stack<char> S;
	for (int i = lo; i <= hi; i++) {
		switch (exp[i]) {
		case '(':case '[':case '{':S.push(exp[i]); break;
		case ')':if (S.empty() || S.pop() != '(')return false; break;
		case ']':if (S.empty() || S.pop() != '[')return false; break;
		case '}':if (S.empty() || S.pop() != '{')return false; break;
		default:break;
		}
	}	
	return S.empty();
}
//优先级表
typedef enum { ADD, SUB, MUL, DIV, POW, FAC, L_P, R_P, EOE } Operator; //运算符枚举类型
// 加、减、乘、除、取模、乘方、左括号、右括号、表达式结束符
const char pri[N_OPTR][N_OPTR] = {
    //+    -    *    /    ^    !    (    )    \0
    {'>', '>', '<', '<', '<', '<', '<', '>', '>'}, // 栈顶 +
    {'>', '>', '<', '<', '<', '<', '<', '>', '>'}, // 栈顶 -
    {'>', '>', '>', '>', '<', '<', '<', '>', '>'}, // 栈顶 *
    {'>', '>', '>', '>', '<', '<', '<', '>', '>'}, // 栈顶 /
    {'>', '>', '>', '>', '>', '<', '<', '>', '>'}, // 栈顶 ^（乘方）
    {'>', '>', '>', '>', '>', '>', '<', '>', '>'}, // 栈顶 !（阶乘）
    {'<', '<', '<', '<', '<', '<', '<', '=', ' '}, // 栈顶 (
    {'>', '>', '>', '>', '>', '>', ' ', '>', '>'}, // 栈顶 )
    {'<', '<', '<', '<', '<', '<', '<', ' ', '='}  // 栈顶 \0（EOE）
};

// 追加操作数到逆波兰式RPN（假设RPN是足够大的字符数组）
void append(char* RPN, float num) {
    char numStr[20];
    sprintf(numStr, "%.2f ", num); // 格式化浮点数为字符串（保留2位小数）
    strcat(RPN, numStr);
}

// 追加运算符到逆波兰式RPN
void append(char* RPN, char op) {
    char opStr[2] = { op, ' ' };
    strcat(RPN, opStr);
}

// 读取表达式中的数字（处理多位数、小数），存入操作数栈opnd
void readNumber(char** S, Stack<float>& opnd) {
    char* ptr = *S;
    bool isDecimal = false; // 是否是小数
    float integerPart = 0, decimalPart = 0;
    int decimalDigits = 0;

    // 读取整数部分
    while (isdigit(*ptr)) {
        integerPart = integerPart * 10 + (*ptr - '0');
        ptr++;
    }
    // 读取小数部分（如果有）
    if (*ptr == '.') {
        isDecimal = true;
        ptr++;
        while (isdigit(*ptr)) {
            decimalPart = decimalPart * 10 + (*ptr - '0');
            decimalDigits++;
            ptr++;
        }
    }

    // 计算最终数值
    float num = integerPart;
    if (isDecimal) {
        for (int i = 0; i < decimalDigits; i++) {
            decimalPart /= 10;
        }
        num += decimalPart;
    }

    opnd.push(num);   // 压入操作数栈
    *S = ptr; // 更新表达式指针到当前位置
}

// 判断栈顶运算符与当前运算符的优先级关系：返回 '<' '=' '>'
char orderBetween(char topOp, char currOp) {
    // 映射运算符到索引
    auto getIndex = [](char op) -> int {
        switch (op) {
        case '+': return ADD;
        case '-': return SUB;
        case '*': return MUL;
        case '/': return DIV;
        case '^': return POW;
        case '!': return FAC;
        case '(': return L_P;
        case ')': return R_P;
        case '\0': return EOE;
        default: exit(-1); // 非法运算符
        }
        };

    int row = getIndex(topOp);
    int col = getIndex(currOp);
    return pri[row][col];
}

// 计算（支持一元、二元运算符）
float calcu(char op, float opnd) { // 一元运算
    switch (op) {
    case '!': return -opnd; // 假设'!'是一元取反
    default: exit(-1);
    }
}

float calcu(float opnd1, char op, float opnd2) { // 二元运算
    switch (op) {
    case '+': return opnd1 + opnd2;
    case '-': return opnd1 - opnd2;
    case '*': return opnd1 * opnd2;
    case '/':
        if (opnd2 == 0) exit(-1); // 除零错误
        return opnd1 / opnd2;
    default: exit(-1);
    }
}

// 表达式求值+逆波兰式转换
float evaluate(char* S, char* RPN) {
    Stack<float> opnd;   // 操作数栈
    Stack<char> optr;    // 运算符栈
    optr.push('\0');     // 哨兵入栈

    while (!optr.empty()) {
        if (isdigit(*S)) { // 当前字符是操作数
            readNumber(&S, opnd);
            append(RPN, opnd.top()); // 追加到逆波兰式
        }
        else { // 当前字符是运算符
            char rel = orderBetween(optr.top(), *S);
            switch (rel) {
            case '<': // 栈顶优先级低：当前运算符入栈
                optr.push(*S);
                S++;
                break;
            case '=': // 优先级相等（括号匹配或哨兵）
                optr.pop();
                S++;
                break;
            case '>': { // 栈顶优先级高：计算并压栈
                char op = optr.top();
                optr.pop();
                append(RPN, op); // 追加运算符到逆波兰式

                if (op == '!') { // 一元运算符
                    float p = opnd.top();
                    opnd.pop();
                    opnd.push(calcu(op, p));
                }
                else { // 二元运算符
                    float p2 = opnd.top(); opnd.pop();
                    float p1 = opnd.top(); opnd.pop();
                    opnd.push(calcu(p1, op, p2));
                }
                break;
            }
            default:
                exit(-1); // 语法错误
            }
        }
    }
    return opnd.top(); // 返回最终结果
}

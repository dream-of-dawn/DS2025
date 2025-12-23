#include"Vector.h"
#include <iostream>
#include <cstdlib>
#include <cctype>
#include <cmath>  // 用于幂运算pow
#define N_OPTR 9
using namespace std;

template <typename T> class Stack : public Vector<T> {
public:
    Stack(int c = DEFAULT_CAPACITY, int s = 0, T v = 0) : Vector<T>(c, s, v) {}
    Stack(T const* A, Rank n) : Vector<T>(A, n) {}
    Stack(T const* A, Rank lo, Rank hi) : Vector<T>(A, lo, hi) {}
    Stack(Vector<T> const& V) : Vector<T>(V) {}
    Stack(Vector<T> const& V, Rank lo, Rank hi) : Vector<T>(V, lo, hi) {}
    void push(T const& e) { this->insert(this->size(), e); }
    T pop() { return this->remove(this->size() - 1); }
    T top() const { return const_cast<Stack<T>*>(this)->operator[](this->size() - 1); }
};

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

typedef enum { ADD, SUB, MUL, DIV, POW, FAC, L_P, R_P, EOE } Operator;
const char pri[N_OPTR][N_OPTR] = {
    {'>', '>', '<', '<', '<', '<', '<', '>', '>'},
    {'>', '>', '<', '<', '<', '<', '<', '>', '>'},
    {'>', '>', '>', '>', '<', '<', '<', '>', '>'},
    {'>', '>', '>', '>', '<', '<', '<', '>', '>'},
    {'>', '>', '>', '>', '>', '<', '<', '>', '>'},
    {'>', '>', '>', '>', '>', '>', '<', '>', '>'},
    {'<', '<', '<', '<', '<', '<', '<', '=', ' '},
    {'>', '>', '>', '>', '>', '>', ' ', '>', '>'},
    {'<', '<', '<', '<', '<', '<', '<', ' ', '='}
};

void readNumber(char** S, Stack<float>& opnd) {
    char* ptr = *S;
    bool isDecimal = false;
    float integerPart = 0, decimalPart = 0;
    int decimalDigits = 0;

    while (isdigit(*ptr)) {
        integerPart = integerPart * 10 + (*ptr - '0');
        ptr++;
    }
    if (*ptr == '.') {
        isDecimal = true;
        ptr++;
        while (isdigit(*ptr)) {
            decimalPart = decimalPart * 10 + (*ptr - '0');
            decimalDigits++;
            ptr++;
        }
    }

    float num = integerPart;
    if (isDecimal) {
        for (int i = 0; i < decimalDigits; i++) {
            decimalPart /= 10;
        }
        num += decimalPart;
    }

    opnd.push(num);
    *S = ptr;
}

char orderBetween(char topOp, char currOp) {
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
        default: exit(-1);
        }
        };

    int row = getIndex(topOp);
    int col = getIndex(currOp);
    return pri[row][col];
}

// 修复阶乘运算（单目运算符!）
float calcu(char op, float opnd) {
    switch (op) {
    case '!': {
        int n = static_cast<int>(opnd);
        if (n < 0 || opnd != n) {  // 仅支持非负整数
            cerr << "错误：阶乘仅支持非负整数" << endl;
            exit(-1);
        }
        float result = 1;
        for (int i = 2; i <= n; ++i) {
            result *= i;
        }
        return result;
    }
    default: exit(-1);
    }
}

// 添加幂运算^实现
float calcu(float opnd1, char op, float opnd2) {
    switch (op) {
    case '+': return opnd1 + opnd2;
    case '-': return opnd1 - opnd2;
    case '*': return opnd1 * opnd2;
    case '/':
        if (opnd2 == 0) {
            cerr << "错误：除零异常" << endl;
            exit(-1);
        }
        return opnd1 / opnd2;
    case '^': return pow(opnd1, opnd2);  // 幂运算
    default: exit(-1);
    }
}

// 移除缓冲区，仅计算表达式结果
float evaluate(char* S) {
    Stack<float> opnd;   // 操作数栈
    Stack<char> optr;    // 运算符栈
    optr.push('\0');     // 哨兵

    while (!optr.empty()) {
        if (isdigit(*S)) {
            readNumber(&S, opnd);  // 读取数字入栈，不构建RPN
        }
        else {
            char rel = orderBetween(optr.top(), *S);
            switch (rel) {
            case '<':
                optr.push(*S);
                S++;
                break;
            case '=':
                optr.pop();
                S++;
                break;
            case '>': {
                char op = optr.top();
                optr.pop();

                if (op == '!') {  // 单目运算
                    float p = opnd.top();
                    opnd.pop();
                    opnd.push(calcu(op, p));
                }
                else {  // 双目运算
                    float p2 = opnd.top(); opnd.pop();
                    float p1 = opnd.top(); opnd.pop();
                    opnd.push(calcu(p1, op, p2));
                }
                break;
            }
            default:
                cerr << "错误：表达式语法错误" << endl;
                exit(-1);
            }
        }
    }
    return opnd.top();  // 返回计算结果
}
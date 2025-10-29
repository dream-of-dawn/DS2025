#include"Vector.h"
#include <iostream>
#include <cstring>
#include <cstdlib> // ����atoi��atof
#include <cctype>  // ����isdigit
#define N_OPTR 9 // �����������
using namespace std;

template <typename T> class Stack : public Vector<T> { //ջģ����
	public:
	Stack(int c = DEFAULT_CAPACITY, int s = 0, T v = 0) : Vector<T>(c, s, v) {} //���캯��
	Stack(T const* A, Rank n) : Vector<T>(A, n) {} //�������帴��
	Stack(T const* A, Rank lo, Rank hi) : Vector<T>(A, lo, hi) {} //���临��
	Stack(Vector<T> const& V) : Vector<T>(V) {} //�������帴��
	Stack(Vector<T> const& V, Rank lo, Rank hi) : Vector<T>(V, lo, hi) {} //���临��
	void push(T const& e) { insert(this->size(), e); } //��ջ����
	T pop() { return this->remove(this->size() - 1); } //��ջ����
	T top() const { return (*this)[this->size() - 1]; } //����ջ��Ԫ��
};
//����ƥ���㷨�������棩
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
//���ȼ���
typedef enum { ADD, SUB, MUL, DIV, POW, FAC, L_P, R_P, EOE } Operator; //�����ö������
// �ӡ������ˡ�����ȡģ���˷��������š������š����ʽ������
const char pri[N_OPTR][N_OPTR] = {
    //+    -    *    /    ^    !    (    )    \0
    {'>', '>', '<', '<', '<', '<', '<', '>', '>'}, // ջ�� +
    {'>', '>', '<', '<', '<', '<', '<', '>', '>'}, // ջ�� -
    {'>', '>', '>', '>', '<', '<', '<', '>', '>'}, // ջ�� *
    {'>', '>', '>', '>', '<', '<', '<', '>', '>'}, // ջ�� /
    {'>', '>', '>', '>', '>', '<', '<', '>', '>'}, // ջ�� ^���˷���
    {'>', '>', '>', '>', '>', '>', '<', '>', '>'}, // ջ�� !���׳ˣ�
    {'<', '<', '<', '<', '<', '<', '<', '=', ' '}, // ջ�� (
    {'>', '>', '>', '>', '>', '>', ' ', '>', '>'}, // ջ�� )
    {'<', '<', '<', '<', '<', '<', '<', ' ', '='}  // ջ�� \0��EOE��
};

// ׷�Ӳ��������沨��ʽRPN������RPN���㹻����ַ����飩
void append(char* RPN, float num) {
    char numStr[20];
    sprintf(numStr, "%.2f ", num); // ��ʽ��������Ϊ�ַ���������2λС����
    strcat(RPN, numStr);
}

// ׷����������沨��ʽRPN
void append(char* RPN, char op) {
    char opStr[2] = { op, ' ' };
    strcat(RPN, opStr);
}

// ��ȡ���ʽ�е����֣������λ����С���������������ջopnd
void readNumber(char** S, Stack<float>& opnd) {
    char* ptr = *S;
    bool isDecimal = false; // �Ƿ���С��
    float integerPart = 0, decimalPart = 0;
    int decimalDigits = 0;

    // ��ȡ��������
    while (isdigit(*ptr)) {
        integerPart = integerPart * 10 + (*ptr - '0');
        ptr++;
    }
    // ��ȡС�����֣�����У�
    if (*ptr == '.') {
        isDecimal = true;
        ptr++;
        while (isdigit(*ptr)) {
            decimalPart = decimalPart * 10 + (*ptr - '0');
            decimalDigits++;
            ptr++;
        }
    }

    // ����������ֵ
    float num = integerPart;
    if (isDecimal) {
        for (int i = 0; i < decimalDigits; i++) {
            decimalPart /= 10;
        }
        num += decimalPart;
    }

    opnd.push(num);   // ѹ�������ջ
    *S = ptr; // ���±��ʽָ�뵽��ǰλ��
}

// �ж�ջ��������뵱ǰ����������ȼ���ϵ������ '<' '=' '>'
char orderBetween(char topOp, char currOp) {
    // ӳ�������������
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
        default: exit(-1); // �Ƿ������
        }
        };

    int row = getIndex(topOp);
    int col = getIndex(currOp);
    return pri[row][col];
}

// ���㣨֧��һԪ����Ԫ�������
float calcu(char op, float opnd) { // һԪ����
    switch (op) {
    case '!': return -opnd; // ����'!'��һԪȡ��
    default: exit(-1);
    }
}

float calcu(float opnd1, char op, float opnd2) { // ��Ԫ����
    switch (op) {
    case '+': return opnd1 + opnd2;
    case '-': return opnd1 - opnd2;
    case '*': return opnd1 * opnd2;
    case '/':
        if (opnd2 == 0) exit(-1); // �������
        return opnd1 / opnd2;
    default: exit(-1);
    }
}

// ���ʽ��ֵ+�沨��ʽת��
float evaluate(char* S, char* RPN) {
    Stack<float> opnd;   // ������ջ
    Stack<char> optr;    // �����ջ
    optr.push('\0');     // �ڱ���ջ

    while (!optr.empty()) {
        if (isdigit(*S)) { // ��ǰ�ַ��ǲ�����
            readNumber(&S, opnd);
            append(RPN, opnd.top()); // ׷�ӵ��沨��ʽ
        }
        else { // ��ǰ�ַ��������
            char rel = orderBetween(optr.top(), *S);
            switch (rel) {
            case '<': // ջ�����ȼ��ͣ���ǰ�������ջ
                optr.push(*S);
                S++;
                break;
            case '=': // ���ȼ���ȣ�����ƥ����ڱ���
                optr.pop();
                S++;
                break;
            case '>': { // ջ�����ȼ��ߣ����㲢ѹջ
                char op = optr.top();
                optr.pop();
                append(RPN, op); // ׷����������沨��ʽ

                if (op == '!') { // һԪ�����
                    float p = opnd.top();
                    opnd.pop();
                    opnd.push(calcu(op, p));
                }
                else { // ��Ԫ�����
                    float p2 = opnd.top(); opnd.pop();
                    float p1 = opnd.top(); opnd.pop();
                    opnd.push(calcu(p1, op, p2));
                }
                break;
            }
            default:
                exit(-1); // �﷨����
            }
        }
    }
    return opnd.top(); // �������ս��
}

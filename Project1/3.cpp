#include <iostream>
#include "Vector.h"
#include "Stack.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

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
            // �����ȣ���߽�Ϊ��ջ������+1���ұ߽�Ϊ��ǰ����-1
            int width = st.empty() ? i : i - st.top() - 1;
            max_area = max(max_area, height * width);
        }
        st.push(i);
    }

    // ����ջ��ʣ������ӣ��Ҳ����޸��͵����ӣ�
    while (!st.empty()) {
        int height = heights[st.top()];
        st.pop();
        // �����ȣ���߽�Ϊ��ջ������+1���ұ߽�Ϊ����ĩβ
        int width = st.empty() ? n : n - st.top() - 1;
        max_area = max(max_area, height * width);
    }

    return max_area;
}


int main() {
    srand(time(0));  // ��ʼ����������ӣ�ȷ��ÿ���������ݲ�ͬ
    for (int i = 0; i < 10; ++i) {
        int length = 10; 
        Vector<int> heights(length);
        for (int j = 0; j < length; ++j) {
            heights[j] = rand() % 105;
        }
        int maxArea = largestRectangleArea(heights);
        cout <<maxArea << endl;
    }

    return 0;
}
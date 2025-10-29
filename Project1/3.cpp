#include <iostream>
#include "Vector.h"
#include "Stack.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

// 计算柱状图中最大矩形面积的函数
int largestRectangleArea(Vector<int>& heights) {
    Stack<int> st;  // 单调栈，存储柱子索引，维护高度递增的序列
    int max_area = 0;
    int n = heights.size();

    for (int i = 0; i < n; ++i) {
        // 当当前柱子高度小于栈顶柱子高度时，计算栈顶柱子能形成的最大矩形
        while (!st.empty() && heights[i] < heights[st.top()]) {
            int height = heights[st.top()];
            st.pop();
            // 计算宽度：左边界为新栈顶索引+1，右边界为当前索引-1
            int width = st.empty() ? i : i - st.top() - 1;
            max_area = max(max_area, height * width);
        }
        st.push(i);
    }

    // 处理栈中剩余的柱子（右侧已无更低的柱子）
    while (!st.empty()) {
        int height = heights[st.top()];
        st.pop();
        // 计算宽度：左边界为新栈顶索引+1，右边界为数组末尾
        int width = st.empty() ? n : n - st.top() - 1;
        max_area = max(max_area, height * width);
    }

    return max_area;
}


int main() {
    srand(time(0));  // 初始化随机数种子，确保每次运行数据不同
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
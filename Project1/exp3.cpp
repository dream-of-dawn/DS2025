#include "Graph.h"
#include <iostream>
#include<string>
using namespace std;
template<typename Tv, typename Te>
void printAdjacencyMatrix(GraphMatrix<Tv, Te>& graph) {
    cout << "邻接矩阵" << endl;
    cout << "  ";
    for (int i = 0; i < graph.n; i++) {
        cout << graph.vertex(i) << " ";
    }
    cout << endl;
    
    for (int i = 0; i < graph.n; i++) {
        cout << graph.vertex(i) << " ";
        for (int j = 0; j < graph.n; j++) {
            if (graph.exists(i, j)) {
                cout << graph.weight(i, j) << " ";
            } else {
                cout << "0 ";
            }
        }
        cout << endl;
    }
    cout << endl;
}

template<typename Tv, typename Te>
void printTraversal(GraphMatrix<Tv, Te>& graph, const string& algorithm) {
    cout << algorithm << "遍历顺序: ";
    for (int i = 0; i < graph.n; i++) {
        if (graph.status(i) == VISITED) {
            cout << graph.vertex(i) << "(" << graph.dTime(i) << ") ";
        }
    }
    cout << endl;
}

template<typename Tv, typename Te>
void printShortestPath(GraphMatrix<Tv, Te>& graph, int start) {
    cout << "从顶点 " << graph.vertex(start) << " 出发的最短路径:" << endl;
    for (int i = 0; i < graph.n; i++) {
        if (i != start) {
            cout << "到顶点" << graph.vertex(i) << ",距离=" << graph.priority(i);
            if (graph.parent(i) != -1) {
                cout << ", 距离=";
                Stack<Tv> path;
                int current = i;
                while (current != -1) {
                    path.push(graph.vertex(current));
                    current = graph.parent(current);
                }
                while (!path.empty()) {
                    cout << path.pop();
                    if (!path.empty()) cout << "->";
                }
            }
            cout << endl;
        }
    }
}
int main() {
    GraphMatrix<char, int> g1;
    // A(0), B(1), C(2), D(3), E(4), F(5), G(6), H(7)
	char a[8] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H' };
	for (int i = 0; i < 8; i++) g1.insert(a[i]);
    g1.insert(0, 4, 0, 1);  // A-B (4)
	g1.insert(0, 4, 1, 0);  // B-A (4)
    g1.insert(0, 6, 0, 3);  // A-D (6)
	g1.insert(0, 6, 3, 0);  // D-A (6)
    g1.insert(0, 7, 0, 6);  // A-G (7)
	g1.insert(0, 7, 6, 0);  // G-A (7)
    g1.insert(0, 12, 1, 2);  // B-C (12)
	g1.insert(0, 12, 2, 1);  // C-B (12)
	g1.insert(0, 9, 2, 3);  // C-D (9)
	g1.insert(0, 9, 3, 2);  // D-C (9)
	g1.insert(0, 1, 2, 4);  // C-E (1)
	g1.insert(0, 1, 4, 2);  // E-C (1)
	g1.insert(0, 2, 2, 5);  // C-F (2)
	g1.insert(0, 2, 5, 2);  // F-C (2)
	g1.insert(0, 10, 2, 7);  // C-H (10)
	g1.insert(0, 10, 7, 2);  // H-C (10)
	g1.insert(0, 2, 3, 6);  // D-G (2)
	g1.insert(0, 2, 6, 3);  // G-D (2)
	g1.insert(0, 13,3,4);  // D- E(13)
	g1.insert(0, 13, 4, 3);  // E-D (13)
	g1.insert(0, 5, 4, 5);  // E-F (5)
	g1.insert(0, 5, 5, 4);  // F-E (5)
	g1.insert(0, 11, 4, 6);  // E-G (11)
	g1.insert(0, 11, 6, 4);  // G-E (11)
	g1.insert(0, 8, 4, 7);  // E-H (8)
	g1.insert(0, 8, 7, 4);  // H-E (8)
	g1.insert(0, 3, 5, 7);  // F-H (3)
	g1.insert(0, 3, 7, 5);  // H-F (3)
	g1.insert(0, 14, 6, 7); // G-H (14)
	g1.insert(0, 14, 7, 6); // H-G (14)
    printAdjacencyMatrix(g1);
    g1.bfs(0); 
    printTraversal(g1, "BFS");
    g1.dfs(0); 
    printTraversal(g1, "BFS");
    g1.dijkstra(0);
    printShortestPath(g1, 0);
    g1.prim(0);
    cout << "最小值支撑树构建完成" << endl;
    GraphMatrix<char, int> g2;
    char b[12] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L' };
    for (int i = 0; i < 12; i++) g2.insert(b[i]);
    // 插入边（无向图，双向插入）
    // 修正后的边插入（所有索引在0-11范围内）
    g2.insert(0, 2, 0, 2);  g2.insert(0, 2, 2, 0);   // A-C (0-2)
    g2.insert(0, 2, 0, 4);  g2.insert(0, 2, 4, 0);   // A-E (0-4)
    g2.insert(0, 2, 2, 5);  g2.insert(0, 2, 5, 2);   // C-F (2-5)
    g2.insert(0, 2, 4, 5);  g2.insert(0, 2, 5, 4);   // E-F (4-5)
    g2.insert(0, 2, 4, 8);  g2.insert(0, 2, 8, 4);   // E-I (4-8)
    g2.insert(0, 2, 5, 8);  g2.insert(0, 2, 8, 5);   // F-I (5-8)
    g2.insert(0, 2, 5, 9);  g2.insert(0, 2, 9, 5);   // F-J (5-9)
    g2.insert(0, 2, 9, 10); g2.insert(0, 2, 10, 9);  // J-K (9-10)  <- 修正
    g2.insert(0, 2, 5, 10); g2.insert(0, 2, 10, 5);  // F-K (5-10)  <- 修正
    g2.insert(0, 2, 5, 6);  g2.insert(0, 2, 6, 5);   // F-G (5-6)
    g2.insert(0, 2, 6, 10); g2.insert(0, 2, 10, 6);  // G-K (6-10)  <- 修正
    g2.insert(0, 2, 10, 11);g2.insert(0, 2, 11, 10); // K-L (10-11) <- 修正
    g2.insert(0, 2, 2, 3);  g2.insert(0, 2, 3, 2);   // C-D (2-3)
    g2.insert(0, 2, 2, 7);  g2.insert(0, 2, 7, 2);   // C-H (2-7)
    g2.insert(0, 2, 3, 7);  g2.insert(0, 2, 7, 3);   // D-H (3-7)
    int starts[] = { 0, 3 }; // A, D
    for (int i = 0; i < 2; i++) {
        int start = starts[i];
        char startName = b[i];
        cout << "\n=== 从顶点" << startName << " 出发的双连通分量分析 ===" << endl;
        g2.bcc(start);
        cout << "双连通分量计算完成" << endl;
    }
    return 0;
}
#ifndef HUFFMAN_H
#define HUFFMAN_H
#include "bintree.h"
#include "bitmap.h"
#include <string>
#include <map>
#include <queue>
#include <vector>
#include <algorithm>

// Huffman树节点数据
struct HuffChar {
    char ch;      // 字符
    int freq;     // 频率
    
    HuffChar(char c = 0, int f = 0) : ch(c), freq(f) {}
    
    // 重载比较运算符，用于优先队列
    bool operator>(const HuffChar& other) const {
        return freq > other.freq;
    }
};

// Huffman编码树
class HuffTree {
private:
    BinTree
<HuffChar>* tree;
    std
::map<char, std::string> codeTable;  // 编码表
    
    void generateCodes(BinNodePosi(HuffChar) node, std::string code) {
        if (!node) return;
        
        // 叶子节点，存储编码
        if (!node->lc && !node->rc) {
            codeTable
[node->data.ch] = code;
            return;
        }
        
        // 左子树编码加'0'
        if (node->lc) generateCodes(node->lc, code + "0");
        // 右子树编码加'1'
        if (node->rc) generateCodes(node->rc, code + "1");
    }
    
public:
    HuffTree() : tree(nullptr) {}
    
    ~HuffTree() {
        if (tree) delete tree;
    }
    
    // 构建Huffman树
    void build(const std::string& text) {
        // 统计字符频率（只考虑26个字母，不分大小写）
        std
::map<char, int> freqMap;
        for (char c : text) {
            if (isalpha(c)) {
                char lowerC = tolower(c);
                freqMap
[lowerC]++;
            }
        }
        
        // 使用优先队列（最小堆）
        auto cmp = [](BinTree<HuffChar>* a, BinTree<HuffChar>* b) {
            return a->root()->data.freq > b->root()->data.freq;
        };
        std
::priority_queue<BinTree<HuffChar>*, std::vector<BinTree<HuffChar>*>, decltype(cmp)> pq(cmp);
        
        // 为每个字符创建单节点树
        for (const auto& pair : freqMap) {
            BinTree
<HuffChar>* singleTree = new BinTree<HuffChar>();
            singleTree
->insertAsRoot(HuffChar(pair.first, pair.second));
            pq
.push(singleTree);
        }
        
        // 构建Huffman树
        while (pq.size() > 1) {
            // 取出两个频率最小的树
            BinTree
<HuffChar>* left = pq.top(); pq.pop();
            BinTree
<HuffChar>* right = pq.top(); pq.pop();
            
            // 创建新树，根节点频率为左右子树频率之和
            BinTree
<HuffChar>* newTree = new BinTree<HuffChar>();
            int sumFreq = left->root()->data.freq + right->root()->data.freq;
            newTree
->insertAsRoot(HuffChar(0, sumFreq));
            
            // 连接左右子树
            newTree
->attachAsLC(newTree->root(), left);
            newTree
->attachAsRC(newTree->root(), right);
            
            pq
.push(newTree);
        }
        
        // 最后剩下的树就是Huffman树
        if (!pq.empty()) {
            tree 
= pq.top();
            // 生成编码表
            generateCodes(tree->root(), "");
        }
    }
    
    // 获取字符的编码
    std
::string getCode(char c) {
        char lowerC = tolower(c);
        auto it = codeTable.find(lowerC);
        if (it != codeTable.end()) {
            return it->second;
        }
        return "";
    }
    
    // 显示编码表
    void displayCodeTable() {
        std
::cout << "Huffman Code Table:" << std::endl;
        for (const auto& pair : codeTable) {
            std
::cout << "'" << pair.first << "': " << pair.second << std::endl;
        }
    }
    
    // 获取树根
    BinNodePosi(HuffChar) root() const {
        return tree ? tree->root() : nullptr;
    }
};

// Huffman编码串类型
class HuffCode {
private:
    Bitmap
* bitmap;
    
public:
    HuffCode() : bitmap(new Bitmap()) {}
    
    ~HuffCode() {
        delete bitmap;
    }
    
    // 编码字符串
    void encode(const std::string& str, HuffTree& huffTree) {
        int bitPos = 0;
        for (char c : str) {
            if (isalpha(c)) {
                std
::string code = huffTree.getCode(c);
                for (char bit : code) {
                    if (bit == '1') {
                        bitmap
->set(bitPos);
                    } else {
                        bitmap
->clear(bitPos);
                    }
                    bitPos
++;
                }
            }
        }
    }
    
    // 获取编码的字符串表示
    std
::string toString(int length) {
        char* str = bitmap->bits2string(length);
        std
::string result(str);
        delete[] str;
        return result;
    }
    
    // 获取位图
    Bitmap
* getBitmap() const {
        return bitmap;
    }
};

#endif
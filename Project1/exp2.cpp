#include <iostream>
#include <fstream>
#include <string>
#include "huffman.h"

// 读取《I have a dream》演讲原文
std::string readSpeechText() {
    // 这里应该是马丁路德金的演讲原文
    // 由于篇幅限制，这里使用简化版本，实际应该从文件读取完整原文
    return "I have a dream that one day this nation will rise up and live out the true meaning of its creed We hold these truths to be self evident that all men are created equal";
}

int main() {
    // 读取演讲原文
    std::string speechText = readSpeechText();
    std::cout << "Original text length: " << speechText.length() << std::endl;
    
    // 构建Huffman树
    HuffTree huffTree;
    huffTree.build(speechText);
    
    // 显示编码表
    huffTree.displayCodeTable();
    
    // 编码单词 "dream"
    std::string word1 = "dream";
    HuffCode code1;
    code1.encode(word1, huffTree);
    
    // 计算编码后的位长度
    int bitLength1 = 0;
    for (char c : word1) {
        if (isalpha(c)) {
            bitLength1 += huffTree.getCode(tolower(c)).length();
        }
    }
    
    std::cout << "\nWord: '" << word1 << "'" << std::endl;
    std::cout << "Encoded: " << code1.toString(bitLength1) << std::endl;
    std::cout << "Code length: " << bitLength1 << " bits" << std::endl;
    
    // 编码其他单词示例
    std::string word2 = "freedom";
    HuffCode code2;
    code2.encode(word2, huffTree);
    
    int bitLength2 = 0;
    for (char c : word2) {
        if (isalpha(c)) {
            bitLength2 += huffTree.getCode(tolower(c)).length();
        }
    }
    
    std::cout << "\nWord: '" << word2 << "'" << std::endl;
    std::cout << "Encoded: " << code2.toString(bitLength2) << std::endl;
    std::cout << "Code length: " << bitLength2 << " bits" << std::endl;
    
    // 编码另一个单词
    std::string word3 = "equal";
    HuffCode code3;
    code3.encode(word3, huffTree);
    
    int bitLength3 = 0;
    for (char c : word3) {
        if (isalpha(c)) {
            bitLength3 += huffTree.getCode(tolower(c)).length();
        }
    }
    
    std::cout << "\nWord: '" << word3 << "'" << std::endl;
    std::cout << "Encoded: " << code3.toString(bitLength3) << std::endl;
    std::cout << "Code length: " << bitLength3 << " bits" << std::endl;
    
    return 0;
}
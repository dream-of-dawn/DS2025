#include <iostream>
#include <fstream>
#include <string>
#include "../Project1/huffman.h"

// 从dream.txt文件中读取《I have a dream》演讲全文
std::string readSpeechText() {
    std::ifstream inputFile("dream.txt");
    std::string speechText, line;
    
    if (!inputFile.is_open()) {
        std::cerr << "错误：无法打开文件 dream.txt" << std::endl;
        return "";
    }
    
    // 逐行读取文件内容
    while (std::getline(inputFile, line)) {
        speechText += line + " ";  // 添加空格分隔行
    }
    
    inputFile.close();
    return speechText;
}

int main() {
    // 读取演讲原文
    std::string speechText = readSpeechText();
    
    // 检查是否成功读取文件
    if (speechText.empty()) {
        std::cerr << "程序终止：无法读取演讲文本" << std::endl;
        return 1;
    }
    
    std::cout << "演讲文本长度: " << speechText.length() << " 个字符" << std::endl;
    
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
    
    std::cout << "\n单词: '" << word1 << "'" << std::endl;
    std::cout << "编码结果: " << code1.toString(bitLength1) << std::endl;
    std::cout << "编码长度: " << bitLength1 << " 位" << std::endl;
    
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
    
    std::cout << "\n单词: '" << word2 << "'" << std::endl;
    std::cout << "编码结果: " << code2.toString(bitLength2) << std::endl;
    std::cout << "编码长度: " << bitLength2 << " 位" << std::endl;
    
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
    
    std::cout << "\n单词: '" << word3 << "'" << std::endl;
    std::cout << "编码结果: " << code3.toString(bitLength3) << std::endl;
    std::cout << "编码长度: " << bitLength3 << " 位" << std::endl;
    
    return 0;
}
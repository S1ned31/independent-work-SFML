#pragma once
#ifndef HUFFMAN_COMPRESSION_H
#define HUFFMAN_COMPRESSION_H

#include <string>
#include <unordered_map>
#include <queue>
#include <vector>

struct HuffmanNode {
    char ch;
    int freq;
    HuffmanNode* left;
    HuffmanNode* right;
    HuffmanNode(char character, int frequency);
};

class Compare {
public:
    bool operator()(HuffmanNode* a, HuffmanNode* b);
};

class HuffmanCompression {
public:
    static std::unordered_map<char, int> calculateFrequencies(const std::string& text);
    static HuffmanNode* buildHuffmanTree(const std::unordered_map<char, int>& frequencies);
    static void generateHuffmanCodes(HuffmanNode* root, const std::string& code,
        std::unordered_map<char, std::string>& huffmanCodes);
    static std::string compress(const std::string& text, const std::unordered_map<char, std::string>& codes);
    static int calculateCompressedSize(const std::string& compressedText);
    static void freeTree(HuffmanNode* root);
};

#endif // HUFFMAN_COMPRESSION_H

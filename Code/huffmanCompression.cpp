#include "huffmanCompression.h"
#include <iostream>

HuffmanNode::HuffmanNode(char character, int frequency)
    : ch(character), freq(frequency), left(nullptr), right(nullptr) {}

bool Compare::operator()(HuffmanNode* a, HuffmanNode* b) {
    return a->freq > b->freq;
}

std::unordered_map<char, int> HuffmanCompression::calculateFrequencies(const std::string& text) {
    std::unordered_map<char, int> frequencies;
    for (char ch : text) {
        frequencies[ch]++;
    }
    return frequencies;
}

HuffmanNode* HuffmanCompression::buildHuffmanTree(const std::unordered_map<char, int>& frequencies) {
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, Compare> pq;
    for (auto& pair : frequencies) {
        pq.push(new HuffmanNode(pair.first, pair.second));
    }
    while (pq.size() > 1) {
        HuffmanNode* left = pq.top(); pq.pop();
        HuffmanNode* right = pq.top(); pq.pop();
        HuffmanNode* parent = new HuffmanNode('\0', left->freq + right->freq);
        parent->left = left;
        parent->right = right;
        pq.push(parent);
    }
    return pq.top();
}

void HuffmanCompression::generateHuffmanCodes(HuffmanNode* root, const std::string& code,
    std::unordered_map<char, std::string>& huffmanCodes) {
    if (!root) return;
    if (!root->left && !root->right) {
        huffmanCodes[root->ch] = code;
    }
    generateHuffmanCodes(root->left, code + "0", huffmanCodes);
    generateHuffmanCodes(root->right, code + "1", huffmanCodes);
}

std::string HuffmanCompression::compress(const std::string& text, const std::unordered_map<char, std::string>& codes) {
    std::string compressedText;
    for (char ch : text) {
        compressedText += codes.at(ch);
    }
    return compressedText;
}

int HuffmanCompression::calculateCompressedSize(const std::string& compressedText) {
    return compressedText.size();
}

void HuffmanCompression::freeTree(HuffmanNode* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

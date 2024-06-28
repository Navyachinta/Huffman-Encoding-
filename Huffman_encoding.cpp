/*
Author: CH Navya

Implementing Huffman coding and comparing it against a random variable length prefix coding scheme.

1. Implemening a function createFrequencyT able(F ILE∗) that reads the char-
acters from a file and creates the list of symbols together with frequencies.

2. Implementing a function buildHuffmanTree() that builds the Huffman
tree based on the frequency table obtained from Task 1.

3. Implementing a function createCodeTable(treeRoot) that takes the root
of a code tree as input and creates a code table. 

4. Implementing functions encode() and decode().

Finally, comparing the size of the encoded files, corresponding to both the random
coding and Huffman coding schemes.
*/
#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <ctime>
#include <cstdlib>
#include <algorithm>

using namespace std;

// Structure for a node in the code tree
struct TreeNode {
    char symbol;
    int frequency;
    TreeNode *left, *right;
};

// Function to create a frequency table from a file
map<char, int> buildFrequencyTable(const char* filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file." << endl;
        exit(EXIT_FAILURE);
    }
    map<char, int> freqTable;
    char c;
    while (file.get(c)) {
        freqTable[c]++;
    }
    file.close();
    return freqTable;
}

// Comparator for the priority queue used in Huffman tree construction
struct CompareTreeNodes {
    bool operator()(const TreeNode* lhs, const TreeNode* rhs) const {
        return lhs->frequency > rhs->frequency;
    }
};

// Function to create a Huffman tree from a frequency table
TreeNode* buildHuffmanTree(const map<char, int>& freqTable) {
    priority_queue<TreeNode*, vector<TreeNode*>, CompareTreeNodes> pq;
    for (const auto& pair : freqTable) {
        TreeNode* newNode = new TreeNode;
        newNode->symbol = pair.first;
        newNode->frequency = pair.second;
        newNode->left = nullptr;
        newNode->right = nullptr;
        pq.push(newNode);
    }
    while (pq.size() > 1) {
        TreeNode* left = pq.top(); pq.pop();
        TreeNode* right = pq.top(); pq.pop();
        TreeNode* parent = new TreeNode;
        parent->symbol = '\0'; // Internal node symbol
        parent->frequency = left->frequency + right->frequency;
        parent->left = left;
        parent->right = right;
        pq.push(parent);
    }
    return pq.top();
}

// Recursive function to print Huffman tree structure
void printHuffmanTreeStructure(TreeNode* root, int depth = 0) {
    if (root == nullptr) return;
    for (int i = 0; i < depth; ++i) {
        cout << "  ";
    }
    if (root->symbol == '\0') {
        cout << "Internal Node: " << root->frequency << endl;
    } else {
        cout << root->symbol << ": " << root->frequency << endl;
    }
    printHuffmanTreeStructure(root->left, depth + 1);
    printHuffmanTreeStructure(root->right, depth + 1);
}

// Recursive function to traverse the Huffman tree and generate the code table
void generateCodeTable(TreeNode* root, string code, map<char, string>& codeTable) {
    if (root->left == nullptr && root->right == nullptr) {
        codeTable[root->symbol] = code;
        return;
    }
    generateCodeTable(root->left, code + '0', codeTable);
    generateCodeTable(root->right, code + '1', codeTable);
}

// Function to encode a message using a code table
string encodeMessage(const string& message, const map<char, string>& codeTable) {
    string encodedMessage;
    for (char c : message) {
        encodedMessage += codeTable.at(c);
    }
    return encodedMessage;
}

// Function to decode an encoded message using a Huffman tree
string decodeMessage(const string& encodedMessage, TreeNode* root) {
    string decodedMessage;
    TreeNode* current = root;
    for (char bit : encodedMessage) {
        if (bit == '0') {
            current = current->left;
        } else {
            current = current->right;
        }
        if (current->left == nullptr && current->right == nullptr) {
            decodedMessage += current->symbol;
            current = root;
        }
    }
    return decodedMessage;
}

// Function to generate a random code tree
TreeNode* createRandomCodeTree(const string& message) {
    srand(time(0)); // Seed for random number generation
    int size = message.size();
    char* symbolSet = new char[size];
    copy(message.begin(), message.end(), symbolSet);
    TreeNode* root = new TreeNode;
    if (size == 1) {
        root->symbol = symbolSet[0];
        root->left = nullptr;
        root->right = nullptr;
    } else {
        root->symbol = '\0'; // Internal node symbol

        // Randomly determine left subtree size
        int leftSize = rand() % (size - 1) + 1; // Ensure left size is at least 1

        char* leftSet = new char[leftSize];
        char* rightSet = new char[size - leftSize];

        // Populate left and right symbol sets randomly
        for (int i = 0; i < size; ++i) {
            if (i < leftSize) {
                leftSet[i] = symbolSet[i];
            } else {
                rightSet[i - leftSize] = symbolSet[i];
            }
        }
        // Recursively build left and right subtrees
        root->left = createRandomCodeTree(string(leftSet, leftSize));
        root->right = createRandomCodeTree(string(rightSet, size - leftSize));

        delete[] leftSet;
        delete[] rightSet;
    }
    delete[] symbolSet;
    return root;
}

// Recursive function to build encoded message using random code tree
void encodeUsingRandomTree(TreeNode* root, const string& message, string& encodedMessage, string currentCode) {
    if (root->symbol != '\0') {
        encodedMessage += currentCode;
        return;
    }
    encodeUsingRandomTree(root->left, message, encodedMessage, currentCode + '0');
    encodeUsingRandomTree(root->right, message, encodedMessage, currentCode + '1');
}

// Function to encode a message using a random code tree
string encodeWithRandomTree(TreeNode* root, const string& message) {
    string encodedMessage;
    encodeUsingRandomTree(root, message, encodedMessage, "");
    return encodedMessage;
}

// Function to decode a message using a random code tree
string decodeWithRandomTree(TreeNode* root, const string& encodedMessage) {
    string decodedMessage;
    TreeNode* current = root;
    for (char bit : encodedMessage) {
        if (current->symbol == '\0') {
            if (bit == '0') {
                current = current->left;
            } else {
                current = current->right;
            }
        } else {
            decodedMessage += current->symbol;
            current = root;
        }
    }
    if (current->symbol != '\0') {
        decodedMessage += current->symbol;
    }
    return decodedMessage;
}

// Function to print the size of the message in bytes
void printMessageByteSize(const string& message) {
    cout << "Size of message in bytes: " << message.size() << endl;
}

int main() {
    const char* filename = "2203306.txt";
    map<char, int> freqTable = buildFrequencyTable(filename);
    cout << "Frequency Table:" << endl;
    for (const auto& pair : freqTable) {
        cout << pair.first << ": ";
        cout << pair.second << endl;
    }

    // Huffman coding
    TreeNode* huffmanTree = buildHuffmanTree(freqTable);
    map<char, string> codeTable;
    generateCodeTable(huffmanTree, "", codeTable);

    // Print Huffman tree structure
    cout << "Huffman Tree:" << endl;
    printHuffmanTreeStructure(huffmanTree);

    // Encode original message using Huffman coding
    ifstream inputFile(filename);
    string message;
    if (inputFile.is_open()) {
        char c;
        while (inputFile.get(c)) {
            message += c;
        }
        inputFile.close();
    } else {
        cerr << "Error: Unable to open file." << endl;
        return 1;
    }
    cout << "Original message: " << message << endl;
    string encodedMessageHuffman = encodeMessage(message, codeTable);
    cout << "Encoded message (Huffman): " << encodedMessageHuffman << endl;
    printMessageByteSize(encodedMessageHuffman);
    string decodedMessageHuffman = decodeMessage(encodedMessageHuffman, huffmanTree);
    cout << "Decoded message (Huffman): " << decodedMessageHuffman << endl;

    // Random coding
    cout << "Random Coding:" << endl;
    TreeNode* randomCodeRoot = createRandomCodeTree(message);
    string encodedMessageRandom = encodeWithRandomTree(randomCodeRoot, message);
    cout << "Encoded message (Random): " << encodedMessageRandom << endl;
    printMessageByteSize(encodedMessageRandom);
    string decodedMessageRandom = decodeWithRandomTree(randomCodeRoot, encodedMessageRandom);
    cout << "Decoded message (Random): " << decodedMessageRandom << endl;

    // Compare encoded file sizes
    cout << "Comparison of Encoded File Sizes:" << endl;
    cout << "Size of Encoded message (Huffman): " << encodedMessageHuffman.size() << " bytes" << endl;
    cout << "Size of Encoded message (Random): " << encodedMessageRandom.size() << " bytes" << endl;

    return 0;
}


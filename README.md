# Huffman Coding vs Random Variable-Length Prefix Coding

Author: ** Navya chinta**

This project implements Huffman coding and compares its efficiency with a randomly generated variable-length prefix coding scheme. It encodes and decodes messages read from a file and evaluates the compression performance of both methods.

## Features

- Build frequency table from an input file
- Construct Huffman coding tree based on symbol frequencies
- Generate Huffman code table from the tree
- Encode and decode using Huffman coding
- Construct a random prefix-free code tree
- Encode and decode using the random code tree
- Compare encoded message sizes for both methods

## Files

- [`Huffman_encoding.cpp`](https://github.com/Navyachinta/Huffman-Encoding-/blob/main/Huffman_encoding.cpp) – Main implementation file
- [`Huffman_input.txt`](https://github.com/Navyachinta/Huffman-Encoding-/blob/main/Huffman_input.txt) – Input text file

## How It Works

1. **Read File & Build Frequency Table**
   - Reads characters from the given file and counts their frequency.

2. **Huffman Tree Construction**
   - Constructs a binary tree where each leaf represents a symbol and paths represent the Huffman codes.

3. **Code Table Generation**
   - Generates a map from characters to binary codes using the Huffman tree.

4. **Encoding & Decoding**
   - Encodes the original message into a binary string using the code table.
   - Decodes the binary string back to the original message using the Huffman tree.

5. **Random Prefix Coding**
   - Builds a randomly generated binary tree with the same symbols.
   - Encodes and decodes using the random tree.

6. **Comparison**
   - Compares the size (in bits/bytes) of the encoded messages from both schemes.

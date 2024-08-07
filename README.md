# Huffman Coding Project

## Overview

This project implements a file compression and decompression utility using Huffman Coding, a well-known algorithm for lossless data compression.
The utility provides modules for handling binary data, such as `BitReader`, `BitWriter`, `Node`, and `PriorityQueue`, which are integral to the Huffman coding process.
The program efficiently compresses files by encoding data based on frequency and then decompresses them to restore the original data.

## Purpose

The primary goal of this project is to demonstrate how Huffman Coding can be used to optimize data storage and retrieval. By encoding data in a way that frequently occurring symbols use shorter codes, 
and less frequent symbols use longer codes, this program significantly reduces the size of files while retaining all original information.

## How to Use the Program

### Compilation

To compile the program, use the provided Makefile with the following commands:
- `make` or `make all`: Compiles all necessary files.

### Compression

To compress a file, use the following command:
`./huff -i input_file -o output_file`

-`-i`: Specifies the input file to compress.
-`-o`: Specifies the output file where the decompressed data will be stored.

### Additional Options
-`-h`: Displays a help message.
-`-v`: Provides verbose output, giving more information about the file processing (supported in specific versions).

### Example Usage

Compress a file:
`./huff -i data.txt -o compressed.huff`

Decompress a file:
`./dehuff -i compressed.huff -o data.txt`

## Program Design

### Data Structures

The program relies on several key data structures:

**Node Struct**: Represents each symbol and its associated Huffman code, including its frequency, code length, and pointers to left and right child nodes.
**PriorityQueue Struct**: Maintains the nodes in a priority queue based on their frequency, which is essential for building the Huffman tree.

These structures are fundamental in managing the Huffman coding process, from tree construction to encoding and decoding.

### Algorithms

The core algorithms are implemented in two main components:

1. Huffman Encoder (huff.c):
  -Calculates the frequency histogram of the input data.
  -Constructs the Huffman tree based on the frequency data.
  -Encodes the input file by replacing symbols with their corresponding Huffman codes.
   
2. Huffman Decoder (dehuff.c):
  - Reads the encoded file.
  - Reconstructs the Huffman tree.
  - Decodes the file back to its original form.

### Key Functions

- fill_histogram(): Generates a frequency histogram from the input file.
- create_tree(): Constructs the Huffman tree using the frequency histogram.
- huff_compress_file(): Compresses the input file by replacing symbols with their Huffman codes.
- dehuff_decompress_file(): Decompresses the encoded file back to its original content.

## Results

This program successfully compresses and decompresses files as intended. The efficiency of the Huffman Coding algorithm is evident in the reduced file sizes after compression, 
while the integrity of the original data is maintained during decompression. The program has been tested with various input files to ensure its robustness and correctness.

## Conclusion

The Huffman Coding project showcases the power of efficient data structures and algorithms in the context of file compression. By implementing and optimizing these components, 
the project achieves significant file size reductions while ensuring accurate data recovery. This utility serves as a practical example of lossless compression techniques in computer science.


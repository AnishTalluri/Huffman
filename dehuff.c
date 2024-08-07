#include "bitreader.h"
#include "bitwriter.h"
#include "node.h"
#include "pq.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void stack_push(Node **stack, int *top, Node *node) {
    if (*top == 63) {
        fprintf(stderr, "Stack overflow\n");
        exit(1);
    }
    stack[++(*top)] = node;
}

Node *stack_pop(Node **stack, int *top) {
    if (*top == -1) {
        fprintf(stderr, "Stack underflow\n");
        exit(1);
    }
    return stack[(*top)--];
}

void decompressFile(FILE *fout, BitReader *inbuf) {
    uint8_t type1 = bit_read_uint8(inbuf);
    uint8_t type2 = bit_read_uint8(inbuf);

    assert(type1 == 'H' && type2 == 'C');

    uint32_t filesize = bit_read_uint32(inbuf);
    uint16_t num_leaves = bit_read_uint16(inbuf);

    uint16_t num_nodes = 2 * num_leaves - 1;
    Node *node;
    Node *stack[64];
    int top = -1;

    for (uint16_t i = 0; i < num_nodes; ++i) {
        int bit = bit_read_bit(inbuf);
        if (bit == 1) {
            uint8_t symbol = bit_read_uint8(inbuf);
            node = node_create(symbol, 0);
        } else {
            node = node_create(0, 0);
            node->right = stack_pop(stack, &top);
            node->left = stack_pop(stack, &top);
        }
        stack_push(stack, &top, node);
    }

    Node *code_tree = stack_pop(stack, &top);

    for (uint32_t i = 0; i < filesize; ++i) {
        node = code_tree;
        while (1) {
            int bit = bit_read_bit(inbuf);
            if (bit == 0) {
                node = node->left;
            } else {
                node = node->right;
            }
            if (node->left == NULL && node->right == NULL) {
                fputc(node->symbol, fout);
                break;
            }
        }
    }
    node_free(&code_tree);
}

void print_help(void) {
    printf("Usage: huff/dehuff -i infile -o outfile\n");
    printf("       huff -h\n");
}

int main(int argc, char *argv[]) {
    int opt;
    char *input_file = NULL;
    char *output_file = NULL;

    while ((opt = getopt(argc, argv, "i:o:")) != -1) {
        switch (opt) {
        case 'i': input_file = optarg; break;
        case 'o': output_file = optarg; break;
        default: fprintf(stderr, "Usage: %s -i input_file -o output_file\n", argv[0]); return 1;
        }
    }

    if (input_file == NULL || output_file == NULL) {
        fprintf(stderr, "Usage: %s -i input_file -o output_file\n", argv[0]);
        return 1;
    }

    FILE *infile = fopen(input_file, "rb");
    if (infile == NULL) {
        perror("Error opening input file");
        return 1;
    }

    FILE *outfile = fopen(output_file, "wb");
    if (outfile == NULL) {
        perror("Error opening output file");
        fclose(infile);
        return 1;
    }

    BitReader *inbuf = bit_read_open(input_file);
    if (inbuf == NULL) {
        perror("Error opening input file as BitReader");
        fclose(infile);
        fclose(outfile);
        return 1;
    }

    decompressFile(outfile, inbuf);

    fclose(infile);
    fclose(outfile);
    bit_read_close(&inbuf);

    return 0;
}

#include "bitreader.h"
#include "bitwriter.h"
#include "node.h"
#include "pq.h"

#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

typedef struct Code {
    uint64_t code;
    uint8_t code_length;
} Code;

uint32_t fill_histogram(FILE *fin, uint32_t *histogram) {
    for (int i = 0; i < 256; i++)
        histogram[i] = 0;

    ++histogram[0x00];
    ++histogram[0xff];

    uint32_t size = 0;
    int byte;
    while ((byte = fgetc(fin)) != EOF) {
        ++histogram[byte];
        ++size;
    }

    fseek(fin, 0, SEEK_SET);

    return size;
}

Node *create_tree(uint32_t *histogram, uint16_t *num_leaves) {
    PriorityQueue *pq = pq_create();

    for (int i = 0; i < 256; i++) {
        if (histogram[i] != 0) {
            Node *new_node = node_create((uint8_t) i, histogram[i]);
            enqueue(pq, new_node);
            (*num_leaves) += 1;
        }
    }

    while (!pq_size_is_1(pq)) {
        Node *left = dequeue(pq);
        Node *right = dequeue(pq);

        Node *new_node = node_create(0, (uint32_t) (left->weight + right->weight));
        new_node->left = left;
        new_node->right = right;

        enqueue(pq, new_node);
    }

    Node *root = dequeue(pq);
    pq_free(&pq);

    return root;
}

void fill_code_table(Code *code_table, Node *node, uint64_t code, uint8_t code_length) {
    if (node == NULL)
        return;

    if (node->left != NULL || node->right != NULL) {
        fill_code_table(code_table, node->left, code, code_length + 1);

        code |= (uint64_t) 1 << code_length;

        fill_code_table(code_table, node->right, code, code_length + 1);

        code &= ~((uint64_t) 1 << code_length);
    } else {
        code_table[node->symbol].code = code;
        code_table[node->symbol].code_length = code_length;
    }
}

void huff_write_tree(BitWriter *outbuf, Node *node) {
    if (node->left == NULL) {
        bit_write_bit(outbuf, 1);
        bit_write_uint8(outbuf, node->symbol);
    } else {
        huff_write_tree(outbuf, node->left);
        huff_write_tree(outbuf, node->right);
        bit_write_bit(outbuf, 0);
    }
}

void huff_compress_file(BitWriter *outbuf, FILE *fin, uint32_t filesize, uint16_t num_leaves,
    Node *code_tree, Code *code_table) {
    bit_write_uint8(outbuf, 'H');
    bit_write_uint8(outbuf, 'C');
    bit_write_uint32(outbuf, filesize);
    bit_write_uint16(outbuf, num_leaves);
    huff_write_tree(outbuf, code_tree);
    while (true) {
        int b = fgetc(fin);
        if (b == EOF) {
            break;
        }
        uint64_t code = code_table[b].code;
        uint8_t code_length = code_table[b].code_length;
        for (uint8_t i = 0; i < code_length; i++) {
            bit_write_bit(outbuf, (code & 1));
            code >>= 1;
        }
    }
}

void print_help(void) {
    printf("Usage: huff -i infile -o outfile\n");
    printf("       huff -h\n");
}

int main(int argc, char **argv) {
    int opt = 0;
    BitReader *br = NULL;
    BitWriter *bw = NULL;
    FILE *infile = stdin;

    int input_flag = 0;
    int output_flag = 0;

    if (argc == 1) {
        printf("huff:  -i option is required\n");
        print_help();
        return 1;
    }

    if (argc == 2) {
        printf("huff:  unknown or poorly formatted option %s\n", argv[1]);
        print_help();
        return 1;
    }

    while ((opt = getopt(argc, argv, "vhi:o:")) != -1) {
        switch (opt) {
        case 'h': print_help(); return 1;
        case 'i':
            br = bit_read_open(optarg);
            if (br == NULL) {
                printf("huff:  error reading input file %s\n", optarg);
                print_help();
                bit_read_close(&br);
                return 1;
            }
            infile = fopen(optarg, "r");
            if (infile == NULL) {
                fclose(infile);
                return 1;
            }
            input_flag = 1;
            break;
        case 'o':
            bw = bit_write_open(optarg);
            if (bw == NULL) {
                bit_write_close(&bw);
                return 1;
            }
            output_flag = 1;
            break;
        }
    }

    if (input_flag == 0) {
        printf("huff: -i option is required\n");
        print_help();
        return 1;
    }

    if (output_flag == 0) {
        printf("huff: -o option is required\n");
        print_help();
        return 1;
    }

    uint32_t histogram[256];
    uint32_t filesize = fill_histogram(infile, histogram);

    uint16_t num_leaves = 0;
    Node *code_tree = create_tree(histogram, &num_leaves);

    Code *code_table = (Code *) calloc(256, sizeof(Code));
    fill_code_table(code_table, code_tree, 0, 0);

    huff_compress_file(bw, infile, filesize, num_leaves, code_tree, code_table);

    node_free(&code_tree);
    free(code_table);

    fclose(infile);
    infile = NULL;
    assert(infile == NULL);
    bit_write_close(&bw);
    bit_read_close(&br);
    assert(br == NULL);
    assert(bw == NULL);

    return 0;
}

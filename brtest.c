/*
* File:     brtest.c
* Purpose:  Test bitreader.c
* Author:   Kerry Veenstra
*/

#include "bitreader.h"

#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define V1(BIT)   assert(bit_read_bit(buf) == (BIT))
#define V8(BYTE)  assert(bit_read_uint8(buf) == (BYTE))
#define V16(WORD) assert(bit_read_uint16(buf) == (WORD))
#define V32(WORD) assert(bit_read_uint32(buf) == (WORD))

int main(void) {
    /*
    * Create a file.
    */
    FILE *f = fopen("brtest.in", "w");
    if (f == NULL) {
        fprintf(stderr, "error creating brtest.in\n");
        exit(1);
    }
    fprintf(f, "ABCDEFGHIJKLMN\xFF\xAA\x55\n");
    fputc(0x00, f);
    if (fclose(f) == EOF) {
        fprintf(stderr, "error closing brtest.in\n");
        exit(1);
    }

    /*
    * Verify the file.
    */
    BitReader *buf = bit_read_open("brtest.in");
    if (!buf) {
        fprintf(stderr, "error opening brtest.in\n");
        exit(1);
    }

    /*
    * Read the bits from a file using all of the bitreader.c functions.
    * We deliberately choose bits that created a text file.
    */

    /*
    * 0x41 'A' 01000001
    * 0x42 'B' 01000010
    *
    * 'A' followed by 'B' is 0x41 followed by 0x42.
    * In binary that's 01000010 01000001, which can be read as bits/byte.
    *                  ^^^^^^^\_______/^
    *                  |     |     ^   |___ start with this bit 1
    *                  | ... |     |_______ then the byte 00100000 = 0x20
    *                  |     |_____________ etc.
    *                  |___________________
    */
    V1(1);
    V8(0x20);
    V1(1);
    V1(0);
    V1(0);
    V1(0);
    V1(0);
    V1(1);
    V1(0);

    /*
    * 0x43 'C' 01000011
    * 0x44 'D' 01000100
    * 0x45 'E' 01000101
    * 0x46 'F' 01000110
    * 0x47 'G' 01000111
    *
    * 01000111 01000110 01000101 01000100 01000011
    * ^^^^^\__________________________________/^^^
    *       11101000 11001000 10101000 10001000
    */
    V1(1);
    V1(1);
    V1(0);
    V32(0xe8c8a888);
    V1(0);
    V1(0);
    V1(0);
    V1(1);
    V1(0);

    /*
    * 0x48 'H' 01001000
    * 0x49 'I' 01001001
    * 0x4a 'J' 01001010
    *
    * 01001010 01001001 01001000
    * ^^^^^^\_______/^^ ^^^^^^^^
    *        10010010 = 0x92
    */
    V1(0);
    V1(0);
    V1(0);
    V1(1);
    V1(0);
    V1(0);
    V1(1);
    V1(0);

    V1(1);
    V1(0);

    V8(0x92);

    V1(0);
    V1(1);
    V1(0);
    V1(0);
    V1(1);
    V1(0);

    /*
    * 0x4b 'K' 01001011
    * 0x4c 'L' 01001100
    * 0x4d 'M' 01001101
    * 0x4e 'N' 01001110
    *
    * 01001110 01001101 01001100 01001011
    * ^\_______/\_______/\_______/^^^^^^^
    *  10011100 10011010 10011000
    *    0x9c     0x9a     0x98
    */
    V1(1);
    V1(1);
    V1(0);
    V1(1);
    V1(0);
    V1(0);
    V1(1);

    V16(0x9a98);
    V8(0x9c);

    V1(0);

    V8(0xFF);
    V8(0xAA);
    V8(0x55);
    V8('\n');
    V8(0x00);

    bit_read_close(&buf);

    printf("brtest, as it is, reports no errors\n");
    return 0;
}

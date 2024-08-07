/*
* File:     bwtest.c
* Purpose:  Test bitwriter.c
* Author:   Kerry Veenstra
*/

#include "bitwriter.h"

#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    /*
    * Create a file.
    */
    BitWriter *buf = bit_write_open("bwtest.out");
    if (!buf) {
        fprintf(stderr, "error opening bwtest.out\n");
        exit(1);
    }

    /*
    * Write the bits to a file using all of the bitwriter.c functions.
    * We deliberately choose bits that will create a text file.
    */

    /*
    * 0x41 'A' 01000001
    * 0x42 'B' 01000010
    *
    * 'A' followed by 'B' is 0x41 followed by 0x42.
    * In binary that's 01000010 01000001, which can be written as bits/byte.
    *                  ^^^^^^^\_______/^
    *                  |     |     ^   |___ start with this bit 1
    *                  | ... |     |_______ then the byte 00100000 = 0x20
    *                  |     |_____________ etc.
    *                  |___________________
    */
    bit_write_bit(buf, 1);
    bit_write_uint8(buf, 0x20);
    bit_write_bit(buf, 1);
    bit_write_bit(buf, 0);
    bit_write_bit(buf, 0);
    bit_write_bit(buf, 0);
    bit_write_bit(buf, 0);
    bit_write_bit(buf, 1);
    bit_write_bit(buf, 0);

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
    bit_write_bit(buf, 1);
    bit_write_bit(buf, 1);
    bit_write_bit(buf, 0);
    bit_write_uint32(buf, 0xe8c8a888);
    bit_write_bit(buf, 0);
    bit_write_bit(buf, 0);
    bit_write_bit(buf, 0);
    bit_write_bit(buf, 1);
    bit_write_bit(buf, 0);

    /*
    * 0x48 'H' 01001000
    * 0x49 'I' 01001001
    * 0x4a 'J' 01001010
    *
    * 01001010 01001001 01001000
    * ^^^^^^\_______/^^ ^^^^^^^^
    *        10010010 = 0x92
    */
    bit_write_bit(buf, 0);
    bit_write_bit(buf, 0);
    bit_write_bit(buf, 0);
    bit_write_bit(buf, 1);
    bit_write_bit(buf, 0);
    bit_write_bit(buf, 0);
    bit_write_bit(buf, 1);
    bit_write_bit(buf, 0);

    bit_write_bit(buf, 1);
    bit_write_bit(buf, 0);

    bit_write_uint8(buf, 0x92);

    bit_write_bit(buf, 0);
    bit_write_bit(buf, 1);
    bit_write_bit(buf, 0);
    bit_write_bit(buf, 0);
    bit_write_bit(buf, 1);
    bit_write_bit(buf, 0);

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
    bit_write_bit(buf, 1);
    bit_write_bit(buf, 1);
    bit_write_bit(buf, 0);
    bit_write_bit(buf, 1);
    bit_write_bit(buf, 0);
    bit_write_bit(buf, 0);
    bit_write_bit(buf, 1);

    bit_write_uint16(buf, 0x9a98);
    bit_write_uint8(buf, 0x9c);

    bit_write_bit(buf, 0);

    /*
    * '\n' = 10
    */
    bit_write_uint8(buf, 10);

    bit_write_close(&buf);

    /*
    * Verify the data that was written to bwtest.out
    */
    FILE *f = fopen("bwtest.out", "r");
    if (!f) {
        fprintf(stderr, "error opening bwtest.out for verification\n");
        exit(1);
    }

    char expect_data[] = "ABCDEFGHIJKLMN\n";
    for (char *p = expect_data; *p != '\0'; ++p) {
        int ch = fgetc(f);
        if (ch == EOF) {
            fprintf(stderr, "bwtest.out: expected %02x but got EOF\n", *p);
            exit(1);
        }
        if (*p != ch) {
            fprintf(stderr, "bwtest.out: expected %02x but got %02x\n", *p, ch);
            exit(1);
        }
    }

    printf("bwtest, as it is, reports no errors\n");
    return 0;
}

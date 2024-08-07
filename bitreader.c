#include "bitreader.h"

#include <stdio.h>
#include <stdlib.h>

struct BitReader {
    FILE *underlying_stream;
    uint8_t byte;
    uint8_t bit_position;
};

/*
Open binary filename using fopen() and return a pointer to a BitReader. On error, return NULL. The
pseudocode is below. Notice in line 6 that the byte field is being set to an unexpected value of 8 rather than
0. This value forces bit_read_bit() to read the first byte of the file when it is first called
*/
BitReader *bit_read_open(const char *filename) {
    BitReader *reader = (BitReader *) malloc(sizeof(BitReader));
    if (reader == NULL) {
        return NULL;
    }

    FILE *f = fopen(filename, "rb");
    if (f == NULL) {
        //fprintf(stderr, "ERROR (bit_read_open()): unable to open file\n");
        free(reader);
        return NULL;
    }

    reader->underlying_stream = f;

    reader->byte = 0;
    reader->bit_position = 8;

    return reader;
}

/*
Using values in the BitReader pointed to by *pbuf, close (*pbuf)->underlying_stream, free the BitReader
object, and set the *pbuf pointer to NULL. You must check all function return values and report a fatal error
if any of them report a failure
*/
void bit_read_close(BitReader **pbuf) {
    if (*pbuf != NULL) {
        fclose((*pbuf)->underlying_stream);
        //fprintf(stderr, "ERROR: Failed to close underlying stream\n");
        free(*pbuf);
        *pbuf = NULL;
    }
}

/*
Read 32 bits from buf by calling bit_read_bit() 32 times. Collect these bits into a uint32_t starting with
the LSB (least-significant, or rightmost, bit)
*/
uint32_t bit_read_uint32(BitReader *buf) {
    uint32_t word = 0x00000000;
    for (int i = 0; i < 32; i++) {
        uint32_t b = bit_read_bit(buf);
        word |= (b & 1) << i;
    }

    return word;
}

/*
Read 16 bits from buf by calling bit_read_bit() 16 times. Collect these bits into a uint16_t starting with
the LSB (least-significant, or rightmost, bit)
*/
uint16_t bit_read_uint16(BitReader *buf) {
    uint16_t word = 0x0000;
    for (int i = 0; i < 16; i++) {
        uint16_t b = bit_read_bit(buf);
        word |= (b & 1) << i;
    }

    return word;
}

/*
Read 8 bits from buf by calling bit_read_bit() 8 times. Collect these bits into a uint8_t starting with
the LSB (least-significant, or rightmost, bit)
*/
uint8_t bit_read_uint8(BitReader *buf) {
    uint8_t byte = 0x00;
    for (int i = 0; i < 8; i++) {
        uint8_t b = bit_read_bit(buf);
        byte |= (b & 1) << i;
    }

    return byte;
}

/*
This is the main reading function. It reads a single bit using values in the BitReader pointed to by buf
*/
uint8_t bit_read_bit(BitReader *buf) {
    if (buf->bit_position > 7) {
        int byte = fgetc(buf->underlying_stream);
        buf->bit_position = 0;
        buf->byte = (uint8_t) byte;
    }
    uint8_t extracted_bit = (buf->byte >> (buf->bit_position)) & 1;
    buf->bit_position += 1;

    return extracted_bit;
}

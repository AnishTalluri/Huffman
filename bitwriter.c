#include "bitwriter.h"

#include <stdio.h>
#include <stdlib.h>

struct BitWriter {
    FILE *underlying_stream;
    uint8_t byte;
    uint8_t bit_position;
};

/*
Open binary filename for write using fopen() and return a pointer to a BitWriter. You must check all
function return values and return NULL if any of them report a failure. The pseudocode is below.
*/
BitWriter *bit_write_open(const char *filename) {
    BitWriter *writer = (BitWriter *) malloc(sizeof(BitWriter));
    if (writer == NULL) {
        //fprintf(stderr, "ERROR: unable to allocate memory\n");
        return NULL;
    }

    FILE *f = fopen(filename, "wb");
    if (f == NULL) {
        //fprintf(stderr, "ERROR (bit_write_open()): unable to open file\n");
        free(writer);
        return NULL;
    }

    writer->underlying_stream = f;

    writer->byte = 0;
    writer->bit_position = 0;
    if (writer->byte != 0 || writer->bit_position != 0) {
        //fprintf(stderr, "ERROR: byte or bit position is not set to 0\n");
        fclose(f);
        free(writer);
        return NULL;
    }

    return writer;
}

/*
Using values in the BitWriter pointed to by *pbuf, flush any data in the byte buffer, close
underlying_stream, free the BitWriter object, and set the *pbuf pointer to NULL. You must check all
function return values and report a fatal error if any of them report a failure.
*/
void bit_write_close(BitWriter **pbuf) {
    if (*pbuf != NULL) {
        if ((*pbuf)->bit_position > 0) {
            fputc((*pbuf)->byte, (*pbuf)->underlying_stream);
        }

        fclose((*pbuf)->underlying_stream);
        free(*pbuf);
        *pbuf = NULL;
    }
}

/*
This is the main writing function. It writes a single bit, bit, using values in the BitWriter pointed to by
buf. This function collects 8 bits into the buffer byte before writing it using fputc(). You must check all
function return values and report a fatal error if any of them report a failure.
*/
void bit_write_bit(BitWriter *buf, uint8_t bit) {
    if (buf->bit_position > 7) {
        fputc(buf->byte, buf->underlying_stream);
        buf->bit_position = 0;
        buf->byte = 0;
    }

    if (bit == 0) {
        // Clear the bit at bit_position by using a bitwise AND with a mask that has all bits set except the one at bit_position
        buf->byte &= ~(1 << buf->bit_position);
    } else {
        // Set the bit at bit_position by using a bitwise OR with a mask that has only the bit at bit_position set
        buf->byte |= (1 << buf->bit_position);
    }

    buf->bit_position += 1;
}

/*
Write the 16 bits of function parameter x by calling bit_write_bit() 16 times. Start with the LSB (leastsignificant, or rightmost, bit) of x
*/
void bit_write_uint16(BitWriter *buf, uint16_t x) {
    for (int i = 0; i < 16; i++) {
        bit_write_bit(buf, (x >> i) & 1);
    }
}

/*
Write the 32 bits of function parameter x by calling bit_write_bit() 16 times. Start with the LSB (leastsignificant, or rightmost, bit) of x
*/
void bit_write_uint32(BitWriter *buf, uint32_t x) {
    for (int i = 0; i < 32; i++) {
        bit_write_bit(buf, (x >> i) & 1);
    }
}

/*
Write the 8 bits of function parameter x by calling bit_write_bit() 8 times. Start with the LSB (leastsignificant, or rightmost, bit) of x
*/
void bit_write_uint8(BitWriter *buf, uint8_t byte) {
    for (int i = 0; i < 8; i++) {
        bit_write_bit(buf, (byte >> i) & 1);
    }
}

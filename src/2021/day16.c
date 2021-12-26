#include "aoc.h"
#include <stdint.h>
#include <stdlib.h>

#define min(A, B) ((A) < (B) ? (A) : (B))

typedef struct {
    const char *buf;
    int index; // byte offset into buffer
    int bit;   // current bit-offset, i.e. index-within-byte of the next bit to
               // read
} bitreader_t;

// Read "num_bits" bits from byte, starting at "start"
uint8_t read_bits_from_byte(uint8_t byte, uint8_t start, uint8_t num_bits) {
    assert(start + num_bits <= 8);
    assert(start >= 0);
    assert(num_bits > 0);
    return (byte >> (8 - start - num_bits)) & ((1 << num_bits) - 1);
}

int64_t read_bits(bitreader_t *br, int num_bits) {
    int64_t num = 0;
    int remaining_bits_to_read = num_bits;
    assert(num_bits < 64);

    // Read from what is left of the current byte
    int b = min(8 - br->bit, num_bits);
    num = read_bits_from_byte(br->buf[br->index], br->bit, b);
    br->bit += b;
    remaining_bits_to_read -= b;
    if (remaining_bits_to_read == 0) {
        // We've read all the bytes we need (less than what was left
        // in the current byte)
        return num;
    }

    // Skip to next byte if
    assert(br->bit <= 8);
    if (br->bit == 8) {
        br->bit = 0;
        br->index++;
    }

    while (num_bits > 0) {
    }

    return 0;
}

int hex_char_to_byte(char c) {
    if (c >= 'A')
        return (c - 'A') + 10;
    else
        return (c - '0');
}

void day16_selftest() {
    //              01234567
    uint8_t bin = 0b11001100;
    assert(3 == read_bits_from_byte(bin, 0, 2));
    assert(2 == read_bits_from_byte(bin, 5, 2));
    assert(1 == read_bits_from_byte(bin, 3, 2));
}

aoc_result_t day16(const char *input, int len) {
    aoc_result_t result = {0};
    int bytelen = len / 2;
    // printf("bytelen = %d\n", bytelen);
    uint8_t *bytes = malloc(bytelen);

    day16_selftest();

    for (int i = 0; i < bytelen; i++) {
        bytes[i] = hex_char_to_byte(input[2 * i]) << 4 |
                   hex_char_to_byte(input[2 * i + 1]);
    }

    // for (int i = 0; i < bytelen; i++) {
    //     printf("%02X", bytes[i]);
    // }
    return result;
}

#include "aoc.h"
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define min(A, B) ((A) < (B) ? (A) : (B))

enum PacketTypeID {
    Sum = 0,
    Product = 1,
    Minimum = 2,
    Maximum = 3,
    Literal = 4,
    GreaterThan = 5,
    LessThan = 6,
    EqualTo = 7
};

enum LengthTypeID { TotalBitLength = 0, NumberOfSubPackets = 1 };

typedef struct {
    int64_t version_sum;
    int64_t packet_value;
    int bit_length; // number of bits in this subpacket
} packet_info_t;

typedef struct {
    uint8_t *buf;
    int buf_len;
    int bit_offset; // next bit to read
} bitreader_t;

int hex_char_to_byte(char c) {
    if (c >= 'A')
        return (c - 'A') + 10;
    else
        return (c - '0');
}

void init_bitreader(bitreader_t *br, const char *hexstr, int len) {
    br->buf_len = len / 2;
    br->buf = malloc(br->buf_len);
    br->bit_offset = 0;
    for (int i = 0; i < br->buf_len; i++) {
        br->buf[i] = hex_char_to_byte(hexstr[2 * i]) << 4 |
                     hex_char_to_byte(hexstr[2 * i + 1]);
    }
}

void deinit_bitreader(bitreader_t *br) { free(br->buf); }

int64_t bitreader_read_bits(bitreader_t *br, int num_bits) {
    int64_t num = 0;
    assert(br->bit_offset + num_bits <= (br->buf_len * 8));
    for (int i = 0; i < num_bits; i++) {
        int bit_offset = br->bit_offset++;
        int bit_index = bit_offset % 8; // index of bit within byte
        uint8_t byte = br->buf[bit_offset / 8];
        int mask = 1 << (7 - bit_index);
        num = num << 1;
        if (byte & mask) {
            num |= 1;
        }
    }
    return num;
}

#define MAX_SUBPACKETS 60

packet_info_t parse_packet(bitreader_t *br, int level) {
    UNUSED int indent = level * 3;
    packet_info_t packet_info = {0};
    packet_info.version_sum = bitreader_read_bits(br, 3);
    enum PacketTypeID packet_type_id = bitreader_read_bits(br, 3);
    packet_info.bit_length += 6; // header is always 6 bits
    if (packet_type_id == Literal) {
        int64_t literal = 0;
        uint8_t prefix = 0;
        do {
            prefix = bitreader_read_bits(br, 1);
            literal = literal << 4 | bitreader_read_bits(br, 4);
            packet_info.bit_length += 5;
        } while (prefix == 1);
        packet_info.packet_value = literal;
    } else {
        packet_info_t *subpacket_infos =
            calloc(MAX_SUBPACKETS, sizeof(packet_info_t));
        enum LengthTypeID length_type_id = bitreader_read_bits(br, 1);
        packet_info.bit_length++;
        int subpkg = 0;
        switch (length_type_id) {
        case TotalBitLength: {
            int total_bit_length = bitreader_read_bits(br, 15);
            packet_info.bit_length += 15;
            int subpacket_bit_length = 0;
            do {
                packet_info_t subpacket_info = parse_packet(br, level + 1);
                assert(subpkg < MAX_SUBPACKETS);
                subpacket_infos[subpkg++] = subpacket_info;
                subpacket_bit_length += subpacket_info.bit_length;
                packet_info.bit_length += subpacket_info.bit_length;
                packet_info.version_sum += subpacket_info.version_sum;
            } while (subpacket_bit_length < total_bit_length);
            break;
        }
        case NumberOfSubPackets: {
            int num_subpackets = bitreader_read_bits(br, 11);
            packet_info.bit_length += 11;
            for (int i = 0; i < num_subpackets; i++) {
                assert(subpkg < MAX_SUBPACKETS);
                packet_info_t subpacket_info = parse_packet(br, level + 1);
                subpacket_infos[subpkg++] = subpacket_info;
                packet_info.bit_length += subpacket_info.bit_length;
                packet_info.version_sum += subpacket_info.version_sum;
            }
            break;
        }
        default:
            assert(0);
        }

        int num_subpackets = subpkg;
        switch (packet_type_id) {
        case Sum: {
            int64_t sum = 0;
            for (int64_t i = 0; i < num_subpackets; i++) {
                sum += subpacket_infos[i].packet_value;
            }
            packet_info.packet_value = sum;
            break;
        }
        case Product: {
            int64_t product = 1;
            for (int i = 0; i < num_subpackets; i++) {
                product *= subpacket_infos[i].packet_value;
            }
            packet_info.packet_value = product;
            break;
        }
        case Minimum: {
            int64_t minimum = INT_MAX;
            for (int i = 0; i < num_subpackets; i++) {
                int64_t v = subpacket_infos[i].packet_value;
                if (v < minimum)
                    minimum = v;
            }
            packet_info.packet_value = minimum;
            break;
        }
        case Maximum: {
            int64_t maximum = INT_MIN;
            for (int i = 0; i < num_subpackets; i++) {
                int64_t v = subpacket_infos[i].packet_value;
                if (v > maximum)
                    maximum = v;
            }
            packet_info.packet_value = maximum;
            break;
        }
        case GreaterThan: {
            assert(num_subpackets == 2);
            packet_info_t a = subpacket_infos[0];
            packet_info_t b = subpacket_infos[1];
            packet_info.packet_value =
                (a.packet_value > b.packet_value) ? 1 : 0;
            break;
        }
        case LessThan: {
            assert(num_subpackets == 2);
            packet_info_t a = subpacket_infos[0];
            packet_info_t b = subpacket_infos[1];
            packet_info.packet_value =
                (a.packet_value < b.packet_value) ? 1 : 0;
            break;
        }
        case EqualTo: {
            assert(num_subpackets == 2);
            packet_info_t a = subpacket_infos[0];
            packet_info_t b = subpacket_infos[1];
            packet_info.packet_value =
                (a.packet_value == b.packet_value) ? 1 : 0;
            break;
        }
        default:
            break;
        }

        free(subpacket_infos);
    }

    return packet_info;
}

aoc_result_t day16(const char *input, int len) {
    aoc_result_t result = {0};
    bitreader_t br;
    init_bitreader(&br, input, len);
    packet_info_t packet_info = parse_packet(&br, 0);
    result.p1 = packet_info.version_sum;
    result.p2 = packet_info.packet_value;
    deinit_bitreader(&br);
    return result;
}

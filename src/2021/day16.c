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

char *packet_type_id_str(enum PacketTypeID packet_type_id) {
    switch (packet_type_id) {
    case Sum:
        return "Sum";
    case Product:
        return "Product";
    case Minimum:
        return "Minimum";
    case Maximum:
        return "Maximum";
    case Literal:
        return "Literal";
    case GreaterThan:
        return "GreaterThan";
    case LessThan:
        return "LessThan";
    case EqualTo:
        return "EqualTo";
    default:
        assert(0);
    }
}

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

#if 0
void bitreader_print_state(bitreader_t *br) {
    printf("--[ bitreader state ]--\n");
    printf("Data: ");
    for (int i = 0; i < br->buf_len; i++) {
        printf("%02X", br->buf[i]);
    }
    printf("\n");
    int left_col = 20;
    printf("%-*s", left_col, "Bits:");
    for (int i = 0; i < br->buf_len * 8; i++) {
        int byte_offset = i / 8;
        int bit_offset = i % 8;
        int bit_value = (br->buf[byte_offset] >> (7 - bit_offset)) & 1;
        if (bit_value) {
            putchar('1');
        } else {
            putchar('0');
        }
    }
    printf("\n");
    printf("%*s", left_col, " ");
    for (int i = 0; i < br->buf_len * 8; i++) {
        putchar(i % 8 + '0');
    }
    printf("\n");
    printf("Bit offset (%d):.....%*s\n", br->bit_offset, br->bit_offset, "^");
    printf("\n");
    printf("--[ end bitreader state ]--\n");
}
#endif

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

void day16_selftest() {
    bitreader_t br;
    const char *hexstr = "D2FE28";
    init_bitreader(&br, hexstr, strlen(hexstr));
    assert(6 == bitreader_read_bits(&br, 3));      // packet version
    assert(4 == bitreader_read_bits(&br, 3));      // packet type id
    assert(1 == bitreader_read_bits(&br, 1));      // subpacket A prefix
    assert(0b0111 == bitreader_read_bits(&br, 4)); // subpacket A
    assert(1 == bitreader_read_bits(&br, 1));      // subpacket B prefix
    assert(0b1110 == bitreader_read_bits(&br, 4)); // subpacket B
    assert(0 == bitreader_read_bits(&br, 1));      // subpacket C prefix
    assert(0b0101 == bitreader_read_bits(&br, 4)); // subpacket C
}

#define MAX_SUBPACKETS 100

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
        // printf("%*sLITERAL(%ld, version_sum=%ld)\n", indent, " ", literal,
        //        packet_info.version_sum);
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
            // printf("%*sBEGIN OPERATOR(%s, bit_length = %d,
            // version_sum=%ld)\n",
            //        indent, " ", packet_type_id_str(packet_type_id),
            //        total_bit_length, packet_info.version_sum);
            int subpacket_bit_length = 0;
            do {
                packet_info_t subpacket_info = parse_packet(br, level + 1);
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

            // printf("%*sBEGIN OPERATOR(%s, num_sub_packets = %d, "
            //        "version_sum=%ld)\n",
            //        indent, " ", packet_type_id_str(packet_type_id),
            //        num_subpackets, packet_info.version_sum);
            for (int i = 0; i < num_subpackets; i++) {
                // printf("%*ssubpacket\n", indent, " ");
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

        // Handle the subpackets
        int num_subpackets = subpkg;
        assert(num_subpackets < MAX_SUBPACKETS);
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
    }

    // printf("%*sEND PACKET(type_id=%s, version_sum=%ld, packet_value=%ld, "
    //        "bit_length=%d)\n",
    //        indent, " ", packet_type_id_str(packet_type_id),
    //        packet_info.version_sum, packet_info.packet_value,
    //        packet_info.bit_length);

    return packet_info;
}

packet_info_t parse_hexstr(const char *hexstr) {
    bitreader_t br;
    init_bitreader(&br, hexstr, strlen(hexstr));
    // printf("Parsing packets: %s\n", hexstr);
    return parse_packet(&br, 0);
}

aoc_result_t day16(const char *input, int len) {
    aoc_result_t result = {0};

    // day16_selftest();
    // assert(parse_hexstr("D2FE28").packet_value == 2021);
    // assert(parse_hexstr("8A004A801A8002F478").version_sum == 16);
    // assert(parse_hexstr("620080001611562C8802118E34").version_sum == 12);
    // assert(parse_hexstr("A0016C880162017C3686B18A3D4780").version_sum == 31);
    // assert(parse_hexstr("C200B40A82").packet_value == 3);
    // assert(parse_hexstr("9C0141080250320F1802104A08").packet_value == 1);
    // assert(parse_hexstr("CE00C43D881120").packet_value == 9);

    bitreader_t br;
    init_bitreader(&br, input, len);
    packet_info_t packet_info = parse_packet(&br, 0);
    result.p1 = packet_info.version_sum;
    result.p2 = packet_info.packet_value;
    return result;
}

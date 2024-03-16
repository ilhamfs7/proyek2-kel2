#include "decodeLSB.h"
#include <stdio.h>
#include <string.h>

void menu() {
    
}

void read_bmp_header(FILE *file, unsigned char header[]) {
    fread(header, sizeof(unsigned char), 54, file);
}

char read_lsb_bit(unsigned char pixel_data[]) {
    return (pixel_data[0] & 1);  // bit terendah dari komponen warna biru
}

void extract_message(FILE *file, char extracted_message[]) {
    unsigned char pixel_data[3];
    int message_index = 0;
    int bit_index = 0;

    memset(extracted_message, '\0', sizeof(extracted_message));

    while (fread(&pixel_data, sizeof(unsigned char), 3, file)) {
        char current_bit = read_lsb_bit(pixel_data);

        extracted_message[message_index] |= (current_bit << (7 - bit_index));
        bit_index++;

        if (bit_index == 8) {
            if (extracted_message[message_index] == '\0') {
                break;
            }
            message_index++;
            bit_index = 0;
        }
    }
}
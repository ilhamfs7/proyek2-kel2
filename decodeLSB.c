#include "decodeLSB.h"
#include <stdio.h>
#include <string.h>


void decodeLSB(const char* input_filename) {
    FILE *file_in;
    unsigned char header[54];
    unsigned char pixel_data[3];
    char extracted_message[1000] = {0}; // Inisialisasi dengan null

    file_in = fopen(input_filename, "rb");
    if (file_in == NULL) {
        printf("File tidak dapat dibuka.");
        return;
    }

    fread(header, sizeof(unsigned char), 54, file_in);

    unsigned char panjang_pesan_bytes[3];
    fread(panjang_pesan_bytes, sizeof(unsigned char), 3, file_in);
    int panjang_pesan = (panjang_pesan_bytes[0] << 16) | (panjang_pesan_bytes[1] << 8) | panjang_pesan_bytes[2];

    // Mengatur kembali file pointer ke awal data piksel
    fseek(file_in, 54, SEEK_SET); // 54 byte header

    int message_index = 0;
    int bit_index = 0;

    while (fread(&pixel_data, sizeof(unsigned char), 3, file_in)) {
        if (message_index >= panjang_pesan)
            break;

        char current_bit = (pixel_data[0] & 1);

        extracted_message[message_index] |= (current_bit << (7 - bit_index));
        bit_index++;

        if (bit_index == 8) {
            message_index++;
            bit_index = 0;
        }
    }

    fclose(file_in);

    printf("Pesan yang diekstraksi: %s\n", extracted_message);
}

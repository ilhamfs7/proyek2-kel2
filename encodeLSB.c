#include "encodeLSB.h"
#include <stdio.h>
#include <string.h>

void encodeLSB(const char* input_filename, const char* output_filename, const char* message) {
    FILE *fin, *fout;
    unsigned char header[54];
    unsigned char pixel_data[3];
    int index_pesan, panjang_pesan;

    fin = fopen(input_filename, "rb");
    if (fin == NULL) {
        printf("\nFile tidak ada");
        return;
    }

    fout = fopen(output_filename, "wb");
    if (fout == NULL) {
        printf("\nGagal membuat file salinan");
        fclose(fin);
        return;
    }

    fread(header, sizeof(unsigned char), 54, fin);
    fwrite(header, sizeof(unsigned char), 54, fout);

    panjang_pesan = strlen(message);

    unsigned char panjang_pesan_bytes[3];
    panjang_pesan_bytes[0] = (panjang_pesan >> 16) & 0xFF;
    panjang_pesan_bytes[1] = (panjang_pesan >> 8) & 0xFF;
    panjang_pesan_bytes[2] = panjang_pesan & 0xFF;
    fwrite(panjang_pesan_bytes, sizeof(unsigned char), 3, fout);

    index_pesan = 0;
    while (message[index_pesan] != '\0') {
        char sisipPesan = message[index_pesan];
        int i = 7;
        while (i >= 0) {
            fread(&pixel_data, sizeof(unsigned char), 3, fin);
            pixel_data[0] = (pixel_data[0] & 0xFE) | ((sisipPesan >> i) & 1); 
            fwrite(&pixel_data, sizeof(unsigned char), 3, fout);
            i--;
        }
        index_pesan++;
    }

    while (fread(&pixel_data, sizeof(unsigned char), 3, fin)) {
        fwrite(&pixel_data, sizeof(unsigned char), 3, fout);
    }

    fclose(fin);
    fclose(fout);

    printf("\npesan berhasil di sisipkan.\n");
}

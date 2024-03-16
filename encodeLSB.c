#include <stdio.h>
#include <string.h>
#include "encodeLSB.h"

// Membaca 54 bit pertama pada file gambar bmp
void readBMPHeader(FILE *fin, unsigned char header[54]) {
    fread(header, sizeof(unsigned char), 54, fin);
}

// membaca data pixel gambar bmp
void readBMPPixel(FILE *fin, unsigned char pixel_data[3]) {
    fread(pixel_data, sizeof(unsigned char), 3, fin);
}

//menyisipkan pesan ke dalam gambar bmp
void encodeLSB(const char *inputimg, const char *outputimg, const char *pesan) {
    FILE *fin, *fout;
    unsigned char header[54], pixel_data[3];
    int index_pesan;

    // Buka file input dan output
    fin = fopen(inputimg, "rb");
    fout = fopen(outputimg, "wb");

    if (fin == NULL || fout == NULL) {
        printf("Gagal membuka file.");
        return;
    }

    // Baca header BMP
    readBMPHeader(fin, header);
    fwrite(header, sizeof(unsigned char), 54, fout);

    // Sisipkan pesan ke dalam LSB
    index_pesan = 0;
    while (pesan[index_pesan] != '\0') {
        char sisipPesan = pesan[index_pesan];
        for (int i = 7; i >= 0; i--) {
            readBMPPixel(fin, pixel_data);
            pixel_data[0] = (pixel_data[0] & 0xFE) | ((sisipPesan >> i) & 1);
            fwrite(pixel_data, sizeof(unsigned char), 3, fout);
        }
        index_pesan++;
    }

    // Salin sisa gambar tanpa mengubah bit
    while (!feof(fin)) {
        readBMPPixel(fin, pixel_data);
        fwrite(pixel_data, sizeof(unsigned char), 3, fout);
    }

    // Tutup file
    fclose(fin);
    fclose(fout);

    printf("Pesan  telah berhasil disisipkan.\n");
}



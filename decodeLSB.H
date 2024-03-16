#ifndef decodeLSB
#define decodeLSB

#include <stdio.h>

// Fungsi untuk membaca header BMP
void read_bmp_header(FILE *file, unsigned char header[]);

// Fungsi untuk membaca bit LSB dari komponen warna biru
char read_lsb_bit(unsigned char pixel_data[]);

// Fungsi untuk mengekstrak pesan bit per bit dari gambar
void extract_message(FILE *file, char extracted_message[]);

#endif /* BMP_MESSAGE_EXTRACTOR_H */

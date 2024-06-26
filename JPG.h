#ifndef JPEG_H
#define JPEG_H
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Struktur untuk menyimpan data gambar
typedef struct {
    uint8_t *buffer;        // Buffer untuk menyimpan data gambar
    size_t buffer_size;     // Ukuran buffer
} ImageData;
// Fungsi untuk membuka dan membaca file gambar
void open_image(const char *filename, ImageData *image);
// Fungsi untuk menutup buffer gambar
void close_image(ImageData *image);
// Fungsi untuk menyisipkan pesan ke dalam gambar
void encode_jpeg(const char *source_image, const char *dest_image, char *message);
// Fungsi untuk mengekstrak pesan dari gambar
int decode_jpeg(const char *image_path, _Bool* benar);

#endif /* JPEG_H */

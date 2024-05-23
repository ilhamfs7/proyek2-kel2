#include "jpeg.h"
#include <stdlib.h>
#include <string.h>

// Fungsi untuk membuka dan membaca file gambar
void open_image(const char *filename, ImageData *image) {
    // Implementasi fungsi open_image
    FILE *file = fopen(filename, "rb"); 
    if (!file) {
        printf("ERROR:File tidak ditemukan\n");
        exit(EXIT_FAILURE); 
    }
    fseek(file, 0, SEEK_END); 
    image->buffer_size = ftell(file); 
    fseek(file, 0, SEEK_SET);

    image->buffer = (uint8_t *)malloc(image->buffer_size); 
    if (!image->buffer) {
        printf("ERROR:Memory penuh\n"); 
        fclose(file);
        exit(EXIT_FAILURE); 
    }

    fread(image->buffer, 1, image->buffer_size, file);
    fclose(file); 
}

// Fungsi untuk menutup buffer gambar
void close_image(ImageData *image) {
    // Implementasi fungsi close_image
    free(image->buffer);
}

// Fungsi untuk menyisipkan pesan ke dalam gambar
void encode_jpeg(const char *source_image, const char *dest_image, const char *message) {
    // Implementasi fungsi encode_jpeg
    ImageData image;
    open_image(source_image, &image);

    FILE *outfile = fopen(dest_image, "wb"); 
    if (!outfile) {
        printf("ERROR:File tidak dapat dibuka\n"); 
        close_image(&image); 
        exit(EXIT_FAILURE); 
    }

    uint8_t *buffer = image.buffer; // Mendapatkan pointer ke buffer gambar
    size_t buffer_size = image.buffer_size; // Mendapatkan ukuran buffer gambar

    // Menambahkan terminator pada pesan
    size_t message_length = strlen(message);
    size_t total_message_length = message_length; 

    // Memeriksa apakah pesan dapat disisipkan dalam gambar
    if (total_message_length * 8 > buffer_size) {
        printf("ERROR: Ukuran file terlalu kecil\n"); 
        close_image(&image); 
        fclose(outfile); 
        exit(EXIT_FAILURE);
    }
    // Menyalin semua data dari gambar ke gambar yang akan disimpan
    fwrite(buffer, 1, buffer_size, outfile); 
    // Menambahkan marker COM (Comment) untuk menyimpan panjang pesan
    uint16_t message_length_be = total_message_length; 
    fwrite("\xFF\xFE", 1, 2, outfile); 
    fwrite(&message_length_be, sizeof(uint16_t), 1, outfile); 
    // Menambahkan pesan
    fwrite(message, 1, message_length, outfile);
    fwrite("$t3g0", 1, strlen("$t3g0"), outfile); 

    fclose(outfile); 
    close_image(&image);

    printf("Berhasil enkripsi pesan\n"); 
}

// Fungsi untuk mengekstrak pesan dari gambar
void decode_jpeg(const char *image_path) {
  
    ImageData image;
    open_image(image_path, &image); // Membuka file gambar

    uint8_t *buffer = image.buffer; // Mendapatkan pointer ke buffer gambar
    size_t buffer_size = image.buffer_size; // Mendapatkan ukuran buffer gambar

    // Mencari panjang pesan dalam komentar
    size_t pos = 0; 
    while (pos < buffer_size - 4) { 
        if (buffer[pos] == 0xFF && buffer[pos + 1] == 0xFE) { 
            uint16_t message_length_be; 
            memcpy(&message_length_be, &buffer[pos + 2], sizeof(uint16_t)); 

            size_t message_length = message_length_be; 

            char *message = (char *)malloc(message_length + 1); 
            if (!message) {
                printf("ERROR:Memory penuh\n"); 
                close_image(&image);
                exit(EXIT_FAILURE); 
            }

            memcpy(message, &buffer[pos + 4], message_length);
            message[message_length] = '\0'; 

            printf("Pesan Tersembunyi: %s\n", message); 

            free(message); 
            break; 
        }
        pos++; 
    }
    if (pos >= buffer_size - 4) { 
        printf(" Tidak ada pesan yang disembunyikan \n"); 
    }
    close_image(&image); 
}

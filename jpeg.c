#include "jpeg.h"
#include "linkedlistLSB.h"
#include <stdlib.h>
#include <string.h>

// Fungsi untuk membuka dan membaca file gambar
void open_image(const char *filename, ImageData *image) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("ERROR: FILE TIDAK DITEMUKAN!\n");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    image->buffer_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    image->buffer = (uint8_t *)malloc(image->buffer_size);
    if (!image->buffer) {
        printf("ERROR: Memory penuh!\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    fread(image->buffer, 1, image->buffer_size, file);
    fclose(file);
}

// Fungsi untuk menutup buffer gambar
void close_image(ImageData *image) {
    free(image->buffer);
}

// Fungsi untuk menyisipkan pesan ke dalam gambar
void encode(const char *source_image, const char *dest_image, const char *message) {
    ImageData image;
    open_image(source_image, &image);

    FILE *outfile = fopen(dest_image, "wb");
    if (!outfile) {
        printf("ERROR: FILE TIDAK DAPAT DIBUKA!\n");
        close_image(&image);
        exit(EXIT_FAILURE);
    }

    uint8_t *buffer = image.buffer;
    size_t buffer_size = image.buffer_size;

    // Menyimpan pesan dalam linked list
    Node* head = NULL;
    for (size_t i = 0; i < strlen(message); ++i) {
        head = insertEnd(head, message[i]);
    }

    // Memeriksa apakah pesan dapat disisipkan dalam gambar
    if (strlen(message) * 8 > buffer_size) {
        printf("ERROR: Ukuran file terlalu kecil!\n");
        close_image(&image);
        fclose(outfile);
        exit(EXIT_FAILURE);
    }

    // Menyalin semua data dari gambar ke gambar yang akan disimpan
    fwrite(buffer, 1, buffer_size, outfile);

    // Menambahkan marker COM (Comment) untuk menyimpan panjang pesan
    uint16_t message_length_be = strlen(message);
    fwrite("\xFF\xFE", 1, 2, outfile);
    fwrite(&message_length_be, sizeof(uint16_t), 1, outfile);

    // Menambahkan pesan dari linked list
    Node* current = head;
    do {
        fwrite(&current->data, 1, 1, outfile);
        current = current->next;
    } while (current != head);

    fwrite("$t3g0", 1, strlen("$t3g0"), outfile);

    fclose(outfile);
    close_image(&image);
    printf("Berhasil enkripsi pesan \n");
}

// Fungsi untuk mengekstrak pesan dari gambar
void decode(const char *image_path) {
    ImageData image;
    open_image(image_path, &image);

    uint8_t *buffer = image.buffer;
    size_t buffer_size = image.buffer_size;

    // Mencari panjang pesan dalam komentar
    size_t pos = 0;
    while (pos < buffer_size - 4) {
        if (buffer[pos] == 0xFF && buffer[pos + 1] == 0xFE) {
            uint16_t message_length_be;
            memcpy(&message_length_be, &buffer[pos + 2], sizeof(uint16_t));

            size_t message_length = message_length_be;

            // Menyimpan pesan dalam linked list
            Node* head = NULL;
            for (size_t i = 0; i < message_length; ++i) {
                head = insertEnd(head, buffer[pos + 4 + i]);
            }

            // Menampilkan pesan dari linked list
            char* message = (char *)malloc(message_length + 1);
            if (!message) {
                printf("ERROR: Memory penuh!\n");
                close_image(&image);
                exit(EXIT_FAILURE);
            }

            Node* current = head;
            size_t i = 0;
            do {
                message[i++] = current->data;
                current = current->next;
            } while (current != head);
            message[message_length] = '\0';

            printf("Pesan tersembunyi: %s\n", message);
            free(message);
            break;
        }
        pos++;
    }

    if (pos >= buffer_size - 4) {
        printf("Tidak ada pesan yang disembunyikan.\n");
    }

    close_image(&image);
}

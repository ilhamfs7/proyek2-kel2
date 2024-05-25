#include "jpeg.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "Jpeg.h"
#include "linkedlistLSB.h"
#include "decodeLSB.h"
#include "encodeLSB.h"

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
void encode_jpeg(const char *source_image, const char *dest_image, const char *message_file) {
    ImageData image;
    open_image(source_image, &image);

    FILE *outfile = fopen(dest_image, "wb");
    if (!outfile) {
        printf("ERROR: File tidak dapat dibuka\n");
        close_image(&image);
        exit(EXIT_FAILURE);
    }

    char message[1000];
    readAndDeleteFile(message_file, message, sizeof(message));

    // Create doubly circular linked list from message
    Node* head = NULL;
    Node* tail = NULL;
    size_t index_pesan = 0;
    while (message[index_pesan] != '\0') {
        if (head == NULL) {
            head = createNode(message[index_pesan]);
            tail = head;
        } else {
            tail = insertEnd(head, message[index_pesan]);
        }
        index_pesan++;
    }

    // Swap data of every two nodes
    if (head && head->next) {
        Node* current = head;
        do {
            swapNodeData(current, current->next);
            current = current->next->next;
        } while (current != head && current->next != head);
    }

    // Insert random nodes after every two nodes
    Node* current = head;
    srand(time(NULL));
    while (current->next != head) {
        current = insertAfter(current, rand() % 10 + '0');
        current = current->next->next;
        if (current == head) {
            break;
        }
    }

    // Perform circular data shift
    circularKanan(head);

    // Convert the linked list back to message array
    index_pesan = 0;
    current = head;
    Node* temp;
    do {
        message[index_pesan++] = current->data;
        temp = current;
        current = current->next;
        free(temp);
    } while (current != head);
    message[index_pesan] = '\0';

    uint8_t *buffer = image.buffer;
    size_t buffer_size = image.buffer_size;

    size_t message_length = strlen(message);
    size_t total_message_length = message_length;

    if (total_message_length * 8 > buffer_size) {
        printf("ERROR: Ukuran file terlalu kecil\n");
        close_image(&image);
        fclose(outfile);
        exit(EXIT_FAILURE);
    }

    fwrite(buffer, 1, buffer_size, outfile);
    uint16_t message_length_be = total_message_length;
    fwrite("\xFF\xFE", 1, 2, outfile);
    fwrite(&message_length_be, sizeof(uint16_t), 1, outfile);
    fwrite(message, 1, message_length, outfile);
    fwrite("$t3g0", 1, strlen("$t3g0"), outfile);

    fclose(outfile);
    close_image(&image);

    printf("Berhasil enkripsi pesan\n");
}

// Fungsi untuk mengekstrak pesan dari gambar
void decode_jpeg(const char *image_path) {
  	ImageData image;
    open_image(image_path, &image);

    uint8_t *buffer = image.buffer;
    size_t buffer_size = image.buffer_size;

    size_t pos = 0;
    while (pos < buffer_size - 4) {
        if (buffer[pos] == 0xFF && buffer[pos + 1] == 0xFE) {
            uint16_t message_length_be;
            memcpy(&message_length_be, &buffer[pos + 2], sizeof(uint16_t));
            
            size_t message_length = message_length_be;

            char *message = (char *)malloc(message_length + 1);
            if (!message) {
                printf("ERROR: Memory penuh\n");
                close_image(&image);
                exit(EXIT_FAILURE);
            }

            memcpy(message, &buffer[pos + 4], message_length);
            message[message_length] = '\0';

            // Reconstruct the doubly circular linked list from the message
            Node* head = createNode(message[0]);
            Node* tail = head;
            size_t i;
            for (i = 1; i < strlen(message); i++) {
                tail = insertEnd(tail, message[i]);
            }

            // Reverse circular shift
            circularKiri(head);

            // Remove the inserted random nodes after every two nodes
            Node* current = head;
            Node* prev = NULL;
            int count = 0;
            do {
                if (count % 3 == 1 && current != head) {
                    Node* temp = current->next;
                    deleteNode(prev, current);
                    current = temp;
                    if (prev == NULL) {
                        head = current; // Update head jika node pertama dihapus
                    }
                } else {
                    prev = current;
                    current = current->next;
                }
                count++;
            } while (current != head);

            // Swap data of every two nodes back
            if (head && head->next != head) {
                current = head;
                do {
                    swapNodeData(current, current->next);
                    current = current->next->next;
                } while (current != head && current->next != head);
            }

            // Convert the linked list back to message array
            size_t index_pesan = 0;
            current = head;
            do {
                message[index_pesan++] = current->data;
                Node* temp = current;
                current = current->next;
                free(temp);
            } while (current != head);
            message[index_pesan] = '\0';

            createMessageFile(message);

            free(message);
            break;
        }
        pos++;
    }
    if (pos >= buffer_size - 4) {
        printf("Tidak ada pesan yang disembunyikan\n");
    }
    close_image(&image);
}

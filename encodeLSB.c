#include "encodeLSB.h"
#include "linkedlistLSB.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

void readAndDeleteFile(char* filename, char* message, size_t message_size) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    fgets(message, message_size, file);

    fclose(file);

    if (remove(filename) != 0) {
        perror("Error deleting file");
    }
}

void encodeLSB(const char* input_filename, const char* output_filename, char* message) {
    FILE *fin, *fout;
    unsigned char header[54];
    unsigned char pixel_data[3];
    int index_pesan, panjang_pesan;

    fin = fopen(input_filename, "rb");
    if (fin == NULL) {
        printf("File tidak ada");
        return;
    }

    fout = fopen(output_filename, "wb");
    if (fout == NULL) {
        printf("Gagal membuat file salinan");
        fclose(fin);
        return;
    }

    fread(header, sizeof(unsigned char), 54, fin);
    fwrite(header, sizeof(unsigned char), 54, fout);

	Node* head = NULL;
    Node* tail = NULL;
    index_pesan = 0;
    while (message[index_pesan] != '\0') {
        // Membuat linked list dari pesan
        if (head == NULL) {
            head = createNode(message[index_pesan]);
            tail = head;
        } else {
            tail = insertEnd(tail, message[index_pesan]);
        }
        index_pesan++;
    }

    // Melakukan pertukaran informasi pada setiap dua node
    Node* current = head;
    while (current != NULL && current->next != NULL) {
        swapNodeData(current, current->next);
        current = current->next->next;
    }

    // Menyisipkan node baru dengan angka acak setiap dua node
    current = head;
    srand(time(NULL)); // Inisialisasi seed untuk fungsi rand()
    while (current != NULL && current->next != NULL) {
        current = insertAfter(current, rand() % 10 + '0'); // Menghasilkan angka acak antara 0-9
        current = current->next->next;
    }

    // Mengembalikan informasi pada linked list ke variabel message
    index_pesan = 0;
    current = head;
    while (current != NULL) {
        message[index_pesan++] = current->data;
        Node* temp = current;
        current = current->next;
        free(temp); // Membebaskan memori setiap node yang sudah tidak dibutuhkan
    }
    message[index_pesan] = '\0'; // Menambahkan NULL terminator pada akhir pesan

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

    printf("\nsukses sisip pesan\n");
}
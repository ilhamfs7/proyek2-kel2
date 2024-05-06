#include "encodeLSB.h"
#include <stdio.h>
#include <string.h>

Node* createNode(char data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memori penuh!");
        exit(1);
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

Node* insertEnd(Node* head, char data) {
    if (head == NULL) {
        return createNode(data);
    } else {
        Node* temp = head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = createNode(data);
        return head;
    }
}

void freeList(Node* head) {
    Node* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

void encode(const char* input_filename, const char* output_filename, const char* message) {
    FILE *fin, *fout;
    unsigned char header[54];
    unsigned char pixel_data[3];

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

    // Menentukan panjang pesan
    int panjang_pesan = 0;
    Node* temp = message;
    while (temp != NULL) {
        panjang_pesan++;
        temp = temp->next;
    }

    unsigned char panjang_pesan_bytes[3];
    panjang_pesan_bytes[0] = (panjang_pesan >> 16) & 0xFF;
    panjang_pesan_bytes[1] = (panjang_pesan >> 8) & 0xFF;
    panjang_pesan_bytes[2] = panjang_pesan & 0xFF;
    fwrite(panjang_pesan_bytes, sizeof(unsigned char), 3, fout);

    // Memasukkan pesan ke dalam gambar
    temp = message;
    while (temp != NULL) {
        char sisipPesan = temp->data;
        int i = 7;
        while (i >= 0) {
            fread(&pixel_data, sizeof(unsigned char), 3, fin);
            pixel_data[0] = (pixel_data[0] & 0xFE) | ((sisipPesan >> i) & 1); 
            fwrite(&pixel_data, sizeof(unsigned char), 3, fout);
            i--;
        }
        temp = temp->next;
    }

    // Menyalin sisa gambar
    while (fread(&pixel_data, sizeof(unsigned char), 3, fin)) {
        fwrite(&pixel_data, sizeof(unsigned char), 3, fout);
    }

    fclose(fin);
    fclose(fout);

    printf("\npesan berhasil di sisipkan.\n");
}

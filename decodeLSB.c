#include "decodeLSB.h"
#include "linkedlist.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void createMessageFile(const char* pesan) {
    char filename[100];
    printf("Masukkan nama file untuk menyimpan pesan: ");
    scanf("%s", filename);

    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Gagal membuka file untuk pesan.\n");
        return;
    }

    fprintf(file, "%s", pesan);
    fclose(file);

    printf("Pesan berhasil disimpan di file '%s'\n", filename);
}

void decodeLSB(const char* input_filename, char** pesan) {
    FILE *file_in;
    unsigned char header[54];
    unsigned char pixel_data[3];
    char extracted_message[1000] = {0}; 
    int message_index = 0;
    int bit_index = 0;

    file_in = fopen(input_filename, "rb");
    if (file_in == NULL) {
        printf("File tidak dapat dibuka.");
        return;
    }

    fread(header, sizeof(unsigned char), 54, file_in);

    unsigned char panjang_pesan_bytes[3];
    fread(panjang_pesan_bytes, sizeof(unsigned char), 3, file_in);
    int panjang_pesan = (panjang_pesan_bytes[0] << 16) | (panjang_pesan_bytes[1] << 8) | panjang_pesan_bytes[2];

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

    printf("Pesan yang diekstraksi sebelum dikembalikan dengan linkedlist: %s\n", extracted_message);

    // Konversi extracted_message menjadi linked list
    Node* head = createNode(extracted_message[0]);
    Node* tail = head;
    for (int i = 1; i < strlen(extracted_message); i++) {
        tail = insertEnd(tail, extracted_message[i]);
    }
    
	// Melakukan circular data
    circularKiri(head);
    
	// Hapus setiap node diantara dari setiap tiga node
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
    
	// Tukar data setiap dua node
    current = head;
    do {
    	swapNodeData(current, current->next);
        current = current->next->next;
	}while (current != head && current->next != head);
		
    // Konversi linked list kembali ke pesan
    current = head;
    panjang_pesan = 0;
    do {
        panjang_pesan++;
        current = current->next;
    } while (current != head);
	
    // Mengalokasikan memori untuk pesan
    *pesan = (char*)malloc((panjang_pesan + 1) * sizeof(char));
    if (*pesan == NULL) {
        printf("Gagal mengalokasikan memori.\n");
        exit(1);
    }

    current = head;
    int index_pesan = 0;
    do {
        (*pesan)[index_pesan++] = current->data;
        Node* temp = current;
        current = current->next;
        free(temp);
    } while (current != head);

    (*pesan)[index_pesan] = '\0'; // Menambahkan NULL terminator pada akhir pesan

    printf("Pesan yang diekstraksi setelah dikembalikan dengan linkedlist: %s\n", *pesan);

    createMessageFile(*pesan);
}

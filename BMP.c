#include "BMP.h"
#include "linkedlist.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
        printf("Gagal membuat file salinan");-
        fclose(fin);
        return;
    }

	printf("P1\n");

    fread(header, sizeof(unsigned char), 54, fin);
    fwrite(header, sizeof(unsigned char), 54, fout);

    Node* head = NULL;
    Node* tail = NULL;
    index_pesan = 0;
    while (message[index_pesan] != '\0') {
        if (head == NULL) {
            head = createNode(message[index_pesan]);
            tail = head;
        } else {
            tail = insertEnd(head, message[index_pesan]);
        }
        index_pesan++;
    }
    
    printList(head);
    printf("P2\n");
    
    // Melakukan pertukaran informasi pada setiap dua node
    if (head && head->next) { // Jika list memiliki lebih dari satu node
        Node* current = head;
        do {
            swapNodeData(current, current->next);
            current = current->next->next;
        } while (current != head && current->next != head);
    }

	printf("P3\n");

    // Menyisipkan node baru dengan angka acak setiap dua node
    Node* current = head;
    srand(time(NULL)); // Inisialisasi seed untuk fungsi rand()	
    while (current->next != head) {
        current = insertAfter(current, rand() % 10 + '0'); // Menghasilkan angka acak antara 0-9
        current = current->next->next;
        if (current == head) {
        	break;
        }
    }
    
    printf("P4\n");

    // Melakukan circular data
    circularKanan(head);

	printf("P5\n");

    // Mengembalikan informasi pada linked list ke variabel message
    index_pesan = 0;
    current = head;
    do {
        message[index_pesan++] = current->data;
        Node* temp = current;
        current = current->next;
        free(temp);
    } while (current != head);
    
    printf("P6\n");
    
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

void createMessageFile(const char* pesan) {
    char filename[100];
    do {
        printf("Masukkan nama file untuk menyimpan pesan (dengan ekstensi .txt): ");
        scanf("%s", filename);
        char* extension = strrchr(filename, '.');
        if (extension == NULL || strcmp(extension, ".txt") != 0) {
            printf("Nama file harus memiliki ekstensi .txt\n");
        } else {
            break;
        }
    } while (1);

    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Gagal membuka file untuk pesan.\n");
        return;
    }

    fprintf(file, "%s", pesan);
    fclose(file);

    printf("Pesan berhasil disimpan di file '%s'\n", filename);
}

int decodeLSB(const char* input_filename, char** pesan, _Bool* benar) {
    FILE *file_in;
    unsigned char header[54];
    unsigned char pixel_data[3];
    char extracted_message[10000] = {0}; 
    int message_index = 0;
    int bit_index = 0;

    file_in = fopen(input_filename, "rb");
    if (file_in == NULL) {
        printf("File tidak dapat dibuka.");
        return 0;
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

	if (message_index == 0) {
        printf("Tidak ada pesan yang disembunyikan dalam foto tersebut.\n");
        system("pause");
        *benar = true;
		return 0;
    }

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
    *benar = false;
    return 1;
}

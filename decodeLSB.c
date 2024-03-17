#include "decodeLSB.h"
#include <stdio.h>
#include <string.h>

/*void menu() {
	char input_filename[100];
	char output_filename[100]; // Nama file output
    char message[1000]; // Pesan yang akan disisipkan
    int pilihan;

    do {
        printf("\nMenu:\n\n");
        printf("1. Deskripsi\n");
        printf("2. Enkripsi\n");
        printf("3. Keluar\n");
        printf("\nPilih menu: ");
        scanf("%d", &pilihan);

        switch (pilihan) {
            case 1:
         		// Maksimum 100 karakter untuk nama file
                printf("\nMasukkan nama file gambar: ");
                scanf("%s", input_filename);
                decodeLSB(input_filename); // Panggil fungsi deskripsi dengan parameter nama file gambar
                getchar();
                break;
            case 2:
                // Panggil fungsi enkripsi di sini

                printf("Masukkan nama file gambar: ");
                scanf("%s", input_filename);

                printf("Masukkan nama file output: ");
            	scanf("%s", output_filename);

                printf("Masukkan pesan yang akan disisipkan: ");
                scanf("%s", message);
                fgets(message, sizeof(message), stdin); // Menggunakan fgets untuk membaca pesan

                encodeLSB(input_filename, output_filename, message); // Panggil fungsi encodeLSB
                break;
            case 3:
                printf("Terima kasih!\n");
                break;
            default:
                printf("Pilihan tidak valid. Silakan pilih lagi.\n");
                break;
        }
    } while (pilihan != 3);
} */

void decodeLSB(const char* input_filename) {
    FILE *file_in;
    unsigned char header[54];
    unsigned char pixel_data[3];
    char extracted_message[1000] = {0}; // Inisialisasi dengan null

    file_in = fopen(input_filename, "rb");
    if (file_in == NULL) {
        printf("File tidak dapat dibuka.");
        return;
    }

    fread(header, sizeof(unsigned char), 54, file_in);

    unsigned char panjang_pesan_bytes[3];
    fread(panjang_pesan_bytes, sizeof(unsigned char), 3, file_in);
    int panjang_pesan = (panjang_pesan_bytes[0] << 16) | (panjang_pesan_bytes[1] << 8) | panjang_pesan_bytes[2];

    // Mengatur kembali file pointer ke awal data piksel
    fseek(file_in, 54, SEEK_SET); // 54 byte header

    int message_index = 0;
    int bit_index = 0;

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

    printf("Pesan yang diekstraksi: %s\n", extracted_message);
}
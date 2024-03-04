#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char *pesan = NULL; // Pointer untuk menyimpan pesan dari pengguna
    int panjang_pesan = 0; // Panjang pesan yang dimasukkan pengguna
    char karakter;

    printf("Masukkan pesan: ");

    // Membaca setiap karakter hingga ditemukan newline
    while ((karakter = getchar()) != '\n') {
        // Mengalokasikan memori untuk setiap karakter yang dimasukkan pengguna
        pesan = (char *)realloc(pesan, (panjang_pesan + 1) * sizeof(char));

        if (pesan == NULL) {
            printf("Alokasi memori gagal!");
            return 1;
        }

        pesan[panjang_pesan++] = karakter; // Menyimpan karakter ke dalam array dinamis
    }

    pesan = (char *)realloc(pesan, (panjang_pesan + 1) * sizeof(char));
    pesan[panjang_pesan] = '\0'; // Menambahkan null-terminator untuk menandai akhir dari string

    printf("Pesan yang dimasukkan: %s\n", pesan);

    // Mencetak array karakter
    printf("Array karakter: ");
    for (int i = 0; i < panjang_pesan; i++) {
        printf("%c ", pesan[i]);
    }
    printf("\n");

    // Membebaskan memori yang dialokasikan untuk array pesan
    free(pesan);

    return 0;
}

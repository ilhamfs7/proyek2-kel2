#include <iostream>

#include "jpeg.h"
#include <stdio.h>

int main() {
    printf("--LSB Enkripsi-\n");    // Menampilkan judul program
    printf("1: Enkrip code\n");
    printf("2: Dekrip code\n");
    printf("masukan pilihan : ");
    
    char choice;
    scanf("%c", &choice);   // Membaca pilihan dari pengguna

    if (choice == '1') {   										 // Jika pilihan adalah enkripsi
        char src_image[100], dest_image[100], message[1000]; 	// Mendeklarasikan variabel untuk menyimpan path gambar sumber, path gambar tujuan, dan pesan yang akan disisipkan
        printf("Masukkan gambar: ");   					 // Meminta pengguna memasukkan path gambar sumber
        scanf("%s", src_image);									 // Membaca path gambar sumber
        printf("Masukkan pesan yang ingin di sembunyikan : ");
        getchar(); 												// Membersihkan karakter newline dari buffer
        fgets(message, sizeof(message), stdin);   				// Membaca pesan yang akan disisipkan dari pengguna
        message[strcspn(message, "\n")] = '\0'; 				// Menghapus karakter newline dari pesan
        printf("Masukkan nama file yang telah di sisipkan pesan : ");   			// Meminta pengguna memasukkan path gambar tujuan
        scanf("%s", dest_image);    							// Membaca path gambar tujuan
        printf("Encoding...\n");    							// Menampilkan pesan bahwa proses enkripsi dimulai
        encode(src_image, dest_image, message);    				// Memanggil fungsi untuk melakukan enkripsi
    } else if (choice == '2') {    								// Jika pilihan adalah dekripsi
        char src_image[100];    								// Mendeklarasikan variabel untuk menyimpan path gambar
        printf("Masukkan gambar : ");    				// Meminta pengguna memasukkan path gambar
        scanf("%s", src_image); 								// Membaca path gambar
        printf("Decoding...\n");    							// Menampilkan pesan bahwa proses dekripsi dimulai
        decode(src_image);  									// Memanggil fungsi untuk melakukan dekripsi
    } else {
        printf("ERROR: Invalid option chosen\n"); 				// Menampilkan pesan kesalahan jika pilihan tidak valid
    }

    return 0;   // Mengembalikan nilai 0 yang menandakan keluar dari program tanpa kesalahan
}

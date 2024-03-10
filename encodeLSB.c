#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
    
} PIXEL;

int main() {
    FILE *original_image, *new_image;
    char fpath_original[1000], fpath_new[1000];
    char message[10000]; // Deklarasi variabel message untuk menyimpan pesan
    printf("masukkan file original : ");
    scanf("%s", fpath_original);

    original_image = fopen(fpath_original, "rb");
    while (original_image == NULL) {
        printf("Error! Enter file kembali : ");
        scanf("%s", fpath_original);
        original_image = fopen(fpath_original, "rb");
    }

    printf("masukkan nama file dari hasil enkripsi : ");
    scanf("%s", fpath_new);

    printf("masukkan pesan yang ingin di masukkan: "); // Meminta pengguna memasukkan pesan
    scanf(" %[^\n]s", message);       // Membaca pesan dari pengguna

    new_image = fopen(fpath_new, "wb");
    if (new_image == NULL) {
        printf("Error tidak bisa membuat file baru !");
        fclose(original_image);
        return 1;
    }

    // Copying contents of original image to new image
    char ch;
    while (fread(&ch, sizeof(char), 1, original_image)) {
        fwrite(&ch, sizeof(char), 1, new_image);
    }

    fclose(original_image);

    // Open the original image again to hide the message
    original_image = fopen(fpath_original, "rb+");
    if (original_image == NULL) {
        printf("Error opening original file again!");
        fclose(new_image);
        return 1;
    }

    // Hide the message from the original image and write it to the new image
    PIXEL p;
    int i = 0;
    while (!feof(original_image) && i < strlen(message)) {
        fread(&p.blue, sizeof(p.blue), 1, original_image);
        fread(&p.green, sizeof(p.green), 1, original_image);
        fread(&p.red, sizeof(p.red), 1, original_image);

        // Hiding one bit of message in the least significant bit of each color component
        p.blue = (p.blue & 0xFE) | ((message[i] >> 7) & 1);
        p.green = (p.green & 0xFE) | ((message[i] >> 6) & 1);
        p.red = (p.red & 0xFE) | ((message[i] >> 5) & 1);

        fseek(new_image, -3, SEEK_CUR); // Move the file pointer back to overwrite the pixel
        fwrite(&p.blue, sizeof(p.blue), 1, new_image);
        fwrite(&p.green, sizeof(p.green), 1, new_image);
        fwrite(&p.red, sizeof(p.red), 1, new_image);// awalnya original image 
        

        i++;
    }

    // Reset pointer file ke awal gambar asli setelah selesai menyembunyikan pesan
    fseek(original_image, 0, SEEK_SET);

    // Salin kembali seluruh konten gambar asli ke file baru
    /*while (fread(&ch, sizeof(char), 1, original_image)) {
        fwrite(&ch, sizeof(char), 1, new_image);
    }*/// di buat menjadi kek gini 

    fclose(original_image);
    fclose(new_image);

    printf(" file baru telah dimasukkan pesan rahasia  : %s\n", fpath_new);

    return 0;
}


#include <stdio.h>
#include <string.h>
#include "encodeLSB.h"

#include <stdio.h>
#include <string.h>

int main() {
    FILE *fin, *fout;
    char inputimg[100], outputimg[100], pesan[1000];
    int index_pesan, panjang_pesan; // Deklarasi variabel panjang_pesan
    unsigned char header[54], pixel_data[3];
    
    printf("Masukkan nama file(.bmp) : ");
    scanf("%s", inputimg);
    fin = fopen(inputimg, "rb");
    if (fin == NULL) {
        printf("File tidak ada");
        return 1;
    }
    
    printf("Masukkan pesan : ");
    getchar();
    fgets(pesan, sizeof(pesan), stdin);

    // Menghitung panjang pesan
    panjang_pesan = strlen(pesan);

    printf("Masukkan nama file untuk salinan(.bmp) : ");
    scanf("%s", outputimg);
    fout = fopen(outputimg, "wb");
    if (fout == NULL) {
        printf("Gagal membuat file salinan");
        fclose(fin);
        return 1;
    }
    
    fread(header, sizeof(unsigned char), 54, fin);
    fwrite(header, sizeof(unsigned char), 54, fout);
    
    // Menyimpan panjang pesan di 3 byte pertama
    unsigned char panjang_pesan_bytes[3];
    panjang_pesan_bytes[0] = (panjang_pesan >> 16) & 0xFF;
    panjang_pesan_bytes[1] = (panjang_pesan >> 8) & 0xFF;
    panjang_pesan_bytes[2] = panjang_pesan & 0xFF;
    fwrite(panjang_pesan_bytes, sizeof(unsigned char), 3, fout);
    
    index_pesan = 0;
    while (pesan[index_pesan] != '\0') {
        char sisipPesan = pesan[index_pesan];
        for (int i = 7; i >= 0; i--) {
            fread(&pixel_data, sizeof(unsigned char), 3, fin);
            pixel_data[0] = (pixel_data[0] & 0xFE) | ((sisipPesan >> i) & 1); 
            fwrite(&pixel_data, sizeof(unsigned char), 3, fout);
        }
        index_pesan++;
    }
    
    while (fread(&pixel_data, sizeof(unsigned char), 3, fin)) {
        fwrite(&pixel_data, sizeof(unsigned char), 3, fout);
    }
    
    fclose(fin);
    fclose(fout);
    
    printf("\nsukses sisip pesan");
    
    return 0;
}

#include <stdio.h>
#include <string.h>
#include "decodeLSB.H"
#include "encodeLSB.h"

int main() {
    int pilihan;
    char inputimg[100], outputimg[100], pesan[1000];

    do {
        printf("\nMenu:\n");
        printf("1. Enkripsi LSB\n");
        printf("2. Dekripsi LSB\n");
        printf("3. Keluar\n");
        printf("Pilih menu: ");
        scanf("%d", &pilihan);

        switch (pilihan) {
        	case 1:
                printf("\nMasukkan nama file gambar BMP yang akan diubah: ");
                scanf("%s", inputimg);

                printf("\nMasukkan pesan yang akan disisipkan: ");
                getchar();
                fgets(pesan, sizeof(pesan), stdin);
                pesan[strcspn(pesan, "\n")] = 0;

                printf("\nMasukkan nama file hasil penyisipan: ");
                scanf("%s", outputimg);

                encodeLSB(inputimg, outputimg, pesan);
                break;
            case 2:
                {
                    char input_filename[100];
                    printf("\nMasukkan nama file gambar BMP yang akan diekstraksi pesannya: ");
                    scanf("\n%s", input_filename);
                    decodeLSB(input_filename);
                    getchar();
                }
                break;
            
            case 3:
                printf("\nTerima kasih!\n");
                break;
            default:
                printf("Pilihan tidak valid. Silakan pilih lagi.\n");
                break;
        }
    } while (pilihan != 3);

    return 0;
}

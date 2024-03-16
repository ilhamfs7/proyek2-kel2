#include <stdio.h>
#include <string.h>
#include "encodeLSB.h"

int main() {
    char inputimg[100], outputimg[100], pesan[1000];

    printf("Masukkan nama file gambar BMP yang akan diubah: ");
    scanf("%s", inputimg);

    printf("Masukkan pesan yang akan disisipkan: ");
    getchar();
    fgets(pesan, sizeof(pesan), stdin);
    pesan[strcspn(pesan, "\n")] = 0;

    printf("Masukkan nama file hasil penyisipan: ");
    scanf("%s", outputimg);

    encodeLSB(inputimg, outputimg, pesan);

    return 0;
}

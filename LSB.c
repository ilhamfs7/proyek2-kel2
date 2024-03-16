#include <stdio.h>
#include <string.h>
#include "encodeLSB.h"
#include "decodeLSB.h"

void menu() {
    int choice;
    printf("Selamat Datang di Aplikasi LSB!/n/n");
    printf("1. Enkripsi/n");
    printf("2. Dekripsi/n");
    printf("3. Keluar");
    printf("/n/nPilihan Anda: ");
    scanf("%d", choice);
}
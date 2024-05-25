#include "validasi.h"
#include <stdio.h>

int inputBener(int min, int max) {
    int num;
    char term;
    while (1) {
        if (scanf("%d%c", &num, &term) != 2 || term != '\n' || num < min || num > max) {
        	printf("----------------------------------------------------------------\n");
            printf("[!] ]Input tidak valid. Masukkan angka antara %d dan %d: ", min, max);
            while (getchar() != '\n'); // Bersihkan buffer
        } else {
            return num;
        }
    }
}

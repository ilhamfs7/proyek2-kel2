#include <stdio.h>
#include "encodeLSB.h"

int main() {
    char input_filename[100];
    char output_filename[100];
    char message[1000];

    printf("Masukkan nama file gambar BMP yang akan dienkripsi pesannya: ");
    scanf("%s", input_filename);

    printf("Masukkan pesan yang akan disisipkan: ");
    getchar(); // Clear the input buffer
    fgets(message, sizeof(message), stdin);

    printf("Masukkan nama file untuk salinan(.bmp): ");
    scanf("%s", output_filename);

    encodeLSB(input_filename, output_filename, message);

    return 0;
}

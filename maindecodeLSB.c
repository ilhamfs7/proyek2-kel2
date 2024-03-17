#include <stdio.h>
#include <string.h>

int main() {
    FILE *file_in;
    char input_filename[100];
    unsigned char header[54];
    char extracted_message[1500];
    int message_index = 0;
    int bit_index = 0;

    printf("Masukkan nama file gambar BMP yang akan diekstraksi pesannya: ");
    scanf("%s", input_filename);

    file_in = fopen(input_filename, "rb");

    if (file_in == NULL) {
        printf("File tidak dapat dibuka.");
        return 1;
    }

    fread(header, sizeof(unsigned char), 54, file_in);


    fclose(file_in);

    decodeLSB(input_filename);

    return 0;
}
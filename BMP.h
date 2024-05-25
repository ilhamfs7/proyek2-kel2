#ifndef BMP_H
#define BMP_H
#include <stdbool.h>
#include <stdio.h>

void encodeLSB(const char* input_filename, const char* output_filename, char* message);
void readAndDeleteFile(char* filename, char* message, size_t message_size);
void createMessageFile(const char* pesan);
int decodeLSB(const char* input_filename, char** pesan, _Bool* benar);

#endif

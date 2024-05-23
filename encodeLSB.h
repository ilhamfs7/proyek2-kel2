#ifndef ENCODELSB_H
#define ENCODELSB_H
#include <stdio.h>

void encodeLSB(const char* input_filename, const char* output_filename, char* message);
void readAndDeleteFile(char* filename, char* message, size_t message_size);

#endif

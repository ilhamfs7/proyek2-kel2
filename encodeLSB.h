#ifndef ENCODELSB_H
#define ENCODELSB_H

void readBMPHeader(FILE *fin, unsigned char header[54])
void encodeLSB(const char *inputimg, const char *outputimg, const char *pesan);

#endif


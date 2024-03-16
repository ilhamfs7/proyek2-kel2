#ifndef ENCODELSB_H
#define ENCODELSB_H

void readBMPHeader(FILE *fin, unsigned char header[54]);
void readBMPPixel(FILE *fin, unsigned char pixel_data[3]);
void encodeLSB(const char *inputimg, const char *outputimg, const char *pesan);

#endif


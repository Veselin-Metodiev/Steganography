#ifndef STEGANOGRAPHY_BMP_H
#define STEGANOGRAPHY_BMP_H
typedef struct {
    char format[2];
    unsigned int size;
    int reserved;
    int offset;
} header;
#endif //STEGANOGRAPHY_BMP_H

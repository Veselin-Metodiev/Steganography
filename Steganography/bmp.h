#ifndef STEGANOGRAPHY_BMP_H
#define STEGANOGRAPHY_BMP_H
typedef struct {
    // the first 2 bytes. If the image is of BMP format they will be 'B' and 'M'
    char format[2];
    unsigned int size;
    // these 4 bytes are reserved for hardware purposes and must be 0
    int reserved;
    // the byte where the header ends and the data for the pixels begins
    int offset;
} header;
#endif //STEGANOGRAPHY_BMP_H
#ifndef STEGANOGRAPHY_DECODE_H
#define STEGANOGRAPHY_DECODE_H

FILE *create_output_msg_file(char name[]);

int get_msg_length(FILE *img);

void decode_msg(FILE *img, FILE *output, int length);

#endif

#ifndef STEGANOGRAPHY_DECODE_H
#define STEGANOGRAPHY_DECODE_H

// Creates a txt file with the given name
FILE *create_output_msg_file(char name[]);

// Gets the LSB of the first 16 bytes from to image and returns a 16 bit u_int which is the length of the encoded message
unsigned short get_msg_length(FILE *img);

// Grabs the LSB of 8 bytes a number of times specified by length and appends the bits to a null character. Then writes the character to the output file
void decode_msg(FILE *img, FILE *output, int length);

#endif

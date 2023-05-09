#include <stdio.h>
#include <stdbool.h>
#include "bmp.h"

#ifndef STEGANOGRAPHY_ENCODE_H
#define STEGANOGRAPHY_ENCODE_H

// Checks to see if the file is large enough to store the message
bool is_valid_img_size(int img_size, unsigned int msg_length);

// Creates the output image file in the current directory if the user doesn't give us one
FILE *create_output_file();

// Copies the header of the input image to the output image
void copy_header(FILE *img_read, FILE *img_write, int offset);

// Encodes the length of the message for the first 16 bytes using the LSB method. This is required when we decode
void encode_msg_length(FILE *img_read, FILE *img_write, unsigned short msg_length);

// For each bit of each character we read a byte from the input image and set the last bit to the character's bit. We write the encoded and the rest of the bytes to the output image
void encode_msg(FILE *img_read, FILE *img_write, const char msg[]);

#endif

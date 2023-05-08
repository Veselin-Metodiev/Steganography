#include <stdio.h>
#include <stdbool.h>
#include "bmp.h"

#ifndef STEGANOGRAPHY_ENCODE_H
#define STEGANOGRAPHY_ENCODE_H

bool is_valid_img_size(int img_size, unsigned int msg_length);

FILE *create_output_file();

void copy_header(FILE *img_read, FILE *img_write, int offset);

void encode_msg_length(FILE *img_read, FILE *img_write, int msg_length);

void encode_msg(FILE *img_read, FILE *img_write, const char msg[]);

#endif

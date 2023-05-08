#include <stdbool.h>
#include <stdio.h>
#include "bmp.h"

#ifndef STEGANOGRAPHY_COMMON_H
#define STEGANOGRAPHY_COMMON_H

bool is_empty_msg(const char msg[]);

bool are_equal(char str1[], char str2[]);

bool is_valid_file(FILE *file);

char *get_str_without_braces(char str[]);

FILE *open_file(char path[], char mode[]);

bool is_valid_img_format(header header);

header populate_header(FILE *img);

#endif

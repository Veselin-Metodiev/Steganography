#include <stdbool.h>
#include <stdio.h>
#include "bmp.h"

#ifndef STEGANOGRAPHY_COMMON_H
#define STEGANOGRAPHY_COMMON_H

// Checks to see if the given message is NULL
bool is_empty_msg(const char msg[]);

// Checks if two strings are equal
bool are_equal(char str1[], char str2[]);

// Checks if an opened file is NULL
bool is_valid_file(FILE *file);

// Removes the quotes at the beginning and end of a string and returns a pointer to it
char *get_str_without_quotes(char str[]);

// Removes the "/n" at the end of the path. This occurs if the path was the last argument. Then opens and returns a file from the given path
FILE *open_file(char path[], char mode[]);

// Checks if the format of the image is BMP
bool is_valid_img_format(header header);

// Populates and returns a header with useful information from the header of the image
header populate_header(FILE *img);

#endif

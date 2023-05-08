#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "common.h"

bool is_empty_msg(const char msg[]){
    if (msg == NULL) {
        return true;
    }

    return false;
}

bool are_equal(char str1[], char str2[]) {
    if (strcmp(str1, str2) != 0) {
        return false;
    }

    return true;
}

FILE *open_file(char path[], char mode[]) {
    if (path[strlen(path) - 1] == '\n') {
        path[strlen(path) - 1] = '\0';
    }

    path = get_str_without_braces(path);

    return fopen(path, mode);
}

char *get_str_without_braces(char str[]) {
    char *new_str = calloc(strlen(str) - 1, 1);

    for (int i = 0; i < strlen(str) - 2; ++i) {
        new_str[i] = str[i + 1];
    }

    return new_str;
}

bool is_valid_file(FILE *file) {
    if (file == NULL) {
        return false;
    }

    return true;
}

bool is_valid_img_format(header header) {
    if (header.format[0] != 'B' || header.format[1] != 'M') {
        return false;
    }

    return true;
}

header populate_header(FILE *img) {
    header header;
    fread(&header.format, 2, 1, img);
    fread(&header.size, 3 * sizeof(int), 1, img);
    rewind(img);
    return header;
}
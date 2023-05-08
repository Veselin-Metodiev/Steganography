#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <direct.h>
#include "encode.h"

bool is_valid_img_size(int img_size, unsigned int msg_length) {
    if (msg_length * 8 < img_size - 32) {
        return true;
    }
    return false;
}

FILE *create_output_file() {
    char path[200];
    getcwd(path, 200);
    path[strlen(path) - 1] = '\0';

    FILE *output_img;
    output_img = fopen(strcat(path, "/outputimage.bmp"), "wb");
    return output_img;
}

void copy_header(FILE *img_read, FILE *img_write, int offset) {
    for (int i = 0; i < offset; ++i) {
        int cpy_header = fgetc(img_read);
        fputc(cpy_header, img_write);
    }
}

void encode_msg_length(FILE *img_read, FILE *img_write, int msg_length) {
    for (int i = 1; i <= 32; ++i) {
        int length_bit = msg_length >> (32 - i) & 1;

        int img_num = fgetc(img_read);
        int img_lsb = img_num & 1;

        if (img_lsb != length_bit) {
            if (img_num % 2 == 0) {
                img_num += 1;
            } else {
                img_num -= 1;
            }
        }
        fputc(img_num, img_write);
    }
}

void encode_msg(FILE *img_read, FILE *img_write, const char msg[]) {
    bool message_encrypted;
    message_encrypted = false;

    while (!feof(img_read)) {
        if (!message_encrypted) {
            for (int i = 0; i < strlen(msg); ++i) {
                char curr_char = msg[i];
                for (int j = 1; j <= 8; ++j) {
                    int char_bit = curr_char >> (8 - j) & 1;

                    int img_num = fgetc(img_read);
                    int img_lsb = img_num & 1;

                    if (img_lsb != char_bit) {
                        if (img_num % 2 == 0) {
                            img_num += 1;
                        } else {
                            img_num -= 1;
                        }
                    }
                    fputc(img_num, img_write);
                }
            }
            message_encrypted = true;
        } else {
            int img_num = fgetc(img_read);
            fputc(img_num, img_write);
        }
    }
}
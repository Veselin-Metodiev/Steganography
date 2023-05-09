#include <string.h>
#include <stdio.h>
#include "decode.h"
#include "common.h"

FILE *create_output_msg_file(char token[]) {
    FILE *output_msg_file = fopen(strcat(get_str_without_quotes(token), ".txt"), "w");
    return output_msg_file;
}

unsigned short get_msg_length(FILE *img) {
    int length;

    for (int i = 1; i <= 16; ++i) {
        length = length << 1;
        int img_char = fgetc(img);
        int lsb_bit = img_char & 1;
        length |= lsb_bit;
    }
    return length;
}

void decode_msg(FILE *img, FILE *output, int length) {
    for (int i = 0; i < length; i++) {
        int temp_ch = '\0';

        for (int j = 0; j < 8; j++) {
            temp_ch = temp_ch << 1;
            int img_char = fgetc(img);
            int lsb_bit = img_char & 1;
            temp_ch |= lsb_bit;
        }
        fputc(temp_ch, output);
    }
}
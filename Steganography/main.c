#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct BmpHeader {
    char format[2];
    unsigned int size;
    int garbage;
    unsigned int offset;
};

int main() {
    printf("Please enter your input in one of the following patterns:\n");
    printf("For encoding: -e <Message to encode> -i <full path to input file> -o <full path to output file>\n");
    printf("For decoding: -d <name of output file> -i <full path to input file>\n");

    char input[500];
    fgets(input, 500, stdin);

    char msg_to_encode[255];

    char action;
    char *token = strtok(input, " ");

    if (strcmp(token, "-e") == 0) {
        action = 'e';
    } else if (strcmp(token, "-d") == 0) {
        action = 'd';
    } else {
        printf("Incorrect first parameter");
        exit(1);
    }

    FILE *img_read, *img_write, *output_msg_file;

    if (action == 'e') {
        token = strtok(NULL, " ");

        if (strlen(token) > 255) {
            printf("The message has to be less than 256 characters");
            exit(1);
        }
        strcpy(msg_to_encode, token);

        token = strtok(NULL, " ");

        if (strcmp(token, "-i") != 0) {
            printf("Incorrect input format");
            exit(1);
        }

        token = strtok(NULL, " ");
        img_read = fopen(token, "rb");

        if (img_read == NULL) {
            printf("Cannot open the input file for encoding");
            exit(1);
        }

        token = strtok(NULL, " ");

        if (strcmp(token, "-o") != 0) {
            printf("Incorrect input format");
            exit(1);
        }

        token = strtok(NULL, " ");


        if (token == NULL) {
            char path[200];
            getcwd(path, 200);
            path[strlen(path) - 1] = '\0';
            img_write = fopen(strcat(path, "/outputimage.bmp"), "wb");
        } else {
            token[strlen(token) - 1] = '\0';
            img_write = fopen(token, "wb");
        }

        if (img_write == NULL) {
            printf("Cannot open output file");
            exit(1);
        }
    } else {
        token = strtok(NULL, " ");
        char path[20];
        strcpy(path, token);
        output_msg_file = fopen(strcat(path, ".txt"), "w");

        if (output_msg_file == NULL) {
            printf("Cannot open the output file for decoding");
            exit(1);
        }

        token = strtok(NULL, " ");

        if (strcmp(token, "-i") != 0) {
            printf("Incorrect input format");
            exit(1);
        }

        token = strtok(NULL, " ");
        token[strlen(token) - 1] = '\0';
        img_read = fopen(token, "rb");

        if (img_read == NULL) {
            printf("Cannot open the input file for decoding");
            exit(1);
        }
    }

    struct BmpHeader header;

    fread(&header.format, 2, 1, img_read);
    fread(&header.size, 3 * sizeof(int), 1, img_read);
    rewind(img_read);

    if (action == 'e') {
        for (int i = 0; i < header.offset; ++i) {
            int cpy_header = fgetc(img_read);
            fputc(cpy_header, img_write);
        }

        int msg_length = (int) strlen(msg_to_encode);

        fseek(img_read, (long) header.offset, SEEK_SET);
        fseek(img_write, (long) header.offset, SEEK_SET);

        bool message_encrypted;
        message_encrypted = false;

        while (!feof(img_read)) {
            if (!message_encrypted) {
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
                for (int i = 0; i < msg_length; ++i) {
                    char curr_char = msg_to_encode[i];
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
        fclose(img_read);
        fclose(img_write);
    } else {
        fseek(img_read, (long) header.offset, SEEK_SET);
        int length;

        for (int i = 1; i <= 32; ++i) {
            length = length << 1;
            int img_char = fgetc(img_read);
            int lsb_bit = img_char & 1;
            length |= lsb_bit;
        }

        for (int i = 0; i < length; i++) {
            int temp_ch = '\0'; // declaring null character i.e. 0000 0000 ( or 00 hex)

            // Fetching all 8 bits of Image replaced LSB
            for (int j = 0; j < 8; j++) {
                temp_ch = temp_ch << 1;
                int img_char = fgetc(img_read);
                int lsb_bit = img_char & 1;
                temp_ch |= lsb_bit;
            }
            fputc(temp_ch, output_msg_file);
        }

        fclose(img_read);
        fclose(output_msg_file);
    }
}
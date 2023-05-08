#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "encode.h"
#include "decode.h"
#include "common.h"
#include "bmp.h"

int main() {
    printf("Please enter your input in one of the following patterns:\n");
    printf("For encoding: -e <Message to encode> -i <full path to input file> -o <full path to output file>\n");
    printf("For decoding: -d <name of output file> -i <full path to input file>\n");

    char input[500];
    fgets(input, 500, stdin);

    char msg_to_encode[500];

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
    header header;

    if (action == 'e') {
        token = strtok(NULL, " ");

        if (is_empty_msg(token)) {
            printf("Please provide a message to be encoded");
            exit(1);
        }
        token = get_str_without_braces(token);
        strcpy(msg_to_encode, token);
        token = strtok(NULL, " ");

        if (!are_equal(token, "-i")) {
            printf("Please provide the image to be encoded in the following format:");
            printf("-i <full path to input file>");
            exit(1);
        }

        token = strtok(NULL, " ");

        img_read = open_file(token, "rb");

        if (!is_valid_file(img_read)) {
            printf("Could not open the image for encoding");
            exit(1);
        }

        header = populate_header(img_read);

        if (!is_valid_img_format(header)) {
            printf("The program only supports images of BMP format.");
            exit(1);
        }

        if (!is_valid_img_size((int)strlen(msg_to_encode), header.size - header.offset)) {
            printf("The image is too small for the message to be encoded. Please provide a bigger image or a smaller message");
            exit(1);
        }

        token = strtok(NULL, " ");

        if (token != NULL) {
            if (!are_equal(token, "-o")) {
                printf("Please provide the output image in the following format. If you do not provide it the file will be created in the current directory ");
                printf("-o <full path to input file>");
                exit(1);
            }

            token = strtok(NULL, " ");

            img_write = open_file(token, "wb");

            if (!is_valid_file(img_write)) {
                printf("Could not open or create the output file");
                exit(1);
            }
        } else {
            img_write = create_output_file();
        }
    } else {
        token = strtok(NULL, " ");
        output_msg_file = create_output_msg_file(token);

        token = strtok(NULL, " ");

        if (!are_equal(token, "-i")) {
            printf("Please provide the image to be encoded in the following format:");
            printf("-i <full path to input file>");
            exit(1);
        }

        token = strtok(NULL, " ");

        img_read = open_file(token, "rb");

        if (!is_valid_file(img_read)) {
            printf("Could not open the image for decoding");
            exit(1);
        }

       header = populate_header(img_read);

        if (!is_valid_img_format(header)) {
            printf("Make sure the image provided is of BMP format");
            exit(1);
        }
    }

    if (action == 'e') {
        copy_header(img_read, img_write, header.offset);

        for (int i = 0; i < header.offset; ++i) {
            int asd = fgetc(img_read);
            int asds = fgetc(img_write);
            int b = 23;
        }

        int msg_length = (int) strlen(msg_to_encode);

        fseek(img_read, (long) header.offset, SEEK_SET);
        fseek(img_write, (long) header.offset, SEEK_SET);

        encode_msg_length(img_read, img_write, msg_length);
        encode_msg(img_read, img_write, msg_to_encode);

        fclose(img_read);
        fclose(img_write);
    } else {
        fseek(img_read, (long) header.offset, SEEK_SET);
        int length = get_msg_length(img_read);

        decode_msg(img_read, output_msg_file, length);

        fclose(img_read);
        fclose(output_msg_file);
    }
}
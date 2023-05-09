#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "encode.h"
#include "decode.h"
#include "common.h"
#include "bmp.h"

const int max_input_length = 1000;

int main() {
    // Printing the functionality of the program
    printf("Please enter your input as follows:\n");
    printf("For encoding: -e \"Message to encode\" -i \"full path to input file\" -o \"full path to where the output file will be stored\"\n");
    printf("*Note: if you don`t specify the output path the file will be created in your current directory\n");
    printf("For decoding: -d \"name of output file\" -i \"full path to input file\"\n");

    //allocating memory for user`s input and resizing it to the actual input
    char *input = malloc(max_input_length);
    if (input == NULL) {
        printf("An error occurred while allocating memory");
    }
    fgets(input, max_input_length, stdin);
    realloc(input, strlen(input) + 1);

    char action;
    // I`m using strtok to split the input into a series of tokens that are separated by space
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
    char *msg_to_encode;

    if (action == 'e') {
        // The function stores the original string and each time we call it using NULL it gives us the next token
        token = strtok(NULL, " ");

        if (is_empty_msg(token)) {
            printf("Please provide a message to be encoded");
            exit(1);
        }
        token = get_str_without_quotes(token);
        msg_to_encode = malloc(strlen(token) + 1);
        if (input == NULL) {
            printf("An error occurred while allocating memory");
        }
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

        // We check if the user has provided us with an output path. If not we create the file in the current directory.
        if (token != NULL) {
            if (!are_equal(token, "-o")) {
                printf("Please provide the output image in the following format. If you do not provide it the file will be created in the current directory ");
                printf("-o <full path to input file>");
                exit(1);
            }

            token = strtok(NULL, " ");

            img_write = open_file(token, "wb");
            // We no longer need the input string, so we free it
            free(input);

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
        free(input);

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
        unsigned short msg_length = (unsigned short) strlen(msg_to_encode);

        // We set the cursor on the file to where the actual pixel data is
        fseek(img_read, (long) header.offset, SEEK_SET);
        fseek(img_write, (long) header.offset, SEEK_SET);

        encode_msg_length(img_read, img_write, msg_length);
        encode_msg(img_read, img_write, msg_to_encode);

        free(msg_to_encode);
        fclose(img_read);
        fclose(img_write);
    } else {
        fseek(img_read, (long) header.offset, SEEK_SET);
        unsigned short length = get_msg_length(img_read);

        decode_msg(img_read, output_msg_file, length);

        fclose(img_read);
        fclose(output_msg_file);
    }
}
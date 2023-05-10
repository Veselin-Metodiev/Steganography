#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "encode.h"
#include "decode.h"
#include "common.h"
#include "bmp.h"



int main() {
    const int max_input_length = 1000;
    const int max_output_path_length = 300;

    // Printing the functionality of the program
    printf("Please enter your input as follows:\n");
    printf("For encoding: -e \"Message to encode\" -i \"full path to input file\" -o \"full path to where the output file will be stored\"\n");
    printf("*Note: if you don`t specify the output path the file will be created in your current directory\n");
    printf("For decoding: -d \"name of output file\" -i \"full path to input file\"\n");

    //allocating memory for user`s input and resizing it to the actual input
    char *input = malloc(max_input_length);
    if (input == NULL) {
        printf("An error occurred while allocating memory");
        exit(1);
    }
    gets(input);
    realloc(input, strlen(input) + 1);

    char action;
    // I`m using strtok to split the input into a series of tokens that are separated by space
    char *token = strtok(input, " ");
    FILE *img_read, *img_write, *output_msg_file;
    header header;
    char *msg_to_encode;
    char *output_path = calloc(max_output_path_length, sizeof (char));

    if (strcmp(token, "-e") == 0) {
        action = 'e';
    } else if (strcmp(token, "-d") == 0) {
        action = 'd';
    } else {
        printf("Incorrect first parameter");
        exit(1);
    }

    if (action == 'e') {
        // The function stores the original string and each time we call it using NULL it gives us the next token
        token = strtok(NULL, " ");

        if (is_empty_msg(token)) {
            printf("Please provide a message to be encoded");
            exit(1);
        }
        token = get_str_without_quotes_and_newline(token);
        if (are_equal(token, "\"\"") || strlen(token) < 2) {
            printf("The provided message must be at least 2 characters");
            exit(1);
        }

        msg_to_encode = malloc(strlen(token) + 1);
        if (msg_to_encode == NULL) {
            printf("An error occurred while allocating memory");
            exit(1);
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

        if (!is_valid_img_size((int) strlen(msg_to_encode), header.size - header.offset)) {
            printf("The image is too small for the message to be encoded. Please provide a bigger image or a smaller message");
            exit(1);
        }

        token = get_str_without_quotes_and_newline(token);
        // We save the path so later on we can prevent users from using the same path as input and output
        char *input_path = token;
        // The output file must be of the same name as the input, so we save it
        char *file_name = get_file_name(token);

        token = strtok(NULL, " ");

        // We check if the user has provided us with an output path. If not we create the file in the current directory.
        if (token != NULL) {
            if (!are_equal(token, "-o")) {
                printf("Please provide the output image in the following format. If you do not provide it the file will be created in the current directory ");
                printf("-o <full path to input file>");
                exit(1);
            }

            token = strtok(NULL, " ");
            token = get_str_without_quotes_and_newline(token);
            token = strcat(token, "\\");
            output_path = strcpy(output_path, token);
        }
        strcat(output_path, file_name);

        if (are_equal(input_path, output_path)) {
            printf("Make sure your input and output directories are different to avoid corrupting the original image");
            exit(1);
        }

        img_write = fopen(output_path, "wb");
        // We no longer need the input string, so we free it
        free(input);

        if (!is_valid_file(img_write)) {
            printf("Could not open or create the output file");
            exit(1);
        }
    } else {
        token = strtok(NULL, " ");
        output_msg_file = create_output_msg_file(token);

        if (!is_valid_file(output_msg_file)) {
            printf("An error occurred while creating the output file");
            exit(1);
        }

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
        if (fopen(output_path, "rb") == NULL) {
            printf("An error occurred while encoding the message");
        }
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

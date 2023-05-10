# Steganography using LSB Method

This is a steganography project that enables you to hide secret messages within digital images using the Least Significant Bit (LSB) method. The project provides functionality for both encoding and decoding images.

Steganography is the art of concealing information within other non-secret data to ensure confidentiality and security. The LSB method is a widely used technique in image steganography, where the least significant bit of each pixel in an image is modified to encode a hidden message. This alteration is typically imperceptible to the human eye.

## Features

- **Image Encoding:** Encode a secret message within an image by modifying the least significant bit of each pixel.
- **Image Decoding:** Extract hidden messages from encoded images by analyzing the least significant bit of each pixel.

## Usage

1. Compile the project:

   ```bash
   gcc steganography.c -o steganography
2. Simly run the program and an explanation of it's functionality will be printed:

    ```bash
   ./steganography
   Please enter your input as follows:
   For encoding: -e "Message to encode" -i "full path to input file" -o "full path to where the output file will be stored"
   *Note: if you don`t specify the output path the file will be created in your current directory
   For decoding: -d "name of output file" -i "full path to input file"

## Requirements
To compile and run this project, ensure that you have a C compiler installed on your system.

## Flowchart
Here is a flowchart of the program:

![Steganography Flowchart](/uploads/6a286279341041d9a1779b8cc32c8927/StegDiagram.png)

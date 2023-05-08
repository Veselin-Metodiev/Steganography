cmake_minimum_required(VERSION 3.25)
project(Steganography C)

set(CMAKE_C_STANDARD 17)

add_executable(Steganography main.c common.c decode.c encode.c common.h encode.h decode.h bmp.h)
cmake_minimum_required(VERSION 3.25)
project(Steganography C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Steganography
        bmp.h
        common.c
        common.h
        decode.c
        decode.h
        encode.c
        encode.h
        main.c)

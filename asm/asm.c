#include "labels_table.h"
#include "encoder.h"

#define MAX_LINE_LENGTH 44

void check_argc(int argc) {
    if (argc != 3) {
        puts("Invalid number of arguments, it should be equal 2");
        exit(-1);
    }
}

void check_file_exist(FILE *fp) {
    if (fp == NULL) {
        perror("Error");
        exit(-1);
    }
}


int main(int argc, char *argv[]) {
    FILE *input_file;
    FILE *output_file;
    check_argc(argc);
    input_file = fopen(argv[1], "r");
    output_file = fopen(argv[2], "wb");
    check_file_exist(input_file);
    init_labels_from_file(input_file);
    encode_commands(input_file);
    fwrite(commands, sizeof(int16_t), comands_size, output_file);
    return 2;
}
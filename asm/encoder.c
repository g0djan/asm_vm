#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "encoder.h"

enum command codes[8][8];
size_t sizes[8];

void init_codes() {
    enum command c110[8] = {add, sub, mul, div_, and, or, xor, mov};
    enum command c011[2] = {je, jne};
    enum command c010[8] = {not, push , pop, shl, shr, mov1, mov2};
    enum command c001[3] = {call, mov3, jmp};
    enum command c000[5] = {in, out, reset, nop, ret};
    memcpy(codes[6], c110, 6), sizes[6] = 8;
    memcpy(codes[3], c011, 2), sizes[3] = 2;
    memcpy(codes[2], c010, 8), sizes[2] = 8;
    memcpy(codes[1], c001, 3), sizes[1] = 3;
    memcpy(codes[0], c000, 5), sizes[0] = 5;
}

bool has_only_2RX_operands(struct asm_command *command) {
    return command->operands_cnt == 2 &&
           command->operands[0].type == RX &&
           command->operands[1].type == RX;
}

bool has_type_operand(struct asm_command *command, enum operand_type type) {
    bool has = false;
    for (size_t i = 0; i < command->operands_cnt; ++i) {
        has |= command->operands[i].type == type;
    }
    return has;
}

int8_t get_command_code(struct asm_command *command, int16_t code) {
    for (int8_t i = 0; i < sizes[code]; ++i) {
        if (command->name == codes[code][i]) {
            return i;
        }
    }
    puts("Invalid code for this command");
    exit(-1);
}


uint16_t encode_command(struct asm_command *command) {
    uint16_t bitmask = 0;
    bitmask |= has_only_2RX_operands(command) << 0;
    bitmask |= has_type_operand(command, RX) << 1;
    bitmask |= has_type_operand(command, imm8) << 2;
    bitmask |= get_command_code(command, bitmask) << 3;
    return bitmask;
}

int8_t get_shift(int16_t num) {
    return (int8_t)ceil(log2(num));;
}

int8_t get_register_code(struct operand *operand) {
    if (operand->type == RX || operand->type == _RX_) {
        return operand->number;
    }
    if (operand->type == RL) {
        return operand->number << 1;
    }
    if (operand->type == RH) {
        return (operand->number << 1) + (int8_t )1;
    }
    puts("It's not register");
    exit(-1);
}

uint16_t encode_operands(struct asm_command *command, uint16_t bitmask) {
    int8_t shift = get_shift(bitmask);
    int8_t register_code;
    for (size_t i = 0; i < command->operands_cnt; ++i) {
        if (command->operands[i].type == imm8) {
            bitmask |= command->operands[i].number << 8;
        } else {
            register_code = get_register_code(&command->operands[i]) << shift;
            shift = get_shift(register_code);
            bitmask |= register_code;
        }
    }
    return bitmask;
}

void encode_commands(FILE *fp) {
    char line[MAX_LINE_LENGTH];
    struct asm_command command;
    int8_t shift;
    for (int16_t i = 0; fgets(line, MAX_LINE_LENGTH, fp); ++i) {
        command = parse_command(line, i);
        commands[i] = encode_command(&command);
        commands[i] = encode_operands(&command, commands[i]);
    }
}
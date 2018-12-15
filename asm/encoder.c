#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "encoder.h"

size_t sizes[8] = {5, 0, 8, 8, 3, 0, 2};
enum command codes[8][8] = {
/*000*/    {in,   out,  reset, nop,  ret},
/*001*/    {},
/*010*/    {not,  push, pop,   shl,  shr, mov1, mov2, cmpxchg},
/*011*/    {add,  sub,  mul,   div_, and, or,   xor,  mov},
/*100*/    {call, mov3, jmp},
/*101*/    {},
/*110*/    {je,   jne}};

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

int8_t get_shift(int8_t shift, enum operand_type type) {
    switch (type) {
        case RX:
        case _RX_:
            return shift + (int8_t)3;
        case RH:
        case RL:
            return shift + (int8_t)4;
        default:
            puts("Getting shift only for registers");
            exit(-1);
    }
}

int8_t get_register_code(struct operand *operand) {
    if (operand->type == RX || operand->type == _RX_) {
        return operand->number;
    }
    if (operand->type == RL) {
        return operand->number << 1;
    }
    if (operand->type == RH) {
        return (operand->number << 1) + (int8_t) 1;
    }
    puts("It's not register");
    exit(-1);
}

uint16_t encode_operands(struct asm_command *command, uint16_t bitmask) {
    int8_t shift = (int8_t) ceil(log2(bitmask));
    int8_t register_code;
    for (size_t i = 0; i < command->operands_cnt; ++i) {
        if (command->operands[i].type == imm8) {
            bitmask |= command->operands[i].number << 8;
        } else {
            register_code = get_register_code(&command->operands[i]) << shift;
            shift = get_shift(shift, command->operands[i].type);
            bitmask |= register_code;
        }
    }
    return bitmask;
}

void encode_commands(FILE *fp) {
    char line[MAX_LINE_LENGTH];
    struct asm_command command;
    for (; fgets(line, MAX_LINE_LENGTH, fp); ++comands_size) {
        command = parse_command(line, (uint16_t)comands_size);
        commands[comands_size] = encode_command(&command);
        commands[comands_size] = encode_operands(&command, commands[comands_size]);
    }
}
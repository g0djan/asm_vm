#ifndef ASM_VM_ENCODER_H
#define ASM_VM_ENCODER_H

#endif //ASM_VM_ENCODER_H
#include <stdio.h>
#include "constants.h"
#include "parser.h"

struct operand_pair {
    enum operand_type first;
    enum operand_type second;
};

#define OPERAND_PAIR(first, second) (struct operand_pair){first, second}

struct operand_pair operand_classes[] = {
        OPERAND_PAIR(RX, RX),
        OPERAND_PAIR(RX, _RX_),
        OPERAND_PAIR(_RX_, RX),
        OPERAND_PAIR(RX, empty),
        OPERAND_PAIR(imm8, empty),
        OPERAND_PAIR(RX, imm4),
        OPERAND_PAIR(empty, empty),
        OPERAND_PAIR(RH, empty),
        OPERAND_PAIR(RL, empty)};

enum command short_commands[] = {mov3, je, jne, cmpxchg};
enum command long_commands[9][8] = {
        {mov, add, sub, mul, div_, and, or, xor},
        {mov1},
        {mov2},
        {push, pop, not},
        {call, jmp},
        {shl, shr},
        {ret, reset, nop},
        {in, out}
};
uint8_t sizes1[8] = {8, 1, 1, 3, 2, 2, 3, 2};

uint16_t commands[MAX_COMMANDS];
size_t comands_size;

void encode_commands(FILE *fp);
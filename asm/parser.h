#include <stdint.h>
#include <stdlib.h>
#include "constants.h"

#ifndef ASM_VM_PARSER_H
#define ASM_VM_PARSER_H

#endif //ASM_VM_PARSER_H

// believe in format [label]:command[ operand1][ operand2]\n\0

enum operand_type {
    _RX_, RX, RH, RL, imm4, imm8, label
};

enum command {
    mov, mov1,mov2,mov3,
    je, jne, jmp, call,
    add, sub, mul, div_,
    and, or, xor, not,
    push, pop, shl, shr,
    in, out, reset,
    nop, ret, cmpxchg
};

struct operand {
    enum operand_type type;
    uint8_t number;
};

struct asm_command {
    enum command name;
    struct operand operands[3];
    size_t operands_cnt;
};

struct asm_command parse_command(char *line, uint16_t line_number);

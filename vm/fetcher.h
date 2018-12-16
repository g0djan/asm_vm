#include "process.h"
#include "instructions.h"
#include <math.h>

#ifndef ASM_VM_FETCHER_H
#define ASM_VM_FETCHER_H

#endif //ASM_VM_FETCHER_H

struct tuple3 {
    uint16_t element[3];
    uint8_t len;
};
#define MAKE_TUPLE(first, second, third, len) \
    (struct tuple3){(uint16_t)first, (uint16_t)second, (uint16_t)third, (uint8_t)len}


struct tuple3 short_operand_masks[] = {
        MAKE_TUPLE(0x0000000001111000, 0x1111111100000000, 0, 2),
        MAKE_TUPLE(0x1111111100000000, 0x0000000000111000, 0, 2),
        MAKE_TUPLE(0x1111111100000000, 0x0000000000111000, 0, 2),
        MAKE_TUPLE(0x0000000000111000, 0x0000000111000000, 0x0000111000000000, 3)
};

struct tuple3 long_operand_masks[] = {
        MAKE_TUPLE(0x0000011100000000, 0x0011100000000000, 0, 2),
        MAKE_TUPLE(0x0000011100000000, 0x0011100000000000, 0, 2),
        MAKE_TUPLE(0x0000011100000000, 0x0011100000000000, 0, 2),
        MAKE_TUPLE(0x0000011100000000, 0x0000000000000000, 0, 1),
        MAKE_TUPLE(0x1111111100000000, 0x0000000000000000, 0, 1),
        MAKE_TUPLE(0x0000011100000000, 0x0111100000000000, 0, 2),
        MAKE_TUPLE(0x0000000000000000, 0x0000000000000000, 0, 0),
        MAKE_TUPLE(0x0000111100000000, 0x0000000000000000, 0, 1),
};

struct tuple3 short_shifts[] = {
        MAKE_TUPLE(3, 8, 0, 2),
        MAKE_TUPLE(8, 3, 0, 2),
        MAKE_TUPLE(8, 3, 0, 2),
        MAKE_TUPLE(3, 6, 9, 3),
};

struct tuple3 long_shifts[] = {
        MAKE_TUPLE(8, 11, 0, 2),
        MAKE_TUPLE(8, 11, 0, 2),
        MAKE_TUPLE(8, 11, 0, 2),
        MAKE_TUPLE(8, 0, 0, 1),
        MAKE_TUPLE(8, 0, 0, 1),
        MAKE_TUPLE(8, 11, 0, 2),
        MAKE_TUPLE(0, 0, 0, 0),
        MAKE_TUPLE(8, 0, 0, 1),
};

uint16_t fetch_command(struct process *process);
uint8_t fetch_args_cnt(uint16_t command);
asm_func fetch_func(uint16_t command);
void fetch_args(uint16_t *args, uint16_t command);
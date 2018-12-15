#include <stdint.h>

#ifndef ASM_VM_FUNCTIONS_H
#define ASM_VM_FUNCTIONS_H

#endif //ASM_VM_FUNCTIONS_H

int mov(uint16_t **args, uint8_t cnt);

int mov1(uint16_t **args, uint8_t cnt);

int mov2(uint16_t **args, uint8_t cnt);

int mov3(uint16_t **args, uint8_t cnt);

int push(uint16_t **args, uint8_t cnt);

int pop(uint16_t **args, uint8_t cnt);

int add(uint16_t **args, uint8_t cnt);

int sub(uint16_t **args, uint8_t cnt);

int mul(uint16_t **args, uint8_t cnt);

int div(uint16_t **args, uint8_t cnt);

int and(uint16_t **args, uint8_t cnt);

int or(uint16_t **args, uint8_t cnt);

int xor(uint16_t **args, uint8_t cnt);

int not(uint16_t **args, uint8_t cnt);

int in(uint16_t **args, uint8_t cnt);

int out(uint16_t **args, uint8_t cnt);

int call(uint16_t **args, uint8_t cnt);

int ret(uint16_t **args, uint8_t cnt);

int reset(uint16_t **args, uint8_t cnt);

int nop(uint16_t **args, uint8_t cnt);

int shl(uint16_t **args, uint8_t cnt);

int shr(uint16_t **args, uint8_t cnt);

int jmp(uint16_t **args, uint8_t cnt);

int je(uint16_t **args, uint8_t cnt);

int jne(uint16_t **args, uint8_t cnt);

int cmpxchg(uint16_t **args, uint8_t cnt);


int (*instructions[])(uint16_t **args, uint8_t cnt) =
        {mov, mov1, mov2, mov3, push, pop, add, sub, mul, div, and, or, xor,
         not, in, out, call, ret, reset, nop, shl, shr, jmp, je, jne, cmpxchg};
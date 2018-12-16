#include <stdint.h>
#include <string.h>
#include "process.h"

#ifndef ASM_VM_FUNCTIONS_H
#define ASM_VM_FUNCTIONS_H

#endif //ASM_VM_FUNCTIONS_H

int mov(uint16_t *args, struct process *process);

int mov1(uint16_t *args, struct process *process);

int mov2(uint16_t *args, struct process *process);

int mov3(uint16_t *args, struct process *process);

int push(uint16_t *args, struct process *process);

int pop(uint16_t *args, struct process *process);

int add(uint16_t *args, struct process *process);

int sub(uint16_t *args, struct process *process);

int mul(uint16_t *args, struct process *process);

int div_(uint16_t *args, struct process *process);

int and(uint16_t *args, struct process *process);

int or(uint16_t *args, struct process *process);

int xor(uint16_t *args, struct process *process);

int not(uint16_t *args, struct process *process);

int in(uint16_t *args, struct process *process);

int out(uint16_t *args, struct process *process);

int call(uint16_t *args, struct process *process);

int ret(uint16_t *args, struct process *process);

int reset(uint16_t *args, struct process *process);

int nop(uint16_t *args, struct process *process);

int shl(uint16_t *args, struct process *process);

int shr(uint16_t *args, struct process *process);

int jmp(uint16_t *args, struct process *process);

int je(uint16_t *args, struct process *process);

int jne(uint16_t *args, struct process *process);

int cmpxchg(uint16_t *args, struct process *process);

typedef int (*asm_func)(uint16_t *, struct process *process);

asm_func short_instructions[] = {mov3, je, jne, cmpxchg};
asm_func long_instructions[8][8] = {
        {mov, add, sub, mul, div_, and, or, xor},
        {mov1},
        {mov2},
        {push, pop, not},
        {call, jmp},
        {shl, shr},
        {ret, reset, nop},
        {in, out}
};
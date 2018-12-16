#ifndef ASM_VM_FUNCTIONS_H
#define ASM_VM_FUNCTIONS_H

#include <stdint.h>
#include <string.h>
#include "process.h"


int mov(unsigned short *args, struct process *process);

int mov1(unsigned short *args, struct process *process);

int mov2(unsigned short *args, struct process *process);

int mov3(unsigned short *args, struct process *process);

int push(unsigned short *args, struct process *process);

int pop(unsigned short *args, struct process *process);

int add(unsigned short *args, struct process *process);

int sub(unsigned short *args, struct process *process);

int mul(unsigned short *args, struct process *process);

int div_(unsigned short *args, struct process *process);

int and(unsigned short *args, struct process *process);

int or(unsigned short *args, struct process *process);

int xor(unsigned short *args, struct process *process);

int not(unsigned short *args, struct process *process);

int in(unsigned short *args, struct process *process);

int out(unsigned short *args, struct process *process);

int call(unsigned short *args, struct process *process);

int ret(unsigned short *args, struct process *process);

int reset(unsigned short *args, struct process *process);

int nop(unsigned short *args, struct process *process);

int shl(unsigned short *args, struct process *process);

int shr(unsigned short *args, struct process *process);

int jmp(unsigned short *args, struct process *process);

int je(unsigned short *args, struct process *process);

int jne(unsigned short *args, struct process *process);

int cmpxchg(unsigned short *args, struct process *process);

typedef int (*asm_func)(unsigned short *, struct process *process);

asm_func short_instructions[] = {&mov3, &je, &jne, &cmpxchg};
asm_func long_instructions[8][8] = {
        {&mov, &add, &sub, &mul, &div_, &and, &or, &xor},
        {&mov1},
        {&mov2},
        {&push, &pop, &not},
        {&call, &jmp},
        {&shl, &shr},
        {&ret, &reset, &nop},
        {&in, &out}
};

#endif //ASM_VM_FUNCTIONS_H
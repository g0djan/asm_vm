#include "instructions.h"

int mov(unsigned short *args, struct process *process) {
    process->registers[args[0]] = process->registers[args[1]];
    return 0;
}

int mov1(unsigned short *args, struct process *process) {
    process->registers[args[0]] = *(unsigned short *) process->registers[args[1]];
    return 0;
}

int mov2(unsigned short *args, struct process *process) {
    *(unsigned short *) process->registers[args[0]] = process->registers[args[1]];
    return 0;
}

int mov3(unsigned short *args, struct process *process) {
    uint8_t n = (uint8_t) (args[0] >> 1);
    unsigned short half_mask = (unsigned short) 0x0000000011111111 << !(args[0] & 1);
    process->registers[n] &= half_mask;
    process->registers[n] |= args[1] << (args[0] & 1);
    return 0;
}

int push(unsigned short *args, struct process *process) {
    *(unsigned short *) process->registers[7] = args[0];
    --process->registers[7];
    return 0;
}

int pop(unsigned short *args, struct process *process) {
    *(unsigned short *) process->registers[7] = 0;
    ++process->registers[7];
    return 0;
}

int add(unsigned short *args, struct process *process) {
    process->registers[args[0]] += process->registers[args[1]];
    return 0;
}

int sub(unsigned short *args, struct process *process) {
    process->registers[args[0]] -= process->registers[args[1]];
    return 0;
}

int mul(unsigned short *args, struct process *process) {
    process->registers[args[0]] *= process->registers[args[1]];
    return 0;
}

int div_(unsigned short *args, struct process *process) {
    process->registers[args[0]] /= process->registers[args[1]];
    return 0;
}

int and(unsigned short *args, struct process *process) {
    process->registers[args[0]] &= process->registers[args[1]];
    return 0;
}

int or(unsigned short *args, struct process *process) {
    process->registers[args[0]] |= process->registers[args[1]];
    return 0;
}

int xor(unsigned short *args, struct process *process) {
    process->registers[args[0]] ^= process->registers[args[1]];
    return 0;
}

int not(unsigned short *args, struct process *process) {
    process->registers[args[0]] = ~process->registers[args[0]];
    return 0;
}

int in(unsigned short *args, struct process *process) {
    uint8_t n = (uint8_t) (args[0] >> 1);
    unsigned short half_mask = (unsigned short) 0x0000000011111111 << !(args[0] & 1);
    process->registers[n] &= half_mask;
    process->registers[n] |= getchar() << (args[0] & 1);
    return 0;
}

int out(unsigned short *args, struct process *process) {
    uint8_t n = (uint8_t) (args[0] >> 1);
    unsigned short half_mask = (unsigned short) 0x0000000011111111 << (args[0] & 1);
    uint8_t byt = (uint8_t) (process->registers[n] & half_mask);
    byt >>= (args[0] & 1);
    putchar(byt);
    return 0;
}

int call(unsigned short *args, struct process *process) {
    push(process->ip, process);
    process->ip += (int8_t) args[0];
    return 0;
}

int ret(unsigned short *args, struct process *process) {
    process->ip = (unsigned short *) process->registers[7] + 1;
    return 0;
}

int reset(unsigned short *args, struct process *process) {
    return process->registers[0] & 0x0000000011111111;
}

int nop(unsigned short *args, struct process *process) { return 0; }

int shl(unsigned short *args, struct process *process) {
    process->registers[args[0]] <<= args[1];
    return 0;
}

int shr(unsigned short *args, struct process *process) {
    process->registers[args[0]] >>= args[1];
    return 0;
}

int jmp(unsigned short *args, struct process *process) {
    process->ip += args[0];
    return 0;
}

int je(unsigned short *args, struct process *process) {
    process->ip += !process->registers[args[1]] ? args[0] : 0;
    return 0;
}

int jne(unsigned short *args, struct process *process) {
    process->ip += process->registers[args[1]] ? args[0] : 0;
    return 0;
}

int cmpxchg(unsigned short *args, struct process *process) {
    unsigned short from_addr = *(unsigned short *)args[0];
    if (from_addr == process->registers[args[1]]) {
        *(unsigned short *)args[0] = process->registers[args[2]];
        process->registers[args[2]] = process->registers[args[1]];
    }
    return 0;
}
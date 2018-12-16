#include "instructions.h"

int mov(uint16_t *args, struct process *process) {
    process->registers[args[0]] = process->registers[args[1]];
    return 0;
}

int mov1(uint16_t *args, struct process *process) {
    process->registers[args[0]] = *(uint16_t *) process->registers[args[1]];
    return 0;
}

int mov2(uint16_t *args, struct process *process) {
    *(uint16_t *) process->registers[args[0]] = process->registers[args[1]];
    return 0;
}

int mov3(uint16_t *args, struct process *process) {
    uint8_t n = (uint8_t) (args[0] >> 1);
    uint16_t half_mask = (uint16_t) 0x0000000011111111 << !(args[0] & 1);
    process->registers[n] &= half_mask;
    process->registers[n] |= args[1] << (args[0] & 1);
    return 0;
}

int push(uint16_t *args, struct process *process) {
    *(uint16_t *) process->registers[7] = args[0];
    --process->registers[7];
    return 0;
}

int pop(uint16_t *args, struct process *process) {
    *(uint16_t *) process->registers[7] = 0;
    ++process->registers[7];
    return 0;
}

int add(uint16_t *args, struct process *process) {
    process->registers[args[0]] += process->registers[args[1]];
    return 0;
}

int sub(uint16_t *args, struct process *process) {
    process->registers[args[0]] -= process->registers[args[1]];
    return 0;
}

int mul(uint16_t *args, struct process *process) {
    process->registers[args[0]] *= process->registers[args[1]];
    return 0;
}

int div_(uint16_t *args, struct process *process) {
    process->registers[args[0]] /= process->registers[args[1]];
    return 0;
}

int and(uint16_t *args, struct process *process) {
    process->registers[args[0]] &= process->registers[args[1]];
    return 0;
}

int or(uint16_t *args, struct process *process) {
    process->registers[args[0]] |= process->registers[args[1]];
    return 0;
}

int xor(uint16_t *args, struct process *process) {
    process->registers[args[0]] ^= process->registers[args[1]];
    return 0;
}

int not(uint16_t *args, struct process *process) {
    process->registers[args[0]] = ~process->registers[args[0]];
    return 0;
}

int in(uint16_t *args, struct process *process) {
    uint8_t n = (uint8_t) (args[0] >> 1);
    uint16_t half_mask = (uint16_t) 0x0000000011111111 << !(args[0] & 1);
    process->registers[n] &= half_mask;
    process->registers[n] |= getchar() << (args[0] & 1);
    return 0;
}

int out(uint16_t *args, struct process *process) {
    uint8_t n = (uint8_t) (args[0] >> 1);
    uint16_t half_mask = (uint16_t) 0x0000000011111111 << (args[0] & 1);
    uint8_t byt = (uint8_t) (process->registers[n] & half_mask);
    byt >>= (args[0] & 1);
    putchar(byt);
    return 0;
}

int call(uint16_t *args, struct process *process) {
    push(process->ip, process);
    process->ip += (int8_t) args[0];
    return 0;
}

int ret(uint16_t *args, struct process *process) {
    process->ip = (uint16_t *) process->registers[7] + 1;
    return 0;
}

int reset(uint16_t *args, struct process *process) {
    return process->registers[0] & 0x0000000011111111;
}

int nop(uint16_t *args, struct process *process) { return 0; }

int shl(uint16_t *args, struct process *process) {
    process->registers[args[0]] <<= args[1];
    return 0;
}

int shr(uint16_t *args, struct process *process) {
    process->registers[args[0]] >>= args[1];
    return 0;
}

int jmp(uint16_t *args, struct process *process) {
    process->ip += args[0];
    return 0;
}

int je(uint16_t *args, struct process *process) {
    process->ip += !process->registers[args[1]] ? args[0] : 0;
    return 0;
}

int jne(uint16_t *args, struct process *process) {
    process->ip += process->registers[args[1]] ? args[0] : 0;
    return 0;
}

int cmpxchg(uint16_t *args, struct process *process) {
    uint16_t from_addr = *(uint16_t *)args[0];
    if (from_addr == process->registers[args[1]]) {
        *(uint16_t *)args[0] = process->registers[args[2]];
        process->registers[args[2]] = process->registers[args[1]];
    }
    return 0;
}
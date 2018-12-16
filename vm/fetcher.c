#include "fetcher.h"

uint16_t fetch_command(struct process *process) {
    return *process->ip;
}

bool is_short(uint16_t command) {
    return (bool)(command & 1);
}

uint8_t fetch_operand_class(uint16_t command) {
    return (uint8_t)(command & 0x1110) >> 1;
}

uint8_t fetch_command_id(uint16_t command) {
    uint8_t mask = (uint8_t )(is_short(command) ? 0x111 : 0x11111111);
    uint8_t shift = (uint8_t )(is_short(command) ? 3 : 8);
    return (uint8_t)command & mask >> shift;
}

asm_func fetch_func(uint16_t command) {
    if (is_short(command)) {
        return short_instructions[fetch_command_id(command)];
    } else {
        uint8_t operand_class = fetch_operand_class(command);
        return long_instructions[operand_class][fetch_command_id(command)];
    }
}

uint8_t fetch_short_args_cnt(uint16_t command) {
    return short_operand_masks[fetch_command_id(command)].len;
}

uint8_t fetch_long_args_cnt(uint16_t command) {
    uint8_t operand_class = fetch_operand_class(command);
    return long_operand_masks[operand_class].len;

}
uint8_t fetch_args_cnt(uint16_t command) {
    return is_short(command) ? fetch_short_args_cnt(command) : fetch_long_args_cnt(command);
}

void fetch_short_args(uint16_t *args, uint16_t command) {
    uint8_t cnt = fetch_args_cnt(command);
    uint8_t id = fetch_command_id(command);
    for (uint8_t i = 0; i < cnt; ++i) {
        args[i] = short_operand_masks[id].element[i] & command >> short_shifts[id].element[i];
    }
}

void fetch_long_args(uint16_t *args, uint16_t command) {
    uint8_t cnt = fetch_args_cnt(command);
    uint8_t operand_class = fetch_operand_class(command);
    for (uint8_t i = 0; i < cnt; ++i) {
        args[i] = long_operand_masks[operand_class].element[i] & command >> long_shifts[operand_class].element[i];
    }
}

void fetch_args(uint16_t *args, uint16_t command) {
    if (is_short(command)) {
        fetch_short_args(args, command);
    } else {
        fetch_long_args(args, command);
    }
}
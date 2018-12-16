#include "encoder.h"

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
uint8_t sizes[8] = {8, 1, 1, 3, 2, 2, 3, 2};

bool has_type_operand(struct asm_command *command, enum operand_type type) {
    bool has = false;
    for (size_t i = 0; i < command->operands_cnt; ++i) {
        has |= command->operands[i].type == type;
    }
    return has;
}

uint8_t get_operand_class(struct asm_command *command) {
    for (uint8_t i = 0; i < 9; ++i) {
        if (operand_classes[i].first == command->operands[0].type &&
            operand_classes[i].second == command->operands[1].type) {
            return i < 8 ? i : (int8_t)7;
        }
    }
    puts("Invalid operands");
    exit(-1);
}

uint8_t get_command_code(struct asm_command *command, const enum command *command_types, uint8_t size) {
    for (uint8_t i = 0; i < size; ++i) {
        if (command->name == command_types[i]) {
            return i;
        }
    }
    puts("Invalid code for this command");
    exit(-1);
}

uint16_t encode_short(struct asm_command *command, uint16_t bitmask) {
    bitmask |= get_command_code(command, short_commands, 4) << 1;
    return bitmask;
}

uint16_t encode_long(struct asm_command *command, uint16_t bitmask) {
    uint8_t operand_class = get_operand_class(command);
    bitmask |= operand_class << 1;
    bitmask |= get_command_code(command, long_commands[operand_class], sizes[operand_class]) << 4;
    return bitmask;
}

uint16_t encode_command(struct asm_command *command) {
    uint16_t bitmask = 0;
    bitmask |= (command->name == jmp || (!has_type_operand(command, imm8) && command->operands_cnt != 3)) << 0;
    return bitmask ? encode_long(command, bitmask) : encode_short(command, bitmask);
}

uint8_t get_register_code(struct operand *operand) {
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

uint8_t get_shift(uint8_t shift, enum operand_type type) {
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

uint16_t encode_operands(struct asm_command *command, uint16_t bitmask) {
    uint8_t shift = (int8_t) (bitmask & 1 ? 8 : 3);
    uint16_t register_code;
    for (size_t i = 0; i < command->operands_cnt; ++i) {
        if (command->operands[i].type == imm8) {
            bitmask |= (uint16_t )(command->operands[i].number) << 8;
        } else {
            register_code = (uint16_t )(get_register_code(&command->operands[i])) << shift;
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
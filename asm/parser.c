#include "parser.h"
#include "labels_table.h"

const char command_names[COMMAND_NAMES][MAX_COMMAND_LENGTH] = {
        "mov","mov1","mov2","mov3",
        "je", "jne", "jmp", "call",
        "add", "sub", "mul", "div",
        "and", "or", "xor", "not",
        "push", "pop", "shl", "shr",
        "in", "out", "reset",
        "nop", "ret", "cmpxchg"
};

char operand_types[REGISTER_TYPES][MAX_REGISTER_TYPE_LENGTH] =
        {"(RYX)", "RYX", "RYH", "RYL"};

void cut_endl(char *line) {
    size_t len = strlen(line);
    if (line[len - 1] == '\n') {
        line[len - 1] = '\0';
    }
}

bool match_operand_type(char *operand_type, char *line) {
    size_t line_len = strlen(line);
    size_t operand_len = strlen(operand_type);
    if (operand_len > line_len) {
        return false;
    }
    size_t half = operand_len / 2;
    for (size_t j = 0; j < operand_len; ++j) {
        if (line[j] != operand_type[j] && j != half) {
            return false;
        }
    }
    return true;
}

uint8_t get_register_number(char *line) {
    while (line) {
        if ('0' <= line[0] && line[0] <= '7') {
            return (uint8_t) (line[0] - '0');
        }
        ++line;
    }
    puts("Invalid register number");
    exit(-1);
}

uint8_t parse_imm(const char *line) {
    ++line;
    char *occur = strchr(line, ' ');
    size_t len = occur ? occur - line : strlen(line);
    int n = 0;
    for (size_t i = 0; i < len; ++i) {
        n = 10 * n + line[i] - '0';
    }
    if (n > 255) {
        puts("Too big imm");
        exit(-1);
    }
    return (uint8_t) n;
}

void check_diff(uint8_t diff) {
    if (diff > 255) {
        puts("Too far label");
        exit(-1);
    }
}

int8_t convert_label(char *line, uint16_t line_number) {
    char *occur = strchr(line, ' ');
    size_t operand_len = occur
                         ? occur - line
                         : strlen(line);
    uint16_t label_line = get_label_line(line, operand_len);
    uint8_t diff = (uint8_t )(line_number - label_line);
    check_diff(diff);
    return (int8_t)diff;
}


uint8_t get_imm(char *line, uint16_t line_number) {
    return line[0] == '#'
    ? parse_imm(line)
    : convert_label(line, line_number);
}

enum command get_mov(struct asm_command *command) {
    if (command->operands[1].type == _RX_) {
        return mov1;
    }
    if (command->operands[0].type == _RX_) {
        return mov2;
    }
    if (command->operands[1].type == imm8) {
        return mov3;
    }
    return mov;
}

enum command get_command(char *line) {
    size_t len;
    for (int i = 0; i < COMMAND_NAMES; ++i) {
        len = strlen(command_names[i]);
        if (!strncmp(command_names[i], line, len)) {
            return (enum command) i;
        }
    }
    puts("Unknown command");
    exit(-1);
}

struct operand get_operand(char *line, enum command command, uint16_t line_number) {
    for (size_t i = 0; i < 4; ++i) {
        if (match_operand_type(operand_types[i], line)) {
            return (struct operand) {
                    (enum operand_type) i,
                    get_register_number(line)
            };
        }
    }
    return (struct operand) {
            (command == shl || command == shr ? imm4 : imm8),
            get_imm(line, line_number)
    };
}

char *cut_token(char *line, char separator, bool exit) {
    char *ptr = strchr(line, separator);
    if (ptr == NULL) {
        return exit ? NULL : line + 1;
    }
    return ptr + 1;
}

struct asm_command parse_command(char *line, uint16_t line_number) {
    cut_endl(line);
    DEFAULT_ASM_COMMAND(command);
    command.operands_cnt = 0;
    line = cut_token(line, ':', false);
    command.name = get_command(line);
    line = cut_token(line, ' ', true);
    for (; command.operands_cnt < 3 && line; ++command.operands_cnt) {
        command.operands[command.operands_cnt] = get_operand(line, command.name, line_number);
        line = cut_token(line, ' ', true);
    }
    command.name = command.name != mov ? command.name : get_mov(&command);
    return command;
}


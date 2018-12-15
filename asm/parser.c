#include "parser.h"
#include "labels_table.h"

const char command_names[COMMAND_NAMES][MAX_COMMAND_LENGTH] = {
        "mov", "je", "jne", "jmp", "call",
        "add", "sub", "mul", "div", "and",
        "or", "xor", "not", "push", "pop",
        "shl", "shr", "in", "out", "reset",
        "nop", "ret", "cmpxchg"
};

char operand_types[REGISTER_TYPES][MAX_REGISTER_TYPE_LENGTH] =
        {"(RYX)", "RYX", "RYH", "RYL"};

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

int8_t get_register_number(char *line) {
    while (line) {
        if ('0' <= line[0] && line[0] <= '7') {
            return (int8_t) (line - '0');
        }
        ++line;
    }
    puts("Invalid register number");
    exit(-1);
}

int8_t parse_imm(const char *line, size_t len) {
    int n = 0;
    for (size_t i = 0; i < len; ++i) {
        n = 10 * n + line[i] - '0';
    }
    if (n > 255) {
        puts("Too big imm");
        exit(-1);
    }
    return (int8_t) n;
}

void check_diff(int16_t label_line, int16_t diff) {
    if (label_line != -1 && (diff > 127 || diff < -128)) {
        puts("Too far label");
        exit(-1);
    }
}

void check_label(enum command command, int16_t label_line) {
    if ((command == jmp || command == je || command == jne) && label_line == -1) {
        puts("wrong label");
        exit(-1);
    }
}

int8_t get_imm(char *line, int16_t line_number, enum command command) {
    char *occur = strchr(line, ' ');
    size_t operand_len = occur
                         ? occur - line
                         : strlen(line);
    int16_t label_line = get_label_line(line, operand_len);
    int16_t diff = label_line - line_number;
    check_diff(label_line, diff);
    check_label(command, label_line);
    return command == jmp || command == je || command == jne
           ? (int8_t) (diff)
           : parse_imm(line, operand_len);
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
    for (int i = 0; i < 23; ++i) {
        len = strlen(command_names[i]);
        if (!strncmp(command_names[i], line, len)) {
            return (enum command) i;
        }
    }
    puts("Unknown command");
    exit(-1);
}

struct operand get_operand(char *line, enum command command, int16_t line_number) {
    for (size_t i = 0; i < 6; ++i) {
        if (match_operand_type(operand_types[i], line)) {
            return (struct operand) {
                    (enum operand_type) i,
                    get_register_number(line)
            };
        }
    }
    return (struct operand) {
            (command == shl || command == shr ? imm4 : imm8),
            get_imm(line, line_number, command)
    };
}

char *cut_token(char *line, char separator, bool exit) {
    char *ptr = strchr(line, separator);
    if (ptr == NULL) {
        return exit ? NULL : line;
    }
    return ptr + 1;
}

struct asm_command parse_command(char *line, int16_t line_number) {
    struct asm_command command;
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


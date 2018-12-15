#ifndef ASM_VM_ENCODER_H
#define ASM_VM_ENCODER_H

#endif //ASM_VM_ENCODER_H
#include <stdio.h>
#include "constants.h"
#include "parser.h"

uint16_t commands[MAX_COMMANDS];
size_t comands_size;

void encode_commands(FILE *fp);
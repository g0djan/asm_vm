#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "constants.h"

#ifndef ASM_VM_LABELS_TABLE_H
#define ASM_VM_LABELS_TABLE_H

#endif //ASM_VM_LABELS_TABLE_H

char labels[MAX_COMMANDS][MAX_LABEL_LENGTH];
int16_t command_numbers[MAX_COMMANDS];
size_t table_size;

bool line_starts_with_label(char *line);
size_t get_label_len(char * line);

void init_labels_from_file(FILE *fp);
int16_t get_label_line(char *label, size_t n);
void add_record(char *label, size_t label_len, int16_t command_number);

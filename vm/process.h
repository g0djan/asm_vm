#ifndef ASM_VM_PROCESS_H
#define ASM_VM_PROCESS_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "instructions.h"
#include "fetcher.h"

struct process {
    uint16_t registers[8];
    uint16_t *ip;
    bool is_active;
};

void load_process(struct process *process, uint16_t *address, const char *file_path, uint16_t shift);
int execute_process(struct process *process, size_t iterations);

#endif //ASM_VM_PROCESS_H
#include <opencl-c.h>
#include "process.h"
#include "instructions.h"
#include "fetcher.h"

void load_process(struct process *process, uint16_t *address, const char *file_path, uint16_t shift) {
    process->is_active = true;
    process->ip = address + shift;
    process->registers[7] = shift;
    FILE *fp = fopen(file_path, "rb");
    do {
        if (address != NULL) {
            puts("Intersection of programs addresses");
            exit(-1);
        }
    } while(fread(address++, sizeof(int16_t), 1, fp));
    fclose(fp);
}

bool execute_iteration(struct process *process) {
    int func_id = fetch_func_id(process);
    uint8_t args_cnt = fetch_args_cnt(process, func_id);
    uint16_t **args = fetch_args(process, func_id, args_cnt);
    return !(instructions[func_id])(args, args_cnt);
}

int execute_process(struct process *process, size_t iterations) {
    for (size_t j = 0; j < iterations; ++j) {
        if (!process->is_active) {
            return 0;
        }
        process->is_active = execute_iteration(process);
        if (!process->is_active) {
            return 1;
        }
    }
    return 0;
}


#include "process.h"

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
    uint16_t command = fetch_command(process);
    asm_func func = fetch_func(command);
    uint16_t args[3];
    fetch_args(args, command);
    return !(*func)(args, process);
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


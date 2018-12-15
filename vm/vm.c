#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "constants.h"
#include "process.h"

uint16_t memory[MEMORY_SIZE];
struct process *processes;
uint16_t cnt_processes;

uint16_t read_address(const char *address) {
    int addr = atoi(address);
    if (addr >= MEMORY_SIZE || addr < 0) {
        puts("Too big address fro this machine");
        exit(-1);
    }
    return (uint16_t)addr;
}

void load_processes(int argc, const char *argv[]) {
    uint16_t address;
    for (size_t i = 2, j = 0; i < argc; i += 2, ++j) {
        address = read_address(argv[i + 1]);
        load_process(processes + j, memory, argv[i], address);
    }
}

void execute_all(uint16_t max_iterations) {
    uint16_t active_processes = cnt_processes;
    for (uint16_t iter = 0; active_processes > 0 && (max_iterations == 0 || iter < max_iterations); iter += ITERATIONS_BY_PROCESS) {
        for (size_t i = 0; i < cnt_processes; ++i) {
            uint16_t iterations = max_iterations - iter > ITERATIONS_BY_PROCESS
                    ? (uint16_t) ITERATIONS_BY_PROCESS
                    : max_iterations - iter;
            active_processes -= execute_process(processes + i, iterations);
        }
    }
}

int main(int argc, const char *argv[]) {
    if (argc % 2 != 1 || argc < 4) {
        puts("Invalid number of arguments");
    }
    cnt_processes = (uint16_t )(argc / 2);
    processes = (struct process*)malloc(cnt_processes * sizeof(struct process));
    uint16_t max_iterations = (uint16_t)atoi(argv[1]);
    load_processes(argc, argv);
    execute_all(max_iterations);
    free(processes);
    return 0;
}

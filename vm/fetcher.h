#include "process.h"

#ifndef ASM_VM_FETCHER_H
#define ASM_VM_FETCHER_H

#endif //ASM_VM_FETCHER_H

int fetch_func_id(struct process *process);
uint8_t fetch_args_cnt(struct process *process, int func_id);
uint16_t **fetch_args(struct process *process, int func_id, uint8_t args_cnt);
#include "labels_table.h"

void add_record(char *label, size_t label_len, uint16_t command_number) {
    if (table_size == MAX_COMMANDS) {
        perror("Too much lines in file");
        exit(-1);
    }
    memcpy(labels[table_size], label, label_len);
    labels[table_size][label_len] = '\0';
    command_numbers[table_size] = command_number;
    ++table_size;
}

uint16_t get_label_line(char *label, size_t n) {
    for (size_t i = 0; i < table_size; ++i) {
        if (!strncmp(label, labels[i], n)) {
            return command_numbers[i];
        }
    }
    puts("Label not found");
    exit(-1);
}

void init_labels_from_file(FILE *fp) {
    char line[MAX_LINE_LENGTH];
    size_t label_len = 0;
    for (uint16_t i = 0; fgets(line, MAX_LINE_LENGTH, fp); ++i) {
        if (!line_starts_with_label(line))
            continue;
        label_len = get_label_len(line);
        add_record(line, label_len, i);
    }
    rewind(fp);

}

bool line_starts_with_label(char *line) {
    return strlen(line) > 0 && line[0] != ' ' && line[0] != '\n';
}

size_t get_label_len(char *line) {
    char *occur = strchr(line, ':');
    if (occur == NULL) {
        puts("Incorrect label");
        exit(-1);
    }
    return occur - line;
}
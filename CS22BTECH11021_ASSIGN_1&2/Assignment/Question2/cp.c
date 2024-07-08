
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#define BUF_SIZE 1024

void copy_file(const char *source, const char *dest) {
    FILE *source_file = fopen(source, "rb");
    if (!source_file) {
        perror("fopen source");
        exit(EXIT_FAILURE);
    }

    FILE *dest_file = fopen(dest, "wb");
    if (!dest_file) {
        perror("fopen dest");
        fclose(source_file);
        exit(EXIT_FAILURE);
    }

    char buffer[BUF_SIZE];
    size_t num_read;
    while ((num_read = fread(buffer, 1, BUF_SIZE, source_file)) > 0) {
        if (fwrite(buffer, 1, num_read, dest_file) != num_read) {
            perror("fwrite dest");
            fclose(source_file);
            fclose(dest_file);
            exit(EXIT_FAILURE);
        }
    }

    if (ferror(source_file)) {
        perror("fread source");
        fclose(source_file);
        fclose(dest_file);
        exit(EXIT_FAILURE);
    }

    fclose(source_file);
    fclose(dest_file);
}

void move_file(const char *source, const char *dest) {
    if (rename(source, dest) == -1) {
        copy_file(source, dest);
        if (remove(source) == -1) {
            perror("remove source");
            exit(EXIT_FAILURE);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s [-i] [-f] [-n] [-v] <source> <dest>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int ignore_case = 0;
    int force = 0;
    int line_numbers = 0;
    int invert_match = 0;
    int arg_index = 1;

    while (arg_index < argc && argv[arg_index][0] == '-') {
        // Check for command-line options
        char *option = argv[arg_index];
        if (strcmp(option, "-i") == 0) {
            ignore_case = 1;
        } else if (strcmp(option, "-f") == 0) {
            force = 1;
        } else if (strcmp(option, "-n") == 0) {
            line_numbers = 1;
        } else if (strcmp(option, "-v") == 0) {
            invert_match = 1;
        } else {
            fprintf(stderr, "Unknown option: %s\n", option);
            exit(EXIT_FAILURE);
        }
        arg_index++;
    }

    if (arg_index != argc - 2) {
        fprintf(stderr, "Usage: %s [-i] [-f] [-n] [-v] <source> <dest>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *source = argv[arg_index];
    char *dest = argv[arg_index+1];

    if (!force && access(dest, F_OK) == 0) {
        fprintf(stderr, "cp: overwrite '%s'? ", dest);
        char answer[2];
        if (fgets(answer, sizeof(answer), stdin) == NULL || tolower(answer[0]) != 'y') {
            exit(EXIT_SUCCESS);
        }
    }

    move_file(source, dest);

    return EXIT_SUCCESS;
}

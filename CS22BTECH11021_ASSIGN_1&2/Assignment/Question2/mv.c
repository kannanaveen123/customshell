
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define BUF_SIZE 1024

bool interactive_mode = false;
bool force_mode = false;
bool no_clobber_mode = false;
bool verbose_mode = false;

void copy_file(const char *source_path, const char *dest_path) {
    int source_fd = open(source_path, O_RDONLY);
    if (source_fd == -1) {
        perror("open source");
        exit(EXIT_FAILURE);
    }

    int dest_fd = open(dest_path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (dest_fd == -1) {
        perror("open dest");
        close(source_fd);
        exit(EXIT_FAILURE);
    }

    char buffer[BUF_SIZE];
    ssize_t num_read;
    while ((num_read = read(source_fd, buffer, BUF_SIZE)) > 0) {
        if (write(dest_fd, buffer, num_read) != num_read) {
            perror("write dest");
            close(source_fd);
            close(dest_fd);
            exit(EXIT_FAILURE);
        }
    }

    if (num_read == -1) {
        perror("read source");
        close(source_fd);
        close(dest_fd);
        exit(EXIT_FAILURE);
    }

    close(source_fd);
    close(dest_fd);
}

void move_file(const char *source_path, const char *dest_path) {
    if (rename(source_path, dest_path) == -1) {
        if (no_clobber_mode && access(dest_path, F_OK) != -1) {
            fprintf(stderr, "cp: %s: File exists (not copied)\n", dest_path);
            return;
        }

        if (force_mode || interactive_mode) {
            char choice;
            if (interactive_mode) {
                printf("cp: overwrite '%s'?", dest_path);
                scanf(" %c", &choice);  // add space before %c to consume newline character
            } else {
                choice = 'y';
            }

            if (choice == 'y' || choice == 'Y') {
                copy_file(source_path, dest_path);
                if (unlink(source_path) == -1) {
                    perror("unlink source");
                    exit(EXIT_FAILURE);
                }
            }
        } else {
            copy_file(source_path, dest_path);
            if (unlink(source_path) == -1) {
                perror("unlink source");
                exit(EXIT_FAILURE);
            }
        }
    }
}

int main(int argc, char *argv[]) {
    int opt;
    while ((opt = getopt(argc, argv, "ifnrv")) != -1) {
        switch (opt) {
            case 'i':
                interactive_mode = true;
                break;
            case 'f':
                force_mode = true;
                break;
            case 'n':
                no_clobber_mode = true;
                break;
            case 'v':
                verbose_mode = true;
                break;
            default:
                fprintf(stderr, "Usage: %s [-ifnrv] source_file dest_file\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (argc - optind != 2) {
        fprintf(stderr, "Usage: %s [-ifnrv] source_file dest_file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *source_path = argv[optind];
    char *dest_path = argv[optind + 1];

    if (verbose_mode) {
        printf("'%s' -> '%s'\n", source_path, dest_path);
    }

    move_file(source_path,dest_path);
return(0);
}
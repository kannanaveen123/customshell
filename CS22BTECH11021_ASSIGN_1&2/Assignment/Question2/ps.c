#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define MAX_PATH_LEN 256
#define MAX_LINE_LEN 1024

int main(int argc, char *argv[]) {
    char path[MAX_PATH_LEN];
    char line[MAX_LINE_LEN];
    char status;
    char *option = NULL;
    DIR *dir;
    struct dirent *ent;
    FILE *file;

    // Check if an option was specified
    if (argc > 1) {
        option = argv[1];
    }

    // Print the header line
    printf("PID\tSTATUS\tCOMMAND\n");

    // Open the directory containing the process information
    dir = opendir("/proc");
    if (dir == NULL) {
        perror("Failed to open /proc directory");
        return EXIT_FAILURE;
    }

    // Iterate over each directory in /proc
    while ((ent = readdir(dir)) != NULL) {
        // Skip non-numeric directories
        if (sscanf(ent->d_name, "%*[^0-9]%*d") != 1) {
            continue;
        }

        // Build the path to the process status file
        snprintf(path, sizeof(path), "/proc/%s/status", ent->d_name);

        // Open the process status file
        FILE *status_file = fopen(path, "r");
        if (status_file == NULL) {
            continue;
        }

        // Read the process status from the file
        while (fgets(line, sizeof(line), status_file) != NULL) {
            if (strncmp(line, "State:", 6) == 0) {
                status = line[7];
                break;
            }
        }

        // Check if the process status matches the specified option
        if (option == NULL || strcmp(option, "-r") == 0) {
            if (status == 'R') {
                // Process is running
                goto print_process_info;
            }
        }
        if (option == NULL || strcmp(option, "-s") == 0) {
            if (status == 'S') {
                // Process is sleeping
                goto print_process_info;
            }
        }
        if (option == NULL || strcmp(option, "-t") == 0) {
            if (status == 'T') {
                // Process is stopped
                goto print_process_info;
            }
        }
        if (option == NULL || strcmp(option, "-d") == 0) {
            if (status == 'D') {
                // Process is waiting for I/O
                goto print_process_info;
            }
        }
        fclose(status_file);
        continue;

        // Print the process information
        print_process_info:
        // Build the path to the process command file
        snprintf(path, sizeof(path), "/proc/%s/cmdline", ent->d_name);

        // Open the process command file
        FILE *cmdline_file = fopen(path, "r");
        if (cmdline_file == NULL) {
            fclose(status_file);
            continue;
        }

        // Read the process command from the file
        if (fgets(line, sizeof(line), cmdline_file) != NULL) {
            printf("%s\t%c\t%s\n", ent->d_name, status, line);
        }

        fclose(cmdline_file);
        fclose(status_file);
    }

    // Close the directory
    closedir(dir);
    return 0;
}

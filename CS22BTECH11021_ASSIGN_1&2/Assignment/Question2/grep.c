#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256

int main(int argc, char *argv[]) {
    char search_string[MAX_LINE_LENGTH];
    char line[MAX_LINE_LENGTH];
    FILE *file_pointer;
    int line_number, line_count, total_count;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <search_string> <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Copy the search string from command line argument
    strncpy(search_string, argv[1], sizeof(search_string)-1);
    search_string[sizeof(search_string)-1] = '\0'; // Ensure null-termination

    // Open the file
    if ((file_pointer = fopen(argv[2], "r")) == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    // Command -H: Print the file name with each matched line
    printf("%s:\n", argv[2]);

    // Command -n: Print the line number with each matched line
    line_number = 0;
    while (fgets(line, sizeof(line), file_pointer)) {
        line_number++;
        if (strstr(line, search_string)) {
            printf("%d:%s", line_number, line);
        }
    }

    // Command -p: Print the number of lines containing the search string
    rewind(file_pointer);
    line_count = 0;
    while (fgets(line, sizeof(line), file_pointer)) {
        if (strstr(line, search_string)) {
            line_count++;
        }
    }
    printf("Number of lines containing the search string: %d\n", line_count);

    // Command -c: Print the number of occurrences of the search string
    rewind(file_pointer);
    total_count = 0;
    while (fgets(line, sizeof(line), file_pointer)) {
        char *ptr = line;
        int count = 0;
        while ((ptr = strstr(ptr, search_string))) {
            count++;
            ptr++;
        }
        total_count += count;
        printf("%d:%s", count, line);
    }
    printf("Total number of occurrences: %d\n", total_count);

    // Close the file
    fclose(file_pointer);

    return EXIT_SUCCESS;
}


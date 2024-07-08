#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

int main() {
    char prompt[256];
    char cwd[2000];
    char *username = getlogin();
    char *input;

    while(1) {
        // Construct prompt string
        gethostname(prompt, 256);
        strcat(prompt, "@");
        strcat(prompt, username);
        strcat(prompt, ":");
        getcwd(cwd, 3000);
        strcat(prompt, cwd);
        strcat(prompt, "> ");

        // Read input
        input = readline(prompt);

        // Check for exit command
        if(strcmp(input, "exit") == 0) {
            free(input);
            break;
        }

        // Add command to history
        add_history(input);
        if(!strcmp(input,"help")) system("bash -c help");
        // Execute command
        if(system(input) == -1) {
            perror("system");
        }

        free(input);
    }

    return 0;
}

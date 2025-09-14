/* 
Jonathan Stoll
CS 446 - HW1
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>

int parseInput(char*, char [][500], int);
void changeDirectories(const char*);
int executeCommand(char* const*, const char*, const char*);

int main() {
    do {
        char CLI[500];
        char splitWords[100][500];

        // Get command from user
        printf("<jonathanstoll>:<%s>$", getcwd(NULL, 0));
        fgets(CLI, sizeof(CLI), stdin);
        int numWords = parseInput(CLI, splitWords, 100);


        // Process and execute user command
        if (numWords == 0)
            continue; // skip loop iteration and prompt again
        else if (strcmp(splitWords[0], "cd") == 0) {
            if (numWords == 2)
                changeDirectories(splitWords[1]);
            else
                printf("Path Not Formatted Correctly\n");
        }
        else if (strcmp(splitWords[0], "exit") == 0) {
            return 0;
        }
        else {
            char infile[500] = "";
            char outfile[500] = "";
            char *enteredCommand[numWords + 1];
            int numCommands = 0;
            int badRedirection = 0;

            // Parse splitWords, copying to enteredCommand until < or > are uncountered or numWords is reached
            for (int i = 0; i < numWords; i++) {

                if (strcmp(splitWords[i], "<") == 0) {
                    if (i + 1 < numWords)
                        strcpy(infile, splitWords[i + 1]);
                    else {
                        printf("No input file specified after '<'\n");
                        badRedirection = 1;
                    }
                    break;
                }
                else if (strcmp(splitWords[i], ">") == 0) {
                    if (i + 1 < numWords)
                        strcpy(outfile, splitWords[i + 1]);
                    else {
                        printf("No output file specified after '>'\n");
                        badRedirection = 1;
                    }
                    break;
                }
                else {
                    enteredCommand[numCommands++] = splitWords[i]; // everything before < or > gets copied directly to enteredCommand
                }

            }
            enteredCommand[numCommands] = NULL; // insert nullptr at last element

            if (badRedirection == 1)
                    continue; // Skip iteration if no in/out file provided to >/<

            executeCommand(enteredCommand, infile, outfile);
        }

    } while(1); // loop until user enters "exit"

    return 0;
}

// Execute given command
int executeCommand(char * const* enteredCommand, const char* infile, const char* outfile) {
    int forkResult = fork();

    if (forkResult == -1) { // Parent, fork failed
        printf("%s%s\n", "fork Failed: ", strerror(errno));
        return -1;
    }
    else if (forkResult == 0) { // Child

        if (infile != NULL && infile[0] != '\0') {
            int fd = open(infile, O_RDONLY, 0666);
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        else if (outfile != NULL && outfile[0] != '\0') {
            int fd = open(outfile, O_WRONLY|O_CREAT|O_TRUNC, 0666);
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        execvp(enteredCommand[0], enteredCommand);

        // Executes if execvp failed
        printf("%s%s\n", "exec Failed: ", strerror(errno));
        _exit(1);
        
    }
    else { // Parent, wait for child
        int status;
        wait(&status);

        if (WIFEXITED(status) != 0 && WEXITSTATUS(status) != 0)
            printf("Child finished with error status: %d\n", WEXITSTATUS(status));

        return 0;
    }
}

// Change working directory
void changeDirectories(const char* path){
    int chdirOutput = chdir(path);

    if (chdirOutput == -1) {
        printf("%s%s\n", "chdir Failed: ", strerror(errno));
    }
}

// Split user input into separate words
int parseInput(char* input, char splitWords[][500], int maxWords) {
    int count = 0;
    char* word = strtok(input, " \t\n"); // get first word, split on whitespace

    while (word != NULL && count < maxWords) {
        strcpy(splitWords[count], word); // copy word into array row
        count++;
        word = strtok(NULL, " \t\n"); // get next word, split on whitespace
    }

    return count;
}
// TODO: Impliment executeCommand(), debug seg fault, ensure executeCommand() is recieving correct contents

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

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
        if (strcmp(splitWords[0], "cd") == 0) {
            if (numWords == 2)
                changeDirectories(splitWords[1]);
            else
                printf("Path Not Formatted Correctly\n");
        }
        else if (strcmp(splitWords[0], "exit") == 0) {
            return 0;
        }
        else {
            int numCommands = numWords + 1; // 1 larger than splitWords to include NULL pointer at end
            char infile[500];
            char outfile[500];
            char *enteredCommand[numCommands];

            // Parse splitWords, copying to enteredCommand until < or > are uncountered or numWords is reached
            for (int i = 0; i < numWords; i++) {
                if (strcmp(splitWords[i], "<") == 0) {
                    strcpy(infile, splitWords[i+1]);
                    break;
                }
                else if (strcmp(splitWords[i], ">") == 0) {
                    strcpy(outfile, splitWords[i+1]);
                    break;
                }
                strcpy(enteredCommand[i], splitWords[i]); // everything before < or > gets copied directly to enteredCommand
            }
            enteredCommand[numCommands - 1] = NULL; // insert nullptr at last element


            executeCommand(enteredCommand, infile, outfile);
        }

    } while(1); // while true

    return 0;
}

// Execute given command
int executeCommand(char * const* enteredCommand, const char* infile, const char* outfile) {
    for (int i = 0; enteredCommand[i] != NULL; i++) {
        printf("%s\n", enteredCommand[i]);
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
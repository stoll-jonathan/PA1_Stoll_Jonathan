// TODO: Impliment changeDirectories() and executeCommand(), finish control direction logic in if statement, remove debugging section

#include <stdio.h>
#include <string.h>
#include <unistd.h>

int parseInput(char *, char [][500], int);
//changeDirectories();
//executeCommand();

int main() {
    do {
    char CLI[100];
    char splitWords[50][500];

    // Get command from user
    printf("<jonathanstoll>:<%s>$", getcwd(NULL, 0));
    fgets(CLI, sizeof(CLI), stdin);
    int numWords = parseInput(CLI, splitWords, 50);


    // FOR DEBUGGING, REMOVE LATER: Print each word in user's command on a different line
    for (int i = 0; i < numWords; i++) {
        printf("%s\n", splitWords[i]);
    }


    // Process and execute user command
    if (strcmp(splitWords[0], "exit") == 0) {
        return 0;
    }

    } while(1); // while true

    return 0;
}

// Split user input into separate words
int parseInput(char *input, char splitWords[][500], int maxWords) {
    int count = 0;
    char *word = strtok(input, " \t\n"); // get first word, split on whitespace

    while (word != NULL && count < maxWords) {
        strcpy(splitWords[count], word); // copy word into array row
        count++;
        word = strtok(NULL, " \t\n"); // get next word, split on whitespace
    }

    return count;
}
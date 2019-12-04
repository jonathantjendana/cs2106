/*************************************
 * Lab 2 Exercise 3
 * Name: Yang Chenglong
 * Student No: A0167107H
 * Lab Group: 03 
 *************************************
 Warning: Make sure your code works on
 lab machine (Linux on x86)
 *************************************/

#include <stdio.h>
#include <fcntl.h>      //For stat()
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>   //for waitpid()
#include <unistd.h>     //for fork(), wait()
#include <string.h>     //for string comparison etc
#include <stdlib.h>     //for malloc()

// max token num of one single cmd
#define MAXTOKENNUM 11
#define MAXTOKENSIZE 19
#define MAXCOMMANDNUM 10
// max token num of one line
#define MAXTOKENSNUM (11*10+9)

char** readTokens(char* buffer, int* readTokenNum, int maxTokenNum);
void chainCommand(char*** commands, int commandsNum, int* tokensNum);
void freeTokenArray(char** strArr, int size);
void freeCommandsArray(char*** commandsTokens, int commandNum, int* tokenNum);

int main() {
    char *basePath = "/bin/";
    int pid, commandNum, tokenNum;
    int commandsTokenNum[MAXCOMMANDNUM];
    // more than enough
    char buffer[3000] = {0};
    printf("GENIE>");
    fflush(stdout);
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        fflush(stdin);
        buffer[strlen(buffer)-1] = '\0'; // get rid of \n
        char **tokens = readTokens(buffer, &tokenNum, MAXTOKENSNUM);

        memset(buffer, 0, sizeof(buffer));
        memset(commandsTokenNum, 0, sizeof(commandsTokenNum));
        char ***commandsTokens = malloc(MAXCOMMANDNUM*sizeof(char**));
        for (int i = 0; i < MAXCOMMANDNUM; ++i) {
            commandsTokens[i] = NULL;
        }

        for (pid = 0, commandNum = 0; pid < tokenNum; ++pid, ++commandNum) {
            commandsTokens[commandNum] = malloc(MAXTOKENNUM*sizeof(char*));
            for (int i = 0; i < MAXTOKENNUM; ++i) {
                commandsTokens[commandNum][i] = NULL;
            }
            while (pid < tokenNum && strcmp(tokens[pid], "|")) {
                commandsTokens[commandNum][commandsTokenNum[commandNum]] = malloc(MAXTOKENSIZE*sizeof(char));
                strcpy(commandsTokens[commandNum][commandsTokenNum[commandNum]++], tokens[pid++]);
            }
        }

        freeTokenArray(tokens, tokenNum);

        for (int i = 0; i < commandNum; ++i) {
            if (!strcmp(commandsTokens[i][0], "quit")) {
                freeCommandsArray(commandsTokens, commandNum, commandsTokenNum);
                printf("Goodbye!\n");
                fflush(stdout);
                return 0;
            } else if (commandsTokens[i][0][0] != '/') {
                // simple command, prepend `/bin/` in front
                char *tmp = malloc((MAXTOKENSIZE+5)*sizeof(*tmp));
                memcpy(tmp, basePath, 5);
                for (int j = 0; commandsTokens[i][0][j]; ++j) {
                    tmp[5+j] = commandsTokens[i][0][j];
                }
                tmp[5+strlen(commandsTokens[i][0])] = '\0';
                free(commandsTokens[i][0]);
                commandsTokens[i][0] = tmp;
            }
        }

        
        if ((pid = fork())== -1) {
            perror("failed fork\n");
            exit(1);
        }
        // parent process
        if (pid) {
            freeCommandsArray(commandsTokens, commandNum, commandsTokenNum);
            wait(NULL);
            printf("GENIE>");
            fflush(stdout);
            continue;
        }
        // child process 
        for (int i = 0; i < commandNum; ++i) {
            struct stat status;
            int notExist = stat(commandsTokens[i][0], &status);
            if (notExist) {
                fprintf(stderr, "%s not found\n", commandsTokens[i][0]);
                freeCommandsArray(commandsTokens, commandNum, commandsTokenNum);
                return 0;
            }
        }
        chainCommand(commandsTokens, commandNum, commandsTokenNum);
        freeCommandsArray(commandsTokens, commandNum, commandsTokenNum);
        return 0;
    }
    return 0;
}


// the provided readTokens is useless when dealing with argument like "abc def    ghijk"
// the space will get ignored, and there is no way you can know how many spaces you omitted.
// out of frustration of the original readTokens function, I decided to write my own one.
char** readTokens(char* buffer, int* readTokenNum, int maxTokenNum) {
    char** tokenStrArr;
    int i,cnt = 0;

    //allocate token array, each element is a char*
    tokenStrArr = (char**) malloc(sizeof(char*) * maxTokenNum);
    
    //Nullify all entries
    for (i = 0; i < maxTokenNum; i++) {
        tokenStrArr[i] = NULL;
    }

    char *tmp = malloc(MAXTOKENSIZE);
    i = 0;
    while (buffer[i]) {
        memset(tmp, 0, MAXTOKENSIZE);
        int pos = 0;
        while (1) {
            while (buffer[i] == '"') {
                ++i;
                while (buffer[i] != '"') { // pair up the double quotes
                    tmp[pos++] = buffer[i++];
                }
                ++i;
            }
            if (buffer[i] == ' ' || buffer[i] == '\0') { // end of one argument
                tokenStrArr[cnt] = malloc(sizeof(char)*MAXTOKENSIZE);
                strncpy(tokenStrArr[cnt++], tmp, MAXTOKENSIZE-1);
                ++i;
                break;
            }
            // normal condition
            tmp[pos++] = buffer[i++];
        }
    }
    *readTokenNum = cnt;
    free(tmp);
    return tokenStrArr;
}


void chainCommand(char*** commands, int commandsNum, int* tokensNum) {
    if (commandsNum == 1) {
        execv(commands[0][0], commands[0]);        
        perror("execv failed\n");
        exit(1);
    }
    // n commands need n-1 pipes, to generalize the code below
    // I created n pipes
    int *fd = malloc((commandsNum*2)*sizeof(int));
    memset(fd, 0, sizeof(*fd)*commandsNum*2);
    int pid;
    for (int i = 0; i < commandsNum; ++i) {
        if (pipe(fd+i*2) == -1) {
            perror("pipe failed\n");
            exit(1);
        }
        if ((pid = fork()) == -1) {
            perror("fork failed\n");
            exit(1);
        }
        // child process
        else if (!pid) {
            if (i == 0) { // first command
                close(fd[0]); // close read end of pipe for first command
                dup2(fd[1],STDOUT_FILENO); // write to end of pipe
            } else if (i == commandsNum-1) { // last command
                close(fd[i*2-1]); // close write end of the pipe
                dup2(fd[i*2-2], STDIN_FILENO); // read from end of pipe
                for (int j = 0; j < 2*i+2; ++j) {
                    if (j != i*2-2) {
                        close(fd[j]);
                    }
                }
            } else {
                dup2(fd[i*2-2], STDIN_FILENO); // read from previous pipe
                dup2(fd[i*2+1], STDOUT_FILENO); // write to end of pipe
                for (int j = 0; j < 2*i+2; ++j) {
                    if (j != i*2-2 && j != i*2+1) {
                        close(fd[j]);
                    }
                }
            }
            execv(commands[i][0], commands[i]);
            perror("execv failed\n");
            return;
        }
    }
    for (int i = 0; i < commandsNum*2; ++i) {
        close(fd[i]);
    }
    free(fd);
    for (int i = 0; i < commandsNum; ++i) {
        wait(NULL);
    }
    return;
}


void freeCommandsArray(char*** commandsTokens, int commandNum, int* tokenNum) {
    for (int i = 0; i < commandNum; ++i) {
        if (!commandsTokens[i]) break;
        for (int j = 0; j < tokenNum[i]; ++j) {
            if (!commandsTokens[i][j]) break;
            free(commandsTokens[i][j]);
            commandsTokens[i][j] = NULL;
        }
        free(commandsTokens[i]);
        commandsTokens[i] = NULL;
    }
    free(commandsTokens);
    return;
}


void freeTokenArray(char** tokenStrArr, int size) {
    for (int i = 0; i < size; i++) {
        free(tokenStrArr[i]);
        tokenStrArr[i] = NULL;
    }
    free(tokenStrArr);
    return;
}

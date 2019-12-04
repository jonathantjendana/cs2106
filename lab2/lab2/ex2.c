/*************************************
 * Lab 2 Exercise 2
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

#define MAXTOKENNUM 10
#define MAXTOKENSIZE 19

char** readTokens(char* buffer, int* readTokenNum);
void freeTokenArray(char** strArr, int size);

int main() {
    int pid;
    char *basePath = "/bin/";
    // more than enough
    char buffer[300] = {0};
    printf("GENIE>");
    fflush(stdout);
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        fflush(stdin);
        buffer[strlen(buffer)-1] = '\0'; // get rid of \n
        int tokenNum;
        char **tokens = readTokens(buffer, &tokenNum);
        memset(buffer, 0, sizeof(buffer));
        if (!strcmp(tokens[0], "quit")) {
            printf("Goodbye!\n");
            fflush(stdout);
            freeTokenArray(tokens, tokenNum);
            return 0;
        } else {
            if ((pid = fork()) == -1) {
                perror("failed fork\n");
                exit(1);
            }
            // parent process
            if (pid) {
                wait(NULL);
                printf("GENIE>");
                fflush(stdout);
                freeTokenArray(tokens, tokenNum);
                continue;
            }
            // child process
            if (tokens[0][0] != '/') {
                //simple command, prepend `/bin` in front
                char *tmp = malloc((MAXTOKENSIZE+5)*sizeof(*tmp));
                memcpy(tmp, basePath, 5);
                for (int i = 0; tokens[0][i]; ++i) {
                    tmp[5+i] = tokens[0][i];
                }
                tmp[5+strlen(tokens[0])] = '\0';
                free(tokens[0]);
                tokens[0] = tmp;
            }
            struct stat status;
            int notExist = stat(tokens[0], &status);
            if (notExist) {
                fprintf(stderr,"%s not found\n", tokens[0]);
                freeTokenArray(tokens, tokenNum);
                return 0;
            }
            execv(tokens[0], tokens);
            perror("execv failed\n");
            exit(1);
        }
    }
    return 0;
}

// the provided readTokens is useless when dealing with argument like "abc def    ghijk"
// the space will get ignored, and there is no way you can know how many spaces you omitted.
// out of frustration of the original readTokens function, I decided to write my own one.
char** readTokens(char* buffer, int* readTokenNum) {
    char** tokenStrArr;
    int i,cnt = 0;

    //allocate token array, each element is a char*
    tokenStrArr = (char**) malloc(sizeof(char*) * MAXTOKENNUM);
    
    //Nullify all entries
    for (i = 0; i < MAXTOKENNUM; i++) {
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


void freeTokenArray(char** tokenStrArr, int size) {
    int i = 0;

    //Free every string stored in tokenStrArr
    for (i = 0; i < size; i++) {
        if (tokenStrArr[i] != NULL) {
            free(tokenStrArr[i]);
            tokenStrArr[i] = NULL;
        }
    }
    //Free entire tokenStrArr
    free(tokenStrArr);

    //Note: Caller still need to set the strArr parameter to NULL
    //      afterwards
}



/*************************************
 * Lab 2 Exercise 1
 * Name: Yang Chenglong
 * Student No: A0167107H
 * Lab Group: 03
 *************************************
 Warning: Make sure your code works on
 lab machine (Linux on x86)
 *************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     //for fork(), wait()
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void readFromFile(char who[50], int fd);
void freadFromFile(char who[50], FILE* fp);

int main(int argc, char **argv) {
    char* fileName = NULL;
    int nChild = 0;
    int fd = 0;
    char who[50];
    int pids[65535] = {0};
    pid_t childPID;
    
    if (argc < 3)
    {
        printf ("Usage: %s <filename> <number_of_processes>\n", argv[0]);
        return 1;
    }
    
    fileName = argv[1];
    nChild = atoi(argv[2]);

    fd = open( fileName, O_RDONLY );
    //TODO check the error code
    if (fd == -1) {
        fprintf(stderr, "Error when opening the file\n");
        exit(-1);
    }

    sprintf(who, "Parent [%d]", getpid());
    // Read 1 character from the file using the system call read
    char charBuf = 0;
    char strBuf[128];
    ssize_t readBytes = read(fd, &charBuf, 1);
    if (readBytes != 1) {
        if (readBytes == 0) {
            printf("EOF\n");
            return 0;
        } else if (readBytes == -1) {
            fprintf(stderr, "Error when opening the file\n");
            exit(-1);
        }
    }
    sprintf(strBuf, "%s: %c\n", who, charBuf);
    write (STDOUT_FILENO, strBuf, strlen(strBuf));

    // Create n child processes
    int i; 
    for (i = 0; i < nChild; ++i) {
       childPID = fork();
       if (!childPID) {
           break;
       }
       pids[childPID] = i+1;
    }
    if (childPID) {
        sprintf(who, "Parent [%d]", getpid());
        readFromFile(who, fd);
    } else {
        sprintf(who, "Child %d[%d]", i+1, getpid());
        readFromFile(who, fd);
        close(fd);
        return 0;
    }
    for (i = 0; i < nChild; ++i) {
        childPID = wait(NULL);
        if (childPID != -1)
            printf("Parent: Child %d[%d] done.\n", pids[childPID], childPID);
    }
    close (fd);

    // one more time, using fopen
    FILE *fp = fopen(fileName, "r");

    sprintf(who, "Parent [%d]", getpid());
    // Read 1 character from the file using the system call read
    charBuf = 0;
    readBytes = fread(&charBuf, sizeof(charBuf), 1, fp);
    if (readBytes != 1) {
        if (readBytes == 0) {
            printf("EOF\n");
            return 0;
        }
    }
    sprintf(strBuf, "%s: %c\n", who, charBuf);
    fwrite (strBuf, sizeof(char), strlen(strBuf), stdout);
    // Create n child processes
    for (i = 0; i < nChild; ++i) {
       childPID = fork();
       if (!childPID) {
           break;
       }
       pids[childPID] = i+1;
    }
    if (childPID) {
        sprintf(who, "Parent [%d]", getpid());
        freadFromFile(who, fp);
    } else {
        sprintf(who, "Child %d[%d]", i+1, getpid());
        freadFromFile(who, fp);
    }
    for (i = 0; i < nChild; ++i) {
        childPID = wait(NULL);
        if (childPID != -1)
            printf("Parent: Child %d[%d] done.\n", pids[childPID], childPID);
    }
    fclose (fp);
    return 0;
}   


void readFromFile(char who[50], int fd) {
	ssize_t readBytes = 1;
	char charBuf = 0;
    char strBuf[128];

	while (readBytes > 0) {
		usleep (1000);
		charBuf = 0;
		readBytes = read(fd, &charBuf, 1);
        if (charBuf == '\n') {
            return;
        }

		if( readBytes != 1 ) 
        {
			if( readBytes == 0 ) 
            {
				//printf( "EOF\n");
				return;
			}
		}
        sprintf(strBuf, "%s: %c\n", who, charBuf);
	    write (STDOUT_FILENO, strBuf, strlen(strBuf));
		//TODO add your code
	}
    return;
}


void freadFromFile(char who[50], FILE* fp) {
    ssize_t readBytes = 1;
	char charBuf = 0;
    char strBuf[128];

	while (readBytes > 0) {
		usleep (1000);
		charBuf = 0;
		readBytes = fread(&charBuf, sizeof(charBuf), 1, fp);
        if (charBuf == '\n') {
            return;
        }

		if( readBytes != 1 ) {
			if( readBytes == 0 ) 
            {
				//printf( "EOF\n");
				return;
			}
		}
        sprintf(strBuf, "%s: %c\n", who, charBuf);
	    fwrite (strBuf, sizeof(char), strlen(strBuf), stdout);
		//TODO add your code
	}
    return;
}

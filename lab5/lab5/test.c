#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "my_stdio.h"

int main() {
    MY_FILE *test = my_fopen("test.txt", "a+");
    char *buffer = calloc(50000,sizeof(char));
    printf("buffer is at %p\n", buffer);
    char buffer1[] = "aaaa";
    int tmp = my_fread(buffer, 1, 8192, test);
    printf("read %d chars, content is: %s\n", tmp, buffer);
    printf("%s\n", buffer);
    printf("buffer is at %p\n", buffer);
    tmp = my_fread(buffer, 1, 8192, test);
    printf("read %d chars, content is: %s\n", tmp, buffer);
    printf("buffer is at %p\n", buffer);
    free(buffer);
    /*
    write(test,buffer1, sizeof(buffer1)-1);
    memset(buffer, 0, sizeof(buffer));
    lseek(test, 0, SEEK_SET);
    flag = read(test, buffer, 4);
    printf("read %d chars, content is:\n", flag);
    printf("%s\n", buffer);
    write(test,buffer1, sizeof(buffer1)-1);
    */
}

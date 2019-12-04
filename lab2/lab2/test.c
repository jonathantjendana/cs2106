#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main(void) {
    char a = 0;
    FILE *fp = fopen("file_abc", "r");
    if (!fp) {
        printf("here\n");
        return 0;
    }
    for (int i = 0; i < 10; ++i) {
        fread(&a, sizeof(typeof(a)), sizeof(a), fp);
        fwrite(&a, sizeof(typeof(a)), sizeof(a), stdout);
        printf("test\n");
    }
    return 0;
}

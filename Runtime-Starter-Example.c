#include <stdio.h>
#include <stdlib.h>

void starter(void);

int stack[1024];
int stackPtr = 0;

void *buffer;
int ptr;
void (*fun[])(void) = {};
void (*imp)(); 

int main(int argc, char const *argv[]){
    FILE *file = fopen(argv[1], "rb");
    fseek(file, 0, SEEK_END);
    int tmp = ftell(file);
    fseek(file, 0, SEEK_SET);
    buffer = malloc(tmp);
    fread(buffer, tmp, 1, file);
    imp = fun[*(int*)(buffer + (ptr = 0))];
    while (1)imp();
}

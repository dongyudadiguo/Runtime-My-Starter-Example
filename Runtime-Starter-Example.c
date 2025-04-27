#include <stdio.h>
#include <stdlib.h>

void STK(void);
int *stack;
int stackPtr = 0;
int relativePtr = 0;

void *buffer;
int ptr;
void (*fun[])(void) = {STK};
void (*imp)(); 

void STK(void){
    stack[stackPtr++] = relativePtr;
    stack[stackPtr++] = ptr;
    imp = fun[*(int*)(buffer + (ptr = (relativePtr = ptr + *(int*)(buffer + ptr + sizeof(int)))))];
}


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

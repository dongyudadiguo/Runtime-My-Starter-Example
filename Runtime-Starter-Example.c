#include "raylib.h"
#include <stdlib.h>
#include <string.h>

void starter(void);

void *buffer;
int ptr;
void *std;
void (*fun[])(void) = {starter};
void (*imp)(); 

void starter(void)
{
    static int selected_start = -1;
    static int selected_end = -1;
    static int fixed_start = 0;
    static int fixed_size = 0;
    static int relative = 0;
    static bool isSelecting = true;
    static bool isInputting = true;
    //static char inputText[256] = "";

    int key = GetCharPressed();
    while (key > 0)
    {
        char *tmp;
        (tmp = (char*)(std + strlen(std)))[0] = (char)key;
        tmp[1] = '\0';
        key = GetCharPressed();
        isInputting = true;
    }
    if (IsKeyPressed(KEY_ENTER)){
        if (isInputting == 0)
        {
            fun[*(int*)std];
        }
        *(int*)std = atoi(std);
        ((char*)std)[sizeof(int)] = '\0';
        isInputting = false;
    }

    int currentX = 10;
    for (int i = 0; i < 256; i++)
    {
        char currentChar = ((char *)std)[i];
        int textWidth = MeasureText(TextFormat("%c", currentChar), 20) + 2; // 字体大小20, 额外的2像素

        int xPos = currentX;
        currentX += textWidth;

        bool isSelected = (i >= selected_start && i <= selected_end && selected_start >= 0);
        DrawText(TextFormat("%c", currentChar), xPos, 10, 20, isSelected ? WHITE : GRAY); // 字体大小20

        Rectangle charRect = {xPos, 10, textWidth, 20};
        if (CheckCollisionPointRec(GetMousePosition(), charRect)) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (isSelecting) {
                    selected_start = selected_end = i;
                } else {
                    fixed_start = selected_start;
                    fixed_size = selected_end - selected_start;
                    relative = i - selected_start;
                }
            }
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                if (isSelecting) {
                    selected_end = i; 
                }else {
                    selected_start = i - relative;
                    selected_end = selected_start + fixed_size;
                }
            }
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                if (isSelecting) {
                    isSelecting = false;
                } else {
                    memmove((char *)std + selected_start,
                            (char *)std + fixed_start,
                            fixed_size + 1);
                    selected_start = selected_end = fixed_start = fixed_size = -1;
                    isSelecting = true;
                }
            }
        }
    }
    
    EndDrawing();
    BeginDrawing();
    ClearBackground(BLACK);

    imp = fun[(ptr = 0)];
}

int main(){
    InitWindow(800, 450, "Runtime-Starter-Example");
    imp = fun[ptr = (*(int*)(buffer = malloc(sizeof(void *))) = 0)];
    std = malloc(256);
    char* str = "\0Runtime-Starter-Example";
    memcpy(std, str, strlen(str + 1) + 1);
    while (1){ imp();}
}

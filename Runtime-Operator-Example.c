#include "raylib.h"
#include <stdlib.h>
#include <string.h>

void Operator(void);

int stack[1024];
int stackPtr = 0;

void *buffer;
int ptr;
void (*fun[])(void) = {Operator};
void (*imp)(); 

void Operator(void)
{
    static int selected_start = -1;
    static int selected_end = -1;
    static int fixed_start = 0;
    static int fixed_size = 0;
    static int relative = 0;
    static int type_location = 0;
    static bool isSelecting = true;
    static bool isMouseMove = false;

    char *tmp = buffer + type_location;
    if (IsKeyPressed(KEY_BACKSPACE)) {
        *(tmp) = '\0';
    }else if (IsKeyPressed(KEY_ENTER)) {
        *(int*)tmp = atoi(tmp);
    }else{
        int key = GetCharPressed();
        while (key > 0)
        {
            *(tmp = (char*)(tmp + strlen(tmp))) = (char)key;
            *(tmp + 1) = '\0';
            key = GetCharPressed();
        }
    }

    int currentX = 10;
    for (int i = 0; i < 256; i++)
    {
        char currentChar = ((char *)buffer)[i];
        int textWidth = MeasureText(TextFormat("%c", currentChar), 20) + 2; // 字体大小20, 额外的2像素

        int xPos = currentX;
        currentX += textWidth;
        
        bool isSelected = (i >= selected_start && i <= selected_end && selected_start >= 0);
        if (i == type_location) DrawRectangle(xPos, 10, textWidth, 20, GREEN);
        if (isSelected && textWidth == 2) DrawRectangle(xPos, 10, textWidth, 20, PURPLE);
        DrawText(TextFormat("%c", currentChar), xPos, 10, 20, isSelected ? WHITE : GRAY); // 字体大小20    

        Rectangle charRect = {xPos, isMouseMove?0:10, textWidth,isMouseMove?256:20};
        if (CheckCollisionPointRec(GetMousePosition(), charRect)) {
            if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
                type_location = i;
                DrawText(TextFormat("%d", type_location), xPos, 35, 20, WHITE);
                isMouseMove = true;
            }
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (isSelecting) {
                    selected_start = selected_end = i;
                } else {
                    fixed_start = selected_start;
                    fixed_size = selected_end - selected_start;
                    relative = i - selected_start;
                }
                isMouseMove = true;
            }
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                if (isSelecting) {
                    if (i < selected_start) {
                        selected_end = selected_start;
                        selected_start = i;
                    } else {
                        selected_end = i;
                    }
                }else {
                    if ((selected_start = i - relative) < 0) selected_start = 0;
                    selected_end = selected_start + fixed_size;
                }
                DrawText(TextFormat("%d %d", selected_start,selected_end), xPos, 35, 20, WHITE);
            }
        }
    }
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        if (isSelecting) {
            isSelecting = false;
        } else {
            memmove((char *)buffer + selected_start,
                    (char *)buffer + fixed_start,
                    fixed_size + 1);
            selected_start = selected_end = fixed_start = fixed_size = -1;
            isSelecting = true;
        }
        isMouseMove = false;
    }
    EndDrawing();
    BeginDrawing();
    ClearBackground(BLACK);

    imp = fun[*(int*)(buffer + (ptr = 0))];
}

int main(){
    InitWindow(800, 450, "Runtime-Operator-Example");
    imp = fun[ptr = (*(int*)(buffer = malloc(sizeof(void *)*256)) = 0)];
    while (1){ imp();}
}

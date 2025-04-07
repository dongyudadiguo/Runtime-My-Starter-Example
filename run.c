


#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <stdlib.h>
#include <string.h>



void (*imp)();
void *bse;
void *crt;
void *std;



void test(void){
    printf("test\n"); 
}

void (*fun[])(void) = { test };
size_t fun_size = sizeof(fun) / sizeof(fun[0]);  // 使用size_t类型
char *str = "test\0";



void load(void){
    
    static char searchText[256] = {0};
    static int scrollIndex = 0;

    static int selected_start = -1;
    static int selected_end = -1;
    static int fixed_start = 0;
    static int fixed_size = 0;
    static int relative = 0;
    
    int wheel = GetMouseWheelMove();
    scrollIndex -= wheel;
    if (scrollIndex < 0) scrollIndex = 0;

    int key = GetCharPressed();
    while (key > 0) {
        if (strlen(searchText) < 255) {
            searchText[strlen(searchText)] = (char)key;
            scrollIndex = 0;
        }
        key = GetCharPressed();
    }
    if (IsKeyPressed(KEY_BACKSPACE) && strlen(searchText) > 0) {
        searchText[strlen(searchText)-1] = '\0';
        scrollIndex = 0;
    }

    char* current = str;
    int originalTotal = 0;
    int filteredTotal = 0;
    while (*current != '\0' && originalTotal < 11011) {
        if (strstr(current, searchText)) filteredTotal++;
        current += strlen(current) + 1;
        originalTotal++;
    }
    
    if (scrollIndex > filteredTotal - 1) scrollIndex = filteredTotal > 0 ? filteredTotal - 1 : 0;

    current = str;
    int currentIndex = 0;
    int yPos = 30;
    int visibleIndex = 0;
    while (*current != '\0' && currentIndex < originalTotal) {
        if (strstr(current, searchText)) {
            if (visibleIndex >= scrollIndex) {
                Rectangle textRect = { 10, (float)yPos, (float)MeasureText(current, 10), 20 };
                DrawText(current, 10, yPos, 10, WHITE);
                if (CheckCollisionPointRec(GetMousePosition(), textRect)) {
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        fun[currentIndex]();
                    }
                    DrawRectangleLinesEx(textRect, 1, YELLOW);
                }
                yPos += 20;
            }
            visibleIndex++;
        }
        current += strlen(current) + 1;
        currentIndex++;
    }


    int max_chars = 25;
    
    int currentX = 10;
    for (int i = 0; i < max_chars; i++) {
        char currentChar = ((char*)std)[i];
        int textWidth = MeasureText(TextFormat("%c", currentChar), 20);  // 字体大小20
        
        int xPos = currentX;
        currentX += textWidth;

        Color textColor = (i >= selected_start && i <= selected_end && selected_start >= 0) ? WHITE : GRAY;
        DrawText(TextFormat("%c", currentChar), xPos, 10, 20, textColor);  // 字体大小20

        Rectangle charRect = { (float)xPos, 10, (float)textWidth, 20 };
        if (CheckCollisionPointRec(GetMousePosition(), charRect)) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
                selected_start = selected_end = i;
            }
            if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
                selected_end = i;
            }
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                fixed_start = selected_start;
                fixed_size = selected_end - selected_start;
                relative = i - selected_start;
            }
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                int newStart = i - relative;
                int newEnd = newStart + fixed_size;
                if (newEnd < max_chars && newStart >= 0) {
                    selected_start = newStart;
                    selected_end = newEnd;
                }
            }
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                if (selected_start >= 0 && selected_end < max_chars && 
                    fixed_start >= 0 && (fixed_start + fixed_size) < max_chars) {
                    memmove((char*)std + fixed_start, 
                           (char*)std + selected_start, 
                           selected_end - selected_start + 1);
                }
                selected_start = selected_end = fixed_start = fixed_size = -1;
            }
        }
    }

    EndDrawing();
    ClearBackground(BLACK);
    BeginDrawing();

    imp = (*(void(**)(void))(crt = bse));
}



int main(){
    imp = (*(void(**)(void))(bse = crt = malloc(sizeof(void (*)())))) = load;
    
    InitWindow(800, 600, "run.c");
    std = malloc(256);
    strcpy((char*)std, "1234567890\0");
    
    while (1) imp();
}



// gcc run.c -o run.exe -L. -lraylib -lgdi32 -lwinmm

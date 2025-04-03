#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <stdlib.h>



void (*imp)(), *bse, *crt, *std;
void (*fun[])(void) = {...};
int fun_size = sizeof(fun) / sizeof(fun[0]);

const char *fun_names = "func1;func2...";



void load(void){

    static int active = 0;
    static int scrollIndex = 0;
    
    GuiListView((Rectangle){ 50, 50, 100, 150 }, fun_names, &scrollIndex, &active);
    
    if (ismousebuttonpressed(MOUSE_BUTTON_LEFT)) fun[active]();

    DrawText(TextFormat("Active: %d, ScrollIndex: %d", active, scrollIndex), 10, 10, 20, WHITE);

    EndDrawing();
    ClearBackground(BLACK);
    BeginDrawing();

    imp = (*(void(**)(void))(crt = bse));
}



int main(){
    imp = (*(void(**)(void))(bse = crt = malloc(sizeof(void (*)())))) = load;
    
    InitWindow(800, 600, "run.c");
    
    while (1) imp();
}


// gcc run.c -o run.exe -L. -lraylib -lgdi32 -lwinmm
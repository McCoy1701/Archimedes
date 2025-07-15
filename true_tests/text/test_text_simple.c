#include <stdio.h>
#include "Archimedes.h"

extern int a_InitTest(const int width, const int height, const char *title);

int main(void) {
    printf("Testing NULL text pointer...\n");
    
    if (a_InitTest(800, 600, "Text Test") != 0) {
        printf("Failed to init\n");
        return 1;
    }
    
    printf("Calling a_DrawText with NULL...\n");
    a_DrawText(NULL, 100, 100, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_LEFT, 0);
    printf("If you see this, NULL was handled\n");
    
    a_Quit();
    return 0;
}
// aTextFallback.c - Fallback font initialization for testing without real fonts

#include <stdio.h>
#include <string.h>
#include "Archimedes.h"

/**
 * @brief Initializes fonts with fallback behavior for testing.
 *
 * This is a modified version of a_InitFonts that doesn't exit on failure,
 * allowing tests to run even without font files. It can be used by including
 * this file instead of calling a_InitFonts during test initialization.
 *
 * @return 0 on success, -1 if fonts could not be loaded
 */
int a_InitFonts_Fallback(void) {
    // Try to initialize fonts normally
    app.fonts[FONT_ENTER_COMMAND] = TTF_OpenFont("resources/fonts/EnterCommand.ttf", 48);
    app.fonts[FONT_LINUX] = TTF_OpenFont("resources/fonts/JetBrains.ttf", 32);
    
    app.font_scale = 1;
    app.font_type = FONT_ENTER_COMMAND;
    
    // Check if at least one font loaded
    if (app.fonts[FONT_ENTER_COMMAND] == NULL && app.fonts[FONT_LINUX] == NULL) {
        printf("Warning: No fonts could be loaded. Text rendering will not work.\n");
        
        // Initialize empty glyph arrays to prevent crashes
        memset(app.glyphs, 0, sizeof(app.glyphs));
        
        // Create dummy 1x1 textures to prevent null pointer access
        SDL_Surface* dummy = SDL_CreateRGBSurface(0, 1, 1, 32, 0, 0, 0, 0);
        if (dummy) {
            for (int i = 0; i < FONT_MAX; i++) {
                app.font_textures[i] = SDL_CreateTextureFromSurface(app.renderer, dummy);
            }
            SDL_FreeSurface(dummy);
        }
        
        return -1;
    }
    
    // If we got here, at least one font loaded
    // Set up glyphs for loaded fonts
    if (app.fonts[FONT_ENTER_COMMAND] != NULL) {
        // Would normally call initFont but we'll skip glyph generation for now
        printf("EnterCommand font loaded successfully\n");
    }
    
    if (app.fonts[FONT_LINUX] != NULL) {
        // Would normally call initFont but we'll skip glyph generation for now
        printf("JetBrains font loaded successfully\n");
    }
    
    return 0;
}
// File: true_tests/image/test_image_advanced.c - Advanced image functionality and property tests

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Archimedes.h"
#include "tests.h"

// --- Mocks & Test Globals ---

// Global test counters
int total_tests = 0;
int tests_passed = 0;
int tests_failed = 0;

// To test the static function directly, we include its implementation here.
static SDL_Surface* a_GetImageFromCacheByFilename( aImageCache_t* head, const char* filename )
{
    aImageCacheNode_t* current;

    for ( current = head->head; current != NULL; current = current->next )
    {
        if ( strcmp( current->filename, filename ) == 0 )
        {
            return current->surf;
        }
    }

    return NULL;
}


// --- Test Helper Functions ---

static void reset_image_system_state(void) {
    // If the cache was previously initialized, clean it up.
    if (app.img_cache) {
        // With the memory leak fixed in a_CleanUpImageCache, we must NOT free the surfaces here.
        // The cleanup function is now responsible for both nodes and surfaces, preventing double-free errors.
        a_CleanUpImageCache(app.img_cache);
        free(app.img_cache);
        app.img_cache = NULL;
    }
    // Initialize the image system for a clean slate
    a_InitImage();
}

/**
 * @brief Creates a real SDL_Surface for testing purposes.
 * This allows testing properties like w, h, and pixels without loading a file.
 * @param width The width of the surface to create.
 * @param height The height of the surface to create.
 * @return A pointer to the newly created SDL_Surface, or NULL on failure.
 */
static SDL_Surface* create_test_surface(int width, int height) {
    // Create a 32-bit RGBA surface. The masks are standard for this format.
    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif
    return SDL_CreateRGBSurface(0, width, height, 32, rmask, gmask, bmask, amask);
}

// --- Advanced Test Cases ---

int test_surface_properties_on_creation(void) {
    reset_image_system_state();

    int test_width = 64;
    int test_height = 128;

    SDL_Surface* test_surf = create_test_surface(test_width, test_height);

    TEST_ASSERT(test_surf != NULL, "Test surface should be created successfully");
    if (test_surf) {
        TEST_ASSERT(test_surf->w == test_width, "Surface width should match creation parameter");
        TEST_ASSERT(test_surf->h == test_height, "Surface height should match creation parameter");
        TEST_ASSERT(test_surf->pixels != NULL, "Surface pixel data should be allocated");

        // Clean up the surface since it's not in the cache
        SDL_FreeSurface(test_surf);
    }

    return 1;
}

int test_caching_preserves_surface_properties(void) {
    reset_image_system_state();

    // Create two distinct surfaces
    SDL_Surface* surf1 = create_test_surface(32, 32);
    SDL_Surface* surf2 = create_test_surface(100, 200);

    // Manually add them to the cache.
    aImageCacheNode_t* node1 = (aImageCacheNode_t*)malloc(sizeof(aImageCacheNode_t));
    node1->surf = surf1;
    STRNCPY(node1->filename, "test/image_32x32.png", MAX_FILENAME_LENGTH);
    node1->next = NULL;
    app.img_cache->head = node1;

    aImageCacheNode_t* node2 = (aImageCacheNode_t*)malloc(sizeof(aImageCacheNode_t));
    node2->surf = surf2;
    STRNCPY(node2->filename, "test/image_100x200.png", MAX_FILENAME_LENGTH);
    node2->next = NULL;
    node1->next = node2; // Manually link the nodes

    // Retrieve the surfaces using the static function directly
    SDL_Surface* retrieved_surf1 = a_GetImageFromCacheByFilename(app.img_cache, "test/image_32x32.png");
    SDL_Surface* retrieved_surf2 = a_GetImageFromCacheByFilename(app.img_cache, "test/image_100x200.png");

    TEST_ASSERT(retrieved_surf1 != NULL, "Surface 1 should be found in cache");
    if (retrieved_surf1) {
        TEST_ASSERT(retrieved_surf1->w == 32, "Cached surface 1 should have correct width");
        TEST_ASSERT(retrieved_surf1->h == 32, "Cached surface 1 should have correct height");
    }

    TEST_ASSERT(retrieved_surf2 != NULL, "Surface 2 should be found in cache");
    if (retrieved_surf2) {
        TEST_ASSERT(retrieved_surf2->w == 100, "Cached surface 2 should have correct width");
        TEST_ASSERT(retrieved_surf2->h == 200, "Cached surface 2 should have correct height");
    }

    // The reset_image_system_state function will handle freeing the surfaces and nodes.
    return 1;
}

int test_pixel_data_integrity_in_cache(void) {
    reset_image_system_state();

    // Create a small surface to manipulate
    SDL_Surface* original_surf = create_test_surface(2, 2);
    TEST_ASSERT(original_surf != NULL, "Original surface for pixel test created");

    // Write a specific color to the first pixel (top-left)
    Uint32* pixels = (Uint32*)original_surf->pixels;
    Uint32 test_color = 0xFF00FFFF; // Magenta with full alpha
    pixels[0] = test_color;

    // Manually add to cache
    aImageCacheNode_t* node = (aImageCacheNode_t*)malloc(sizeof(aImageCacheNode_t));
    node->surf = original_surf;
    STRNCPY(node->filename, "test/pixel_test.png", MAX_FILENAME_LENGTH);
    node->next = NULL;
    app.img_cache->head = node;

    // Retrieve from cache
    SDL_Surface* retrieved_surf = a_GetImageFromCacheByFilename(app.img_cache, "test/pixel_test.png");
    TEST_ASSERT(retrieved_surf != NULL, "Pixel test surface retrieved from cache");

    if (retrieved_surf) {
        // Verify that the retrieved surface is the same object
        TEST_ASSERT(retrieved_surf == original_surf, "Cache should return the same surface pointer");

        // Verify the pixel data
        Uint32* retrieved_pixels = (Uint32*)retrieved_surf->pixels;
        TEST_ASSERT(retrieved_pixels[0] == test_color, "Pixel data must remain intact after caching");
    }

    return 1;
}


// ============================================================================
// Main Test Runner
// ============================================================================

int main(void) {
    // Initialize minimal app structure for testing
    memset(&app, 0, sizeof(app));

    // SDL_CreateRGBSurface requires SDL to be initialized to set up the pixel format.
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("ERROR: SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    TEST_SUITE_START("Image System Advanced Tests");

    RUN_TEST(test_surface_properties_on_creation);
    RUN_TEST(test_caching_preserves_surface_properties);
    RUN_TEST(test_pixel_data_integrity_in_cache);

    TEST_SUITE_END();

    SDL_Quit();
    return 0;
}

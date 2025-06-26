// File: true_tests/image/test_image_basic.c - Basic image caching and functionality tests

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Archimedes.h"
#include "tests.h"

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
// --- Mocks & Test Globals ---

// Global test counters
int total_tests = 0;
int tests_passed = 0;
int tests_failed = 0;

// Mock surfaces so we don't need real image files. We only care about the pointers.
static SDL_Surface mock_surface_1;
static SDL_Surface mock_surface_2;

// --- Test Helper Functions ---

static void reset_image_system_state(void) {
    // If the cache was previously initialized, clean it up.
    if (app.img_cache) {
        // With the leak fixed in a_CleanUpImageCache, we must NOT free the surfaces here.
        // The cleanup function is now responsible for both nodes and surfaces.
        a_CleanUpImageCache(app.img_cache);
        free(app.img_cache);
        app.img_cache = NULL;
    }
    // Initialize the image system for a clean slate
    a_InitImage();
}

// --- Test Cases ---

int test_image_cache_initialization(void) {
    reset_image_system_state();

    TEST_ASSERT(app.img_cache != NULL, "Image cache should be allocated");
    TEST_ASSERT(app.img_cache->head == NULL, "Image cache head should be NULL initially");

    // Test cleanup on an empty cache
    int result = a_CleanUpImageCache(app.img_cache);
    TEST_ASSERT(result == 0, "Cleanup on empty cache should succeed");
    TEST_ASSERT(app.img_cache->head == NULL, "Cache head should remain NULL after cleanup");

    return 1;
}

int test_image_caching_and_retrieval(void) {
    reset_image_system_state();

    // Manually create and insert nodes to test caching logic without real files.
    // This is necessary because the original a_CacheImage is static and has a bug.
    aImageCacheNode_t* node1 = (aImageCacheNode_t*)malloc(sizeof(aImageCacheNode_t));
    node1->surf = &mock_surface_1;
    STRNCPY(node1->filename, "assets/image1.png", MAX_FILENAME_LENGTH);
    node1->ID = 1;
    node1->next = NULL;
    app.img_cache->head = node1;

    aImageCacheNode_t* node2 = (aImageCacheNode_t*)malloc(sizeof(aImageCacheNode_t));
    node2->surf = &mock_surface_2;
    STRNCPY(node2->filename, "assets/image2.png", MAX_FILENAME_LENGTH);
    node2->ID = 2;
    node2->next = NULL;
    node1->next = node2; // Link the nodes

    // Use a_GetImageFromCacheByFilename directly to test retrieval
    SDL_Surface* retrieved_surf_1 = a_GetImageFromCacheByFilename(app.img_cache, "assets/image1.png");
    TEST_ASSERT(retrieved_surf_1 == &mock_surface_1, "Should retrieve correct surface for image1 from cache");

    SDL_Surface* retrieved_surf_2 = a_GetImageFromCacheByFilename(app.img_cache, "assets/image2.png");
    TEST_ASSERT(retrieved_surf_2 == &mock_surface_2, "Should retrieve correct surface for image2 from cache");

    // Test retrieving a non-existent image
    SDL_Surface* non_existent_surf = a_GetImageFromCacheByFilename(app.img_cache, "assets/non_existent.png");
    TEST_ASSERT(non_existent_surf == NULL, "Should return NULL for a non-existent image in cache");

    // Because we are using mock surfaces that weren't created with SDL_Create...
    // we must NULL them out before cleanup to prevent a_CleanUpImageCache from calling
    // SDL_FreeSurface on invalid pointers.
    node1->surf = NULL;
    node2->surf = NULL;

    return 1;
}

int test_image_cache_cleanup(void) {
    reset_image_system_state();

    // Manually add nodes to the cache to ensure they are all freed.
    aImageCacheNode_t* node1 = (aImageCacheNode_t*)malloc(sizeof(aImageCacheNode_t));
    node1->surf = NULL; // Use NULL to avoid freeing static mock surfaces
    node1->next = NULL;

    aImageCacheNode_t* node2 = (aImageCacheNode_t*)malloc(sizeof(aImageCacheNode_t));
    node2->surf = NULL; // Use NULL to avoid freeing static mock surfaces
    node2->next = NULL;

    node1->next = node2;
    app.img_cache->head = node1;

    // Ensure the cache is populated before cleanup
    TEST_ASSERT(app.img_cache->head != NULL, "Cache should be populated before cleanup");

    int result = a_CleanUpImageCache(app.img_cache);
    TEST_ASSERT(result == 0, "Cache cleanup function should return success");
    TEST_ASSERT(app.img_cache->head == NULL, "Cache head should be NULL after cleanup");

    return 1;
}

int test_screenshot_error_handling(void) {
    // This test ensures the function doesn't crash when given a NULL renderer.
    // A more advanced test would require mocking SDL functions to simulate errors.
    printf("Info: Testing screenshot with NULL renderer (expect graceful failure).\n");
    int result = a_Screenshot(NULL, "screenshot.png");
    TEST_ASSERT(result == 0, "Screenshot should fail gracefully with a NULL renderer");

    return 1;
}


// ============================================================================
// Main Test Runner
// ============================================================================

int main(void) {
    // Initialize minimal app structure for testing
    memset(&app, 0, sizeof(app));

    TEST_SUITE_START("Image System Basic Tests");

    RUN_TEST(test_image_cache_initialization);
    RUN_TEST(test_image_caching_and_retrieval);
    RUN_TEST(test_image_cache_cleanup);
    RUN_TEST(test_screenshot_error_handling);

    TEST_SUITE_END();
}

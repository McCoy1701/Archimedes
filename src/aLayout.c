/*
 * Archimedes FlexBox Layout System
 * CSS-inspired automatic layout engine for UI positioning
 */

#include "../include/Archimedes.h"
#include <stdlib.h>
#include <string.h>

// Simple dynamic array implementation
typedef struct {
    void* data;
    size_t count;
    size_t capacity;
    size_t elem_size;
} SimpleArray_t;

static SimpleArray_t* simple_array_init(size_t capacity, size_t elem_size) {
    SimpleArray_t* arr = malloc(sizeof(SimpleArray_t));
    arr->data = malloc(capacity * elem_size);
    arr->count = 0;
    arr->capacity = capacity;
    arr->elem_size = elem_size;
    return arr;
}

static void simple_array_destroy(SimpleArray_t* arr) {
    if (arr) {
        free(arr->data);
        free(arr);
    }
}

static void simple_array_append(SimpleArray_t* arr, void* elem) {
    if (arr->count >= arr->capacity) {
        arr->capacity *= 2;
        arr->data = realloc(arr->data, arr->capacity * arr->elem_size);
    }
    memcpy((char*)arr->data + (arr->count * arr->elem_size), elem, arr->elem_size);
    arr->count++;
}

static void* simple_array_index(SimpleArray_t* arr, size_t idx) {
    if (idx >= arr->count) return NULL;
    return (char*)arr->data + (idx * arr->elem_size);
}

#define FLEX_ARRAY_INIT(count, size) simple_array_init((count), (size))
#define FLEX_ARRAY_DESTROY(arr) simple_array_destroy((SimpleArray_t*)(arr))
#define FLEX_ARRAY_APPEND(arr, data) simple_array_append((SimpleArray_t*)(arr), (data))
#define FLEX_ARRAY_INDEX(arr, idx) simple_array_index((SimpleArray_t*)(arr), (idx))
#define FLEX_ARRAY_COUNT(arr) (((SimpleArray_t*)(arr))->count)

// ============================================================================
// LIFECYCLE
// ============================================================================

FlexBox_t* a_CreateFlexBox(int x, int y, int w, int h) {
    FlexBox_t* box = malloc(sizeof(FlexBox_t));
    if (!box) {
        return NULL;
    }

    box->x = x;
    box->y = y;
    box->w = w;
    box->h = h;
    box->direction = FLEX_DIR_ROW;
    box->justify = FLEX_JUSTIFY_START;
    box->align = FLEX_ALIGN_START;
    box->gap = 0;
    box->padding = 0;
    box->dirty = 1;

    // Initialize children array (start with 8 items)
    box->children = FLEX_ARRAY_INIT(8, sizeof(FlexItem_t));
    if (!box->children) {
        free(box);
        return NULL;
    }

    return box;
}

void a_DestroyFlexBox(FlexBox_t** box) {
    if (!box || !*box) {
        return;
    }

    if ((*box)->children) {
        FLEX_ARRAY_DESTROY((*box)->children);
    }

    free(*box);
    *box = NULL;
}

// ============================================================================
// CONFIGURATION
// ============================================================================

void a_FlexSetDirection(FlexBox_t* box, FlexDirection_t direction) {
    if (!box) return;
    box->direction = direction;
    box->dirty = 1;
}

void a_FlexSetJustify(FlexBox_t* box, FlexJustify_t justify) {
    if (!box) return;
    box->justify = justify;
    box->dirty = 1;
}

void a_FlexSetAlign(FlexBox_t* box, FlexAlign_t align) {
    if (!box) return;
    box->align = align;
    box->dirty = 1;
}

void a_FlexSetGap(FlexBox_t* box, int gap) {
    if (!box) return;
    box->gap = gap;
    box->dirty = 1;
}

void a_FlexSetPadding(FlexBox_t* box, int padding) {
    if (!box) return;
    box->padding = padding;
    box->dirty = 1;
}

void a_FlexSetBounds(FlexBox_t* box, int x, int y, int w, int h) {
    if (!box) return;
    box->x = x;
    box->y = y;
    box->w = w;
    box->h = h;
    box->dirty = 1;
}

// ============================================================================
// ITEM MANAGEMENT
// ============================================================================

int a_FlexAddItem(FlexBox_t* box, int w, int h, void* user_data) {
    if (!box || !box->children) {
        return -1;
    }

    FlexItem_t item;
    item.w = w;
    item.h = h;
    item.calc_x = 0;
    item.calc_y = 0;
    item.user_data = user_data;

    FLEX_ARRAY_APPEND(box->children, &item);
    box->dirty = 1;

    return FLEX_ARRAY_COUNT(box->children) - 1;
}

void a_FlexRemoveItem(FlexBox_t* box, int index) {
    if (!box || !box->children) return;

    // Simple array doesn't support removal - just mark as zero-size
    FlexItem_t* item = FLEX_ARRAY_INDEX(box->children, index);
    if (item) {
        item->w = 0;
        item->h = 0;
    }

    box->dirty = 1;
}

void a_FlexClearItems(FlexBox_t* box) {
    if (!box || !box->children) return;

    FLEX_ARRAY_DESTROY(box->children);
    box->children = FLEX_ARRAY_INIT(8, sizeof(FlexItem_t));
    box->dirty = 1;
}

int a_FlexGetItemCount(const FlexBox_t* box) {
    if (!box || !box->children) {
        return 0;
    }
    return FLEX_ARRAY_COUNT(box->children);
}

const FlexItem_t* a_FlexGetItem(const FlexBox_t* box, int index) {
    if (!box || !box->children || index < 0 || index >= a_FlexGetItemCount(box)) {
        return NULL;
    }
    return (const FlexItem_t*)FLEX_ARRAY_INDEX(box->children, index);
}

void a_FlexUpdateItem(FlexBox_t* box, int index, int w, int h) {
    if (!box || !box->children) return;

    FlexItem_t* item = FLEX_ARRAY_INDEX(box->children, index);
    if (item) {
        item->w = w;
        item->h = h;
        box->dirty = 1;
    }
}

// ============================================================================
// LAYOUT CALCULATION
// ============================================================================

int a_FlexLayout(FlexBox_t* box) {
    if (!box || !box->children) {
        return -1;
    }

    int item_count = a_FlexGetItemCount(box);
    if (item_count == 0) {
        box->dirty = 0;
        return 0;
    }

    // Calculate content area (minus padding)
    int content_x = box->x + box->padding;
    int content_y = box->y + box->padding;
    int content_w = box->w - (2 * box->padding);
    int content_h = box->h - (2 * box->padding);

    // Calculate total size of items + gaps
    int total_main_size = 0;
    int max_cross_size = 0;

    for (int i = 0; i < item_count; i++) {
        FlexItem_t* item = FLEX_ARRAY_INDEX(box->children, i);
        if (!item) continue;

        if (box->direction == FLEX_DIR_ROW) {
            total_main_size += item->w;
            if (item->h > max_cross_size) {
                max_cross_size = item->h;
            }
        } else {
            total_main_size += item->h;
            if (item->w > max_cross_size) {
                max_cross_size = item->w;
            }
        }
    }

    // Add gaps (n-1 gaps for n items)
    total_main_size += (item_count - 1) * box->gap;

    // Calculate starting position based on justify
    int main_pos = 0;
    int gap_spacing = box->gap;

    switch (box->justify) {
        case FLEX_JUSTIFY_START:
            main_pos = 0;
            break;

        case FLEX_JUSTIFY_CENTER:
            main_pos = (box->direction == FLEX_DIR_ROW ? content_w : content_h) / 2 - total_main_size / 2;
            break;

        case FLEX_JUSTIFY_END:
            main_pos = (box->direction == FLEX_DIR_ROW ? content_w : content_h) - total_main_size;
            break;

        case FLEX_JUSTIFY_SPACE_BETWEEN:
            main_pos = 0;
            if (item_count > 1) {
                int available = (box->direction == FLEX_DIR_ROW ? content_w : content_h) - total_main_size + (item_count - 1) * box->gap;
                gap_spacing = available / (item_count - 1);
            }
            break;

        case FLEX_JUSTIFY_SPACE_AROUND: {
            int available = (box->direction == FLEX_DIR_ROW ? content_w : content_h) - total_main_size + (item_count - 1) * box->gap;
            gap_spacing = available / item_count;
            main_pos = gap_spacing / 2;
            break;
        }
    }

    // Position each item
    for (int i = 0; i < item_count; i++) {
        FlexItem_t* item = FLEX_ARRAY_INDEX(box->children, i);
        if (!item) continue;

        // Main axis position
        if (box->direction == FLEX_DIR_ROW) {
            item->calc_x = content_x + main_pos;
            main_pos += item->w + gap_spacing;
        } else {
            item->calc_y = content_y + main_pos;
            main_pos += item->h + gap_spacing;
        }

        // Cross axis position (based on align)
        int cross_offset = 0;
        int item_cross_size = (box->direction == FLEX_DIR_ROW) ? item->h : item->w;
        int container_cross_size = (box->direction == FLEX_DIR_ROW) ? content_h : content_w;

        switch (box->align) {
            case FLEX_ALIGN_START:
                cross_offset = 0;
                break;

            case FLEX_ALIGN_CENTER:
                cross_offset = (container_cross_size - item_cross_size) / 2;
                break;

            case FLEX_ALIGN_END:
                cross_offset = container_cross_size - item_cross_size;
                break;
        }

        if (box->direction == FLEX_DIR_ROW) {
            item->calc_y = content_y + cross_offset;
        } else {
            item->calc_x = content_x + cross_offset;
        }
    }

    box->dirty = 0;
    return 0;
}

int a_FlexIsDirty(const FlexBox_t* box) {
    return box ? box->dirty : 0;
}

// ============================================================================
// UTILITIES
// ============================================================================

int a_FlexGetItemX(const FlexBox_t* box, int index) {
    const FlexItem_t* item = a_FlexGetItem(box, index);
    return item ? item->calc_x : 0;
}

int a_FlexGetItemY(const FlexBox_t* box, int index) {
    const FlexItem_t* item = a_FlexGetItem(box, index);
    return item ? item->calc_y : 0;
}

void a_FlexDebugRender(const FlexBox_t* box) {
    if (!box) return;

    // Draw container bounds (white)
    a_DrawRect(box->x, box->y, box->w, box->h, 255, 255, 255, 255);

    // Draw padding area (yellow)
    if (box->padding > 0) {
        a_DrawRect(box->x + box->padding, box->y + box->padding,
                   box->w - (2 * box->padding), box->h - (2 * box->padding),
                   255, 255, 0, 128);
    }

    // Draw each item (cyan)
    int count = a_FlexGetItemCount(box);
    for (int i = 0; i < count; i++) {
        const FlexItem_t* item = a_FlexGetItem(box, i);
        if (item) {
            a_DrawRect(item->calc_x, item->calc_y, item->w, item->h, 0, 255, 255, 255);
        }
    }
}

# ADR-003: Font Configuration Struct Over Multi-Parameter Function

## Context

The `a_DrawText()` function has 8 parameters, making it hard to read and maintain:

```c
a_DrawText("Title", SCREEN_WIDTH/2, 100, 255, 255, 255,
           FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
```

**Alternatives Considered:**

1. **Status Quo** (8-parameter function)
   - Pros: No changes needed, everyone knows it
   - Cons: Cognitive overload, repetitive, hard to extend, inconsistent with `aColor_t` usage elsewhere
   - Cost: Every new option = another parameter

2. **Font Config Struct** (`aFontConfig_t`)
   - Pros: 3 params + 1 config, reusable styles, extensible, uses `aColor_t`
   - Cons: Extra struct allocation, two APIs during migration
   - Cost: Learning curve for struct-based API

3. **Builder Pattern** (fluent API)
   - Pros: Readable, chainable
   - Cons: Un-C-like, complex to implement, no static configs
   - Cost: Significant implementation complexity

4. **Global State Setters** (OpenGL-style)
   - Pros: Simple calls after setup
   - Cons: Hidden state, thread-unsafe, hard to debug
   - Cost: Implicit dependencies, action-at-a-distance bugs

## Decision

**Add `aFontConfig_t` struct and `a_DrawTextStyled()` function, keep old `a_DrawText()` as deprecated wrapper.**

**Implementation:**
```c
typedef struct {
    int type;              // FONT_ENTER_COMMAND, FONT_GAME, etc.
    aColor_t color;        // Text color
    int align;             // TEXT_ALIGN_LEFT, CENTER, RIGHT
    int wrap_width;        // 0 = no wrap
    float scale;           // 1.0 = default size
} aFontConfig_t;

void a_DrawTextStyled(const char* text, int x, int y, const aFontConfig_t* config);
extern aFontConfig_t a_default_font_config;

// Old API now wraps new one
void a_DrawText(char* text, int x, int y, int r, int g, int b,
                int font_type, int align, int max_width);
```

**Justification:**

1. **Reduced Cognitive Load**: 3 essential params (text, x, y) + optional config pointer

2. **Reusable Styles**: Define once at file scope, use everywhere
   ```c
   aFontConfig_t title = {FONT_ENTER_COMMAND, white, TEXT_ALIGN_CENTER, 0, 1.0f};
   a_DrawTextStyled("Title 1", x, y, &title);
   a_DrawTextStyled("Title 2", x, y, &title);
   ```

3. **Type Consistency**: Uses existing `aColor_t` like other drawing functions

4. **Extensible**: Add shadow/outline/rotation without breaking API

5. **Zero Breaking Changes**: Old code works unchanged, new code opts in

## Consequences

**Positive:**
- ✅ Single-line rendering in most cases
- ✅ Font styles defined once, used everywhere
- ✅ NULL config = sensible defaults
- ✅ Future-proof (add fields to struct, not parameters)
- ✅ Consistent with `aColor_t` usage in drawing API

**Negative:**
- ❌ Two APIs exist (old + new)
- ❌ ~20 bytes stack per config struct
- ❌ Learning curve for new developers

**Accepted Trade-offs:**
- ✅ Accept two APIs for backward compatibility
- ✅ Accept stack overhead for cleaner code
- ❌ NOT accepting: Breaking existing code - wrapper maintains compatibility

**Migration Path:**
- V1 (complete): New API available, old API marked deprecated in docs
- V2 (future): Update examples to use new API
- V3 (major version): Consider removing deprecated function

**Success Metrics:**
- New template code uses `a_DrawTextStyled()`
- Example applications define font configs at file scope
- Zero complaints about compatibility breaks
- Users voluntarily migrate for readability

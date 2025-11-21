# ADR 01: Naming Conventions

**Status:** Proposed

To figure out - consistent naming and parameter handling of fg/bg in rect struct rather than in draw rect params.

## Context

Archimedes needs consistent naming conventions across all public APIs to ensure discoverability, predictability, and a cohesive developer experience. The naming system must balance IDE autocomplete ergonomics with logical organization of functionality.

**Options Considered:**

1. **SDL-style action-first** - `SDL_RenderDrawRect`, `SDL_CreateWindow` - action verb leads
2. **Object-first (noun-verb)** - `a_TimerCreate`, `a_TimerStart` - object groups related functions
3. **Action-first (verb-noun)** - `a_CreateTimer`, `a_StartTimer` - verb leads for imperative style
4. **Hybrid with Draw exception** - Object-first for most APIs, but `a_Draw*` groups all rendering

## Decision

Use **hybrid with Draw exception** (Option 4).

### Function Naming

**Pattern:** `a_ObjectAction()` for most functions

Functions are named with the `a_` prefix, followed by the object/noun, then the action/verb:
- `a_TimerCreate()`, `a_TimerStart()`, `a_TimerPause()`
- `a_FlexSetDirection()`, `a_FlexAddItem()`, `a_FlexLayout()`
- `a_ViewportDrawRect()`, `a_ViewportIsRectVisible()`

**Exception 1: Drawing functions use `a_Draw*` pattern**

All primitive drawing operations are grouped under `a_Draw` for discoverability:
- `a_DrawRect()`, `a_DrawFilledRect()`
- `a_DrawCircle()`, `a_DrawFilledCircle()`
- `a_DrawText()`
- `a_DrawLine()`, `a_DrawPoint()`
- `a_DrawTriangle()`, `a_DrawFilledTriangle()`
- `a_DrawWidgets()`

This allows developers to type `a_Draw` and see all rendering primitives in autocomplete.

**Exception 2: Blitting functions use `a_Blit*` pattern**

Blitting operations (copying surfaces/textures to the renderer) are distinct from drawing primitives and use `a_Blit` prefix:
- `a_BlitSurface()` - blit a surface at x,y coordinates
- `a_BlitSurfaceRect()` - blit a surface with scaling and clipping

Blitting is a separate rendering concept from drawing primitives, so it deserves its own namespace. This maintains semantic clarity: drawing creates pixels procedurally, blitting copies pre-rendered pixel data.

### Type Naming

**Pattern:** `aObjectName_t`

Types use a lowercase `a` prefix (no underscore), PascalCase object name, and `_t` suffix:
- `aColor_t`, `aRectf_t`, `aRecti_t`
- `aTimer_t`, `aWidget_t`, `aTextStyle_t`
- `aPoint2f_t`, `aPoint3f_t`

### Enum Naming

**Pattern:** `PREFIX_VALUE` (all caps with underscores)

Enums use all-caps with a prefix that groups related values:
- Font types: `FONT_GAME`, `FONT_ENTER_COMMAND`, `FONT_LINUX`
- Text alignment: `TEXT_ALIGN_LEFT`, `TEXT_ALIGN_CENTER`, `TEXT_ALIGN_RIGHT`
- Flex layout: `FLEX_DIR_ROW`, `FLEX_DIR_COLUMN`, `FLEX_JUSTIFY_START`
- Widget types: `WT_BUTTON`, `WT_SLIDER`, `WT_INPUT`

### Parameter Naming

**Pattern:** Short, consistent names across all APIs

Color parameters use `fg` (foreground) and `bg` (background) consistently:
- `aTextStyle_t` has `.fg` and `.bg`
- `aWidget_t` has `.fg` and `.bg`
- `aStyledRectf_t` has `.fg` and `.bg` to go along with a `aRectf_t` within its struct. Rect is pure, StyledRect has more.
- Drawing functions use `color` for single-color operations

Text-related parameters:
- `content` for the text string being rendered
- `style` for `aTextStyle_t*` configuration structs

### Global Defaults

**Pattern:** `a_default_*` for default configuration instances

- `a_default_text_style` - default white, left-aligned text style

**Justification:**

1. **Object-first aids discovery** - Typing `a_Timer` shows all timer operations; typing `a_Flex` shows all flexbox operations
2. **Draw exception improves UX** - `a_Draw` shows everything you can render, matching how developers think ("I want to draw something")
3. **`_t` suffix is idiomatic C** - POSIX convention, familiar to C developers
4. **Consistent parameter names reduce cognitive load** - `fg`/`bg` always mean the same thing everywhere

## Consequences

**Positive:**
- IDE autocomplete becomes powerful navigation tool
- Developers can predict function names without documentation
- Related functions cluster together alphabetically
- New contributors can follow established patterns

**Negative:**
- Draw and Blit exceptions create inconsistency (not purely object-first)
- Some functions have long names (`a_ViewportCalculateScale`)
- Enum prefixes can be verbose (`FLEX_JUSTIFY_SPACE_BETWEEN`)

**Accepted Trade-offs:**
- The Draw and Blit exceptions' discoverability benefits outweigh naming purity
- Long descriptive names are preferred over short cryptic ones
- Verbose enum names provide self-documentation

## Object Classification

For fitness function verification, objects are classified as:

**Drawn Objects** (use `a_Draw*` pattern):
- Text, Rect, Circle, Line, Point, Triangle, Widgets, FilledRect, FilledCircle, FilledTriangle, HorizontalLine, VerticalLine

**Blit Objects** (use `a_Blit*` pattern):
- Surface, SurfaceRect

**Used Objects** (use `a_Object*` pattern):
- Timer, Viewport, Flex, Widget, Image, Audio, AUF, Glyph, Font, Texture, Init, Quit, Input, Error, Color

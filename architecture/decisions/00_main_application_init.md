# ADR 00: Main Application Initialization

**Status:** Active

## Context

Archimedes needs a standard way to initialize the SDL2-based application lifecycle, including subsystem setup, resource management, and cleanup. The initialization process must handle multiple failure points (SDL, IMG, TTF, window creation) while maintaining clear error reporting and proper cleanup on failure.

**Options Considered:**

1. **Monolithic initialization** - Single `a_Init()` function handling all setup inline with nested error handling
2. **Staged initialization** - Separate functions for each subsystem (`a_InitSDL()`, `a_InitWindow()`, etc.) called sequentially
3. **Validated initialization with error propagation** - Single `a_Init()` entry point with internal validation function that handles cleanup on failure
4. **Builder pattern** - Fluent API for configuring and initializing the application with method chaining

## Decision

Use **validated initialization with error propagation** (Option 3).

The initialization follows this pattern:
- Single entry point: `a_Init(width, height, title)`
- Internal validation: `a_ValidateSubsystems()` handles SDL/IMG/TTF setup
- Internal cleanup: `a_CleanupSubsystems()` mirrors validation (TTF→IMG→SDL)
- Automatic cleanup on failure at each stage
- Explicit error codes for each failure type
- Single exit point: `a_Quit()` handles all resource cleanup

**Justification**:

1. **Clear separation of concerns** - Validation logic is isolated from app state initialization, making both easier to test and modify
2. **Predictable error handling** - Each failure type has a distinct error code (`INIT_ERROR_SDL`, `INIT_ERROR_IMG`, `INIT_ERROR_TTF`, `INIT_ERROR_WINDOW`)
3. **Automatic cleanup on failure** - Failed subsystems are immediately cleaned up, preventing resource leaks
4. **Single global app state** - The `aApp_t app` global structure provides deterministic initialization and shutdown semantics

## Consequences

**Positive:**
- Users have one function to call: `a_Init()` returns 0 on success, negative value on failure
- Error codes are self-documenting through enum names
- Cleanup order is deterministic (reverse of initialization order: timers before SDL, TTF→IMG→SDL)
- Internal validation can be tested independently
- Reduces cognitive load - function is scannable at a glance

**Negative:**
- Global `app` state prevents multiple Archimedes instances in same process
- Internal `a_ValidateSubsystems()` function is not exposed, limiting fine-grained control
- Error codes must be manually documented in header/documentation
- No callbacks or hooks for custom initialization steps

**Accepted Trade-offs:**
- Single instance limitation is acceptable for game engine use case (one window per process)
- Simplified API is more important than fine-grained control for target users (game developers, not engine developers)
- Standard SDL cleanup order is sufficient for all known use cases
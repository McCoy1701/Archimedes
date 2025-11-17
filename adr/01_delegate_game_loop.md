# ADR-001: Delegate-Based Game Loop Over Direct Function Calls

## Context

Game engines need a way for user code to hook into the main loop for logic updates and rendering.

**Alternatives Considered:**

1. **Delegate pattern** (`app.delegate.logic` / `app.delegate.draw`)
   - Pros: Decouples framework from user code, clean separation, user sets callbacks once
   - Cons: Indirect function calls, requires initialization step
   - Cost: One extra pointer dereference per frame

2. **Direct weak symbols** (user implements `__attribute__((weak)) void GameLoop()`)
   - Pros: No indirection, simple linking
   - Cons: Hard to override, breaks on Windows, inflexible (single implementation)
   - Cost: Platform-specific behavior

3. **Inheritance/Virtual** (user subclasses `Game` base class)
   - Pros: OOP pattern, multiple override points
   - Cons: Un-C-like, requires C++, complex for simple games
   - Cost: Vtable overhead, language change

4. **Global function pointers** (user assigns `g_logic_func` / `g_render_func`)
   - Pros: Simple, no struct needed
   - Cons: Global namespace pollution, no optional callbacks (like `onExit`)
   - Cost: Multiple globals to manage

## Decision

**Use delegate pattern with function pointers stored in `aApp_t` struct.**

**Implementation:**
```c
typedef struct {
  void (*logic)( float deltaTime );
  void (*draw)( float deltaTime );
  void (*onExit)( void );
} aDelegate_t;

typedef struct {
  aDelegate_t delegate;
  // ... other app state
} aApp_t;

// User sets delegates once during initialization
void aInitGame( void ) {
  app.delegate.logic = aDoLoop;
  app.delegate.draw  = aRenderLoop;
  app.delegate.onExit = aCleanup;  // optional
}

// Framework calls delegates in main loop
void aMainloop( void ) {
  a_PrepareScene();
  app.delegate.logic( a_GetDeltaTime() );
  app.delegate.draw( a_GetDeltaTime() );
  a_PresentScene();
}
```

**Justification:**

1. **Clean Separation**: Framework doesn't know about user code, user code doesn't implement framework internals

2. **Flexible**: Easy to add optional callbacks (`onExit`, `onPause`, etc.) without breaking existing code

3. **Web Compatible**: Works with Emscripten's `emscripten_set_main_loop` without special handling

4. **Testable**: Can swap delegates at runtime for testing different game states

5. **Idiomatic C**: Function pointers in structs is standard pattern (like `FILE*` operations)

6. **Single Source of Truth**: All callbacks live in `app.delegate`, clear what's hookable

## Consequences

**Positive:**
- ✅ Framework and user code completely decoupled
- ✅ Easy to add new hook points without API changes
- ✅ Optional callbacks (NULL check before calling)
- ✅ Runtime swappable (change logic/draw callbacks for different game states)
- ✅ Works on native and web (Emscripten)

**Negative:**
- ❌ One pointer dereference overhead per frame (~negligible)
- ❌ Requires initialization step (user must set delegates)
- ❌ NULL delegates crash if not checked

**Accepted Trade-offs:**
- ✅ Accept pointer dereference for flexibility
- ✅ Accept initialization requirement for cleaner architecture
- ❌ NOT accepting: NULL crashes - framework checks before calling optional delegates

**Pattern Used:**
```c
// Required delegates (assumed set, no check)
app.delegate.logic( dt );
app.delegate.draw( dt );

// Optional delegates (NULL check)
if ( app.delegate.onExit ) {
  app.delegate.onExit();
}
```

**Success Metrics:**
- Template code demonstrates pattern clearly
- Adding new callbacks doesn't break existing games
- Zero complaints about indirection performance
- Web and native builds use same pattern

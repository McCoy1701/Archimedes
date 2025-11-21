#!/usr/bin/env python3
"""
FF-000: Main Application Init Verification

Ensures the initialization pattern from ADR-000 is maintained:
1. InitStatus_t enum exists with proper error codes
2. a_ValidateSubsystems() function exists and is static
3. a_Init() uses InitStatus_t (no magic numbers for error returns)
4. a_Init() calls a_ValidateSubsystems() and checks the result
5. a_Init() doesn't directly call SDL_Init/IMG_Init/TTF_Init
6. Window creation failure cleans up all subsystems
7. Cleanup order is reverse of initialization order
8. a_CleanupSubsystems() exists as mirror of a_ValidateSubsystems()
9. a_Quit() cleans up timers before SDL_Quit, subsystems in reverse order

Based on: ADR-000 (Main Application Initialization)

Architecture Characteristic: Safety, Maintainability, Self-Documentation
"""

import re
from pathlib import Path
from typing import List, Dict, Any, Tuple


# Required enum values that must exist
REQUIRED_ENUM_VALUES = [
    'INIT_SUCCESS',
    'INIT_ERROR_SDL',
    'INIT_ERROR_IMG',
    'INIT_ERROR_TTF',
    'INIT_ERROR_WINDOW',
]

# Pattern to match InitStatus_t enum definition
INIT_STATUS_ENUM_PATTERN = re.compile(
    r'typedef enum \{([^}]+)\} InitStatus_t;',
    re.DOTALL
)

# Pattern to match a_ValidateSubsystems function (must be static)
VALIDATE_SUBSYSTEMS_PATTERN = re.compile(
    r'static\s+InitStatus_t\s+a_ValidateSubsystems\s*\(\s*void\s*\)',
    re.MULTILINE
)

# Pattern to detect magic number returns (return -1, return -2, etc.)
MAGIC_NUMBER_RETURN_PATTERN = re.compile(
    r'return\s+(-\d+)\s*;'
)

# Pattern to detect proper enum returns
ENUM_RETURN_PATTERN = re.compile(
    r'return\s+(INIT_ERROR_\w+|INIT_SUCCESS)\s*;'
)


def check_init_status_enum(content: str) -> Tuple[bool, List[str]]:
    """
    Verify InitStatus_t enum exists with all required values.

    Returns: (success, list of issues)
    """
    issues = []

    match = INIT_STATUS_ENUM_PATTERN.search(content)
    if not match:
        issues.append("InitStatus_t enum not found in aInitialize.c")
        return False, issues

    enum_body = match.group(1)

    # Check for each required enum value
    for required in REQUIRED_ENUM_VALUES:
        if required not in enum_body:
            issues.append(f"Missing required enum value: {required}")

    # Verify INIT_SUCCESS = 0
    if 'INIT_SUCCESS = 0' not in enum_body and 'INIT_SUCCESS=0' not in enum_body:
        # Check if it's the first value (implicitly 0)
        first_value = enum_body.strip().split(',')[0].strip()
        if 'INIT_SUCCESS' not in first_value:
            issues.append("INIT_SUCCESS must equal 0 (either explicitly or as first enum value)")

    return len(issues) == 0, issues


def check_validate_subsystems(content: str) -> Tuple[bool, List[str]]:
    """
    Verify a_ValidateSubsystems() exists and is static.

    Returns: (success, list of issues)
    """
    issues = []

    if not VALIDATE_SUBSYSTEMS_PATTERN.search(content):
        # Check if it exists but isn't static
        if 'a_ValidateSubsystems' in content:
            issues.append("a_ValidateSubsystems() must be declared as 'static InitStatus_t'")
        else:
            issues.append("a_ValidateSubsystems() function not found")
        return False, issues

    return True, issues


def check_no_magic_numbers(content: str) -> Tuple[bool, List[str]]:
    """
    Verify a_Init() doesn't use magic number returns.

    Returns: (success, list of issues)
    """
    issues = []

    # Find a_Init function body
    init_match = re.search(
        r'int\s+a_Init\s*\([^)]*\)\s*\{',
        content
    )

    if not init_match:
        issues.append("a_Init() function not found")
        return False, issues

    # Extract a_Init function body (rough extraction - find matching braces)
    start = init_match.end()
    brace_count = 1
    end = start

    while end < len(content) and brace_count > 0:
        if content[end] == '{':
            brace_count += 1
        elif content[end] == '}':
            brace_count -= 1
        end += 1

    init_body = content[start:end]

    # Check for magic number returns
    magic_returns = MAGIC_NUMBER_RETURN_PATTERN.findall(init_body)

    for magic in magic_returns:
        issues.append(f"Magic number return found: 'return {magic};' - use InitStatus_t enum instead")

    return len(issues) == 0, issues


def check_init_uses_validation(content: str) -> Tuple[bool, List[str]]:
    """
    Ensure a_Init() calls a_ValidateSubsystems() and checks the result.

    Returns: (success, list of issues)
    """
    issues = []

    # Extract a_Init body
    init_match = re.search(r'int\s+a_Init\s*\([^)]*\)\s*\{', content)
    if not init_match:
        issues.append("Could not parse a_Init() body")
        return False, issues

    start = init_match.end()
    brace_count = 1
    end = start

    while end < len(content) and brace_count > 0:
        if content[end] == '{':
            brace_count += 1
        elif content[end] == '}':
            brace_count -= 1
        end += 1

    init_body = content[start:end]

    if 'a_ValidateSubsystems()' not in init_body:
        issues.append("a_Init() must call a_ValidateSubsystems()")

    # Should capture and check the result
    if 'InitStatus_t' not in init_body and 'status' not in init_body.lower():
        issues.append("a_Init() should capture validation result in a variable")

    return len(issues) == 0, issues


def check_no_direct_subsystem_calls(content: str) -> Tuple[bool, List[str]]:
    """
    Ensure a_Init() doesn't directly call SDL_Init/IMG_Init/TTF_Init.
    All subsystem init should be in a_ValidateSubsystems().

    Returns: (success, list of issues)
    """
    issues = []

    init_match = re.search(r'int\s+a_Init\s*\([^)]*\)\s*\{', content)
    if not init_match:
        return True, issues

    start = init_match.end()
    brace_count = 1
    end = start

    while end < len(content) and brace_count > 0:
        if content[end] == '{':
            brace_count += 1
        elif content[end] == '}':
            brace_count -= 1
        end += 1

    init_body = content[start:end]

    forbidden_calls = ['SDL_Init(', 'IMG_Init(', 'TTF_Init(']
    for call in forbidden_calls:
        if call in init_body:
            issues.append(f"{call[:-1]}() should only be called in a_ValidateSubsystems()")

    return len(issues) == 0, issues


def check_window_creation_cleanup(content: str) -> Tuple[bool, List[str]]:
    """
    Verify window creation failure cleans up all subsystems (TTF, IMG, SDL).

    Returns: (success, list of issues)
    """
    issues = []

    # Find a_Init body
    init_match = re.search(r'int\s+a_Init\s*\([^)]*\)\s*\{', content)
    if not init_match:
        return True, issues

    start = init_match.end()
    brace_count = 1
    end = start

    while end < len(content) and brace_count > 0:
        if content[end] == '{':
            brace_count += 1
        elif content[end] == '}':
            brace_count -= 1
        end += 1

    init_body = content[start:end]

    # Find window creation error block
    window_block_match = re.search(
        r'if\s*\([^)]*SDL_CreateWindowAndRenderer[^)]*\)[^{]*\{([^}]+)\}',
        init_body,
        re.DOTALL
    )

    if window_block_match:
        window_cleanup = window_block_match.group(1)

        # Preferred pattern: use a_CleanupSubsystems() (ADR-000)
        if 'a_CleanupSubsystems()' in window_cleanup:
            # Correct - using the cleanup function (DRY principle)
            pass
        else:
            # Legacy pattern: explicit cleanup calls
            required_cleanups = ['TTF_Quit()', 'IMG_Quit()', 'SDL_Quit()']

            for cleanup in required_cleanups:
                if cleanup not in window_cleanup:
                    issues.append(f"Window creation failure must call {cleanup} (or use a_CleanupSubsystems())")

            # Check order: TTF -> IMG -> SDL
            ttf_pos = window_cleanup.find('TTF_Quit()')
            img_pos = window_cleanup.find('IMG_Quit()')
            sdl_pos = window_cleanup.find('SDL_Quit()')

            if ttf_pos != -1 and img_pos != -1 and ttf_pos > img_pos:
                issues.append("Window cleanup order wrong: TTF_Quit() must come before IMG_Quit()")
            if img_pos != -1 and sdl_pos != -1 and img_pos > sdl_pos:
                issues.append("Window cleanup order wrong: IMG_Quit() must come before SDL_Quit()")

    return len(issues) == 0, issues


def check_cleanup_order(content: str) -> Tuple[bool, List[str]]:
    """
    Verify cleanup in a_ValidateSubsystems follows reverse order.

    SDL -> IMG -> TTF means cleanup should be TTF -> IMG -> SDL

    Returns: (success, list of issues)
    """
    issues = []

    # Find a_ValidateSubsystems body
    match = re.search(
        r'static\s+InitStatus_t\s+a_ValidateSubsystems\s*\(\s*void\s*\)\s*\{',
        content
    )

    if not match:
        return True, issues  # Already reported in other check

    start = match.end()
    brace_count = 1
    end = start

    while end < len(content) and brace_count > 0:
        if content[end] == '{':
            brace_count += 1
        elif content[end] == '}':
            brace_count -= 1
        end += 1

    func_body = content[start:end]

    # Check IMG failure block - extract the if block specifically
    img_block_match = re.search(
        r'if\s*\([^)]*IMG_Init[^)]*\)[^{]*\{([^}]+)\}',
        func_body,
        re.DOTALL
    )
    if img_block_match:
        img_cleanup_block = img_block_match.group(1)
        if 'SDL_Quit()' not in img_cleanup_block:
            issues.append("IMG failure should call SDL_Quit() before returning")

    # Check TTF failure block - extract the if block specifically
    ttf_block_match = re.search(
        r'if\s*\([^)]*TTF_Init[^)]*\)[^{]*\{([^}]+)\}',
        func_body,
        re.DOTALL
    )
    if ttf_block_match:
        ttf_cleanup_block = ttf_block_match.group(1)

        img_pos = ttf_cleanup_block.find('IMG_Quit()')
        sdl_pos = ttf_cleanup_block.find('SDL_Quit()')

        if img_pos == -1:
            issues.append("TTF failure should call IMG_Quit() before returning")
        if sdl_pos == -1:
            issues.append("TTF failure should call SDL_Quit() before returning")

        # Verify order: IMG_Quit before SDL_Quit (lower position = earlier in code)
        if img_pos != -1 and sdl_pos != -1 and img_pos > sdl_pos:
            issues.append("Cleanup order wrong: IMG_Quit() must come before SDL_Quit()")

    return len(issues) == 0, issues


def check_cleanup_subsystems_exists(content: str) -> Tuple[bool, List[str]]:
    """
    Verify a_CleanupSubsystems() exists as mirror of a_ValidateSubsystems().

    Returns: (success, list of issues)
    """
    issues = []

    if 'a_CleanupSubsystems' not in content:
        issues.append("Missing a_CleanupSubsystems() - should mirror a_ValidateSubsystems()")
    else:
        # Check it's static
        if not re.search(r'static\s+void\s+a_CleanupSubsystems\s*\(', content):
            issues.append("a_CleanupSubsystems() should be 'static void'")

    return len(issues) == 0, issues


def check_quit_cleanup_order(content: str) -> Tuple[bool, List[str]]:
    """
    Verify a_Quit() cleans up in proper order:
    1. Timers before SDL shutdown
    2. SDL subsystems in reverse order (TTF -> IMG -> SDL)

    Returns: (success, list of issues)
    """
    issues = []

    quit_match = re.search(
        r'void\s+a_Quit\s*\(\s*void\s*\)\s*\{',
        content
    )

    if not quit_match:
        issues.append("a_Quit() function not found")
        return False, issues

    start = quit_match.end()
    brace_count = 1
    end = start

    while end < len(content) and brace_count > 0:
        if content[end] == '{':
            brace_count += 1
        elif content[end] == '}':
            brace_count -= 1
        end += 1

    quit_body = content[start:end]

    # Check timer cleanup happens before SDL_Quit
    timer_free_pos = quit_body.find('a_TimerFree')
    sdl_quit_pos = quit_body.find('SDL_Quit()')

    if timer_free_pos != -1 and sdl_quit_pos != -1:
        if timer_free_pos > sdl_quit_pos:
            issues.append("Timers must be freed before SDL_Quit() (timers depend on SDL)")

    # Check SDL subsystem shutdown order: TTF -> IMG -> SDL
    ttf_pos = quit_body.find('TTF_Quit()')
    img_pos = quit_body.find('IMG_Quit()')

    if ttf_pos != -1 and img_pos != -1 and ttf_pos > img_pos:
        issues.append("Shutdown order wrong: TTF_Quit() must come before IMG_Quit()")
    if img_pos != -1 and sdl_quit_pos != -1 and img_pos > sdl_quit_pos:
        issues.append("Shutdown order wrong: IMG_Quit() must come before SDL_Quit()")

    return len(issues) == 0, issues


def verify_main_application_init(project_root: Path) -> bool:
    """
    Main verification function for FF-000.

    Returns: True if all checks pass, False otherwise
    """
    print("=" * 70)
    print("FF-000: Main Application Init Verification")
    print("=" * 70)
    print()
    print("Enforcing ADR-000: Validated initialization with error propagation")
    print()

    init_file = project_root / "src" / "aInitialize.c"

    if not init_file.exists():
        print(f"FAILURE: Could not find {init_file}")
        return False

    with open(init_file, 'r', encoding='utf-8') as f:
        content = f.read()

    all_issues = []

    # Check 1: InitStatus_t enum
    print("Checking InitStatus_t enum...")
    success, issues = check_init_status_enum(content)
    if success:
        print("  PASS: InitStatus_t enum correctly defined")
    else:
        for issue in issues:
            print(f"  FAIL: {issue}")
        all_issues.extend(issues)

    # Check 2: a_ValidateSubsystems function
    print("Checking a_ValidateSubsystems()...")
    success, issues = check_validate_subsystems(content)
    if success:
        print("  PASS: a_ValidateSubsystems() correctly defined as static")
    else:
        for issue in issues:
            print(f"  FAIL: {issue}")
        all_issues.extend(issues)

    # Check 3: No magic numbers
    print("Checking for magic number returns...")
    success, issues = check_no_magic_numbers(content)
    if success:
        print("  PASS: No magic number returns in a_Init()")
    else:
        for issue in issues:
            print(f"  FAIL: {issue}")
        all_issues.extend(issues)

    # Check 4: a_Init uses validation function
    print("Checking a_Init() uses a_ValidateSubsystems()...")
    success, issues = check_init_uses_validation(content)
    if success:
        print("  PASS: a_Init() properly calls and checks a_ValidateSubsystems()")
    else:
        for issue in issues:
            print(f"  FAIL: {issue}")
        all_issues.extend(issues)

    # Check 5: No direct subsystem calls in a_Init
    print("Checking no direct subsystem init calls in a_Init()...")
    success, issues = check_no_direct_subsystem_calls(content)
    if success:
        print("  PASS: All subsystem init calls are in a_ValidateSubsystems()")
    else:
        for issue in issues:
            print(f"  FAIL: {issue}")
        all_issues.extend(issues)

    # Check 6: Window creation cleanup
    print("Checking window creation failure cleanup...")
    success, issues = check_window_creation_cleanup(content)
    if success:
        print("  PASS: Window creation failure properly cleans up all subsystems")
    else:
        for issue in issues:
            print(f"  FAIL: {issue}")
        all_issues.extend(issues)

    # Check 7: Cleanup order in a_ValidateSubsystems
    print("Checking cleanup order in a_ValidateSubsystems()...")
    success, issues = check_cleanup_order(content)
    if success:
        print("  PASS: Cleanup follows reverse initialization order")
    else:
        for issue in issues:
            print(f"  FAIL: {issue}")
        all_issues.extend(issues)

    # Check 8: a_CleanupSubsystems exists
    print("Checking a_CleanupSubsystems() exists...")
    success, issues = check_cleanup_subsystems_exists(content)
    if success:
        print("  PASS: a_CleanupSubsystems() exists as mirror of validation")
    else:
        for issue in issues:
            print(f"  FAIL: {issue}")
        all_issues.extend(issues)

    # Check 9: a_Quit cleanup order
    print("Checking a_Quit() cleanup order...")
    success, issues = check_quit_cleanup_order(content)
    if success:
        print("  PASS: a_Quit() cleans up in correct order")
    else:
        for issue in issues:
            print(f"  FAIL: {issue}")
        all_issues.extend(issues)

    print()

    if all_issues:
        print("=" * 70)
        print("ARCHITECTURE VIOLATION DETECTED")
        print("=" * 70)
        print()
        print(f"Found {len(all_issues)} issue(s):")
        for i, issue in enumerate(all_issues, 1):
            print(f"  {i}. {issue}")
        print()
        print("See: architecture/decisions/00_main_application_init.md")
        print()
        return False

    print("SUCCESS: All main application init checks passed")
    return True


if __name__ == "__main__":
    script_dir = Path(__file__).parent
    project_root = script_dir.parent.parent

    success = verify_main_application_init(project_root)
    exit(0 if success else 1)

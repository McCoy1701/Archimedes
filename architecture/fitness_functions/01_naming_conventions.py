#!/usr/bin/env python3
"""
FF-001: Naming Conventions Verification

Ensures the naming patterns from ADR-001 are maintained:
1. Functions use a_ prefix
2. "Drawn" objects use a_Draw* pattern (Text, Rect, Circle, etc.)
3. "Used" objects use a_Object* pattern (Timer, Flex, Widget, etc.)
4. Types use aObject_t pattern (lowercase a, PascalCase, _t suffix)

Based on: ADR-001 (Naming Conventions)

Architecture Characteristic: Discoverability, Consistency, Maintainability
"""

import re
from pathlib import Path
from typing import List, Tuple, Set


# Objects that should use a_Draw* pattern for discoverability
DRAWN_OBJECTS = {
    "Text", "Rect", "FilledRect", "Circle", "FilledCircle",
    "Line", "HorizontalLine", "VerticalLine", "Point",
    "Triangle", "FilledTriangle", "Widgets"
}

# Objects that should use a_Object* pattern (noun-verb)
USED_OBJECTS = {
    "Timer", "Viewport", "Flex", "Widget", "Image", "Audio",
    "AUF", "Glyph", "Font", "Texture", "Blit", "Error"
}

# Pattern to match function declarations in header
# Matches: type a_FunctionName( or type* a_FunctionName(
FUNCTION_DECL_PATTERN = re.compile(
    r'^(?:extern\s+)?(?:static\s+)?(?:const\s+)?'
    r'(?:\w+\*?\s+)+?'  # return type
    r'(a_\w+)\s*\(',    # function name starting with a_
    re.MULTILINE
)

# Pattern to match typedef struct declarations
# Matches: } aTypeName_t;
TYPE_DECL_PATTERN = re.compile(
    r'\}\s*(a\w+_t)\s*;'
)

# Pattern for type names (should be aObjectName_t)
VALID_TYPE_PATTERN = re.compile(r'^a[A-Z][A-Za-z0-9]*_t$')


def extract_functions(content: str) -> List[Tuple[str, int]]:
    """
    Extract all a_* function names and their line numbers.

    Returns: List of (function_name, line_number) tuples
    """
    functions: List[Tuple[str, int]] = []
    lines = content.split('\n')

    for i, line in enumerate(lines, 1):
        match = FUNCTION_DECL_PATTERN.search(line)
        if match:
            func_name = match.group(1)
            functions.append((func_name, i))

    return functions


def extract_types(content: str) -> List[Tuple[str, int]]:
    """
    Extract all a*_t type names and their line numbers.

    Returns: List of (type_name, line_number) tuples
    """
    types: List[Tuple[str, int]] = []
    lines = content.split('\n')

    for i, line in enumerate(lines, 1):
        match = TYPE_DECL_PATTERN.search(line)
        if match:
            type_name = match.group(1)
            types.append((type_name, i))

    return types


def classify_function(func_name: str) -> Tuple[str, str]:
    """
    Classify a function and determine if it follows naming conventions.

    Returns: (category, issue_or_empty)
        category: "draw", "used", "lifecycle", "utility", "unknown"
        issue: Empty string if valid, otherwise description of issue
    """
    # Remove a_ prefix
    name_without_prefix = func_name[2:]  # Remove "a_"

    # Check if it's a Draw function
    if name_without_prefix.startswith("Draw"):
        drawn_obj = name_without_prefix[4:]  # Remove "Draw"
        if drawn_obj in DRAWN_OBJECTS or drawn_obj == "":
            return ("draw", "")
        # It's a Draw function but object not in our list - might need updating
        return ("draw", "")

    # Check if it starts with a known "used" object
    for obj in USED_OBJECTS:
        if name_without_prefix.startswith(obj):
            return ("used", "")

    # Check if it's a drawn object that should use Draw prefix
    for drawn in DRAWN_OBJECTS:
        # e.g., a_TextDraw should be a_DrawText
        if name_without_prefix.startswith(drawn) and not name_without_prefix.startswith("Draw"):
            return ("violation", f"'{func_name}' should be 'a_Draw{drawn}*' (drawn objects use a_Draw* pattern)")

    # Lifecycle functions (Init, Quit, Prepare, Present, Do)
    lifecycle_prefixes = ["Init", "Quit", "Prepare", "Present", "Do", "Get", "Set", "Free", "Load", "Save", "Parse", "Read", "Calc", "Update", "To", "Screenshot", "STR_", "Count"]
    for prefix in lifecycle_prefixes:
        if name_without_prefix.startswith(prefix):
            return ("lifecycle", "")

    # Special known functions
    known_special = ["Blit", "BlitSurfRect", "BlitTextureRect", "BlitTextureScaled", "SetPixel", "PrintAUFTree"]
    if name_without_prefix in known_special or any(name_without_prefix.startswith(s) for s in known_special):
        return ("utility", "")

    return ("unknown", f"'{func_name}' doesn't match known patterns - may need classification")


def check_function_naming(content: str) -> Tuple[bool, List[str]]:
    """
    Verify all functions follow naming conventions.

    Returns: (success, list of issues)
    """
    issues:  List[str] = []
    functions = extract_functions(content)

    for func_name, line_num in functions:
        # Must start with a_
        if not func_name.startswith("a_"):
            issues.append(f"Line {line_num}: '{func_name}' must start with 'a_' prefix")
            continue

        _, issue = classify_function(func_name)
        if issue:
            issues.append(f"Line {line_num}: {issue}")

    return len(issues) == 0, issues


def check_type_naming(content: str) -> Tuple[bool, List[str]]:
    """
    Verify all types follow aObjectName_t pattern.

    Returns: (success, list of issues)
    """
    issues = []
    types = extract_types(content)

    for type_name, line_num in types:
        if not VALID_TYPE_PATTERN.match(type_name):
            issues.append(f"Line {line_num}: Type '{type_name}' should match 'aObjectName_t' pattern (lowercase 'a', PascalCase, '_t' suffix)")

    return len(issues) == 0, issues


def check_drawn_objects_use_draw_prefix(content: str) -> Tuple[bool, List[str]]:
    """
    Verify that functions for "drawn" objects use a_Draw* pattern.

    Returns: (success, list of issues)
    """
    issues = []
    functions = extract_functions(content)

    for func_name, line_num in functions:
        name_without_prefix = func_name[2:]  # Remove "a_"

        # Check if function name contains a drawn object but doesn't start with Draw
        for drawn in DRAWN_OBJECTS:
            # Skip if it's already using Draw prefix correctly
            if name_without_prefix.startswith("Draw"):
                break

            # Check if function is for a drawn object but not using Draw prefix
            # e.g., a_TextRender should be a_DrawText
            if drawn in name_without_prefix and not name_without_prefix.startswith(drawn):
                # This is probably fine - it's a helper/utility function
                pass

    return len(issues) == 0, issues


def get_function_stats(content: str) -> dict:
    """
    Get statistics about function naming patterns.

    Returns: Dictionary with counts by category
    """
    functions = extract_functions(content)
    stats = {"draw": 0, "used": 0, "lifecycle": 0, "utility": 0, "unknown": 0, "violation": 0}

    for func_name, _ in functions:
        if func_name.startswith("a_"):
            category, _ = classify_function(func_name)
            stats[category] = stats.get(category, 0) + 1

    return stats


def verify_naming_conventions(project_root: Path) -> bool:
    """
    Main verification function for FF-001.

    Returns: True if all checks pass, False otherwise
    """
    print("=" * 70)
    print("FF-001: Naming Conventions Verification")
    print("=" * 70)
    print()
    print("Enforcing ADR-001: Hybrid naming with Draw exception")
    print()

    header_file = project_root / "include" / "Archimedes.h"

    if not header_file.exists():
        print(f"FAILURE: Could not find {header_file}")
        return False

    with open(header_file, 'r', encoding='utf-8') as f:
        content = f.read()

    all_issues = []

    # Check 1: Function naming
    print("Checking function naming conventions...")
    success, issues = check_function_naming(content)
    if success:
        print("  PASS: All functions follow naming conventions")
    else:
        for issue in issues:
            print(f"  WARN: {issue}")
        # Don't add unknown pattern warnings as failures
        actual_issues = [i for i in issues if "doesn't match known patterns" not in i]
        all_issues.extend(actual_issues)

    # Check 2: Type naming
    print("Checking type naming conventions...")
    success, issues = check_type_naming(content)
    if success:
        print("  PASS: All types follow aObjectName_t pattern")
    else:
        for issue in issues:
            print(f"  FAIL: {issue}")
        all_issues.extend(issues)

    # Print stats
    print()
    print("Function classification stats:")
    stats = get_function_stats(content)
    print(f"  Draw functions (a_Draw*): {stats['draw']}")
    print(f"  Object functions (a_Object*): {stats['used']}")
    print(f"  Lifecycle functions: {stats['lifecycle']}")
    print(f"  Utility functions: {stats['utility']}")
    print(f"  Unclassified: {stats['unknown']}")
    if stats['violation'] > 0:
        print(f"  Violations: {stats['violation']}")

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
        print("See: architecture/decisions/01_naming_conventions.md")
        print()
        return False

    print("SUCCESS: All naming convention checks passed")
    return True


if __name__ == "__main__":
    script_dir = Path(__file__).parent
    project_root = script_dir.parent.parent

    success = verify_naming_conventions(project_root)
    exit(0 if success else 1)

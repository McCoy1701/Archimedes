#!/usr/bin/env python3
"""
Fitness Function Runner

Executes all architecture fitness functions and reports results.
Exits with non-zero code if any fitness function fails.

Usage:
    python architecture/fitness_functions/main.py
    make verify
"""

import sys
from pathlib import Path
from typing import List, Tuple

# Import fitness functions directly
import importlib.util

# FF-000: Main Application Init
spec0 = importlib.util.spec_from_file_location(
    "main_application_init",
    Path(__file__).parent / "00_main_application_init.py"
)
main_application_init = importlib.util.module_from_spec(spec0)
spec0.loader.exec_module(main_application_init)


def run_fitness_functions() -> List[Tuple[str, bool]]:
    """Run all fitness functions and collect results"""
    results = []

    print("=" * 70)
    print("ARCHITECTURE FITNESS FUNCTIONS")
    print("=" * 70)
    print()

    script_dir = Path(__file__).parent
    project_root = script_dir.parent.parent

    # FF-000: Main Application Init
    try:
        success = main_application_init.verify_main_application_init(project_root)
        results.append(("FF-000: Main Application Init", success))
    except Exception as e:
        print(f"FF-000 crashed: {e}")
        results.append(("FF-000: Main Application Init", False))

    print()

    return results


def print_summary(results: List[Tuple[str, bool]]) -> bool:
    """Print summary and return overall pass/fail"""
    print("=" * 70)
    print("SUMMARY")
    print("=" * 70)
    print()

    passed = sum(1 for _, success in results if success)
    failed = len(results) - passed

    for name, success in results:
        status = "PASS" if success else "FAIL"
        print(f"{status}: {name}")

    print()
    print(f"Total: {passed}/{len(results)} passed, {failed}/{len(results)} failed")
    print()

    if failed == 0:
        print("All fitness functions passed!")
        return True
    else:
        print("Some fitness functions failed - see details above")
        return False


if __name__ == '__main__':
    results = run_fitness_functions()
    overall_success = print_summary(results)
    sys.exit(0 if overall_success else 1)

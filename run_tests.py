#!/usr/bin/env python3
import argparse
import os
import shutil
import subprocess
import sys
from pathlib import Path


def first_difference(expected: bytes, actual: bytes):
    size = min(len(expected), len(actual))
    for i in range(size):
        if expected[i] != actual[i]:
            return i
    if len(expected) != len(actual):
        return size
    return -1


def print_fragment(label: str, data: bytes, pos: int):
    start = max(0, pos - 40)
    end = min(len(data), pos + 80)
    print(f"    {label} fragment:")
    print(f"    {data[start:end]!r}")


def run_test(program: Path, test_dir: Path, work_dir: Path, timeout: float) -> bool:
    input_path = test_dir / "in.txt"
    expected_path = test_dir / "expected.txt"

    if not input_path.exists() or not expected_path.exists():
        print(f"[BAD ] {test_dir.name}: missing in.txt or expected.txt")
        return False

    shutil.copyfile(input_path, work_dir / "in.txt")
    result_path = work_dir / "result.txt"

    if result_path.exists():
        result_path.unlink()

    completed = subprocess.run(
        [str(program), "in.txt"],
        cwd=work_dir,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        timeout=timeout,
    )

    if not result_path.exists():
        print(f"[FAIL] {test_dir.name}")
        print("    result.txt was not created")
        print("    return code:", completed.returncode)
        if completed.stdout:
            print("    stdout:", completed.stdout.decode("utf-8", errors="replace"))
        if completed.stderr:
            print("    stderr:", completed.stderr.decode("utf-8", errors="replace"))
        return False

    expected = expected_path.read_bytes()
    actual = result_path.read_bytes()

    if actual == expected:
        print(f"[ OK ] {test_dir.name}")
        return True

    print(f"[FAIL] {test_dir.name}")
    print("    return code:", completed.returncode)
    print("    expected bytes:", len(expected))
    print("    actual bytes:  ", len(actual))

    pos = first_difference(expected, actual)
    if pos >= 0:
        print("    first difference at byte:", pos)
        if pos < len(expected):
            print("    expected byte:", expected[pos])
        else:
            print("    expected byte: <EOF>")
        if pos < len(actual):
            print("    actual byte:  ", actual[pos])
        else:
            print("    actual byte:   <EOF>")
        print_fragment("expected", expected, pos)
        print_fragment("actual", actual, pos)

    if completed.stdout:
        print("    stdout:")
        print(completed.stdout.decode("utf-8", errors="replace"))
    if completed.stderr:
        print("    stderr:")
        print(completed.stderr.decode("utf-8", errors="replace"))

    return False


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Run Terracraft bot tests and compare result.txt byte by byte."
    )
    parser.add_argument("program", nargs="?", default="./task")
    parser.add_argument("--tests", default="tests")
    parser.add_argument("--work-dir", default=".")
    parser.add_argument("--timeout", type=float, default=5.0)
    args = parser.parse_args()

    program = Path(args.program).resolve()
    tests_dir = Path(args.tests).resolve()
    work_dir = Path(args.work_dir).resolve()

    if not program.exists():
        print("Program not found:", program)
        return 1

    if not tests_dir.exists():
        print("Tests directory not found:", tests_dir)
        return 1

    test_dirs = [p for p in sorted(tests_dir.iterdir()) if p.is_dir()]

    passed = 0
    failed = 0

    for test_dir in test_dirs:
        ok = run_test(program, test_dir, work_dir, args.timeout)
        if ok:
            passed += 1
        else:
            failed += 1

    print()
    print("Passed:", passed)
    print("Failed:", failed)
    print("Total: ", passed + failed)

    if failed != 0:
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())

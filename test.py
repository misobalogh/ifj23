#!/usr/bin/python

from pathlib import Path
import subprocess
from io import TextIOWrapper
from enum import IntEnum
from typing import List, Tuple, Iterator
from difflib import Differ


GREEN = '\033[92m'
RED = '\033[91m'
BLUE = '\033[94m'
MAGENTA = '\033[95m'
RESET = '\033[0m'

PATH = './tests_code'


class TestResultType(IntEnum):
    SUCCESS = 0
    COMPILATION_FAILED = 1
    INTERPRETATION_FAILED = 2
    OUTPUT_DIFFERS = 3
    TIMEOUT = 4


class TestResult:
    def __init__(self, result_type: TestResultType, code: int = -1, diff: 'Iterator[str]|None' = None):
        self.result_type = result_type
        self.exit_code = code
        self.diff = diff


def test(test_file: TextIOWrapper) -> TestResult:
    FILE = 'tests_code/output.code'
    TIMEOUT = 8

    try:
        print('generating code')
        with open(FILE, 'w') as code_file:
            result = subprocess.run('./main',
                                    stdin=test_file,
                                    stdout=code_file,
                                    # stderr=subprocess.DEVNULL,
                                    timeout=TIMEOUT)

        if result.returncode != 0:
            return TestResult(TestResultType.COMPILATION_FAILED, result.returncode)

        print('interpreting code')
        int_pipe = subprocess.Popen(['ic23int', FILE], stdout=subprocess.PIPE,
                                    # stderr=subprocess.DEVNULL
                                    )
        int_out, _ = int_pipe.communicate(timeout=TIMEOUT)
        if int_pipe.returncode != 0:
            return TestResult(TestResultType.INTERPRETATION_FAILED, int_pipe.returncode)

        test_file.seek(0)

        print('comparing interpreter output with swift output')
        with open('ifj23.swift', 'r') as builtins:
            bts = builtins.read().encode() + b'\n' + test_file.read().encode()
            swift_process = subprocess.Popen(['swift', '-'],
                                           stdin=subprocess.PIPE,
                                           stdout=subprocess.PIPE,
                                           stderr=subprocess.DEVNULL)

        swift_out, _  = swift_process.communicate(input=bts)
        swift_process.wait()

        int_out = int_out.decode()
        swift_out = swift_out.decode()

        if int_out != swift_out:
            d = Differ().compare(int_out, swift_out)
            print(d)
            return TestResult(TestResultType.OUTPUT_DIFFERS, diff=d)

    except TimeoutError:
        return TestResult(TestResultType.TIMEOUT)

    return TestResult(TestResultType.SUCCESS)

def print_test(name: str, result: TestResult):
    if result.result_type == TestResultType.COMPILATION_FAILED:
        print(f'{RED}generating code for test {name} failed with {result.exit_code}{RESET}')
    elif result.result_type == TestResultType.INTERPRETATION_FAILED:
        print(f'{RED}interpreting test {name} failed with {result.exit_code}{RESET}')
    elif result.result_type == TestResultType.OUTPUT_DIFFERS:
        print(f'{RED}test {name} failed, outputs differ{RESET}')
        print(f'{BLUE}writing diff to file {name}.diff{RESET}')

        with open(f'./tests_code/{name}.diff', 'w') as file:
            file.writelines(result.diff or ['No diff'])

    elif result.result_type == TestResultType.SUCCESS:
        print(f'{GREEN}test {name} successful{RESET}')
    elif result.result_type == TestResultType.TIMEOUT:
        print(f'{RED}test {name} timed out{RESET}')
    else:
        raise ValueError()


def print_table(table: List[Tuple[str, TestResult]]):
    cellsize = len(max(table, key=lambda t : len(t[0]))[0])
    table.sort(key=lambda t : t[1].result_type)

    for test_name, test_result in table:
        color = GREEN if test_result.result_type == TestResultType.SUCCESS else RED
        print(f'{color}{test_name:{cellsize}} {test_result.result_type.name}{RESET}')

test_files = [f.resolve() for f in Path(PATH).iterdir() if f.is_file() and f.match('*.swift')]
test_results = dict()
passed_count = 0

for test_name in test_files:
    print(f'running test {test_name.name}')

    with open(test_name, 'r') as test_file:
        result = test(test_file)
        print_test(test_name.name, result)
        print()
        test_results[test_name.name] = result
        if result.result_type == TestResultType.SUCCESS:
            passed_count += 1

print('\nsummary:')
print_table(list(test_results.items()))
print(f'passed {passed_count} / {len(test_files)}')


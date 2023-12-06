#!/usr/bin/python

from pathlib import Path
import subprocess
from io import TextIOWrapper
from enum import IntEnum
from typing import List, Tuple
from difflib import Differ


GREEN = '\033[92m'
RED = '\033[91m'
BLUE = '\033[94m'
MAGENTA = '\033[95m'
CYAN = '\033[96m'
RESET = '\033[0m'

PATH = './tests_code'


class TestResultType(IntEnum):
    PASSED = 0
    COMPILATION_FAILED = 1
    INTERPRETATION_FAILED = 2
    OUTPUT_DIFFERS = 3
    TIMEOUT = 4


class TestResult:
    def __init__(self, result_type: TestResultType, code: 'int|None' = None, diff: 'List[str]|None' = None):
        self.result_type = result_type
        self.exit_code = code
        self.diff = diff


def test(test_file: TextIOWrapper) -> TestResult:
    CODE_FILE = 'tests_code/output.code'
    TEMP_FILE = 'tests_code/temp'
    TIMEOUT_SECONDS = 16
    BUILTINS_FILE = 'ifj23.swift'
    file_name = Path(test_file.name).name

    try:
        print('[generating code]')
        with open(CODE_FILE, 'w') as code_file:
            result = subprocess.run('./main', stdin=test_file, stdout=code_file, timeout=TIMEOUT_SECONDS)

        if result.returncode != 0:
            return TestResult(TestResultType.COMPILATION_FAILED, result.returncode)

        try:
            with open('tests_code/' + file_name.removesuffix('.swift') + '.in', 'r') as input_file:
                test_input = input_file.read().encode()
        except FileNotFoundError:
              test_input = b''

        print('[interpreting code]')
        int_pipe = subprocess.Popen(['ic23int', CODE_FILE],
                                    stdin=subprocess.PIPE,
                                    stdout=subprocess.PIPE)
        int_out, _ = int_pipe.communicate(input=test_input, timeout=TIMEOUT_SECONDS)
        int_pipe.wait()
        if int_pipe.returncode != 0:
            return TestResult(TestResultType.INTERPRETATION_FAILED, int_pipe.returncode)

        test_file.seek(0)

        print('[comparing interpreter output with swift output]')
        with open(BUILTINS_FILE, 'r') as builtins_file, open(TEMP_FILE, 'w') as temp_file:
            temp_file.write(builtins_file.read() + '\n' + test_file.read())

        swift_process = subprocess.Popen(['swift', TEMP_FILE],
                                       stdin=subprocess.PIPE,
                                       stdout=subprocess.PIPE,
                                       stderr=subprocess.DEVNULL)

        swift_out, _  = swift_process.communicate(input=test_input, timeout=TIMEOUT_SECONDS)
        swift_process.wait()

        int_out = int_out.decode()
        swift_out = swift_out.decode()

        if int_out != swift_out:
            d = list(Differ().compare(int_out.splitlines(), swift_out.splitlines()))
            print_diff(d)
            return TestResult(TestResultType.OUTPUT_DIFFERS, diff=d)

    except subprocess.TimeoutExpired:
        return TestResult(TestResultType.TIMEOUT)

    return TestResult(TestResultType.PASSED, 0)

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

    elif result.result_type == TestResultType.PASSED:
        print(f'{GREEN}test {name} passed{RESET}')
    elif result.result_type == TestResultType.TIMEOUT:
        print(f'{RED}test {name} timed out{RESET}')
    else:
        raise ValueError()


def print_table(table: List[Tuple[str, TestResult]]):
    cellsize = len(max(table, key=lambda t : len(t[0]))[0]) + 1
    table.sort(key=lambda t : t[1].result_type)
    resultsize = max(len(r.name) for r in TestResultType) + 1

    print('Test'.ljust(cellsize), 'Result'.ljust(resultsize), 'Exit code', sep='')

    for test_name, test_result in table:
        color = GREEN if test_result.result_type == TestResultType.PASSED else RED
        print(test_name.ljust(cellsize, '.'),
              (color + test_result.result_type.name + RESET).ljust(31, '.'),
              test_result.exit_code,
              sep='')

def print_diff(diff: List[str]):
    print('-------------------- diff --------------------')
    for line in diff:
        if line[0] == '-':
            color = MAGENTA
        elif line[0] == '+':
            color = CYAN
        else:
            color = RESET

        print(f'{color}{line}{RESET}')
    print('------------------- end diff -----------------')

test_files = [f.resolve() for f in Path(PATH).iterdir() if f.is_file() and f.match('*.swift')]
test_results = dict()
passed_count = 0

for test_name in test_files:
    print(f'[running test {test_name.name}]')

    with open(test_name, 'r') as test_file:
        result = test(test_file)
        print_test(test_name.name, result)
        print()
        test_results[test_name.name] = result
        if result.result_type == TestResultType.PASSED:
            passed_count += 1

print('\nsummary:')
print_table(list(test_results.items()))
print(f'passed {passed_count} / {len(test_files)}')

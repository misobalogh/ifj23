import os
import sys
import subprocess
from subprocess import TimeoutExpired

timeout_seconds = 1

GREEN = "\033[92m"
RED = "\033[91m"
BLUE = "\033[94m"
MAGENTA = "\033[95m"
RESET = "\033[0m"


test_dir = "tests_integration/"
if not os.path.isdir(test_dir):
    print(
        f"""
{RED}
=========================================================================
Error: Directory with integration tests doesn't exist.                  |
=========================================================================
{RESET}
"""
    )
    exit(1)

if len(sys.argv) > 1:
    test_file = sys.argv[1] + ".swift"
    if "test_" not in test_file:
        test_file = "test_" + test_file
    test_files = [test_file]
else:
    test_files = [f for f in os.listdir(test_dir) if f.startswith("test_")]

main = "./main"
if not os.path.isfile(main):
    print(
        f"""
{RED}
=========================================================================
Error: 'main' doesn't exists.
=========================================================================
{RESET}"""
    )
    exit(1)

tests_passed = 0
failed_tests = []

for test_file in test_files:
    test_file_path = os.path.join(test_dir, test_file)

    run_test = f"{main} < {test_file_path}"

    print(
        f"""
{BLUE}
=========================================================================
Running test {test_file}...
{RESET}"""
    )

    try:
        completed_process = subprocess.run(
            run_test, shell=True, timeout=timeout_seconds
        )
        return_code = completed_process.returncode

    except TimeoutExpired:
        print(
            f"\n{RED}Test '{test_file}' timed out after {timeout_seconds} seconds. Test failed.{RESET}"
        )
        failed_tests.append((test_file, -1))
        continue

    except Exception as e:
        print(f"\n{RED}Encountered error while running test '{test_file}': {e}{RESET}")
        continue

    if return_code == 0:
        print(f"\n{GREEN}Test '{test_file}' passed.{RESET}")
        tests_passed += 1
    else:
        failed_tests.append((test_file, return_code))
        print(
            f"\n{RED}Test '{test_file}' failed with return code {return_code}.{RESET}"
        )

    print(
        f"""
{BLUE}
{test_file} finished with reutrn code {return_code}
=========================================================================
{RESET}"""
    )

col = GREEN if tests_passed / len(test_files) > 0.5 else RED

print(
    f"""
{RED}Failed tests:
========================================================================="""
)
for failed, ret_codes in failed_tests:
    print(f"{failed.ljust(55)}Return code:{ret_codes}")
print(
    f"""=========================================================================
{RESET}"""
)


print(
    f"""
{BLUE}SUMMARY:
=========================================================================
|  DON'T GIVE UP, YOU CAN DO IT! 💪{20*" "}{col}{tests_passed}/{len(test_files)} tests passed{BLUE}                   
|      💯😎🤑💲🐵🤖📈 {40*" "}{col}{tests_passed/len(test_files)*100:.2f}%{BLUE}    
=========================================================================
{RESET}"""
)

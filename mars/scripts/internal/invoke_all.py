import sys
import os
import subprocess

from util import get_bool_environ, load_json, simple_usage_message, wait_process_success
from color_util import cprint, cprinterr, colors
from invoke_util import get_short_verdict, is_verdict_expected
import tests_util as tu


INTERNALS_DIR = os.environ.get('INTERNALS')
LOGS_DIR = os.environ.get('LOGS_DIR')
SUBTASKS_JSON = os.environ.get('SUBTASKS_JSON')
SOLUTIONS_JSON = os.environ.get('SOLUTIONS_JSON')
SPECIFIC_TESTS = get_bool_environ('SPECIFIC_TESTS')
SPECIFIED_TESTS_PATTERN = os.environ.get('SPECIFIED_TESTS_PATTERN')
SOLUTION_DIR = os.environ.get('SOLUTION_DIR')
SKIP_CHECK = False


if __name__ == '__main__':
    if len(sys.argv) != 2:
        simple_usage_message("<tests-dir>")
    tests_dir = sys.argv[1]

    try:
        test_name_list = tu.get_test_names_from_tests_dir(tests_dir)
    except tu.MalformedTestsException as e:
        cprinterr(colors.ERROR, "Error:")
        sys.stderr.write("{}\n".format(e))
        sys.exit(4)

    if SPECIFIC_TESTS:
        tu.check_pattern_exists_in_test_names(SPECIFIED_TESTS_PATTERN, test_name_list)
        test_name_list = tu.filter_test_names_by_pattern(test_name_list, SPECIFIED_TESTS_PATTERN)

    available_tests, missing_tests = tu.divide_tests_by_availability(test_name_list, tests_dir)
    if missing_tests:
        cprinterr(colors.WARN, "Missing tests: "+(", ".join(missing_tests)))

    subtasks_tests_dict = tu.get_subtasks_tests_dict_from_tests_dir(tests_dir)
    
    print("Subtask summary")
    header_line = "%-30s %-5s" % ("Filename", "Pts")
    for subtask_index, (subtask, tests) in enumerate(subtasks_tests_dict.items()):
        num_available_tests = len(set(tests).intersection(set(available_tests)))
        command = [
            'bash',
            os.path.join(INTERNALS_DIR, 'subtask_summary.sh'),
            str(subtask_index),
            subtask,
            str(len(tests)),
            str(num_available_tests)
        ]
        wait_process_success(subprocess.Popen(command))

        if num_available_tests > 0:
            header_line += " %-11s" % "[{}]".format(subtask_index)

    print()
    print("Run result")
    print(header_line)

    subtasks_data = dict(load_json(SUBTASKS_JSON))['subtasks']
    solutions_data = dict(load_json(SOLUTIONS_JSON))
    unmatched_verdicts = []
    for solution_filename, solution_data in solutions_data.items():
        command = [
            'bash',
            os.path.join(INTERNALS_DIR, 'compile_solution.sh'),
            os.path.join(SOLUTION_DIR, solution_filename)
        ]
        ret = subprocess.Popen(command, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL).wait()
        if ret != 0:
            cprint(colors.FAIL, "{} does not compile".format(solution_filename))
            unmatched_verdicts.append((solution_filename, '-', 'Compilation Failed', solution_data.get("verdict", None)))
            continue

        for test_name in available_tests:
            command = [
                'bash',
                os.path.join(INTERNALS_DIR, 'invoke_test.sh'),
                tests_dir,
                test_name,
            ]
            wait_process_success(subprocess.Popen(command))

        total_points = 0
        solution_summary_data = []
        for subtask_index, (subtask, tests) in enumerate(subtasks_tests_dict.items()):
            subtask_result = None
            max_execution_time = None

            for test in tests:
                score = verdict = execution_time = None
                try:
                    with open(os.path.join(LOGS_DIR, "{}.score".format(test)), 'r') as sf:
                        score = float(sf.readlines()[0].strip('\n'))
                    with open(os.path.join(LOGS_DIR, "{}.verdict".format(test)), 'r') as vf:
                        verdict = vf.readlines()[0].strip('\n')
                    with open(os.path.join(LOGS_DIR, "{}.time".format(test)), 'r') as tf:
                        execution_time = float(tf.readlines()[0].strip('\n'))
                except FileNotFoundError:
                    pass
                else:
                    if subtask_result is None or score < subtask_result[0]:
                        subtask_result = (score, verdict, test)
                    if max_execution_time is None or max_execution_time < execution_time:
                        max_execution_time = execution_time

            if subtask_result is not None:
                subtask_score = subtask_result[0] * subtasks_data[subtask]['score']
                
                short_verdict_color = "ok"
                expected_verdict = solution_data.get("verdict", None)
                if "except" in solution_data:
                    expected_verdict = solution_data["except"].get(subtask, expected_verdict)
                if is_verdict_expected(subtask_result[0], subtask_result[1], expected_verdict):
                    short_verdict_color = "ok"
                else:
                    short_verdict_color = "fail"
                    unmatched_verdicts.append((solution_filename, subtask, subtask_result[1], expected_verdict))

                solution_summary_data.append(get_short_verdict(subtask_result[1]))
                solution_summary_data.append(short_verdict_color)
                solution_summary_data.append(str(max_execution_time))

                total_points += subtask_score

        command = [
            'bash',
            os.path.join(INTERNALS_DIR, 'solution_summary.sh'),
            solution_filename,
            '{:g}'.format(round(total_points, 2)),
        ] + solution_summary_data
        wait_process_success(subprocess.Popen(command))

    if len(unmatched_verdicts) == 0:
        cprint(colors.OK, "All verdict matches with solutions.json")
    else:
        cprint(colors.FAIL, "Found one or more subtasks mismatch with solutions.json")
        for solution_filename, subtask, verdict, expected_verdict in unmatched_verdicts:
            print("{:40}: got {:20}, expected '{}'".format("[{}] subtask '{}'".format(solution_filename, subtask), "'{}'".format(verdict), expected_verdict))

    if missing_tests:
        cprinterr(colors.WARN, "Missing {} {}!".format(len(missing_tests), "tests" if len(missing_tests) != 1 else "test"))

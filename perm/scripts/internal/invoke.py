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
SKIP_CHECK = get_bool_environ('SKIP_CHECK')


if __name__ == '__main__':
    if len(sys.argv) != 3:
        simple_usage_message("<tests-dir> <solution-path>")
    tests_dir = sys.argv[1]
    solution_filename = os.path.basename(sys.argv[2])

    solutions_data = dict(load_json(SOLUTIONS_JSON))
    solution_data = solutions_data.get(solution_filename, None)

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

    for test_name in available_tests:
        command = [
            'bash',
            os.path.join(INTERNALS_DIR, 'invoke_test.sh'),
            tests_dir,
            test_name,
        ]
        wait_process_success(subprocess.Popen(command))

    print()
    print("Subtask summary")

    if solution_data is None:
        cprint(colors.WARN, "Solution does not exist in solutions.json. Skipped checking verdict")

    subtasks_data = dict(load_json(SUBTASKS_JSON))['subtasks']
    total_points = total_full_points = 0
    unmatched_verdicts = []
    for subtask_index, (subtask, tests) in enumerate(tu.get_subtasks_tests_dict_from_tests_dir(tests_dir).items()):
        subtask_result = None
        max_execution_time = None
        testcases_run = 0

        for test in tests:
            score = verdict = execution_time = None
            if not SKIP_CHECK:
                try:
                    with open(os.path.join(LOGS_DIR, "{}.score".format(test)), 'r') as sf:
                        score = float(sf.readlines()[0].strip('\n'))
                    with open(os.path.join(LOGS_DIR, "{}.verdict".format(test)), 'r') as vf:
                        verdict = vf.readlines()[0].strip('\n')
                except FileNotFoundError:
                    pass
                else:
                    if subtask_result is None or score < subtask_result[0]:
                        subtask_result = (score, verdict, test)
            try:
                with open(os.path.join(LOGS_DIR, "{}.time".format(test)), 'r') as tf:
                    execution_time = float(tf.readlines()[0].strip('\n'))
            except FileNotFoundError:
                pass
            else:
                if max_execution_time is None or max_execution_time < execution_time:
                    max_execution_time = execution_time
                testcases_run += 1

        if max_execution_time is None:
            command = [
                'bash',
                os.path.join(INTERNALS_DIR, 'subtask_summary.sh'),
                str(subtask_index),
                subtask,
                str(len(tests))
            ]
            wait_process_success(subprocess.Popen(command))
        elif subtask_result is None:
            command = [
                'bash',
                os.path.join(INTERNALS_DIR, 'subtask_summary.sh'),
                str(subtask_index),
                subtask,
                str(len(tests)),
                str(testcases_run),
                str(max_execution_time)
            ]
            wait_process_success(subprocess.Popen(command))
        else:
            subtask_score = subtask_result[0] * subtasks_data[subtask]['score']

            short_verdict_color = "warn"
            if solution_data is not None:
                expected_verdict = solution_data.get("verdict", None)
                if "except" in solution_data:
                    expected_verdict = solution_data["except"].get(subtask, expected_verdict)
                if is_verdict_expected(subtask_result[0], subtask_result[1], expected_verdict):
                    short_verdict_color = "ok"
                else:
                    short_verdict_color = "fail"
                    unmatched_verdicts.append((subtask, subtask_result[1], expected_verdict))

            subtask_score_color = "ok"
            if subtask_result[0] == 0:
                subtask_score_color = "fail"
            elif subtask_result[0] < 1:
                subtask_score_color = "warn"
            
            command = [
                'bash',
                os.path.join(INTERNALS_DIR, 'subtask_summary.sh'),
                str(subtask_index),
                subtask,
                str(len(tests)),
                str(testcases_run),
                str(max_execution_time),
                get_short_verdict(subtask_result[1]),
                short_verdict_color,
                '{:g}'.format(round(subtask_score, 2)),
                subtask_score_color,
                str(subtasks_data[subtask]['score']),
                subtask_result[2]
            ]
            wait_process_success(subprocess.Popen(command))

            total_points += subtask_score
            total_full_points += subtasks_data[subtask]['score']

    if not SKIP_CHECK:
        color = colors.OK
        if total_points == 0:
            color = colors.ERROR
        elif total_points < total_full_points:
            color = colors.WARN
        cprint(color, "{:g}/{} pts".format(round(total_points, 2), total_full_points))

        if solution_data is not None:
            if len(unmatched_verdicts) == 0:
                cprint(colors.OK, "All verdict matches with solutions.json")
            else:
                cprint(colors.FAIL, "Found one or more subtasks mismatch with solutions.json")
                for subtask, verdict, expected_verdict in unmatched_verdicts:
                    print("[{}] got '{}', expected '{}'".format(subtask, verdict, expected_verdict))

    if missing_tests:
        cprinterr(colors.WARN, "Missing {} {}!".format(len(missing_tests), "tests" if len(missing_tests) != 1 else "test"))

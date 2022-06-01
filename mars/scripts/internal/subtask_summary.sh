#!/bin/bash

set -euo pipefail

source "${INTERNALS}/util.sh"

subtask_index="$1"; shift
subtask_name="$1"; shift
num_tests="$1"; shift

echo -n "[${subtask_index}] "
printf "%-${STATUS_PAD}s" "${subtask_name}"
if [ $# -gt 0 ]; then
  num_tests_run="$1"; shift

  if [ "${num_tests_run}" -eq "${num_tests}" ]; then
    tests_color="ok"
  elif [ "${num_tests_run}" -eq "0" ]; then
    tests_color="fail"
  else
    tests_color="warn"
  fi
  export BOX_PADDING=13
  boxed_echo "${tests_color}" "${num_tests_run}/${num_tests} tests"

  if [ $# -gt 0 ]; then
    max_execution_time="$1"; shift
    hspace 2
    printf "%7s" "${max_execution_time}"

    if [ $# -gt 0 ]; then
      short_verdict="$1"; shift
      short_verdict_color="$1"; shift
      subtask_score="$1"; shift
      subtask_score_color="$1"; shift
      full_subtask_score="$1"; shift
      test_name="$1"; shift

      hspace 2
      export BOX_PADDING=3
      boxed_echo "${short_verdict_color}" "${short_verdict}"

      hspace 2
      export BOX_PADDING=13
      boxed_echo "${subtask_score_color}" "${subtask_score}/${full_subtask_score} pts"

      hspace 2
      echo "${test_name}"
    else
      echo
    fi
  else
    echo
  fi
else
  boxed_echo "fail" "0/${num_tests} tests"
  echo
fi

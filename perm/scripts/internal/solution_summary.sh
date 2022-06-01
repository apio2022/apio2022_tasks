#!/bin/bash

set -euo pipefail

source "${INTERNALS}/util.sh"

solution_filename="$1"; shift
score="$1"; shift
printf "%-30s %-5s" "${solution_filename}" "${score}"
while [ $# -gt 0 ]; do
  short_verdict="$1"; shift
  short_verdict_color="$1"; shift
  max_execution_time="$1"; shift

  hspace 1
  export BOX_PADDING=3
  boxed_echo "${short_verdict_color}" "${short_verdict}"

  hspace 1
  printf "%-5s" "${max_execution_time}"
done

echo

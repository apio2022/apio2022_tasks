
# This file must be "source"d from "invoke_test.sh".
# This is the script for extracting the results from the checker outputs.
# You should modify this file if the checker is going to respond differently.

# The score is written on the first line of checker standard output.
score="$(sed -n 1p "${checker_stdout}")"

# The verdict is written on the first line of checker standard error.
verdict="$(sed -n 1p "${checker_stderr}")"

# Convert CMS-style messages to TPS-style messages.
if [ "${verdict}" == "Output is correct" ]; then
  verdict="Correct"
elif [ "${verdict}" == "Output isn't correct" ]; then
  verdict="Wrong Answer"
elif [ "${verdict}" == "Output is partially correct" ]; then
  verdict="Partially Correct"
fi

# The verdict reason (checker message) is written on the second line of checker standard error.
reason="$(sed -n 2p "${checker_stderr}")"

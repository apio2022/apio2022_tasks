def get_short_verdict(verdict):
    if verdict == "Correct":
        return "AC"
    elif verdict == "Runtime Error":
        return "RTE"
    elif verdict == "Time Limit Exceeded":
        return "TLE"
    elif verdict == "Wrong Answer":
        return "WA"
    elif verdict == "Partially Correct":
        return "OK"
    elif verdict == "Protocol Violation":
        return "PV"
    else:
        return "JE"


def is_verdict_expected(score, verdict, expected_verdict):
    if expected_verdict in ["correct", "model_solution"]:
        return verdict == "Correct" and score == 1
    elif expected_verdict == "time_limit":
        return verdict == "Time Limit Exceeded"
    elif expected_verdict == "memory_limit":
        return verdict == "Runtime Error"
    elif expected_verdict == "incorrect":
        return verdict == "Wrong Answer"
    elif expected_verdict == "runtime_error":
        return verdict == "Runtime Error"
    elif expected_verdict == "failed":
        return verdict != "Correct" or score == 0
    elif expected_verdict == "time_limit_and_runtime_error":
        return verdict in ["Time Limit Exceeded", "Runtime Error"]
    elif expected_verdict == "partially_correct":
        return 0 < score < 1
    else:
        raise ValueError("Invalid verdict")

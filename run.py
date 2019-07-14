from sys import argv
import math
import os
from os import listdir
from os.path import isfile, join

from mph.program import Program

DEFAULT_NUM_TRIALS = 10
NANOSECONDS_IN_SECOND = 1000000000
BENCHMARKS_PATH = os.getcwd() + "/bin"


class NoSuchBenchmarkError(Exception):
    pass


class BenchmarkFailedError(Exception):
    pass


def run_benchmark(name, num_elems):
    prog_handle = None
    benchmarks = [f for f in listdir(BENCHMARKS_PATH)]

    for benchmark in benchmarks:
        if name in benchmark:
            prog_handle = Program(join(BENCHMARKS_PATH, benchmark),
                    [str(num_elems)])
            break

    if prog_handle is None:
        raise NoSuchBenchmarkError()
    
    prog_handle.exec()

    if prog_handle.retval == 0:
        elems = str(prog_handle.stdout, "utf-8").strip().split(" ")
        fractional = float(elems[1]) / float(NANOSECONDS_IN_SECOND)
        benchmark_result = float(float(elems[0]) + fractional)
    else:
        raise BenchmarkFailedError()

    return benchmark_result

def run_test(benchmark_name, num_elems, num_samples=DEFAULT_NUM_TRIALS):
    results = []

    for i in range(num_samples):
        results.append(run_benchmark(benchmark_name, num_elems))

    mean = sum(results) / len(results)
    stddev = math.sqrt(sum([(f - mean)**2 for f in results]) / len(results))

    return (mean, stddev)

if __name__ == "__main__":
    if len(argv) != 3:
        print("usage: python run.py [num_elems] [num_trials]")
        exit(-1)

    num_elems = int(argv[1])
    num_trials = int(argv[2])

    benchmarks = [f for f in listdir(BENCHMARKS_PATH)]

    for b in benchmarks:
        mean, stddev = run_test(b, num_elems)

        print("{}: mu={}s sigma={} n={}".format(b, mean, stddev,
            DEFAULT_NUM_TRIALS))


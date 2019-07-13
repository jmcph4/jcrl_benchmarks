# JCRL Benchmarks #
---

This repository contains various performance benchmarks for [JCRL](https://github.com/jmcph4/jcrl).

## Building ##
THe benchmarks have two dependencies: JCRL itself (obviously) and [MPH](https://github.com/jmcph4/mph) (in order to run the benchmark script).

    $ git clone git@github.com:jmcph4/jcrl.git
    $ git clone git@github.com:jmcph4/mph.git
    $ git clone git@github.com:jmcph4/jcrl_benchmarks.git
    $ cp jcrl/src/*.h jcrl_benchmarks/include/jcrl -r
    $ cp mph/mph jcrl_benchmarks -r

Building the benchmark executables is straightforward:

    $ make all

This will create executables under `bin/`.

## Running ##
Each benchmarks is runnable as a standalone program; however, the main script is recommended as it handles statistical calculations and automatically runs all of the benchmarks.

    $ python run.py [num_elems] [num_trials]

This will output the benchmark name, mean running time, and the standard deviation of the running time.

Each executable itself outputs two numbers to `stdout`: the number of seconds elapsed and the number of nanoseconds (note this wraps).


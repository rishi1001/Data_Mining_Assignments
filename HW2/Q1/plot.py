import sys
import matplotlib.pyplot as plt
import numpy as np
from subprocess import STDOUT, TimeoutExpired, check_output
from timeit import timeit

def run_process_fsg(alg, thresh, inFile):
    command = f"./{alg} -s {thresh} {inFile}"
    print(command)
    try:
        check_output(command, shell=True, stderr=STDOUT)
    except TimeoutExpired:
        pass

def run_process_gSpan(alg, thresh, inFile):
    command = f"./{alg} -f {inFile} -s {thresh}"
    print(command)
    try:
        check_output(command, shell=True, stderr=STDOUT)
    except TimeoutExpired:
        pass

def run_process_gaston(alg, thresh, inFile):
    command = f"./{alg} {thresh} {inFile}"
    print(command)
    try:
        check_output(command, shell=True, stderr=STDOUT)
    except TimeoutExpired:
        pass

if __name__ == '__main__':
    inFile = sys.argv[1]
    outFile = sys.argv[2]
    totGraphs = int(open(sys.argv[3], 'r').read())
    min_support = [5,10,25,50,95]
    # min_support = [95]

    algs = ['Q1/fsg', 'Q1/gSpan','Q1/gaston']
    results = {alg:[3600 for x in min_support] for alg in algs}
    for i,thresh in enumerate(min_support):
        alg = 'Q1/fsg'
        setup = f"from __main__ import run_process_fsg;alg = \"{alg}\";thresh = {thresh};inFile = \"{inFile}\""
        # print(setup)
        time_taken = timeit(
            setup=setup,
            stmt="run_process_fsg(alg, thresh, inFile)",
            number=1
        )
        print(alg,thresh,time_taken)
        results[alg][i]= time_taken
        alg = 'Q1/gSpan'
        gSpan_thresh = thresh/100
        setup = f"from __main__ import run_process_gSpan;alg = \"{alg}\";thresh = {gSpan_thresh};inFile = \"{inFile}\""
        # print(setup)
        time_taken = timeit(
            setup=setup,
            stmt="run_process_gSpan(alg, thresh, inFile)",
            number=1
        )
        print(alg,thresh,time_taken)
        results[alg][i]= time_taken
        alg = 'Q1/gaston'
        gaston_thresh = thresh*totGraphs
        setup = f"from __main__ import run_process_gaston;alg = \"{alg}\";thresh = {gaston_thresh};inFile = \"{inFile}\""
        # print(setup)
        time_taken = timeit(
            setup=setup,
            stmt="run_process_gaston(alg, thresh, inFile)",
            number=1
        )
        print(alg,thresh,time_taken)
        results[alg][i]= time_taken
        for alg in algs:
            plt.plot(min_support, results[alg], marker='s', label=alg)
        plt.legend()
        plt.xlabel("Support %")
        plt.ylabel("Time (s)")
        plt.title("fsg vs gSpan vs gaston Performance Comparison")
        plt.savefig(outFile)
        plt.clf()

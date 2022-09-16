import sys
import matplotlib.pyplot as plt
import numpy as np
from subprocess import STDOUT, TimeoutExpired, check_output
from timeit import timeit

def run_process_fsg(alg, thresh, inFile):
    command = f"./{alg} -s {thresh} {inFile}"
    print(command)
    try:
        check_output(command, shell=True, stderr=STDOUT,timeout=3600)
    except TimeoutExpired:
        pass

def run_process_gSpan(alg, thresh, inFile):
    command = f"./{alg} -f {inFile} -s {thresh} -o -i"
    print(command)
    try:
        check_output(command, shell=True, stderr=STDOUT,timeout=3600)
    except TimeoutExpired:
        pass

def run_process_gaston(alg, thresh, inFile):
    command = f"./{alg} {thresh} {inFile}"
    print(command)
    try:
        check_output(command, shell=True, stderr=STDOUT,timeout=3600)
    except TimeoutExpired:
        pass

if __name__ == '__main__':
    inFile = sys.argv[1]
    outFile = sys.argv[2]
    min_support = [5,10,25,50,95]
    algs = ['fsg', 'gSpan','gaston']
    results = {alg:[3600 for x in min_support] for alg in algs}
    for i,thresh in enumerate(min_support):
        alg = 'fsg'
        setup = f"from __main__ import run_process_fsg;alg = \"{alg}\";thresh = {thresh};inFile = \"{inFile}\""
        # print(setup)
        time_taken = timeit(
            setup=setup,
            stmt="run_process_fsg(alg, thresh, inFile)",
            number=1
        )
        print(alg,thresh,time_taken)
        results[alg][i]= time_taken
        alg = 'gSpan'
        setup = f"from __main__ import run_process_gSpan;alg = \"{alg}\";thresh = {thresh};inFile = \"{inFile}\""
        # print(setup)
        time_taken = timeit(
            setup=setup,
            stmt="run_process_gSpan(alg, thresh, inFile)",
            number=1
        )
        print(alg,thresh,time_taken)
        alg = 'gaston'
        setup = f"from __main__ import run_process_gaston;alg = \"{alg}\";thresh = {thresh};inFile = \"{inFile}\""
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

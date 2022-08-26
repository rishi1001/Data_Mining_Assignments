import sys
import matplotlib.pyplot as plt
import numpy as np
from subprocess import STDOUT, TimeoutExpired, check_output
from timeit import timeit

def run_process(alg, thresh, inFile, outFile):
    command = f"./{alg} {inFile} {thresh} {outFile}.dat"
    print(command)
    try:
        check_output(command, shell=True, stderr=STDOUT,timeout=3600)
    except TimeoutExpired:
        pass

if __name__ == '__main__':
    inFile = sys.argv[1]
    outFile = sys.argv[2]
    thresholds = [90, 50, 25, 10, 5]
    algs = ['fpt', 'apriori']
    results = {alg:[3600 for x in thresholds] for alg in algs}
    for i,thresh in enumerate( thresholds):
        for alg in algs:
            setup = f"from __main__ import run_process;alg = \"{alg}\";thresh = {thresh};inFile = \"{inFile}\";outFile = \"{outFile}\""
            # print(setup)
            time_taken = timeit(
                setup=setup,
                stmt="run_process(alg, thresh, inFile, outFile)",
                number=1
            )
            print(alg,thresh,time_taken)
            results[alg][i]= time_taken
        for alg in algs:
            plt.plot(thresholds, results[alg], marker='s', label=alg)
        plt.legend()
        plt.xlabel("Support %")
        plt.ylabel("Time (s)")
        plt.title("Apriori vs FPTree Performance Comparison")
        plt.savefig(outFile)
        plt.clf()

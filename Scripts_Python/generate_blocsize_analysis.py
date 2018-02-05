import sys
import subprocess

def main():
    try:
        analysisType = sys.argv[1]
    except IndexError:
        print("You have to put an argument : \"time\" or \"cache\"")
        return

    if analysisType == "time":
        try:
            algorithm = sys.argv[2]
        except IndexError:
            print("You have to choose an algorithm : \"FW1_L\", \"FW1_C\", \"FW_SEQ_TILED_LAYOUT\", \"FW_PAR_TILED_LAYOUT\"")
            return

    def timeAnalysis(algorithm):
        subprocess.call(["make"])
        for i in [16, 18, 20, 22, 24, 40, 80, 200, 400, 4000]:
            print("size = " + str(i))
            subprocess.call(["./main", "-a", algorithm, "-g", "reference_graph5", \
                    "-s", str(i)])

    def cacheAnalysis():
        subprocess.call(["make", "debug"])
        for i in [20,40,80,200,2000]:
            subprocess.call(["valgrind", "--tool=cachegrind", "./debug", "-a", "FW_SEQ_TILED_LAYOUT", "-g", "test_cache_graph20", \
                    "-s", str(i)])

    if analysisType == "time":
        timeAnalysis(algorithm)
    if analysisType == "cache":
        cacheAnalysis()
                
main()

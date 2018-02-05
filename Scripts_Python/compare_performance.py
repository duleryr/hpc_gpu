import sys
import operator

def main():
    try:
        category = sys.argv[1]
    except IndexError:
        print("You have to choose a category : simple, medium, complex, very_complex, or huge")
        return 

    times = {"FWSeq" : 0.001, "FWSqM" : 0.001, "FWSMS" : 0.001, "FWP1C" : 0.001, "FWP1L" : 0.001, "FWSTI" : 0.001, "FWPTI" : 0.001, "FWSTB" : 0.001, "FWPTB" : 0.001, "FWS1D" : 0.001, "FWP1D" : 0.001}
    totalTimes = {"FWSeq" : 0, "FWSqM" : 0, "FWSMS" : 0, "FWP1C" : 0, "FWP1L" : 0, "FWSTI" : 0, "FWPTI" : 0, "FWSTB" : 0, "FWPTB" : 0, "FWS1D" : 0, "FWP1D" : 0}
    timesBetterThanSeq = {"FWSqM" : 0, "FWSMS" : 0, "FWP1C" : 0, "FWP1L" : 0, "FWSTI" : 0, "FWPTI" : 0, "FWSTB" : 0, "FWPTB" : 0, "FWS1D" : 0, "FWP1D" : 0}
    bestCase = {"FWSeq" : 0, "FWSqM" : -100, "FWSMS" : -100, "FWP1C" : -100, "FWP1L" : -100, "FWSTI" : -100, "FWPTI" : -100, "FWSTB" : -100, "FWPTB" : -100, "FWS1D" : -100, "FWP1D" : -100}
    worstCase = {"FWSeq" : 100, "FWSqM" : 100, "FWSMS" : 100, "FWP1C" : 100, "FWP1L" : 100, "FWSTI" : 100, "FWPTI" : 100, "FWSTB" : 100, "FWPTB" : 100, "FWS1D" : 100, "FWP1D" : 100}

    for file_number in range(30):
        file = open("../Calcul_Times/" + category + "_graph" + str(file_number) + ".txt", "r")
        content = file.read()
        content_algorithm = content.split(';')
        content_algorithm.pop()
        if len(content_algorithm) > 0:
            del content_algorithm[0]
        for txt in content_algorithm:
            detail = txt.split('::')
            algorithm = detail[0]
            algorithm = algorithm[-6:-1]
            time = detail[2]
            time = time[1:-2]
            times[algorithm] = float(time)
        file.close()

        for algo, time in times.items():
            if time < times["FWSeq"]:
                timesBetterThanSeq[algo] += 1
            totalTimes[algo] += time
            percent = (int) (100 * (times["FWSeq"] - time) / time)
            if percent > bestCase[algo]:
                bestCase[algo] = percent
            if percent < worstCase[algo]:
                worstCase[algo] = percent


    print("\nTimes better than the basic algorithm :")
    for algo, time in timesBetterThanSeq.items():
        print("    {} : {} times".format(algo, time))

    sorted_times = sorted(totalTimes.items(), key=operator.itemgetter(1))
    print("\nTotal time for each algorithm :")
    for algo, time in sorted_times:
        if (time == 30000):
            continue
        percent = (int) (100 * (totalTimes["FWSeq"] - time) / time)
        if percent > 0:
            print("    {} : ".format(algo) + "{0:.2f}".format(time) + ", {}% better than the basic algorithm".format(percent))
        elif percent == 0:
            print("    {} : ".format(algo) + "{0:.2f}s".format(time))
        else:
            print("    {} : ".format(algo) + "{0:.2f}".format(time) + ", {}% worse than the basic algorithm".format(-percent))

    sorted_bestCase = sorted(bestCase.items(), key=operator.itemgetter(1))
    print("\nBest case in comparison to the basic algorithm :")
    for algo, percent in sorted_bestCase:
        if (algo == "FWSeq" or percent == -99):
            continue
        print("    {} : {}% better".format(algo, percent))

    sorted_worstCase = sorted(worstCase.items(), key=operator.itemgetter(1))
    print("\nWorst case in comparison to the basic algorithm :")
    for algo, percent in sorted_worstCase:
        if (algo == "FWSeq" or percent == -99):
            continue
        print("    {} : {}% better".format(algo, percent))
main()

"""
    This file will generate random input files.
    The different parameters are the number of vertices, the number of edges, and the range of weights.
"""

import os
import sys
from random import randrange

def main():
    os.chdir("../Input_Files")
    
    try:
        categoryName = sys.argv[1]
    except IndexError:
        print("First argument should be the name of the files you want to create")
        return
    
    try:
        V = int(sys.argv[2])
    except IndexError:
        print("Second argument should be the maximum number of vertex")
        return
    
    try:
        E = int(sys.argv[3])
    except IndexError:
        print("Third argument should be the maximum number of edges")
        return
    
    for file_number in range(30):
        file = open(categoryName + str(file_number) + ".txt", "w")
        tmpV = randrange(V/2, V)
        tmpE = randrange(E/2, E)
        
        file.write(str(tmpV) + ' ' + str(tmpE) + "\n")
        for i in range(tmpE):
            origen = randrange(0, tmpV)
            destination = randrange(0, tmpV)
            weight = randrange(1, 100)
            file.write("{} {} {}\n".format(origen, destination, weight))
        file.close()

main()

import os
import sys
from random import randrange

def main():
    os.chdir("../Input_Files")

    try:
        filename = sys.argv[1]
    except IndexError:
        print("First argument should be the name of the file you want to create")
        return
    
    try:
        V = int(sys.argv[2])
    except IndexError:
        print("Second argument should be the number of vertex")
        return
    
    try:
        E = int(sys.argv[3])
    except IndexError:
        print("Third argument should be the number of edges")
        return

    file = open(filename + ".txt", "w")
    
    file.write(str(V) + ' ' + str(E) + "\n")
    for i in range(E):
        origen = randrange(0, V)
        destination = randrange(0, V)
        weight = randrange(1, 100)
        file.write("{} {} {}\n".format(origen, destination, weight))
    file.close()

main()

"""
    This file will compare the output of the different algorithms to the ground_truth that has been made manually
"""

from pprint import pprint
from difflib import Differ

d = Differ()
errorFound = {"FW_seq" : False, "FW_par1_l" : False, "FW_seq_tiled_layout" : False, "FW_seq_mem_skip" : False}
for i in range(0, 6):
    with open('../Ground_Truth/basic_graph' + str(i) + '.txt', 'r') as file1:
        file1_content = file1.readlines()
        for filename in ['../Output_Files/basic_graph' + str(i) + 'FW_seq.txt',
                         '../Output_Files/basic_graph' + str(i) + 'FW_par1_l.txt',
                         '../Output_Files/basic_graph' + str(i) + 'FW_seq_tiled_layout.txt',
                         '../Output_Files/basic_graph' + str(i) + 'FW_seq_mem_skip.txt']:
            with open(filename, 'r') as file2:
                result = list(d.compare(file1_content, file2.readlines()))
                for line in result:
                    try:
                        assert(line[0] != '-' and line[0] != '+' and line[0] != '?')
                    except AssertionError:
                        if filename[-10:] == 'FW_seq.txt':
                            errorFound["FW_seq"] = True
                        if filename[-13:] == 'FW_par1_l.txt':
                            errorFound["FW_par1_l"] = True
                        if filename[-16:] == 'tiled_layout.txt':
                            errorFound["FW_seq_tiled_layout"] = True
                        if filename[-19:] == 'FW_seq_mem_skip.txt':
                            errorFound["FW_par2"] = True
                        if (line[0] != '?'):
                            print('There is a difference between the files ../Ground_Truth/basic_graph' + str(i) + '.txt and ' + filename + ':')
                            print(line)
                            
if errorFound["FW_seq"]:
    print("There was an error in Floyd_Warshall sequential")
else:
    print("Floyd Warshall sequential OK")
if errorFound["FW_par1_l"]:
    print("There was an error in Floyd_Warshall parallel 1D lines")
else:
    print("Floyd_Warshall parallel 1D lines OK")
if errorFound["FW_seq_tiled_layout"]:
    print("There was an error in Floyd_Warshall Tiled Layout")
else:
    print("Floyd_Warshall Sequential Tiled Layout OK")
if errorFound["FW_seq_mem_skip"]:
    print("There was an error in Floyd_Warshall with memoization and skipping infinites")
else:
    print("Floyd_Warshall memoization and skip infinites OK")

"""
    This file will draw graphs based on input files, using the package networkx
    They can be saved by uncommenting the line 42
"""

import os
import networkx as nx
import matplotlib.pyplot as plt

def draw_graph(graph, filename):

    # extract nodes from graph
    nodes = set([n1 for n1, n2, n3 in graph] + [n2 for n1, n2, n3 in graph])

    # create networkx graph
    G=nx.DiGraph()

    # add nodes
    for node in nodes:
        G.add_node(node)

    # add edges
    G.add_weighted_edges_from(graph)

    # draw graph
    pos = nx.spring_layout(G, k=0.9)
        # draw networkx
    nx.draw_networkx(G, pos, node_size=250, with_labels=True, \
                     width=0.5, font_size=9, linewidths=0.5)
        # draw edge labels
    edge_labels=dict([((u,v,),d['weight']) for u,v,d in G.edges(data=True)])
    nx.draw_networkx_edge_labels(G,pos,edge_labels=edge_labels)

for filename in os.listdir('../Input_Files'):
    # choose file
    if filename[:5] == "basic":
        graph = []
        f = open('../Input_Files/' + filename, 'r')
        f.readline()
        for line in f:
            graph.append([int(elt) for elt in tuple(line.split())])
        draw_graph(graph, filename)
        #plt.savefig(filename + '.png')
        plt.show()
        f.close()

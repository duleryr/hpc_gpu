/*
	Parser of the input files, unsigned shorto data structures that are the parameters of the different algorithms
*/


#ifndef PARSER_H
#define PARSER_H

# include <iostream>
# include <fstream>
# include <vector>
# include <limits>
# include <string>

#define infinity std::numeric_limits<unsigned short>::max()

class Parser
{
public:
	static std::vector<std::vector<unsigned short>> get_adjacency_matrix(std::string filename)
	{
		int from, to, cost;
		// number of vertices and edges
		int V, E;
		std::ifstream ifile(filename, std::ios::in);
		if (!ifile) {
			std::cerr << "Impossible d'ouvrir le fichier" << std::endl;
			exit(0);
		}
		ifile >> V >> E;

		std::vector<std::vector<unsigned short> > adjacency_matrix(V, std::vector<unsigned short>(V, infinity / 2 - 1));

		// Read edges from input file
		for (int i = 0; i < E; i++)
		{
			ifile >> from >> to >> cost;
			adjacency_matrix[from][to] = cost;
		}
		for (int i = 0; i < V; i++) {
			adjacency_matrix[i][i] = 0;
		}

		ifile.close();
		return adjacency_matrix;
	}
};

#endif

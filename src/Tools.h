#pragma once

#ifndef TOOLS_H
#define TOOLS_H

#include <omp.h>
#include <iostream>
#include <fstream>
#include <vector>

int evalNbThreads()
{
	int nbThreads;
#pragma omp parallel
	{
#pragma omp single
	{
		nbThreads = omp_get_num_threads();
	}
	}
	return nbThreads;
}

void print_matrix(std::vector<std::vector<unsigned short>> matrix)
{
	if (matrix.size() > 30) {
		std::cout << "Matrix too big to be printed" << std::endl;
	}
	else {
		for (int i = 0; i < (int) matrix.size(); i++) {
			for (int j = 0; j < (int) matrix[0].size(); j++) {
				printf("%*d ", 12, matrix[i][j]);
			}
			std::cout << std::endl;
		}
	}
}

std::string matrix_to_string(std::vector<std::vector<unsigned short>> matrix) {
	std::string stringmatrix = "";
	int V = matrix.size();
	for (int i = 0; i < V; i++) {
		for (int j = 0; j < V; j++) {
			if (matrix[i][j] > 32000) {
				stringmatrix += "inf";
			}
			else {
				stringmatrix += std::to_string(matrix[i][j]);
			}
			stringmatrix += " ";
		}
		stringmatrix.pop_back();
		stringmatrix += "\n";
	}
	stringmatrix.pop_back();
	return stringmatrix;
}

void save_in_file(std::vector<std::vector<unsigned short>> distance_matrix, std::string filename, std::string algorithm_used) {
	std::ofstream ofile("./Output_Files/" + filename + algorithm_used + ".txt", std::ios::out);
	std::string input = matrix_to_string(distance_matrix);
	ofile << input;
	ofile.close();
}

#endif // !TOOLS_H

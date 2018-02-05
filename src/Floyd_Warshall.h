#ifndef FLOYD_WARSHALL_H
#define FLOYD_WARSHALL_H

#pragma once
#include <vector>

// Basic Floyd_Warshall algorithm
std::vector<std::vector<unsigned short>> floyd_warshall_seq(std::vector<std::vector<unsigned short>> graph);

// Basic Floyd_Warshall algorithm with memoization
std::vector<std::vector<unsigned short>> floyd_warshall_seq_mem(std::vector<std::vector<unsigned short>> graph);

// Basic Floyd_Warshall algorithm with memoization and skip infinities
std::vector<std::vector<unsigned short>> floyd_warshall_seq_mem_skip(std::vector<std::vector<unsigned short>> graph);

// Parallel Floyd_Warshall algorithm with num_threads < num_nodes, strip-mined on columns
std::vector<std::vector<unsigned short>> floyd_warshall_par1_c(std::vector<std::vector<unsigned short>> graph, int step);

// Parallel Floyd_Warshall algorithm with num_threads < num_nodes, strip-mined on lines
std::vector<std::vector<unsigned short>> floyd_warshall_par1_l(std::vector<std::vector<unsigned short>> graph, int step);

// Tiled implementation of Floyd_Warshall algorithm
std::vector<std::vector<unsigned short>> floyd_warshall_seq_tiled(std::vector<std::vector<unsigned short>> graph, int step);

// Tiled parallel implementation of Floyd_Warshall algorithm
std::vector<std::vector<unsigned short>> floyd_warshall_par_tiled(std::vector<std::vector<unsigned short>> graph, int step);

// Tiled cache-aware implementation of Floyd_Warshall algorithm, with adapted memory layout
std::vector<std::vector<unsigned short>> floyd_warshall_seq_tiled_layout_memPos(std::vector<std::vector<unsigned short>> graph, int step);

// Tiled cache-aware implementation of Floyd_Warshall algorithm, with adapted memory layout
std::vector<std::vector<unsigned short>> floyd_warshall_seq_tiled_layout(std::vector<std::vector<unsigned short>> graph, int step);

// Tiled cache-aware implementation of Floyd_Warshall algorithm, with adapted memory layout
std::vector<std::vector<unsigned short>> floyd_warshall_seq_tiled_layout2(std::vector<std::vector<unsigned short>> graph, int step);

// Tiled parallel cache-aware implementation of Floyd_Warshall algorithm, with adapted memory layout
std::vector<std::vector<unsigned short>> floyd_warshall_par_tiled_layout(std::vector<std::vector<unsigned short>> graph, int step);

// Tiled parallel cache-aware implementation of Floyd_Warshall algorithm, with adapted memory layout
std::vector<std::vector<unsigned short>> floyd_warshall_seq_1d(std::vector<std::vector<unsigned short>> graph);

// Tiled parallel cache-aware implementation of Floyd_Warshall algorithm, with adapted memory layout
std::vector<std::vector<unsigned short>> floyd_warshall_par_1d(std::vector<std::vector<unsigned short>> graph, int numThreads);

// Tiled parallel cache-aware implementation of Floyd_Warshall algorithm, with adapted memory layout
std::vector<std::vector<unsigned short>> floyd_warshall_par_tiled_layout_memPos(std::vector<std::vector<unsigned short>> graph, int step);

// Parallel Floyd_Warshall algorithm with num_threads > num_nodes
std::vector<std::vector<unsigned short>> floyd_warshall_par2(std::vector<std::vector<unsigned short>> graph);

#endif

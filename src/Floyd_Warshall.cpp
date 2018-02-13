# include <iostream>
# include <fstream>
# include <string>
# include <vector>
# include <limits>
# include <omp.h>
# include <cstdlib>
# include <chrono>

#include "Floyd_Warshall.h"
#include "Bloc_Layout.h"
#include "cl.hpp"
#include "util.hpp"

#define MIN(a,b) ((a) < (b) ? (a) : (b))
// Definition of infinity in order to be able to sum 2 infinites without having to define a new class Infinity
#define infinity std::numeric_limits<unsigned short>::max()/2 - 1

#define DEBUG_GPU false

// Basic Floyd_Warshall algorithm
std::vector<std::vector<unsigned short>> floyd_warshall_seq(std::vector<std::vector<unsigned short>> graph)
{
    int i,j,k;
    unsigned short V = graph[0].size();
    for (k = 0; k < V; k++)
    {
        for (i = 0; i < V; i++)
        {
            for (j = 0; j < V; j++)
            {
                if (graph[i][j] > graph[i][k] + graph[k][j]) {
                    graph[i][j] = graph[i][k] + graph[k][j];
                }
            }
        }
    }

    /*
    // Check for negative cycles
    for (int i = 0; i < V; i++)
    {
    if (graph[i][i] != 0)
    {
    std::cout << "Negative cycle at : " << i << " because graph = " << graph[i][i] << std::endl;
    return 0;
    }
    }
    */

    return graph;
}

// Basic Floyd_Warshall algorithm with memoization
std::vector<std::vector<unsigned short>> floyd_warshall_seq_mem(std::vector<std::vector<unsigned short>> graph)
{
    int i,j,k;
    unsigned short V = graph[0].size();
    for (k = 0; k < V; k++)
    {
        for (i = 0; i < V; i++)
        {
            unsigned short distIK = graph[i][k];
            for (j = 0; j < V; j++)
            {
                if (graph[i][j] > distIK + graph[k][j]) {
                    graph[i][j] = distIK + graph[k][j];
                }
            }
        }
    }
    return graph;
}

// Basic Floyd_Warshall algorithm with memoization and skip infinities
std::vector<std::vector<unsigned short>> floyd_warshall_seq_mem_skip(std::vector<std::vector<unsigned short>> graph)
{
    int i,j,k;
    unsigned short V = graph[0].size();
    for (k = 0; k < V; k++)
    {
        for (i = 0; i < V; i++)
        {
            unsigned short distIK = graph[i][k];
            if (distIK == infinity)
                continue;
            for (j = 0; j < V; j++)
            {
                if (graph[i][j] > distIK + graph[k][j]) {
                    graph[i][j] = distIK + graph[k][j];
                }
            }
        }
    }
    return graph;
}

// Strip-mined 1D columns version of Floyd_Warshall algorithm
std::vector<std::vector<unsigned short>> floyd_warshall_par1_c(std::vector<std::vector<unsigned short>> graph, int step) {
    int i, j, k;
    int V = graph[0].size();
    for (k = 0; k < V; ++k)
    {
        int jj;
#pragma omp parallel for private(j) num_threads(4)
        for (j = 0; j < V; j += step)
        {
            for (i = 0; i < V; i++)
            {
                unsigned short distIK = graph[i][k];
                if (distIK == infinity)
                    continue;
                for (jj = j; jj < j + step; jj++)
                {
                    if (jj == V) 
                        break; 
                    if (graph[i][jj] > distIK + graph[k][jj]) {
                        graph[i][jj] = distIK + graph[k][jj];
                    }
                }
            }
        }
    }
    return graph;
}

// Strip-mined 1D lines version of Floyd_Warshall algorithm
std::vector<std::vector<unsigned short>> floyd_warshall_par1_l(std::vector<std::vector<unsigned short>> graph, int step) {
    int i, j, k;
    int V = graph[0].size();
    for (k = 0; k < V; ++k)
    {
        int ii;
#pragma omp parallel for private(i) num_threads(4)
        for (i = 0; i < V; i += step)
        {
            for (j = 0; j < V; j++)
            {
                unsigned short distKJ = graph[k][j];
                if (distKJ == infinity)
                    continue;
                for (ii = i; ii < i + step; ii++)
                {
                    if (ii == V) 
                        break; 
                    if (graph[ii][j] > graph[ii][k] + distKJ) {
                        graph[ii][j] = graph[ii][k] + distKJ;
                    }
                }
            }
        }
    }
    return graph;
}

// Tiled implementation of Floyd_Warshall algorithm,
// step = bloc width
std::vector<std::vector<unsigned short>> floyd_warshall_seq_tiled(std::vector<std::vector<unsigned short>> graph, int step) {
    int V = graph[0].size();
    int i,j;
    for (int kmin = 0; kmin < V; kmin += step)
    {
        int kmax = MIN(V, kmin + step);
        for (int k = kmin; k < kmax; k++) {
            // Bloc (k,k)
            for (i = kmin; i < kmax; i++) {
                unsigned short distIK = graph[i][k];
                if (distIK == infinity)
                    continue;
                for (j = kmin; j < kmax; j++) {
                    if (graph[i][j] > distIK + graph[k][j]) {
                        graph[i][j] = distIK + graph[k][j];
                    }
                }
            }
        }
        for (int k = kmin; k < kmax; k++) {
            // Line (k,j)
            for (i = kmin; i < kmax; i++) {
                unsigned short distIK = graph[i][k];
                if (distIK == infinity)
                    continue;
                for (j = 0; j < kmin; j++) {
                    if (graph[i][j] > distIK + graph[k][j]) {
                        graph[i][j] = distIK + graph[k][j];
                    }
                }
                for (j = kmax; j < V; j++) {
                    if (graph[i][j] > distIK + graph[k][j]) {
                        graph[i][j] = distIK + graph[k][j];
                    }
                }
            }
            // Column (i,k)
            for (j = kmin; j < kmax; j++) {
                unsigned short distKJ = graph[k][j];
                if (distKJ == infinity)
                    continue;
                for (i = 0; i < kmin; i++) {
                    if (graph[i][j] > graph[i][k] + distKJ) {
                        graph[i][j] = graph[i][k] + distKJ;
                    }
                }
                for (i = kmax; i < V; i++) {
                    if (graph[i][j] > graph[i][k] + distKJ) {
                        graph[i][j] = graph[i][k] + distKJ;
                    }
                }
            }
        }
        for (int k = kmin; k < kmax; k++) {
            // Other blocs
            for (i = 0; i < kmin; i++) {
                unsigned short distIK = graph[i][k];
                if (distIK == infinity)
                    continue;
                for (j = 0; j < kmin; j++) {
                    if (graph[i][j] > distIK + graph[k][j]) {
                        graph[i][j] = distIK + graph[k][j];
                    }
                }
                for (j = kmax; j < V; j++) {
                    if (graph[i][j] > distIK + graph[k][j]) {
                        graph[i][j] = distIK + graph[k][j];
                    }
                }
            }
            for (i = kmax; i < V; i++) {
                unsigned short distIK = graph[i][k];
                if (distIK == infinity)
                    continue;
                for (j = 0; j < kmin; j++) {
                    if (graph[i][j] > distIK + graph[k][j]) {
                        graph[i][j] = distIK + graph[k][j];
                    }
                }
                for (j = kmax; j < V; j++) {
                    if (graph[i][j] > distIK + graph[k][j]) {
                        graph[i][j] = distIK + graph[k][j];
                    }
                }
            }
        }
    }
    return graph;
}

// Tiled parallel implementation of Floyd_Warshall algorithm,
// step = bloc width
std::vector<std::vector<unsigned short>> floyd_warshall_par_tiled(std::vector<std::vector<unsigned short>> graph, int step) {
    int V = graph[0].size();
    int i,j;
    for (int kmin = 0; kmin < V; kmin += step)
    {
        int kmax = MIN(V, kmin + step);
        for (int k = kmin; k < kmax; k++) {
            // Bloc (k,k)
#pragma omp parallel for private(i,j)
            for (i = kmin; i < kmax; i++) {
                unsigned short distIK = graph[i][k];
                if (distIK == infinity)
                    continue;
                for (j = kmin; j < kmax; j++) {
                    if (graph[i][j] > distIK + graph[k][j]) {
                        graph[i][j] = distIK + graph[k][j];
                    }
                }
            }
        }
        for (int k = kmin; k < kmax; k++) {
            // Line (k,j)
#pragma omp parallel for private(i,j)
            for (i = kmin; i < kmax; i++) {
                unsigned short distIK = graph[i][k];
                if (distIK == infinity)
                    continue;
                for (j = 0; j < kmin; j++) {
                    if (graph[i][j] > distIK + graph[k][j]) {
                        graph[i][j] = distIK + graph[k][j];
                    }
                }
                for (j = kmax; j < V; j++) {
                    if (graph[i][j] > distIK + graph[k][j]) {
                        graph[i][j] = distIK + graph[k][j];
                    }
                }
            }
            // Column (i,k)
#pragma omp parallel for private(i,j)
            for (j = kmin; j < kmax; j++) {
                unsigned short distKJ = graph[k][j];
                if (distKJ == infinity)
                    continue;
                for (i = 0; i < kmin; i++) {
                    if (graph[i][j] > graph[i][k] + graph[k][j]) {
                        graph[i][j] = graph[i][k] + graph[k][j];
                    }
                }
                for (i = kmax; i < V; i++) {
                    if (graph[i][j] > graph[i][k] + graph[k][j]) {
                        graph[i][j] = graph[i][k] + graph[k][j];
                    }
                }
            }
        }
        for (int k = kmin; k < kmax; k++) {
            // Other blocs
#pragma omp parallel for private(i,j)
            for (i = 0; i < kmin; i++) {
                unsigned short distIK = graph[i][k];
                if (distIK == infinity)
                    continue;
                for (j = 0; j < kmin; j++) {
                    if (graph[i][j] > distIK + graph[k][j]) {
                        graph[i][j] = distIK + graph[k][j];
                    }
                }
                for (j = kmax; j < V; j++) {
                    if (graph[i][j] > distIK + graph[k][j]) {
                        graph[i][j] = distIK + graph[k][j];
                    }
                }
            }
#pragma omp parallel for private(i,j)
            for (i = kmax; i < V; i++) {
                unsigned short distIK = graph[i][k];
                if (distIK == infinity)
                    continue;
                for (j = 0; j < kmin; j++) {
                    if (graph[i][j] > distIK + graph[k][j]) {
                        graph[i][j] = distIK + graph[k][j];
                    }
                }
                for (j = kmax; j < V; j++) {
                    if (graph[i][j] > distIK + graph[k][j]) {
                        graph[i][j] = distIK + graph[k][j];
                    }
                }
            }
        }
    }
    return graph;
}

// Tiled implementation of Floyd_Warshall algorithm, with adapted memory layout in blocs
// step = bloc width
std::vector<std::vector<unsigned short>> floyd_warshall_seq_tiled_layout_memPos(std::vector<std::vector<unsigned short>> adjacency_matrix, int step) {
    int V = adjacency_matrix[0].size();
    std::vector<std::vector<unsigned short>> graph = graphToBlocLayout(adjacency_matrix, step, true);
    std::vector<std::vector<std::pair<int,int>>> memPos(V, std::vector<std::pair<int,int>>(V));
    int i, j, k;

    for (i = 0; i < V; i++) {
        for (j = 0; j < V; j++) {
            memPos[i][j] = posToBlocPos(i, j, V / step, step, true);
        }
    }

    for (int kmin = 0; kmin < V; kmin += step)
    {
        int kmax = MIN(V, kmin + step);
        for (k = kmin; k < kmax; k++) {
            // Bloc (k,k)
            for (i = kmin; i < kmax; i++) {
                for (j = kmin; j < kmax; j++) {
                    if (graph[memPos[i][j].first][memPos[i][j].second] > graph[memPos[i][k].first][memPos[i][k].second] + graph[memPos[k][j].first][memPos[k][j].second])
                    {
                        graph[memPos[i][j].first][memPos[i][j].second] = graph[memPos[i][k].first][memPos[i][k].second] + graph[memPos[k][j].first][memPos[k][j].second];
                    }
                }
            }
        }
        for (k = kmin; k < kmax; k++) {
            // Line (k,j)
            for (i = kmin; i < kmax; i++) {
                for (j = 0; j < kmin; j++) {
                    if (graph[memPos[i][j].first][memPos[i][j].second] > graph[memPos[i][k].first][memPos[i][k].second] + graph[memPos[k][j].first][memPos[k][j].second])
                    {
                        graph[memPos[i][j].first][memPos[i][j].second] = graph[memPos[i][k].first][memPos[i][k].second] + graph[memPos[k][j].first][memPos[k][j].second];
                    }
                }
                for (j = kmax; j < V; j++) {
                    if (graph[memPos[i][j].first][memPos[i][j].second] > graph[memPos[i][k].first][memPos[i][k].second] + graph[memPos[k][j].first][memPos[k][j].second])
                    {
                        graph[memPos[i][j].first][memPos[i][j].second] = graph[memPos[i][k].first][memPos[i][k].second] + graph[memPos[k][j].first][memPos[k][j].second];
                    }
                }
            }
            // Column (i,k)
            for (j = kmin; j < kmax; j++) {
                for (i = 0; i < kmin; i++) {
                    if (graph[memPos[i][j].first][memPos[i][j].second] > graph[memPos[i][k].first][memPos[i][k].second] + graph[memPos[k][j].first][memPos[k][j].second])
                    {
                        graph[memPos[i][j].first][memPos[i][j].second] = graph[memPos[i][k].first][memPos[i][k].second] + graph[memPos[k][j].first][memPos[k][j].second];
                    }
                }
                for (i = kmax; i < V; i++) {
                    if (graph[memPos[i][j].first][memPos[i][j].second] > graph[memPos[i][k].first][memPos[i][k].second] + graph[memPos[k][j].first][memPos[k][j].second])
                    {
                        graph[memPos[i][j].first][memPos[i][j].second] = graph[memPos[i][k].first][memPos[i][k].second] + graph[memPos[k][j].first][memPos[k][j].second];
                    }
                }
            }
        }
        for (k = kmin; k < kmax; k++) {
            // Other blocs
            for (i = 0; i < kmin; i++) {
                for (j = 0; j < kmin; j++) {
                    if (graph[memPos[i][j].first][memPos[i][j].second] > graph[memPos[i][k].first][memPos[i][k].second] + graph[memPos[k][j].first][memPos[k][j].second])
                    {
                        graph[memPos[i][j].first][memPos[i][j].second] = graph[memPos[i][k].first][memPos[i][k].second] + graph[memPos[k][j].first][memPos[k][j].second];
                    }
                }
                for (j = kmax; j < V; j++) {
                    if (graph[memPos[i][j].first][memPos[i][j].second] > graph[memPos[i][k].first][memPos[i][k].second] + graph[memPos[k][j].first][memPos[k][j].second])
                    {
                        graph[memPos[i][j].first][memPos[i][j].second] = graph[memPos[i][k].first][memPos[i][k].second] + graph[memPos[k][j].first][memPos[k][j].second];
                    }
                }
            }
            for (i = kmax; i < V; i++) {
                for (j = 0; j < kmin; j++) {
                    if (graph[memPos[i][j].first][memPos[i][j].second] > graph[memPos[i][k].first][memPos[i][k].second] + graph[memPos[k][j].first][memPos[k][j].second])
                    {
                        graph[memPos[i][j].first][memPos[i][j].second] = graph[memPos[i][k].first][memPos[i][k].second] + graph[memPos[k][j].first][memPos[k][j].second];
                    }
                }
                for (j = kmax; j < V; j++) {
                    if (graph[memPos[i][j].first][memPos[i][j].second] > graph[memPos[i][k].first][memPos[i][k].second] + graph[memPos[k][j].first][memPos[k][j].second])
                    {
                        graph[memPos[i][j].first][memPos[i][j].second] = graph[memPos[i][k].first][memPos[i][k].second] + graph[memPos[k][j].first][memPos[k][j].second];
                    }
                }
            }
        }
    }
    return blocLayoutToGraph(graph, V, step, true);
}

// Tiled implementation of Floyd_Warshall algorithm, with adapted memory layout in blocs
// blocSize1D = bloc width
std::vector<std::vector<unsigned short>> floyd_warshall_seq_tiled_layout(std::vector<std::vector<unsigned short>> adjacency_matrix, int blocSize1D) {
    int V = adjacency_matrix[0].size();
    int blocsPerWidth = V / blocSize1D;
    std::vector<std::vector<unsigned short>> graph = graphToBlocLayout(adjacency_matrix, blocSize1D, true);
    int i, ii, j, jj, k;

    for (int bloc = 0; bloc < blocsPerWidth; bloc++) {
        int numQuad = bloc * (1 + blocsPerWidth);
        for (k = 0; k < blocSize1D; k++) {
            // Bloc (k,k)
            for (i = 0; i < blocSize1D; i++) {
                int distIK = graph[numQuad][i*blocSize1D + k];
                if (distIK == infinity)
                    continue;
                for (j = 0; j < blocSize1D; j++) {
                    if (graph[numQuad][i*blocSize1D + j] > distIK + graph[numQuad][k*blocSize1D + j])
                    {
                        graph[numQuad][i*blocSize1D + j] = distIK + graph[numQuad][k*blocSize1D + j];
                    }
                }
            }
        }
        int numQuadLeft = bloc * blocsPerWidth;
        for (k = 0; k < blocSize1D; k++) {
            // Line (k,j)
            for (i = 0; i < blocSize1D; i++) {
                int distIK = graph[numQuad][i*blocSize1D + k];
                if (distIK == infinity)
                    continue;
                for (j = 0; j < blocsPerWidth; j++) {
                    if (j == bloc)
                        continue;
                    int numCurrentQuad = numQuadLeft + j;
                    for (jj = 0; jj < blocSize1D; jj++) {
                        if (graph[numCurrentQuad][i*blocSize1D+jj] > distIK + graph[numCurrentQuad][k*blocSize1D+jj])
                        {                
                            graph[numCurrentQuad][i*blocSize1D+jj] = distIK + graph[numCurrentQuad][k*blocSize1D+jj];
                        }
                    }
                }
            }
            // Column (i,k)
            for (i = 0; i < blocsPerWidth; i++) {
                if (i == bloc)
                    continue;
                int numCurrentQuad = bloc + i * blocsPerWidth;
                for (ii = 0; ii < blocSize1D; ii++) {
                    int distIK = graph[numCurrentQuad][ii*blocSize1D + k];
                    if (distIK == infinity)
                        continue;
                    for (j = 0; j < blocSize1D; j++) {
                        if (graph[numCurrentQuad][ii*blocSize1D+j] > distIK + graph[numQuad][k*blocSize1D+j])
                        {
                            graph[numCurrentQuad][ii*blocSize1D+j] = distIK + graph[numQuad][k*blocSize1D+j];
                        }
                    }
                }
            }
        }
        for (k = 0; k < blocSize1D; k++) {
            // Other blocs
            for (i = 0; i < blocsPerWidth; i++) {
                if (i == bloc)
                    continue;
                int numQuadIK = i * blocsPerWidth + bloc;
                for (ii = 0; ii < blocSize1D; ii++) {
                    int distIK = graph[numQuadIK][ii*blocSize1D + k];
                    if (distIK == infinity)
                        continue;
                    for (j = 0; j < blocsPerWidth; j++) {
                        if (j == bloc) {
                            continue;
                        }
                        int numQuadIJ = i * blocsPerWidth + j;
                        int numQuadKJ = bloc * blocsPerWidth + j;
                        for (jj = 0; jj < blocSize1D; jj++) {
                            if (graph[numQuadIJ][ii*blocSize1D+jj] > distIK + graph[numQuadKJ][k*blocSize1D+jj])
                            {
                                graph[numQuadIJ][ii*blocSize1D+jj] = distIK + graph[numQuadKJ][k*blocSize1D+jj];
                            }
                        }
                    }
                }
            }
        }
    }
    return blocLayoutToGraph(graph, V, blocSize1D, true);
}

// Tiled parallel implementation of Floyd_Warshall algorithm, with adapted memory layout in blocs
// blocSize1D = bloc width
std::vector<std::vector<unsigned short>> floyd_warshall_par_tiled_layout(std::vector<std::vector<unsigned short>> adjacency_matrix, int blocSize1D) {
    int V = adjacency_matrix[0].size();
    int blocsPerWidth = V / blocSize1D;
    std::vector<std::vector<unsigned short>> graph = graphToBlocLayout(adjacency_matrix, blocSize1D, true);
    int i, ii, j, jj, k;

    for (int bloc = 0; bloc < blocsPerWidth; bloc++) {
        int numQuad = bloc * (1 + blocsPerWidth);
        for (k = 0; k < blocSize1D; k++) {
#pragma omp parallel for private(i,j) num_threads(4)
            // Bloc (k,k)
            for (i = 0; i < blocSize1D; i++) {
                int distIK = graph[numQuad][i*blocSize1D + k];
                if (distIK == infinity)
                    continue;
                for (j = 0; j < blocSize1D; j++) {
                    if (graph[numQuad][i*blocSize1D + j] > distIK + graph[numQuad][k*blocSize1D + j])
                    {
                        graph[numQuad][i*blocSize1D + j] = distIK + graph[numQuad][k*blocSize1D + j];
                    }
                }
            }
        }
        int numQuadLeft = bloc * blocsPerWidth;
        for (k = 0; k < blocSize1D; k++) {
#pragma omp parallel for private(i,j) num_threads(4)
            // Line (k,j)
            for (i = 0; i < blocSize1D; i++) {
                unsigned short distIK = graph[numQuad][i*blocSize1D+k];
                if (distIK == infinity)
                    continue;
                for (j = 0; j < blocsPerWidth; j++) {
                    int numCurrentQuad = numQuadLeft + j;
                    if (j == bloc)
                        continue;
                    for (jj = 0; jj < blocSize1D; jj++) {
                        if (graph[numCurrentQuad][i*blocSize1D+jj] > distIK + graph[numCurrentQuad][k*blocSize1D+jj])
                        {                
                            graph[numCurrentQuad][i*blocSize1D+jj] = distIK + graph[numCurrentQuad][k*blocSize1D+jj];
                        }
                    }
                }
            }
#pragma omp parallel for private(i,j) num_threads(4)
            // Column (i,k)
            for (i = 0; i < blocsPerWidth; i++) {
                int numCurrentQuad = bloc + i * blocsPerWidth;
                if (i == bloc)
                    continue;
                for (ii = 0; ii < blocSize1D; ii++) {
                    unsigned short distIK = graph[numCurrentQuad][ii*blocSize1D+k];
                    if (distIK == infinity)
                        continue;
                    for (j = 0; j < blocSize1D; j++) {
                        if (graph[numCurrentQuad][ii*blocSize1D+j] > distIK + graph[numQuad][k*blocSize1D+j])
                        {
                            graph[numCurrentQuad][ii*blocSize1D+j] = distIK + graph[numQuad][k*blocSize1D+j];
                        }
                    }
                }
            }
        }
        for (k = 0; k < blocSize1D; k++) {
            // Other blocs
#pragma omp parallel for private(i,j) num_threads(4)
            for (i = 0; i < blocsPerWidth; i++) {
                if (i == bloc)
                    continue;
                for (ii = 0; ii < blocSize1D; ii++) {
                    unsigned short distIK = graph[i*blocsPerWidth + bloc][ii*blocSize1D+k];
                    if (distIK == infinity)
                        continue;
                    for (j = 0; j < blocsPerWidth; j++) {
                        int numQuadIJ = i * blocsPerWidth + j;
                        int numQuadKJ = bloc * blocsPerWidth + j;
                        if (j == bloc)
                            continue;
                        for (jj = 0; jj < blocSize1D; jj++) {
                            if (graph[numQuadIJ][ii*blocSize1D+jj] > distIK + graph[numQuadKJ][k*blocSize1D+jj])
                            {
                                graph[numQuadIJ][ii*blocSize1D+jj] = distIK + graph[numQuadKJ][k*blocSize1D+jj];
                            }
                        }
                    }
                }
            }
        }
    }
    return blocLayoutToGraph(graph, V, blocSize1D, true);
}

// Tiled parallel implementation of Floyd_Warshall algorithm, with a 1 dimension matrix
std::vector<std::vector<unsigned short>> floyd_warshall_seq_1d(std::vector<std::vector<unsigned short>> adjacency_matrix) {
    int V = adjacency_matrix[0].size();
    std::vector<unsigned short> graph(V*V);
    int i, j, k;
    for (i = 0; i < V; i++) {
        for (j = 0; j < V; j++) {
            graph[i * V + j] = adjacency_matrix[i][j];
        }
    }

    for (k = 0; k < V; k++) {
        // Bloc (k,k)
        for (i = 0; i < V; i++) {
            int distIK = graph[i*V + k];
            if (distIK == infinity)
                continue;
            for (j = 0; j < V; j++) {
                if (graph[i*V + j] > distIK + graph[k*V + j])
                {
                    graph[i*V + j] = distIK + graph[k*V + j];
                }
            }
        }
    }

    std::vector<std::vector<unsigned short>> output_matrix(V, std::vector<unsigned short>(V));
    for (i = 0; i < V*V; i++) {
        output_matrix[i / V][i % V] = graph[i];
    }

    return output_matrix;
}

// parallel implementation of Floyd_Marshall algorithm, with a 1 dimension matrix with OpenCL
std::vector<std::vector<unsigned short>> floyd_warshall_GPU(std::vector<std::vector<unsigned short>> adjacency_matrix) {
    int V = adjacency_matrix[0].size();
    std::vector<unsigned short> h_graph(V*V);
    int i, j, k;
    for (i = 0; i < V; i++) {
        for (j = 0; j < V; j++) {
            h_graph[i * V + j] = adjacency_matrix[i][j];
        }
    }

#if DEBUG_GPU
    for (i = 0; i < V*V; i++) {
	std::cout << h_graph[i] << " ";
    }
    std::cout << std::endl;
#endif

    // OpenCL part
    util::Timer timer;
    auto begin = std::chrono::high_resolution_clock::now();

    std::string kernel_program(util::loadProgram("src/floyd_warshall.cl"));
    // Create a context
    cl::Context context(DEVICE);
    // Load in kernel source, creating a program object for the context
    cl::Program program(context,kernel_program,true);
    // Get the command queue
    cl::CommandQueue queue(context);
    // Create the kernel functor
    auto floyd_cl = cl::make_kernel<cl::Buffer, 
	  int, int>(program, "floyd_warshall");

    cl::Buffer d_graph;

    d_graph = cl::Buffer(context, std::begin(h_graph), std::end(h_graph), true);

    for (k = 0; k < V; k++) {
	floyd_cl(
		cl::EnqueueArgs(
		    queue,
		    cl::NDRange(V*V)),
		d_graph,
		V,
		k);
	queue.finish();
    }

    cl::copy(queue, d_graph, std::begin(h_graph), std::end(h_graph));

    std::vector<std::vector<unsigned short>> output_matrix(V, std::vector<unsigned short>(V));
#if DEBUG_GPU
    for (i = 0; i < V*V; i++) {
        output_matrix[i / V][i % V] = h_graph[i];
	std::cout << h_graph[i] << " ";
    }
    std::cout << std::endl;
#endif

    return output_matrix;
}


// parallel implementation of Floyd_Marshall algorithm, with a 1 dimension matrix
std::vector<std::vector<unsigned short>> floyd_warshall_par_1d(std::vector<std::vector<unsigned short>> adjacency_matrix, int numThreads) {
    int V = adjacency_matrix[0].size();
    std::vector<unsigned short> graph(V*V);
    int i, j, k;
    for (i = 0; i < V; i++) {
        for (j = 0; j < V; j++) {
            graph[i * V + j] = adjacency_matrix[i][j];
        }
    }

    for (k = 0; k < V; k++) {
#pragma omp parallel for private(i,j) num_threads(numThreads)
        // Bloc (k,k)
        for (i = 0; i < V ; i++) {
            int distIK = graph[i*V + k];
            if (distIK == infinity)
                continue;
            for (j = 0; j < V; j++) {
                if (graph[i*V + j] > distIK + graph[k*V + j])
                {
                    graph[i*V + j] = distIK + graph[k*V + j];
                }
            }
        }
    }

    std::vector<std::vector<unsigned short>> output_matrix(V, std::vector<unsigned short>(V));
    for (i = 0; i < V*V; i++) {
        output_matrix[i / V][i % V] = graph[i];
    }

    return output_matrix;
}

// Tiled parallel implementation of Floyd_Warshall algorithm, with adapted memory layout in blocs
// step = bloc width
std::vector<std::vector<unsigned short>> floyd_warshall_par_tiled_layout_memPos(std::vector<std::vector<unsigned short>> adjacency_matrix, int step) {
    int V = adjacency_matrix[0].size();
    std::vector<std::vector<unsigned short>> graph = graphToBlocLayout(adjacency_matrix, step, true);
    std::vector<std::vector<std::pair<int,int>>> memPos(V, std::vector<std::pair<int,int>>(V));
    int i, j, k;

    for (i = 0; i < V; i++) {
        for (j = 0; j < V; j++) {
            memPos[i][j] = posToBlocPos(i, j, V / step, step, true);
        }
    }

    for (int kmin = 0; kmin < V; kmin += step)
    {
        int kmax = MIN(V, kmin + step);
        for (k = kmin; k < kmax; k++) {
            // Bloc (k,k)
#pragma omp parallel for private(i,j) num_threads(4)
            for (i = kmin; i < kmax; i++) {
                for (j = kmin; j < kmax; j++) {
                    if (graph[memPos[i][j].first][memPos[i][j].second] > graph[memPos[i][k].first][memPos[i][k].second] + graph[memPos[k][j].first][memPos[k][j].second])
                    {
                        graph[memPos[i][j].first][memPos[i][j].second] = graph[memPos[i][k].first][memPos[i][k].second] + graph[memPos[k][j].first][memPos[k][j].second];
                    }
                }
            }
        }
        for (k = kmin; k < kmax; k++) {
#pragma omp parallel for private(i,j) num_threads(4)
            // Line (k,j)
            for (i = kmin; i < kmax; i++) {
                for (j = 0; j < kmin; j++) {
                    if (graph[memPos[i][j].first][memPos[i][j].second] > graph[memPos[i][k].first][memPos[i][k].second] + graph[memPos[k][j].first][memPos[k][j].second])
                    {
                        graph[memPos[i][j].first][memPos[i][j].second] = graph[memPos[i][k].first][memPos[i][k].second] + graph[memPos[k][j].first][memPos[k][j].second];
                    }
                }
                for (j = kmax; j < V; j++) {
                    if (graph[memPos[i][j].first][memPos[i][j].second] > graph[memPos[i][k].first][memPos[i][k].second] + graph[memPos[k][j].first][memPos[k][j].second])
                    {
                        graph[memPos[i][j].first][memPos[i][j].second] = graph[memPos[i][k].first][memPos[i][k].second] + graph[memPos[k][j].first][memPos[k][j].second];
                    }
                }
            }
#pragma omp parallel for private(i,j) num_threads(4)
            // Column (i,k)
            for (j = kmin; j < kmax; j++) {
                for (i = 0; i < kmin; i++) {
                    if (graph[memPos[i][j].first][memPos[i][j].second] > graph[memPos[i][k].first][memPos[i][k].second] + graph[memPos[k][j].first][memPos[k][j].second])
                    {
                        graph[memPos[i][j].first][memPos[i][j].second] = graph[memPos[i][k].first][memPos[i][k].second] + graph[memPos[k][j].first][memPos[k][j].second];
                    }
                }
                for (i = kmax; i < V; i++) {
                    if (graph[memPos[i][j].first][memPos[i][j].second] > graph[memPos[i][k].first][memPos[i][k].second] + graph[memPos[k][j].first][memPos[k][j].second])
                    {
                        graph[memPos[i][j].first][memPos[i][j].second] = graph[memPos[i][k].first][memPos[i][k].second] + graph[memPos[k][j].first][memPos[k][j].second];
                    }
                }
            }
        }
        for (k = kmin; k < kmax; k++) {
#pragma omp parallel for private(i,j) num_threads(4)
            // Other blocs
            for (i = 0; i < kmin; i++) {
                for (j = 0; j < kmin; j++) {
                    if (graph[memPos[i][j].first][memPos[i][j].second] > graph[memPos[i][k].first][memPos[i][k].second] + graph[memPos[k][j].first][memPos[k][j].second])
                    {
                        graph[memPos[i][j].first][memPos[i][j].second] = graph[memPos[i][k].first][memPos[i][k].second] + graph[memPos[k][j].first][memPos[k][j].second];
                    }
                }
                for (j = kmax; j < V; j++) {
                    if (graph[memPos[i][j].first][memPos[i][j].second] > graph[memPos[i][k].first][memPos[i][k].second] + graph[memPos[k][j].first][memPos[k][j].second])
                    {
                        graph[memPos[i][j].first][memPos[i][j].second] = graph[memPos[i][k].first][memPos[i][k].second] + graph[memPos[k][j].first][memPos[k][j].second];
                    }
                }
            }
#pragma omp parallel for private(i,j) num_threads(4)
            for (i = kmax; i < V; i++) {
                for (j = 0; j < kmin; j++) {
                    if (graph[memPos[i][j].first][memPos[i][j].second] > graph[memPos[i][k].first][memPos[i][k].second] + graph[memPos[k][j].first][memPos[k][j].second])
                    {
                        graph[memPos[i][j].first][memPos[i][j].second] = graph[memPos[i][k].first][memPos[i][k].second] + graph[memPos[k][j].first][memPos[k][j].second];
                    }
                }
                for (j = kmax; j < V; j++) {
                    if (graph[memPos[i][j].first][memPos[i][j].second] > graph[memPos[i][k].first][memPos[i][k].second] + graph[memPos[k][j].first][memPos[k][j].second])
                    {
                        graph[memPos[i][j].first][memPos[i][j].second] = graph[memPos[i][k].first][memPos[i][k].second] + graph[memPos[k][j].first][memPos[k][j].second];
                    }
                }
            }
        }
    }
    return blocLayoutToGraph(graph, V, step, true);
}

// Strip-mined 2D version of Floyd_Warshall algorithm
std::vector<std::vector<unsigned short>> floyd_warshall_par2(std::vector<std::vector<unsigned short>> graph) {
    int i, j, k;
    int V = graph[0].size();
    int step = V;
    for (int p = 2; p < 5; p++) {
        if (V % p == 0)
            step = p;
    }
    for (k = 0; k < V; ++k)
    {
#pragma omp parallel for
        for (j = 0; j < V; j += step)
        {
#pragma omp parallel for
            for (i = 0; i < V; i += step)
            {
                for (int jj = j; jj < j + step; jj++)
                {
                    for (int ii = i; ii < i + step; ii++)
                    {
                        if (graph[ii][jj] > graph[ii][k] + graph[k][jj])
                        {
                            graph[ii][jj] = graph[ii][k] + graph[k][jj];
                        }
                    }
                }
            }
        }
    }
    return graph;
}

/*
 * In order to minimize the number of cache defaults, this file provides a memory layout in bloc for 2D vector
 */

#pragma once

#ifndef BLOC_LAYOUT_H
#define BLOC_LAYOUT_H

#include <vector>
#include <utility>


/* Examples :
 * INPUT : (1, 1)
 * OUTPUT : (0, blocSize1D+1) 
 * INPUT : (0, blocSize1D)
 * OUTPUT : (1, 0) */
std::pair<int,int> posToBlocPos(int i, int j, int blocsPerWidth, int blocSize1D, bool reversed) {
    std::pair<int,int> blocPos;
    int numquadI, numquadJ, numquad;
    int iQuad, jQuad, posInQuad;

    // Quadrant matrix Scale
    numquadI = i / blocSize1D;
    numquadJ = j / blocSize1D;
    numquad = numquadI * blocsPerWidth + numquadJ;

    // In-quadrant scale
    iQuad = i % blocSize1D;
    jQuad = j % blocSize1D;
    posInQuad = iQuad * blocSize1D + jQuad;

    if (reversed)
        blocPos = std::make_pair(numquad, posInQuad);
    else
        blocPos = std::make_pair(posInQuad, numquad);

    return blocPos;
}

std::pair<int,int> blocPosToPos(int n, int p, int blocsPerWidth, int blocSize1D, bool reversed) {
    int i, j;

    if (!reversed) {
        int c = n;
        n = p;
        p = c;
    }

    // Quadrant matrix scale
    i = (n / blocsPerWidth) * blocSize1D;
    j = (n % blocsPerWidth) * blocSize1D;
    // In-bloc scale
    i += p / blocSize1D;
    j += p % blocSize1D;

    return std::make_pair(i,j);
}

/* Example
 * INPUT :
 * 0  1  2  3              (0,0) (0,1) (0,2) (0,3)
 * 4  5  6  7              (1,0) (1,1) (1,2) (1,3)
 * 8  9  10 11             (2,0) (2,1) (2,2) (2,3)
 * 12 13 14 15             (3,0) (3,1) (3,2) (3,3)
 *
 * OUTPUT with blocSize1D = 2
 *
 * 0  1 |4  5              (0,0) (0,1) (1,0) (1,1)
 * 2__3_|6__7_             (0,2) (0,3) (1,2) (1,3)
 * 8  9 |12 13             (2,0) (2,1) (3,0) (3,1)
 * 10 11|14 15             (2,2) (2,3) (3,2) (3,3) */         
std::vector<std::vector<unsigned short>> graphToBlocLayout(std::vector<std::vector<unsigned short>> graph, int blocSize1D, bool reversed) {
    int V = graph[0].size();
    if (!reversed) {
        std::vector<std::vector<unsigned short>> blocLayout(blocSize1D*blocSize1D, std::vector<unsigned short>((V*V)/(blocSize1D*blocSize1D)));
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                std::pair<int,int> blocPos = posToBlocPos(i, j, V/blocSize1D, blocSize1D, reversed);
                blocLayout[blocPos.first][blocPos.second] = graph[i][j];
            }
        }
        return blocLayout;
    } else {
        std::vector<std::vector<unsigned short>> blocLayout((V*V)/(blocSize1D*blocSize1D), std::vector<unsigned short>(blocSize1D*blocSize1D));
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                std::pair<int,int> blocPos = posToBlocPos(i, j, V/blocSize1D, blocSize1D, reversed);
                blocLayout[blocPos.first][blocPos.second] = graph[i][j];
            }
        }
        return blocLayout;
    }
}

/* Reverse function of graphToBlocLayout */
std::vector<std::vector<unsigned short>> blocLayoutToGraph(std::vector<std::vector<unsigned short>> blocLayout, int V, int blocSize1D, bool reversed) {
    if (reversed) {
        int blocsPerWidth = V / blocSize1D;
        int blocsNumber = blocLayout.size();
        std::vector<std::vector<unsigned short>> graph(V, std::vector<unsigned short>(V));
        for (int n = 0; n < blocsNumber; n++) {
            for (int p = 0; p < blocSize1D*blocSize1D; p++) {
                std::pair<int,int> pos = blocPosToPos(n, p, blocsPerWidth, blocSize1D, reversed);
                graph[pos.first][pos.second] = blocLayout[n][p];
            }
        }
        return graph;
    } else {
        int blocsPerWidth = V / blocSize1D;
        int blocsNumber = blocLayout[0].size();
        std::vector<std::vector<unsigned short>> graph(V, std::vector<unsigned short>(V));
        for (int p = 0; p < blocsNumber; p++) {
            for (int n = 0; n < blocSize1D*blocSize1D; n++) {
                std::pair<int,int> pos = blocPosToPos(n, p, blocsPerWidth, blocSize1D, reversed);
                graph[pos.first][pos.second] = blocLayout[n][p];
            }
        }
        return graph;
    }
}

#endif //!BLOC_LAYOUT_H

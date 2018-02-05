#ifndef DIJSKTRA_H
#define DIJSKTRA_H

#pragma once
#include "Edge.h"
#include <vector>

// Basic Dijkstra algorithm
int dijkstra_seq(const std::vector<std::vector<Edge>> &graph, int source, int target);

#endif

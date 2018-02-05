#include "Edge.h"
#include "Dijsktra.h"

#include <vector>
#include <utility>
#include <iostream>
#include <set>
#include <omp.h>
#include <limits>

#define infinity std::numeric_limits<int>::max()

using namespace std;

int dijkstra_seq(const vector<vector<Edge> > & graph, int source, int target) {
	vector<int> min_distance(graph.size(), infinity);
	min_distance[source] = 0;
	set< pair<int, int> > active_vertices;
	active_vertices.insert({ 0,source });
	while (!active_vertices.empty()) {
		int where = active_vertices.begin()->second;
		if (where == target) return min_distance[where];
		active_vertices.erase(active_vertices.begin());
		for (auto ed : graph[where]) {
			if (min_distance[ed.to] > min_distance[where] + ed.length) {
				active_vertices.erase({ min_distance[ed.to], ed.to });
				min_distance[ed.to] = min_distance[where] + ed.length;
				active_vertices.insert({ min_distance[ed.to], ed.to });
			}
		}
	}
	return infinity;
}

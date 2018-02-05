#pragma once
#ifndef EDGE_H
#define EDGE_H

class Edge {
public:
	int to;
	int length;
	Edge(int nto, int nlength) :
		to(nto), length(nlength)
	{	}
	~Edge() {
	}
};

#endif

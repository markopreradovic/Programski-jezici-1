#pragma once
#include "WeightedDirectedGraph.h"
#include "WeightedUndirectedGraph.h"
#include "UnweightedDirectedGraph.h"
#include <fstream>

namespace Graph {
	class UnweightedUndirectedGraph : public WeightedUndirectedGraph, public UnweightedDirectedGraph {
	public:
		using WeightedUndirectedGraph::WeightedUndirectedGraph;
		using UnweightedDirectedGraph::UnweightedDirectedGraph;
		UnweightedUndirectedGraph(std::fstream&);

		bool isBipartite(int);
	};
};
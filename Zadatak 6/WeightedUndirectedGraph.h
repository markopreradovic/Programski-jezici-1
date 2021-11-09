#pragma once
#include "WeightedDirectedGraph.h"
#include <fstream>

namespace Graph {
	class WeightedUndirectedGraph : virtual public WeightedDirectedGraph {
	public:
		using WeightedDirectedGraph::WeightedDirectedGraph;
		WeightedUndirectedGraph(std::fstream&);
	};
};
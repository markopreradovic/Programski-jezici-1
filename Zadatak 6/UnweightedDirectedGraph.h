#pragma once
#include "WeightedDirectedGraph.h"
#include <fstream>
#include <map>
#include <set>

namespace Graph {
	class UnweightedDirectedGraph : virtual public WeightedDirectedGraph {
	public:
		using WeightedDirectedGraph::WeightedDirectedGraph;
		UnweightedDirectedGraph(std::fstream&);
	};
};
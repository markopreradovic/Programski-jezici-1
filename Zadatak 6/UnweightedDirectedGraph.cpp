#include "pch.h"
#include "WeightedDirectedGraph.h"
#include "UnweightedDirectedGraph.h"
#include <fstream>

namespace Graph {
	UnweightedDirectedGraph::UnweightedDirectedGraph(std::fstream& inFile) : WeightedDirectedGraph(inFile) {
		for (auto& node : nodes) {
			for (auto& x : node.second)
				x.weight = 1.0;
		}
	}
}
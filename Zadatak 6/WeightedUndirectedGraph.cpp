#include "pch.h"
#include "WeightedDirectedGraph.h"
#include "WeightedUndirectedGraph.h"
#include <fstream>
#include <iostream>

namespace Graph {
	WeightedUndirectedGraph::WeightedUndirectedGraph(std::fstream& inFile) : WeightedDirectedGraph(inFile) {
		for (auto& node : nodes) {
			for (auto& x : node.second)
				connect(x.neighbor, node.first, x.weight);
		}
	}
}
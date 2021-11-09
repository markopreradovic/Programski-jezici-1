#include "pch.h"
#include "WeightedDirectedGraph.h"
#include "WeightedUndirectedGraph.h"
#include "UnweightedDirectedGraph.h"
#include "UnweightedUndirectedGraph.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

namespace Graph {
	UnweightedUndirectedGraph::UnweightedUndirectedGraph(std::fstream& inFile)
		: WeightedDirectedGraph(inFile), WeightedUndirectedGraph(inFile), UnweightedDirectedGraph(inFile) { }

	bool UnweightedUndirectedGraph::isBipartite(int source) {
		checkValidNode(source);

		std::vector<int> colorArray(nodes.size(), -1);
		colorArray[source] = 1;

		std::queue<int> q;
		q.push(source);

		while (!q.empty()) {
			int currentNode = q.front();
			q.pop();
			for (auto& x : nodes[currentNode]) {
				if (x.neighbor == currentNode)
					return false;
			}
			for (auto& x : nodes[currentNode]) {
				if (colorArray[x.neighbor] == -1) {
					colorArray[x.neighbor] = 1 - colorArray[currentNode];
					q.push(x.neighbor);
				}
				else if (colorArray[x.neighbor] == colorArray[currentNode])
					return false;
			}
		}
		return true;
	}
};
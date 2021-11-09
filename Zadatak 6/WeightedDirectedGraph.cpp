#include "pch.h"
#include "WeightedDirectedGraph.h"
#include <sstream>
#include <vector>
#include <iostream>
#include <string>
#include <limits>
#include <algorithm>
#include <exception>

namespace Graph {
	bool WeightedDirectedGraph::Edge::operator==(const Edge& other) const noexcept {
		return (neighbor == other.neighbor && weight == other.weight);
	}

	bool WeightedDirectedGraph::Edge::operator!=(const Edge& other) const noexcept {
		return !(*this == other);
	}

	void WeightedDirectedGraph::setIteratorType(IteratorType type) noexcept {
		iteratorType = type;
	}

	IteratorType WeightedDirectedGraph::getIteratorType() const noexcept {
		return iteratorType;
	}

	void WeightedDirectedGraph::checkValidNode(int node) const noexcept(false) {
		if (node < 0 || node >= nextData)
			throw std::invalid_argument("Cvor ne postoji u grafu.");
	}

	bool WeightedDirectedGraph::iterator::operator==(const WeightedDirectedGraph::iterator& rhs) const noexcept {
		return (currentNode == rhs.currentNode);
	}

	WeightedDirectedGraph::iterator& WeightedDirectedGraph::iterator::operator++() {
		if (it == IteratorType::BFS)
			bfs_advance();
		else
			dfs_advance();
		return *this;
	}

	WeightedDirectedGraph::iterator WeightedDirectedGraph::iterator::operator++(int) {
		auto tmp = *this;
		this->operator++();
		return tmp;
	}

	bool WeightedDirectedGraph::iterator::operator!=(const WeightedDirectedGraph::iterator& rhs) const noexcept {
		return !(*this == rhs);
	}

	int WeightedDirectedGraph::iterator::operator*() const {
		return currentNode;
	}

	void WeightedDirectedGraph::iterator::bfs_advance() {
		auto& neighbors = data[currentNode];
		for (auto& x : neighbors) {
			if (visited.count(x.neighbor) == 0) {
				visited.emplace(x.neighbor);
				toVisitBFS.push(x.neighbor);
			}
		}
		visited.emplace(currentNode);

		if (!toVisitBFS.empty()) {
			currentNode = toVisitBFS.front();
			toVisitBFS.pop();
		}
		else
			currentNode = -1;
	}

	void WeightedDirectedGraph::iterator::dfs_advance() {
		auto& neighbors = data[currentNode];
		for (auto& x : neighbors) {
			if (visited.count(x.neighbor) == 0) {
				visited.emplace(x.neighbor);
				toVisitDFS.push(x.neighbor);
			}
		}
		visited.emplace(currentNode);

		if (!toVisitDFS.empty()) {
			currentNode = toVisitDFS.top();
			toVisitDFS.pop();
		}
		else
			currentNode = -1;
	}

	WeightedDirectedGraph::iterator WeightedDirectedGraph::begin() {
		return iterator(nodes, 0, iteratorType);
	}

	WeightedDirectedGraph::iterator WeightedDirectedGraph::end() {
		return iterator(nodes, -1, iteratorType);
	}

	WeightedDirectedGraph::WeightedDirectedGraph(std::fstream& inFile) { 
		std::string line;
		while (std::getline(inFile, line)) {
			std::istringstream sLine(line);
			double weight;
			int node = 0;
			std::vector<std::pair<int, double>> vector;
			while (sLine >> weight) {
				vector.push_back(std::make_pair(node, weight));
				++node;
			}
			add(vector);
		}
	}

	void WeightedDirectedGraph::add(const std::vector<std::pair<int, double>>& adjList) { 
		for (auto& x : adjList) {
			if (x.second != 0.0)
				nodes[nextData].emplace_back(x.first, x.second);
		}
		++nextData;
	}

	void WeightedDirectedGraph::connect(int source, int dest, double w) {
		auto& vector = nodes[source];
		Edge newEdge(dest, w);
		if (std::find(vector.begin(), vector.end(), newEdge) == vector.end())
			vector.emplace_back(dest, w);
	}

	void WeightedDirectedGraph::printPath(int source, int destination, const std::vector<int>& path) {
		if (source == destination)
			std::cout << destination << " ";
		else {
			if (path[destination] == -1)
				std::cout << "Ne postoji put.";
			else {
				printPath(source, path[destination], path);
				std::cout << destination << " ";
			}
		}
	}

	void WeightedDirectedGraph::shortestPath(int source, int destination) {
		checkValidNode(source);
		checkValidNode(destination);

		std::vector<double> distance(nextData, INFINITY);
		std::vector<int> path(nextData, -1);

		distance[source] = 0.0;

		std::vector<int> toVisit;
		toVisit.push_back(source);

		while (!toVisit.empty()) {
			//uzmi prvi sa najmanjom distance[i]
			auto it = std::min_element(toVisit.begin(), toVisit.end(), [&distance](int i, int j) {return distance[i] < distance[j]; });
			int nodeId = *it;
			toVisit.erase(it);
			//prodji kroz sve susjede
			auto nb = nodes[nodeId];
			for (auto x : nb) {
				//ako je udaljenost od prvog plus udaljenost do susjeda manja od trenutne udaljenosti u distance[susjed]
				if (distance[nodeId] + x.weight < distance[x.neighbor]) {
					//update-uj težinu, dodaj u toVisit i update-uj listu prethodnika
					distance[x.neighbor] = x.weight + distance[nodeId];
					toVisit.push_back(x.neighbor);
					path[x.neighbor] = nodeId;
				}
			}
		}
		std::cout << "Najkraci put : ";
		printPath(source, destination, path);
		std::cout << std::endl;
		std::cout << "Duzina puta je : " << distance[destination] << std::endl;
	}

	void WeightedDirectedGraph::HamiltonianPath(int source) {
		checkValidNode(source);

		std::vector<int> vertex;
		for (size_t i = 0; i < nodes.size(); ++i)
			if (i != source)
				vertex.push_back(i);
		std::vector<int> finalPermutation = vertex;
		double minimalPath = INFINITY;
		do {
			double currentPathWeight = 0;
			int k = source;
			for (int node : vertex) {
				auto neighbors = nodes[k];
				double weight = INFINITY;
				for (auto& x : neighbors) {
					if (x.neighbor == node) {
						weight = x.weight;
						break;
					}
				}
				currentPathWeight += weight;
				k = node;
			}
			auto vector = nodes[k];
			bool isFound = false;
			for (auto& x : vector) {
				if (x.neighbor == source) {
					isFound = true;
					currentPathWeight += x.weight;
					break;
				}
			}
			if (!isFound)
				currentPathWeight += INFINITY;
			if (currentPathWeight < minimalPath) {
				finalPermutation = vertex;
				minimalPath = currentPathWeight;
			}
		} 	while (std::next_permutation(vertex.begin(), vertex.end()));

		std::cout << "Permutacija : ";
		std::cout << source << "->";
		for (auto& x : finalPermutation)
			std::cout << x << "->";
		std::cout << source << std::endl;
		std::cout << "Minimalna putanja kroz sve cvorove grafa : " << minimalPath << std::endl;
	}
};
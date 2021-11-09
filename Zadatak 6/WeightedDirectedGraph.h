#pragma once
#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <vector>
#include <utility>
#include <queue>
#include <stack>

namespace Graph {
	enum class IteratorType {
		BFS, DFS
	};

	class WeightedDirectedGraph {
	protected:
		struct Edge {
			int neighbor;
			double weight;

			Edge(int n, double w = 1.0) : neighbor(n), weight(w) { }
			bool operator==(const Edge&) const noexcept;
			bool operator!=(const Edge&) const noexcept;
		};

		std::map<int, std::vector<Edge>> nodes;
		int nextData = 0;
		IteratorType iteratorType = IteratorType::BFS;

	public:
		class iterator {
		protected:
			std::map<int, std::vector<Edge>>& data;
			std::set<int> visited;
			std::queue<int> toVisitBFS;
			std::stack<int> toVisitDFS;
			int currentNode;
			IteratorType it;
		public:
			iterator(std::map<int, std::vector<Edge>>& map, int node, IteratorType type) : data(map), currentNode(node), it(type) { }

			iterator& operator++();
			iterator operator++(int);
			virtual bool operator!=(const iterator&) const noexcept;
			virtual bool operator==(const iterator&) const noexcept;
			virtual int operator*() const;

		protected:
			void bfs_advance();
			void dfs_advance();
		};

		iterator begin();
		iterator end();

	protected:

	public:
		WeightedDirectedGraph(std::fstream&);

		int size() const noexcept { return nextData; }
		void shortestPath(int, int);
		void HamiltonianPath(int);

		void setIteratorType(IteratorType) noexcept;
		IteratorType getIteratorType() const noexcept;

	protected:
		void add(const std::vector<std::pair<int, double>>&);
		void printPath(int, int, const std::vector<int>& path);
		void connect(int, int, double);
		void checkValidNode(int) const noexcept(false);
	};
};
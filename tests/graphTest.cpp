#include "collin_graph.hpp"
#include "collin_iostream.hpp"
#include "collin_stream.hpp"
#include <iostream>
#include <unordered_set>
#include <iterator>

int main()
{
	collin::directed_graph<int> graph;
	graph.insert_node(5);
	graph.insert_edge(5, 6);
	graph.insert_edge(8, 5);

	const auto nodes = graph.get_nodes();

	std::cout << "Nodes:\n";
	collin::print(nodes, "\n");

	std::cout << "Edges:\n";
	for(const auto& node : nodes)
	{
		std::cout << node << ": ";

		auto edges = collin::to_stream(graph.get_edges(node))
						.map([](const auto& pair) { return pair.first; })
						.collect<std::unordered_set<int>>();

		collin::print(edges, " ");
		std::cout << '\n';
	}
}
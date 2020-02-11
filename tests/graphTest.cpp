#include "collin_graph.hpp"
#include "collin_iostream.hpp"
#include "collin_stream.hpp"
#include <iostream>
#include <unordered_set>
#include <iterator>

int main()
{
	collin::graph<int, int, true> graph;
	graph.insert_node(5);
	graph.insert_edge(5, 6);
	graph.insert_edge(8, 5);

	const auto& nodes = graph.get_nodes();

	std::cout << "Nodes:\n";
	collin::print(nodes, "\n");

	std::cout << "Edges:\n";
	for(const auto& node : nodes)
	{
		std::cout << node << ": ";

		for(const auto& [source, destination, weight] : graph.get_out_edges(node))
		{
			std::cout << destination << " ";
		}

		std::cout << '\n';
	}
}
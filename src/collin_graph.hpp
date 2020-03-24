#pragma once

#include "collin_functional.hpp"
#include <unordered_map>
#include <unordered_set>
#include <stdexcept>

namespace collin
{
	namespace graphs
	{
		template<class Key, class Weight=int, bool directed=false>
		class Graph
		{
			public:
				using value_type = Key;
				using reference = value_type&;
				using const_reference = const reference;
				using pointer = value_type*;
				using const_pointer = const pointer;

				using node = value_type;
				using edge = std::tuple<value_type, value_type, Weight>;
				constexpr static auto source_node_index = 0;
				constexpr static auto destination_node_index = 1;
				constexpr static auto weight_index = 2;

				Graph() = default;
				~Graph() = default;
				Graph(const Graph&) = default;
				Graph(Graph&&) = default;
				Graph& operator=(const Graph&) = default;
				Graph& operator=(Graph&&) = default;

				void insert_node(const node& val)
				{
					nodes.insert(val);
				}

				void insert_edge(const edge& e)
				{
					edges.push_back(e);
				}

				void insert_edge(const node& source, const node& destination, Weight w={})
				{
					insert_node(source);
					insert_node(destination);
					insert_edge(std::make_tuple(source, destination, w));
					if constexpr(!directed)
					{
						insert_edge(std::make_tuple(destination, source, w));
					}
				}

				void remove_node(const_reference val) noexcept
				{
					nodes.erase(val);
					erase_if(edges, [&](const edge& e) {
						return std::get<source_node_index>(e) == val || std::get<destination_node_index>(e) == val;
					});
				}

				void remove_edge(const_reference source, const_reference destination) noexcept
				{
					erase_if(edges, [&](const edge& e) {
						return std::get<source_node_index>(e) == source && std::get<destination_node_index>(e) == destination;
					});
				}

				const std::unordered_set<node>& get_nodes() const noexcept
				{
					return nodes;
				}

				const std::vector<edge>& get_edges() const noexcept
				{
					return edges;
				}

				std::vector<edge> get_out_edges(const node& val) const
				{
					return get_edges_if([&](const edge& e) { return std::get<source_node_index>(e) == val; });
				}

				std::vector<edge> get_in_edges(const node& val) const
				{
					return get_edges_if([&](const edge& e) { return std::get<destination_node_index>(e) == val; });
				}

				void clear() noexcept
				{
					nodes.clear();
					edges.clear();
				}

				auto order() const noexcept
				{
					return std::size(nodes);
				}

				auto size() const noexcept
				{
					if constexpr(directed)
					{
						return std::size(edges);
					}
					else
					{
						return std::size(edges) / 2;
					}
				}

			private:
				std::unordered_set<node> nodes;
				std::vector<edge> edges;

				template<class UnaryPredicate>
				std::vector<edge> get_edges_if(UnaryPredicate fn) const
				{
					std::vector<edge> node_edges;
					std::copy_if(std::begin(edges), std::end(edges), std::back_inserter(node_edges), fn);

					return node_edges;
				}
		};
	}
}
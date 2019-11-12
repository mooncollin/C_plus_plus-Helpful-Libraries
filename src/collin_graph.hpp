#ifndef COLLIN_GRAPH
#define COLLIN_GRAPH

#include "collin_functional.hpp"
#include <unordered_map>
#include <unordered_set>
#include <stdexcept>

namespace collin
{
	template<class Key, class Weight=int>
	class directed_graph
	{
		using edge_set = std::unordered_set<std::pair<Key, Weight>, pair_hash>;

		public:
			using value_type = Key;
			using reference = value_type&;
			using const_reference = const reference;
			using pointer = value_type*;
			using const_pointer = const pointer;

			directed_graph() = default;
			~directed_graph() = default;
			directed_graph(const directed_graph&) = default;
			directed_graph(directed_graph&&) = default;
			directed_graph& operator=(const directed_graph&) = default;
			directed_graph& operator=(directed_graph&&) = default;

			void insert_node(value_type val)
			{
				node_map[std::move(val)] = {};
			}

			void insert_edge(value_type source, value_type destination, Weight w={})
			{
				node_map[std::move(source)].emplace(std::move(destination), std::move(w));
			}

			void remove_node(const_reference val)
			{
				const auto location = node_map.find(val);
				if(location != std::end(node_map))
				{
					node_map.erase(location);
					for(auto& [_, node_set] : node_map)
					{
						node_set.remove(val);
					}
				}
			}

			void remove_edge(const_reference source, const_reference destination)
			{
				auto location = node_map.find(source);
				if(location == std::end(node_map))
				{
					throw std::runtime_error("Source node does not exist");
				}

				location->second.erase(destination);
			}

			std::unordered_set<Key> get_nodes() const
			{
				std::unordered_set<Key> node_set;
				node_set.reserve(std::size(node_map));

				for(const auto& [node, _] : node_map)
				{
					node_set.insert(node);
				}

				return node_set;
			}

			const edge_set& get_edges(value_type key) const
			{
				return node_map.at(key);
			}

		private:
			std::unordered_map<value_type, edge_set> node_map;
			
	};
}

#endif
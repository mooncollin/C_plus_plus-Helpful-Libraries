#pragma once

#include <vector>
#include <array>
#include <utility>
#include <memory>
#include <initializer_list>
#include <algorithm>

#include "collin/concepts.hpp"
#include "collin/utility.hpp"

namespace collin
{
	template<class Key, class T, class Allocator = std::allocator<std::pair<const Key, T>>>
	class flat_map
	{
		public:
			using key_type = Key;
			using mapped_type = T;
			using value_type = std::pair<const Key, T>;
			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;
			using allocator_type = Allocator;
		private:
			using storage_t = std::vector<value_type, allocator_type>;
		public:
			using reference = value_type&;
			using const_refernce = const value_type&;
			using pointer = typename std::allocator_traits<allocator_type>::pointer;
			using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;
			using iterator = typename storage_t::iterator;
			using const_iterator = typename storage_t::const_iterator;
			using reverse_iterator = typename storage_t::reverse_iterator;
			using const_reverse_iterator = typename storage_t::const_reverse_iterator;

			flat_map() = default;
			flat_map(const flat_map&) = default;
			flat_map(flat_map&&) = default;
			flat_map& operator=(const flat_map&) = default;
			flat_map& operator=(flat_map&&) noexcept(std::allocator_traits<allocator_type>::is_always_equal::value) = default;

			flat_map(const flat_map& map, const allocator_type& alloc)
				: data{map, alloc} {}

			template<class InputIt>
			flat_map(InputIt first, InputIt last, const allocator_type& alloc = allocator_type{})
				: data{first, last, alloc} {}

			flat_map(std::initializer_list<value_type> values, const allocator_type& alloc = allocator_type{})
				: data{values, alloc} {}

			flat_map& operator=(std::initializer_list<value_type> values)
			{
				data = values;
				return *this;
			}

			allocator_type get_allocator() const noexcept
			{
				return data.get_allocator();
			}

			[[nodiscard]] mapped_type& at(const key_type& key)
			{
				const auto itr = find(key);
				if (itr != std::end(data))
				{
					return itr->second;
				}

				throw std::range_error{"Not Found"};
			}

			[[nodiscard]] const mapped_type& at(const key_type& key) const
			{
				const auto itr = find(key);
				if (itr != std::end(data))
				{
					return itr->second;
				}

				throw std::range_error{"Not Found"};
			}

			mapped_type& operator[](const key_type& key)
			{
				return insert(std::make_pair(key, mapped_type{}))->second;
			}

			mapped_type& operator[](key_type&& key)
			{
				return insert(std::make_pair(std::move(key), mapped_type{}))->second;
			}

			iterator begin() noexcept
			{
				return std::begin(data);
			}

			const_iterator begin() const noexcept
			{
				return std::begin(data);
			}

			const_iterator cbegin() const noexcept
			{
				return std::cbegin(data);
			}

			iterator end() noexcept
			{
				return std::end(data);
			}

			const_iterator end() const noexcept
			{
				return std::end(data);
			}

			const_iterator cend() const noexcept
			{
				return std::cend(data);
			}

			reverse_iterator rbegin() noexcept
			{
				return std::rbegin(data);
			}

			const_reverse_iterator rbegin() const noexcept
			{
				return std::rbegin(data);
			}

			const_reverse_iterator crbegin() const noexcept
			{
				return std::crbegin(data);
			}

			reverse_iterator rend() noexcept
			{
				return std::rend(data);
			}

			const_reverse_iterator rend() const noexcept
			{
				return std::rend(data);
			}

			const_reverse_iterator crend() const noexcept
			{
				return std::crend(data);
			}

			[[nodiscard]] bool empty() const noexcept
			{
				return data.empty();
			}

			[[nodiscard]] size_type size() const noexcept
			{
				return data.size();
			}

			[[nodiscard]] size_type max_size() const noexcept
			{
				return data.max_size();
			}

			void clear() noexcept
			{
				data.clear();
			}

			std::pair<iterator, bool> insert(const value_type& value)
			{
				const auto itr = find(value.first);
				if (itr != std::end(data))
				{
					return {itr, false};
				}

				data.push_back(value);
				return {end() - 1, true};
			}

			template<class P>
				requires(collin::concepts::constructible<value_type, P&&>)
			std::pair<iterator, bool> insert(P&& value)
			{
				return emplace(std::forward<P>(value));
			}

			std::pair<iterator, bool> insert(value_type&& value)
			{
				const auto itr = find(value.first);
				if (itr != std::end(data))
				{
					return {itr, false};
				}

				data.push_back(std::forward<value_type>(value));
				return {end() - 1, true};
			}

			iterator insert(const_iterator hint, const value_type& value)
			{
				const auto itr = find(value.first);
				if (itr != std::end(data))
				{
					return itr;
				}

				return data.insert(hint, value);
			}

			template<class P>
				requires(collin::concepts::constructible<value_type, P&&>)
			iterator insert(const_iterator hint, P&& value)
			{
				return emplace_hint(std::forward<P>(value));
			}

			iterator insert(const_iterator hint, value_type&& value)
			{
				const auto itr = find(value.first);
				if (itr != std::end(data))
				{
					return itr;
				}

				return data.emplace(hint, std::forward<value_type>(value));
			}

			template<class InputIt>
			void insert(InputIt first, InputIt last)
			{
				while (first != last)
				{
					auto v = *first;
					if (!contains(v.first))
					{
						data.push_back(std::move(v));
					}
					++first;
				}
			}

			void insert(std::initializer_list<value_type> values)
			{
				for (const auto& v : values)
				{
					if (!contains(v.first))
					{
						data.push_back(v);
					}
				}
			}

			template<class M>
			std::pair<iterator, bool> insert_or_assign(const key_type& k, M&& obj)
			{
				const auto itr = find(k);
				if (itr != std::end(data))
				{
					itr->second = std::forward<M>(obj);
					return {itr, false};
				}

				data.emplace_back(k, std::forward<M>(obj));
				return {end() - 1, true};
			}

			template<class M>
			std::pair<iterator, bool> insert_or_assign(key_type&& k, M&& obj)
			{
				const auto itr = find(k);
				if (itr != std::end(data))
				{
					itr->second = std::forward<M>(obj);
					return {itr, false};
				}

				data.emplace_back(std::move(k), std::forward<M>(obj));
				return {end() - 1, true};
			}

			template<class M>
			iterator insert_or_assign(const_iterator hint, const key_type& k, M&& obj)
			{
				const auto itr = find(k);
				if (itr != std::end(data))
				{
					itr->second = std::forward<M>(obj);
					return itr;
				}

				return data.emplace(hint, k, std::forward<M>(obj));
			}

			template<class M>
			iterator insert_or_assign(const_iterator hint, key_type&& k, M&& obj)
			{
				const auto itr = find(k);
				if (itr != std::end(data))
				{
					itr->second = std::forward<M>(obj);
					return itr;
				}

				data.emplace(hint, std::move(k), std::forward<M>(obj));
				return {end() - 1, true};
			}

			template<class... Args>
			std::pair<iterator, bool> emplace(Args&&... args)
			{
				auto v = value_type{std::forward<Args>(args)...};
				const auto itr = find(v.first);
				if (itr != std::end(data))
				{
					return {itr, false};
				}

				data.push_back(std::move(v));
				return {end() - 1, true};
			}

			template<class... Args>
			iterator emplace_hint(const_iterator hint, Args&&... args)
			{
				return data.emplace(hint, std::forward<Args>(args)...);
			}

			template<class... Args>
			std::pair<iterator, bool> try_emplace(const key_type& k, Args&&... args)
			{
				const auto itr = find(k);
				if (itr != std::end(data))
				{
					return {itr, false};
				}

				data.emplace_back(std::piecewise_construct, std::forward_as_tuple(k), std::forward_as_tuple(std::forward<Args>(args)...));
				return {end() - 1, true};
			}

			template<class... Args>
			std::pair<iterator, bool> try_emplace(key_type&& k, Args&&... args)
			{
				const auto itr = find(k);
				if (itr != std::end(data))
				{
					return {itr, false};
				}

				data.emplace_back(std::piecewise_construct, std::forward_as_tuple(std::move(k)), std::forward_as_tuple(std::forward<Args>(args)...));
				return {end() - 1, true};
			}

			template<class... Args>
			iterator try_emplace(const_iterator hint, const key_type& k, Args&&... args)
			{
				const auto itr = find(k);
				if (itr != std::end(data))
				{
					return itr;
				}

				return data.emplace(hint, std::piecewise_construct, std::forward_as_tuple(k), std::forward_as_tuple(std::forward<Args>(args)...));
			}

			template<class... Args>
			iterator try_emplace(const_iterator hint, key_type&& k, Args&&... args)
			{
				const auto itr = find(k);
				if (itr != std::end(data))
				{
					return itr;
				}

				return data.emplace(hint, std::piecewise_construct, std::forward_as_tuple(std::move(k)), std::forward_as_tuple(std::forward<Args>(args)...));
			}

			iterator erase(iterator pos)
			{
				return data.erase(pos);
			}

			iterator erase(const_iterator first, const_iterator last)
			{
				return data.erase(first, last);
			}

			size_type erase(const key_type& key)
			{
				const auto itr = find(key);
				if (itr == std::end(data))
				{
					return 0;
				}

				erase(itr);

				return 1;
			}

			void swap(flat_map& other) noexcept(std::allocator_traits<allocator_type>::is_always_equal::value)
			{
				data.swap(other.data);
			}

			size_type count(const key_type& key) const noexcept
			{
				return static_cast<size_type>(contains(key));
			}

			iterator find(const key_type& key)
			{
				return std::find_if(std::begin(data), std::end(data), [&key](const auto& v) { return v.first == key; });
			}

			const_iterator find(const key_type& key) const
			{
				return std::find_if(std::cbegin(data), std::cend(data), [&key](const auto& v) { return v.first == key; });
			}

			[[nodiscard]] bool contains(const key_type& key) const
			{
				return find(key) != std::cend(data);
			}

			template<class Key, class T, class Alloc>
			friend [[nodiscard]] bool operator==(const flat_map<Key, T, Alloc>& lhs, const flat_map<Key, T, Alloc>& rhs)
			{
				return lhs.data == rhs.data;
			}

			template<class Key, class T, class Alloc>
			friend [[nodiscard]] bool operator!=(const flat_map<Key, T, Alloc>& lhs, const flat_map<Key, T, Alloc>& rhs)
			{
				return !(lhs == rhs);
			}
		private:
			storage_t data;
	};

	template<class Key, class T, class Alloc, class Pred>
	typename flat_map<Key, T, Alloc>::size_type erase_if(flat_map<Key, T, Alloc>& c, Pred pred)
	{
		const auto old_size = c.size();
		for (auto i = c.begin(), last = c.end(); i != last)
		{
			if (pred(*i))
			{
				i = c.erase(i);
			}
			else
			{
				++i;
			}
		}

		return old_size - c.size();
	}

	template<class Key, class T, std::size_t Size>
	class fixed_flat_map
	{
		public:
			using key_type = Key;
			using mapped_type = T;
			using value_type = std::pair<const Key, T>;
			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;
		private:
			using storage_t = std::array<value_type, Size>;
		public:
			using reference = value_type&;
			using const_refernce = const value_type&;
			using pointer = value_type*;
			using const_pointer = const value_type*;
			using iterator = typename storage_t::iterator;
			using const_iterator = typename storage_t::const_iterator;
			using reverse_iterator = typename storage_t::reverse_iterator;
			using const_reverse_iterator = typename storage_t::const_reverse_iterator;

			constexpr fixed_flat_map() = default;

			constexpr fixed_flat_map(std::initializer_list<value_type> values)
				: data{collin::initializer_list_to_array<Size>(values)}
			{
				if (std::size(values) != Size)
				{
					throw std::range_error{"Out of Bounds"};
				}
			}

			template<class... Args>
			constexpr fixed_flat_map(Args&&... values)
				: data{{std::forward<Args>(values)...}} {}

			fixed_flat_map& operator=(std::initializer_list<value_type> values)
			{
				if (std::size(values) != Size)
				{
					throw std::range_error{"Out of Bounds"};
				}

				data = collin::initializer_list_to_array<Size>(values);
				return *this;
			}

			[[nodiscard]] constexpr mapped_type& at(const key_type& key)
			{
				const auto itr = find(key);
				if (itr != std::end(data))
				{
					return itr->second;
				}

				throw std::range_error{"Not Found"};
			}

			[[nodiscard]] constexpr const mapped_type& at(const key_type& key) const
			{
				const auto itr = find(key);
				if (itr != std::end(data))
				{
					return itr->second;
				}

				throw std::range_error{"Not Found"};
			}

			[[nodiscard]] constexpr mapped_type& operator[](const key_type& key)
			{
				return *find(key);
			}

			[[nodiscard]] constexpr const mapped_type& operator[](const key_type& key) const
			{
				return *find(key);
			}

			constexpr iterator begin() noexcept
			{
				return std::begin(data);
			}

			constexpr const_iterator begin() const noexcept
			{
				return std::begin(data);
			}

			constexpr const_iterator cbegin() const noexcept
			{
				return std::cbegin(data);
			}

			constexpr iterator end() noexcept
			{
				return std::end(data);
			}

			constexpr const_iterator end() const noexcept
			{
				return std::end(data);
			}

			constexpr const_iterator cend() const noexcept
			{
				return std::cend(data);
			}

			constexpr reverse_iterator rbegin() noexcept
			{
				return std::rbegin(data);
			}

			constexpr const_reverse_iterator rbegin() const noexcept
			{
				return std::rbegin(data);
			}

			constexpr const_reverse_iterator crbegin() const noexcept
			{
				return std::crbegin(data);
			}

			constexpr reverse_iterator rend() noexcept
			{
				return std::rend(data);
			}

			constexpr const_reverse_iterator rend() const noexcept
			{
				return std::rend(data);
			}

			constexpr const_reverse_iterator crend() const noexcept
			{
				return std::crend(data);
			}

			[[nodiscard]] constexpr bool empty() const noexcept
			{
				return data.empty();
			}

			[[nodiscard]] constexpr size_type size() const noexcept
			{
				return Size;
			}

			[[nodiscard]] constexpr size_type count(const key_type& key) const noexcept
			{
				return static_cast<size_type>(contains(key));
			}

			[[nodiscard]] constexpr iterator find(const key_type& key)
			{
				return std::find_if(std::begin(data), std::end(data), [&key](const auto& v) { return v.first == key; });
			}

			[[nodiscard]] constexpr const_iterator find(const key_type& key) const
			{
				return std::find_if(std::cbegin(data), std::cend(data), [&key](const auto& v) { return v.first == key; });
			}

			[[nodiscard]] constexpr bool contains(const key_type& key) const
			{
				return find(key) != std::cend(data);
			}
			
			template<class Key, class T, std::size_t Size1, std::size_t Size2>
			[[nodiscard]] friend constexpr bool operator==(const fixed_flat_map<Key, T, Size1>& lhs, const fixed_flat_map<Key, T, Size1>& rhs)
			{
				if constexpr (Size1 != Size2)
				{
					return false;
				}
				else
				{
					return lhs.data == lhs.data;
				}
			}

			template<class Key, class T, std::size_t Size1, std::size_t Size2>
			[[nodiscard]] friend constexpr bool operator!=(const fixed_flat_map<Key, T, Size1>& lhs, const fixed_flat_map<Key, T, Size2>& rhs)
			{
				return !(lhs == rhs);
			}
		private:
			storage_t data;
	};

	template<class Key, class T, class... Args>
		requires((collin::concepts::convertible_to<Args, std::pair<Key, T>> && ...))
	constexpr fixed_flat_map<Key, T, sizeof...(Args) + 1> make_fixed_flat_map(const std::pair<Key, T>& arg1, Args&&... args)
	{
		return {arg1, std::forward<Args>(args)...};
	}

	template<class Key, class T, class... Args>
		requires((collin::concepts::convertible_to<Args, std::pair<Key, T>> && ...))
	constexpr fixed_flat_map<Key, T, sizeof...(Args) + 1> make_fixed_flat_map(std::pair<Key, T>&& arg1, Args&&... args)
	{
		return {std::move(arg1), std::forward<Args>(args)...};
	}
}
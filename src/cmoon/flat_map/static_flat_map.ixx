export module cmoon.flat_map.static_flat_map;

import <array>;
import <initializer_list>;
import <utility>;
import <concepts>;
import <stdexcept>;

import cmoon.utility;

namespace cmoon
{
	export
	template<class Key, class T, std::size_t Size>
	class static_flat_map
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

			constexpr static_flat_map() = default;

			constexpr static_flat_map(std::initializer_list<value_type> values)
				: data{cmoon::initializer_list_to_array<Size>(values)}
			{
				if (std::size(values) != Size)
				{
					throw std::out_of_range{"Out of Bounds"};
				}
			}

			template<class... Args>
			constexpr static_flat_map(Args&&... values)
				: data{{std::forward<Args>(values)...}} {}

			static_flat_map& operator=(std::initializer_list<value_type> values)
			{
				if (std::size(values) != Size)
				{
					throw std::out_of_range{"Out of Bounds"};
				}

				data = cmoon::initializer_list_to_array<Size>(values);
				return *this;
			}

			[[nodiscard]] constexpr mapped_type& at(const key_type& key)
			{
				const auto itr = find(key);
				if (itr != std::end(data))
				{
					return itr->second;
				}

				throw std::out_of_range{"Not Found"};
			}

			[[nodiscard]] constexpr const mapped_type& at(const key_type& key) const
			{
				const auto itr = find(key);
				if (itr != std::end(data))
				{
					return itr->second;
				}

				throw std::out_of_range{"Not Found"};
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
			[[nodiscard]] friend constexpr bool operator==(const static_flat_map<Key, T, Size1>& lhs, const static_flat_map<Key, T, Size1>& rhs)
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
			[[nodiscard]] friend constexpr bool operator!=(const static_flat_map<Key, T, Size1>& lhs, const static_flat_map<Key, T, Size2>& rhs)
			{
				return !(lhs == rhs);
			}
		private:
			storage_t data;
	};

	export
	template<class Key, class T, class... Args>
		requires((std::convertible_to<Args, std::pair<Key, T>> && ...))
	constexpr static_flat_map<Key, T, sizeof...(Args) + 1> make_static_flat_map(const std::pair<Key, T>& arg1, Args&&... args)
	{
		return {arg1, std::forward<Args>(args)...};
	}

	export
	template<class Key, class T, class... Args>
		requires((std::convertible_to<Args, std::pair<Key, T>> && ...))
	constexpr static_flat_map<Key, T, sizeof...(Args) + 1> make_static_flat_map(std::pair<Key, T>&& arg1, Args&&... args)
	{
		return {std::move(arg1), std::forward<Args>(args)...};
	}
}
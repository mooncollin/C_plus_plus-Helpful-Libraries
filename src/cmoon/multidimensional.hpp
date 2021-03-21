#pragma once

#include <cstddef>
#include <vector>
#include <array>
#include <tuple>
#include <memory>
#include <initializer_list>
#include <numeric>
#include <functional>
#include <span>
#include <algorithm>
#include <execution>
#include <iterator>
#include <functional>
#include <utility>
#include <ranges>

namespace cmoon
{
	template<class T, std::size_t... Dimensions>
		requires(sizeof...(Dimensions) > 0)
	class constant_multidimensional_array;

	template<class T, std::size_t Dimensions, class Allocator = std::allocator<T>>
	class fixed_multidimensional_array;

	// Totally dynamic in dimensions and size
	template<class T, class Allocator = std::allocator<T>>
	class multidimensional_array
	{
		using storage_t = std::vector<T, Allocator>;
		using dimensions_t = std::vector<std::size_t, typename std::allocator_traits<Allocator>::template rebind_alloc<std::size_t>>;

		public:
			using value_type = typename storage_t::value_type;
			using allocator_type = typename storage_t::allocator_type;
			using size_type = typename storage_t::size_type;
			using difference_type = typename storage_t::difference_type;
			using reference = typename storage_t::reference;
			using const_reference = typename storage_t::const_reference;
			using pointer = typename storage_t::pointer;
			using const_pointer = typename storage_t::const_pointer;
			using iterator = typename storage_t::iterator;
			using const_iterator = typename storage_t::const_iterator;
			using reverse_iterator = typename storage_t::reverse_iterator;
			using const_reverse_iterator = typename storage_t::const_reverse_iterator;

			static_assert(std::same_as<T, typename std::allocator_traits<allocator_type>::value_type>);

			multidimensional_array(std::initializer_list<std::size_t> dimensions, const Allocator& alloc = Allocator{})
				: data_{alloc}, dimensions_{dimensions}, dimensions_cache_(std::ranges::size(dimensions) - 1)
			{
				fill_cache();
				data_.resize(most_significant_dimension() * dimensions_.front());
			}

			template<std::size_t... OtherDimensions>
			multidimensional_array(const constant_multidimensional_array<T, OtherDimensions...>& other, const Allocator& alloc = Allocator{})
				: data_{alloc}, dimensions_{OtherDimensions...}, dimensions_cache_(sizeof...(OtherDimensions) == 1 ? 1 : sizeof...(OtherDimensions) - 1)
			{
				fill_cache();
				data_.resize(most_significant_dimension() * dimensions_.front());
				std::copy(other.begin(), other.end(), begin());
			}

			template<std::size_t OtherDimensions>
			multidimensional_array(const fixed_multidimensional_array<T, OtherDimensions>& other, const Allocator& alloc = Allocator{})
				: data_{alloc}, dimensions_(OtherDimensions), dimensions_cache_(OtherDimensions == 1 ? 1 : OtherDimensions - 1)
			{
				std::ranges::copy(other.dimensions(), std::ranges::begin(dimensions_));
				fill_cache();
				data_.resize(most_significant_dimension() * dimensions_.front());
				std::copy(other.begin(), other.end(), begin());
			}

			multidimensional_array(const multidimensional_array&) = default;
			multidimensional_array(multidimensional_array&&) noexcept = default;

			template<std::convertible_to<value_type> T2>
			multidimensional_array& operator=(const multidimensional_array<T2, Allocator>&other)
			{
				if (this != std::addressof(other))
				{
					data_ = other.data_;
					dimensions_ = other.dimensions_;
					dimensions_cache_ = other.dimensions_cache_;
				}

				return *this;
			}

			template<std::convertible_to<value_type> T2, std::size_t... OtherDimensions>
			multidimensional_array& operator=(const constant_multidimensional_array<T2, OtherDimensions...>& other)
			{
				dimensions_ = {OtherDimensions...};
				if constexpr (sizeof...(OtherDimensions) == 1)
				{
					dimensions_cache_.resize(1);
				}
				else
				{
					dimensions_cache_.resize(sizeof...(OtherDimensions) - 1);
				}
				fill_cache();
				data_.resize(most_significant_dimension() * dimensions_.front());
				std::copy(other.begin(), other.end(), begin());
				return *this;
			}

			template<std::convertible_to<T> T2, std::size_t OtherDimensions>
			multidimensional_array& operator=(const fixed_multidimensional_array<T2, OtherDimensions>& other)
			{
				dimensions_.resize(OtherDimensions);
				std::ranges::copy(other.dimensions(), std::ranges::begin(dimensions_));
				if constexpr (OtherDimensions == 1)
				{
					dimensions_cache_.resize(1);
				}
				else
				{
					dimensions_cache_.resize(OtherDimensions - 1);
				}
				fill_cache();
				data_.resize(most_significant_dimension() * dimensions_.front());
				std::ranges::copy(other, begin());
				return *this;
			}
				
			multidimensional_array& operator=(const multidimensional_array&) = default;
			multidimensional_array& operator=(multidimensional_array&&) noexcept = default;

			multidimensional_array& operator=(std::initializer_list<T> elements)
			{
				std::ranges::copy(elements, begin());
				std::fill(begin() + std::ranges::size(elements), end(), T{});
				return *this;
			}

			[[nodiscard]] const Allocator& get_allocator() const noexcept
			{
				return data_.get_allocator();
			}

			[[nodiscard]] const dimensions_t& dimensions() const noexcept
			{
				return dimensions_;
			}

			template<class... Elements>
				requires(sizeof...(Elements) > 0)
			void dimensions(Elements&&... elements)
			{
				dimensions_.clear();
				dimensions_.reserve(sizeof...(Elements));
				(dimensions_.push_back(std::forward<Elements>(elements)), ...);
				if constexpr (sizeof...(Elements) == 1)
				{
					dimensions_cache_.resize(1);
				}
				else
				{
					dimensions_cache_.resize(sizeof...(Elements) - 1);
				}
				fill_cache();
				data_.resize(most_significant_dimension() * dimensions_.front());
			}

			template<class... Indexes>
			reference get(Indexes&&... indexes) noexcept
			{
				static_assert((sizeof...(Indexes) > 0));
				return data_[index(std::forward<Indexes>(indexes)...)];
			}

			template<class... Indexes>
			const_reference get(Indexes&&... indexes) const noexcept
			{
				static_assert((sizeof...(Indexes) > 0));
				return data_[index(std::forward<Indexes>(indexes)...)];
			}

			reference operator[](std::size_t index) noexcept
			{
				return data_[index];
			}

			const_reference operator[](std::size_t index) const noexcept
			{
				return data_[index];
			}

			[[nodiscard]] pointer data() const noexcept
			{
				return data_.data();
			}

			[[nodiscard]] pointer data() noexcept
			{
				return data_.data();
			}

			[[nodiscard]] iterator begin() noexcept
			{
				return std::ranges::begin(data_);
			}

			[[nodiscard]] const_iterator begin() const noexcept
			{
				return std::ranges::begin(data_);
			}

			[[nodiscard]] const_iterator cbegin() const noexcept
			{
				return std::ranges::cbegin(data_);
			}

			[[nodiscard]] iterator end() noexcept
			{
				return std::ranges::end(data_);
			}

			[[nodiscard]] const_iterator end() const noexcept
			{
				return std::ranges::end(data_);
			}

			[[nodiscard]] const_iterator cend() const noexcept
			{
				return std::ranges::cend(data_);
			}

			[[nodiscard]] reverse_iterator rbegin() noexcept
			{
				return std::ranges::rbegin(data_);
			}

			[[nodiscard]] reverse_iterator rbegin() const noexcept
			{
				return std::ranges::rbegin(data_);
			}

			[[nodiscard]] const_reverse_iterator crbegin() const noexcept
			{
				return std::ranges::crbegin(data_);
			}

			[[nodiscard]] reverse_iterator rend() noexcept
			{
				return std::ranges::rend(data_);
			}

			[[nodiscard]] reverse_iterator rend() const noexcept
			{
				return std::ranges::rend(data_);
			}

			[[nodiscard]] const_reverse_iterator crend() const noexcept
			{
				return std::ranges::crend(data_);
			}

			[[nodiscard]] size_type size() const noexcept
			{
				return std::ranges::size(data_);
			}

			void assign(size_type count, const value_type& value)
			{
				std::ranges::fill_n(begin(), std::min(count, size()), value);
			}

			void swap(multidimensional_array& other) noexcept
			{
				std::swap(data_, other.data_);
				std::swap(dimensions_, other.dimensions_);
				std::swap(dimensions_cache_, other.dimensions_cache_);
			}
		private:
			storage_t data_;
			dimensions_t dimensions_;
			dimensions_t dimensions_cache_;

			template<std::size_t Index = 0, class... Indexes>
			inline std::size_t index(std::size_t current_index, Indexes&&... indexes) const noexcept
			{
				return current_index * dimensions_cache_[Index] + index<Index + 1>(std::forward<Indexes>(indexes)...);
			}

			template<std::size_t TupleIndex>
			inline std::size_t index(std::size_t current_index) const noexcept
			{
				return current_index;
			}

			inline void fill_cache() noexcept
			{
				if (std::size(dimensions_) == 1)
				{
					dimensions_cache_.push_back(1);
				}
				else
				{
					std::partial_sum(std::ranges::crbegin(dimensions_), 
									 std::ranges::crend(dimensions_) - 1, 
									 std::ranges::rbegin(dimensions_cache_), 
									 std::multiplies<>{});
				}
			}

			[[nodiscard]] std::size_t most_significant_dimension() const noexcept
			{
				if (dimensions_cache_.empty())
				{
					return 1;
				}

				return dimensions_cache_.front();
			}
	};

	// Fixed in dimensions, variable in size
	template<class T, std::size_t Dimensions, class Allocator>
	class fixed_multidimensional_array
	{
		static_assert(Dimensions > 0);

		struct empty {};

		using storage_t = std::vector<T, Allocator>;
		using dimensions_t = std::array<std::size_t, Dimensions>;

		static constexpr bool no_dimension_cache = Dimensions == 1;
		static constexpr bool simple_dimension_cache = Dimensions == 2;
		static constexpr bool complex_dimension_cache = !no_dimension_cache && !simple_dimension_cache;

		using dimensions_cache_t = std::conditional_t<no_dimension_cache || simple_dimension_cache,
										empty,
										std::array<std::size_t, Dimensions - 1>
								   >;

		public:
			using value_type = typename storage_t::value_type;
			using allocator_type = typename storage_t::allocator_type;
			using size_type = typename storage_t::size_type;
			using difference_type = typename storage_t::difference_type;
			using reference = typename storage_t::reference;
			using const_reference = typename storage_t::const_reference;
			using pointer = typename storage_t::pointer;
			using const_pointer = typename storage_t::const_pointer;
			using iterator = typename storage_t::iterator;
			using const_iterator = typename storage_t::const_iterator;
			using reverse_iterator = typename storage_t::reverse_iterator;
			using const_reverse_iterator = typename storage_t::const_reverse_iterator;

			static_assert(std::same_as<T, typename std::allocator_traits<allocator_type>::value_type>);

			fixed_multidimensional_array(std::initializer_list<std::size_t> dimensions, const Allocator& alloc = Allocator{})
				: data_{alloc}
			{
				std::ranges::copy(dimensions, std::ranges::begin(dimensions_));
				fill_cache();
				data_.resize(most_significant_dimension() * dimensions_.front());
			}

			template<std::convertible_to<value_type> T2, std::size_t... OtherDimensions>
			fixed_multidimensional_array(const constant_multidimensional_array<T2, OtherDimensions...>& other, const Allocator& alloc = Allocator{})
				: data_{alloc}, dimensions_{OtherDimensions...}
			{
				fill_cache();
				data_.resize(most_significant_dimension() * dimensions_.front());
				std::ranges::copy(other, begin());
			}

			fixed_multidimensional_array(const fixed_multidimensional_array&) = default;
			fixed_multidimensional_array(fixed_multidimensional_array&&) noexcept = default;

			fixed_multidimensional_array& operator=(const fixed_multidimensional_array&) = default;
			fixed_multidimensional_array& operator=(fixed_multidimensional_array&&) noexcept = default;

			fixed_multidimensional_array& operator=(std::initializer_list<value_type> elements) noexcept(std::is_nothrow_copy_assignable_v<value_type> && std::is_nothrow_default_constructible_v<value_type>)
			{
				std::ranges::copy(elements, begin());
				std::fill(begin() + std::ranges::size(elements), end(), value_type{});
				return *this;
			}

			template<std::size_t... OtherDimensions>
			fixed_multidimensional_array& operator=(const constant_multidimensional_array<value_type, OtherDimensions...>& other)
			{
				dimensions_ = {OtherDimensions...};
				fill_cache();
				data_.resize(most_significant_dimension() * dimensions_.front());
				std::ranges::copy(other, begin());
				return *this;
			}

			[[nodiscard]] const dimensions_t& dimensions() const noexcept
			{
				return dimensions_;
			}

			[[nodiscard]] const Allocator get_allocator() const noexcept
			{
				return data_.get_allocator();
			}

			template<class... Elements>
				requires(sizeof...(Elements) > 0)
			void dimensions(Elements&&... elements)
			{
				static_assert((sizeof...(Elements) <= Dimensions));

				dimensions_ = {elements...};
				fill_cache();
				data_.resize(most_significant_dimension() * dimensions_.front());
			}

			template<class... Indexes>
			reference get(Indexes&&... indexes) noexcept
			{
				static_assert((sizeof...(Indexes) > 0) && (sizeof...(Indexes) <= Dimensions));
				return data_[index(std::forward<Indexes>(indexes)...)];
			}

			template<class... Indexes>
			const_reference get(Indexes&&... indexes) const noexcept
			{
				static_assert((sizeof...(Indexes) > 0) && (sizeof...(Indexes) <= Dimensions));
				return data_[index(std::forward<Indexes>(indexes)...)];
			}

			reference operator[](std::size_t index) noexcept
			{
				return data_[index];
			}

			const_reference operator[](std::size_t index) const noexcept
			{
				return data_[index];
			}

			[[nodiscard]] const_pointer data() const noexcept
			{
				return data_.data();
			}

			[[nodiscard]] pointer data() noexcept
			{
				return data_.data();
			}

			[[nodiscard]] iterator begin() noexcept
			{
				return std::ranges::begin(data_);
			}

			[[nodiscard]] const_iterator begin() const noexcept
			{
				return std::ranges::begin(data_);
			}

			[[nodiscard]] const_iterator cbegin() const noexcept
			{
				return std::ranges::cbegin(data_);
			}

			[[nodiscard]] iterator end() noexcept
			{
				return std::ranges::end(data_);
			}

			[[nodiscard]] const_iterator end() const noexcept
			{
				return std::ranges::end(data_);
			}

			[[nodiscard]] const_iterator cend() const noexcept
			{
				return std::ranges::cend(data_);
			}

			[[nodiscard]] reverse_iterator rbegin() noexcept
			{
				return std::ranges::rbegin(data_);
			}

			[[nodiscard]] reverse_iterator rbegin() const noexcept
			{
				return std::ranges::rbegin(data_);
			}

			[[nodiscard]] const_reverse_iterator crbegin() const noexcept
			{
				return std::ranges::crbegin(data_);
			}

			[[nodiscard]] reverse_iterator rend() noexcept
			{
				return std::ranges::rend(data_);
			}

			[[nodiscard]] reverse_iterator rend() const noexcept
			{
				return std::ranges::rend(data_);
			}

			[[nodiscard]] const_reverse_iterator crend() const noexcept
			{
				return std::ranges::crend(data_);
			}

			[[nodiscard]] size_type size() const noexcept
			{
				return std::ranges::size(data_);
			}

			void swap(fixed_multidimensional_array& other) noexcept
			{
				std::swap(data_, other.data_);
				std::swap(dimensions_, other.dimensions_);
				if constexpr (complex_dimension_cache)
				{
					std::swap(dimensions_cache_, other.dimensions_cache_);
				}
			}
		private:
			storage_t data_;
			dimensions_t dimensions_ {};
			dimensions_cache_t dimensions_cache_;

			template<std::size_t Index = 0, class... Indexes>
			[[nodiscard]] inline std::size_t index(std::size_t current_index, Indexes&&... indexes) const noexcept
			{
				if constexpr (no_dimension_cache)
				{
					current_index;
				}
				else if constexpr (simple_dimension_cache)
				{
					return current_index * dimensions_.back() + index<Index + 1>(std::forward<Indexes>(indexes)...);
				}
				else
				{
					return current_index * dimensions_cache_[Index] + index<Index + 1>(std::forward<Indexes>(indexes)...);
				}
			}

			template<std::size_t TupleIndex>
			[[nodiscard]] inline std::size_t index(std::size_t current_index) const noexcept
			{
				return current_index;
			}

			inline void fill_cache() noexcept
			{
				if constexpr (complex_dimension_cache)
				{
					std::partial_sum(std::ranges::crbegin(dimensions_), 
									 std::ranges::crend(dimensions_) - 1, 
									 std::ranges::rbegin(dimensions_cache_), 
									 std::multiplies<>{});
				}
			}

			[[nodiscard]] std::size_t most_significant_dimension() const noexcept
			{
				if constexpr (no_dimension_cache)
				{
					return 1;
				}
				else if constexpr (simple_dimension_cache)
				{
					return dimensions_.back();
				}
				else
				{
					return dimensions_cache_.front();
				}
			}
	};

	// Fixed in dimensions and size
	template<class T, std::size_t... Dimensions>
		requires(sizeof...(Dimensions) > 0)
	class constant_multidimensional_array
	{
		static_assert((... && (Dimensions > 0)));

		static constexpr std::size_t size_ = (... * Dimensions);
		using storage_t = std::array<T, size_>;
		static constexpr auto dimensions_ = std::array{Dimensions...};
		static constexpr auto dimensions_size = sizeof...(Dimensions);

		public:
			using value_type = typename storage_t::value_type;
			using size_type = typename storage_t::size_type;
			using difference_type = typename storage_t::difference_type;
			using reference = typename storage_t::reference;
			using const_reference = typename storage_t::const_reference;
			using pointer = typename storage_t::pointer;
			using const_pointer = typename storage_t::const_pointer;
			using iterator = typename storage_t::iterator;
			using const_iterator = typename storage_t::const_iterator;
			using reverse_iterator = typename storage_t::reverse_iterator;
			using const_reverse_iterator = typename storage_t::const_reverse_iterator;

			constexpr constant_multidimensional_array() noexcept(std::is_nothrow_default_constructible_v<value_type>) {}

			template<std::convertible_to<value_type> T2>
			constexpr constant_multidimensional_array(const constant_multidimensional_array<T2>& other) noexcept(std::is_nothrow_copy_constructible_v<value_type>)
				: data_{other.data_} {}

			template<std::convertible_to<value_type>... Elements>
			constexpr constant_multidimensional_array(Elements&&... elements) noexcept(std::is_nothrow_constructible_v<value_type>)
				: data_{static_cast<value_type>(std::forward<Elements>(elements))...} {}

			constexpr constant_multidimensional_array(const constant_multidimensional_array&) noexcept(std::is_nothrow_copy_constructible_v<value_type>) = default;
			constexpr constant_multidimensional_array(constant_multidimensional_array&&) noexcept = default;

			template<std::convertible_to<value_type> T2>
			constexpr constant_multidimensional_array& operator=(const constant_multidimensional_array<T2>& other) noexcept(std::is_nothrow_copy_assignable_v<value_type>)
			{
				if (this != std::addressof(other))
				{
					data_ = other.data_;
				}

				return *this;
			}

			constexpr constant_multidimensional_array& operator=(const constant_multidimensional_array&) noexcept(std::is_nothrow_copy_assignable_v<value_type>) = default;
			constexpr constant_multidimensional_array& operator=(constant_multidimensional_array&&) noexcept = default;

			[[nodiscard]] constexpr const auto& dimensions() const noexcept
			{
				return dimensions_;
			}

			template<class... Indexes>
				requires(sizeof...(Indexes) == dimensions_size)
			constexpr reference get(Indexes&&... indexes) noexcept
			{
				return data_[index(std::forward<Indexes>(indexes)...)];
			}

			template<class... Indexes>
				requires(sizeof...(Indexes) == dimensions_size)
			constexpr const_reference get(Indexes&&... indexes) const noexcept
			{
				return data_[index(std::forward<Indexes>(indexes)...)];
			}

			constexpr reference operator[](std::size_t index) noexcept
			{
				return data_[index];
			}

			constexpr const_reference operator[](std::size_t index) const noexcept
			{
				return data_[index];
			}

			[[nodiscard]] constexpr const_pointer data() const noexcept
			{
				return data_.data();
			}

			[[nodiscard]] constexpr pointer data() noexcept
			{
				return data_.data();
			}

			[[nodiscard]] constexpr iterator begin() noexcept
			{
				return std::ranges::begin(data_);
			}

			[[nodiscard]] constexpr const_iterator begin() const noexcept
			{
				return std::ranges::begin(data_);
			}

			[[nodiscard]] constexpr const_iterator cbegin() const noexcept
			{
				return std::ranges::cbegin(data_);
			}

			[[nodiscard]] constexpr iterator end() noexcept
			{
				return std::ranges::end(data_);
			}

			[[nodiscard]] constexpr const_iterator end() const noexcept
			{
				return std::ranges::end(data_);
			}

			[[nodiscard]] constexpr const_iterator cend() const noexcept
			{
				return std::ranges::cend(data_);
			}

			[[nodiscard]] constexpr reverse_iterator rbegin() noexcept
			{
				return std::ranges::rbegin(data_);
			}

			[[nodiscard]] constexpr reverse_iterator rbegin() const noexcept
			{
				return std::ranges::rbegin(data_);
			}

			[[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept
			{
				return std::ranges::crbegin(data_);
			}

			[[nodiscard]] constexpr reverse_iterator rend() noexcept
			{
				return std::ranges::rend(data_);
			}

			[[nodiscard]] constexpr reverse_iterator rend() const noexcept
			{
				return std::ranges::rend(data_);
			}

			[[nodiscard]] constexpr const_reverse_iterator crend() const noexcept
			{
				return std::ranges::crend(data_);
			}

			[[nodiscard]] constexpr size_type size() const noexcept
			{
				return constant_multidimensional_array::size_;
			}

			void swap(constant_multidimensional_array& other) noexcept
			{
				std::swap(data_, other.data_);
			}
		private:
			storage_t data_ {};

			template<std::size_t Index = 0, class... Indexes>
			[[nodiscard]] static constexpr std::size_t index(std::size_t current_index, Indexes&&... indexes) noexcept
			{
				return current_index * dimension_multiplier<Index + 1>::value
					+ index<Index + 1>(std::forward<Indexes>(indexes)...);
			}

			template<std::size_t TupleIndex>
			[[nodiscard]] static constexpr std::size_t index(std::size_t current_index) noexcept
			{
				return current_index;
			}

			template<std::size_t Index>
			struct dimension_multiplier
			{
				static constexpr std::size_t value {std::get<Index>(dimensions_) * dimension_multiplier<Index + 1>::value};
			};

			template<>
			struct dimension_multiplier<dimensions_size>
			{
				static constexpr std::size_t value {1};
			};
	};
}

namespace std
{
	template<class T, class Allocator>
	void swap(cmoon::multidimensional_array<T, Allocator>& lhs, cmoon::multidimensional_array<T, Allocator>& rhs) noexcept
	{
		lhs.swap(rhs);
	}

	template<class T, std::size_t Dimensions, class Allocator>
	void swap(cmoon::fixed_multidimensional_array<T, Dimensions, Allocator>& lhs, cmoon::fixed_multidimensional_array<T, Dimensions, Allocator>& rhs) noexcept
	{
		lhs.swap(rhs);
	}

	template<class T, std::size_t... Dimensions>
	void swap(cmoon::constant_multidimensional_array<T, Dimensions...>& lhs, cmoon::constant_multidimensional_array<T, Dimensions...>& rhs) noexcept
	{
		lhs.swap(rhs);
	}
}
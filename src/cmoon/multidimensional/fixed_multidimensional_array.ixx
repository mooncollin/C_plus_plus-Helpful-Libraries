export module cmoon.multidimensional.fixed_multidimensional_array;

import <memory>;
import <initializer_list>;
import <vector>;
import <ranges>;
import <array>;
import <type_traits>;
import <algorithm>;

import "cmoon/multidimensional/container_definitions.hpp";

namespace cmoon
{
	export
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

		using dimensions_cache_t = std::conditional_t<complex_dimension_cache,
										std::array<std::size_t, Dimensions - 1>,
										empty
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
			fixed_multidimensional_array(const static_multidimensional_array<T2, OtherDimensions...>& other, const Allocator& alloc = Allocator{})
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

			fixed_multidimensional_array& operator=(std::initializer_list<value_type> elements)
			{
				auto left = std::copy_n(std::begin(elements), std::min(std::size(elements), size()), begin());
				std::fill(left, end(), value_type{});
				return *this;
			}

			template<std::size_t... OtherDimensions>
			fixed_multidimensional_array& operator=(const static_multidimensional_array<value_type, OtherDimensions...>& other)
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
				return std::begin(data_);
			}

			[[nodiscard]] const_iterator begin() const noexcept
			{
				return std::begin(data_);
			}

			[[nodiscard]] const_iterator cbegin() const noexcept
			{
				return std::cbegin(data_);
			}

			[[nodiscard]] iterator end() noexcept
			{
				return std::end(data_);
			}

			[[nodiscard]] const_iterator end() const noexcept
			{
				return std::end(data_);
			}

			[[nodiscard]] const_iterator cend() const noexcept
			{
				return std::cend(data_);
			}

			[[nodiscard]] reverse_iterator rbegin() noexcept
			{
				return std::rbegin(data_);
			}

			[[nodiscard]] reverse_iterator rbegin() const noexcept
			{
				return std::rbegin(data_);
			}

			[[nodiscard]] const_reverse_iterator crbegin() const noexcept
			{
				return std::crbegin(data_);
			}

			[[nodiscard]] reverse_iterator rend() noexcept
			{
				return std::rend(data_);
			}

			[[nodiscard]] reverse_iterator rend() const noexcept
			{
				return std::rend(data_);
			}

			[[nodiscard]] const_reverse_iterator crend() const noexcept
			{
				return std::crend(data_);
			}

			[[nodiscard]] size_type size() const noexcept
			{
				return std::size(data_);
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
					std::partial_sum(std::crbegin(dimensions_), 
									 std::crend(dimensions_) - 1, 
									 std::rbegin(dimensions_cache_), 
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
}

namespace std
{
	export
	template<class T, std::size_t Dimensions, class Allocator>
	void swap(cmoon::fixed_multidimensional_array<T, Dimensions, Allocator>& lhs, cmoon::fixed_multidimensional_array<T, Dimensions, Allocator>& rhs) noexcept
	{
		lhs.swap(rhs);
	}
}
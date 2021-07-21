export module cmoon.multidimensional.multidimensional_array;

import <memory>;
import <initializer_list>;
import <vector>;
import <ranges>;
import <numeric>;
import <concepts>;

import cmoon.multidimensional.fixed_multidimensional_array;
import cmoon.multidimensional.static_multidimensional_array;

namespace cmoon
{
	export
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
			multidimensional_array(const static_multidimensional_array<T, OtherDimensions...>& other, const Allocator& alloc = Allocator{})
				: data_{alloc}, dimensions_{OtherDimensions...}, dimensions_cache_(sizeof...(OtherDimensions) == 1 ? 1 : sizeof...(OtherDimensions) - 1)
			{
				fill_cache();
				data_.resize(most_significant_dimension() * dimensions_.front());
				std::copy(other.begin(), other.end(), begin());
			}

			template<std::size_t OtherDimensions, class OtherAllocator>
			multidimensional_array(const fixed_multidimensional_array<T, OtherDimensions, OtherAllocator>& other, const Allocator& alloc = Allocator{})
				: data_{alloc}, dimensions_(OtherDimensions), dimensions_cache_(OtherDimensions == 1 ? 1 : OtherDimensions - 1)
			{
				std::ranges::copy(other.dimensions(), std::ranges::begin(dimensions_));
				fill_cache();
				data_.resize(most_significant_dimension() * dimensions_.front());
				std::copy(other.begin(), other.end(), begin());
			}

			template<std::convertible_to<value_type> T2>
			multidimensional_array& operator=(const multidimensional_array<T2, Allocator>& other)
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
			multidimensional_array& operator=(const static_multidimensional_array<T2, OtherDimensions...>& other)
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

			template<std::convertible_to<T> T2, std::size_t OtherDimensions, class OtherAllocator>
			multidimensional_array& operator=(const fixed_multidimensional_array<T2, OtherDimensions, OtherAllocator>& other)
			{
				dimensions_.resize(OtherDimensions);
				std::ranges::copy(other.dimensions(), std::begin(dimensions_));
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

			multidimensional_array& operator=(std::initializer_list<T> elements)
			{
				std::ranges::copy(elements, begin());
				std::fill(begin() + std::size(elements), end(), T{});
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

			void assign(size_type count, const value_type& value)
			{
				std::fill_n(begin(), std::min(count, size()), value);
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
					std::partial_sum(std::crbegin(dimensions_), 
									 std::crend(dimensions_) - 1, 
									 std::rbegin(dimensions_cache_), 
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
}

namespace std
{
	export
	template<class T, class Allocator>
	void swap(cmoon::multidimensional_array<T, Allocator>& lhs, cmoon::multidimensional_array<T, Allocator>& rhs) noexcept
	{
		lhs.swap(rhs);
	}
}
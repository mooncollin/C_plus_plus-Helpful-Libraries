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

namespace collin
{
	namespace multidimensional
	{
		template<class T, std::size_t... Dimensions>
		class constant_multidimensional_array;

		template<class T, std::size_t Dimensions, class Allocator = std::allocator<T>>
		class fixed_multidimensional_array;

		// Totally dynamic in dimensions and size
		template<class T, class Allocator = std::allocator<T>>
		class multidimensional_array
		{
			using storage_t = std::vector<T, Allocator>;
			using dimensions_t = std::vector<std::size_t>;

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

				multidimensional_array(std::initializer_list<std::size_t> dimensions, const Allocator& alloc = Allocator{})
					: data_{alloc}, dimensions_{dimensions}, dimensions_cache_(std::size(dimensions) - 1)
				{
					fill_cache();
					data_.resize(dimensions_cache_.front() * dimensions_.front());
				}

				template<class T2, typename = std::enable_if_t<
		/* requires */	std::is_convertible_v<T2, T>
				>>
				multidimensional_array(const multidimensional_array<T2, Allocator>& other)
					: data_{other.data_}, dimensions_{other.dimensions_}, dimensions_cache_{other.dimensions_cache_} {}

				template<class T2, std::size_t... OtherDimensions>
				multidimensional_array(const constant_multidimensional_array<T2, OtherDimensions...>& other, const Allocator& alloc = Allocator{})
					: data_{alloc}, dimensions_{OtherDimensions...}, dimensions_cache_(sizeof...(OtherDimensions) - 1)
				{
					fill_cache();
					data_.resize(dimensions_cache_.front() * dimensions_.front());
					std::copy(std::begin(other), std::end(other), begin());
				}

				template<class T2, std::size_t OtherDimensions>
				multidimensional_array(const fixed_multidimensional_array<T2, OtherDimensions>& other, const Allocator& alloc = Allocator{})
					: data_{alloc}, dimensions_(OtherDimensions), dimensions_cache_(OtherDimensions - 1)
				{
					std::copy(std::begin(other.dimensions()), std::end(other.dimensions()), std::begin(dimensions_));
					fill_cache();
					data_.resize(dimensions_cache_.front() * dimensions_.front());
					std::copy(std::begin(other), std::end(other), begin());
				}

				multidimensional_array(const multidimensional_array&) = default;
				multidimensional_array(multidimensional_array&&) noexcept = default;

				template<class T2, typename = std::enable_if_t<
		/* requires */	std::is_convertible_v<T2, T>
				>>
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

				template<class T2, std::size_t... OtherDimensions>
				multidimensional_array& operator=(const constant_multidimensional_array<T2, OtherDimensions...>& other)
				{
					dimensions_ = {OtherDimensions...};
					dimensions_cache_.resize(sizeof...(OtherDimensions) - 1);
					fill_cache();
					data_.resize(dimensions_cache_.front() * dimensions_.front());
					std::copy(std::begin(other), std::end(other), begin());
					return *this;
				}

				template<class T2, std::size_t OtherDimensions>
				multidimensional_array& operator=(const fixed_multidimensional_array<T2, OtherDimensions>& other)
				{
					dimensions_.resize(OtherDimensions);
					std::copy(std::begin(other.dimensions()), std::end(other.dimensions()), std::begin(dimensions_));
					dimensions_cache_.resize(OtherDimensions - 1);
					fill_cache();
					data_.resize(dimensions_cache_.front() * dimensions_.front());
					std::copy(std::begin(other), std::end(other), begin());
					return *this;
				}
				
				multidimensional_array& operator=(const multidimensional_array&) = default;
				multidimensional_array& operator=(multidimensional_array&&) noexcept = default;

				multidimensional_array& operator=(std::initializer_list<T> elements)
				{
					std::copy(std::begin(elements), std::end(elements), std::begin(data_));
					std::fill(std::begin(data_) + std::size(elements), std::end(data_), T{});
					return *this;
				}

				const dimensions_t& dimensions() const noexcept
				{
					return dimensions_;
				}

				template<class... Elements, typename = std::enable_if_t<
		/* requires */ (sizeof...(Elements) > 0)
				>>
				void dimensions(Elements&&... elements)
				{
					dimensions_.clear();
					dimensions_.reserve(sizeof...(Elements));
					(dimensions_.push_back(std::forward<Elements>(elements)), ...);
					dimensions_cache_.resize(sizeof...(Elements) - 1);
					fill_cache();
					data_.resize(dimensions_cache_.front() * dimensions_.front());
				}

				template<class... Indexes>
				reference get(Indexes&&... indexes)
				{
					static_assert((sizeof...(Indexes) > 0));
					return data_[index(std::forward<Indexes>(indexes)...)];
				}

				template<class... Indexes>
				const_reference get(Indexes&&... indexes) const
				{
					static_assert((sizeof...(Indexes) > 0));
					return data_[index(std::forward<Indexes>(indexes)...)];
				}

				pointer data() const noexcept
				{
					return data_.data();
				}

				pointer data() noexcept
				{
					return data_.data();
				}

				iterator begin() noexcept
				{
					return std::begin(data_);
				}

				const_iterator begin() const noexcept
				{
					return std::begin(data_);
				}

				const_iterator cbegin() const noexcept
				{
					return std::cbegin(data_);
				}

				iterator end() noexcept
				{
					return std::end(data_);
				}

				const_iterator end() const noexcept
				{
					return std::end(data_);
				}

				const_iterator cend() const noexcept
				{
					return std::cend(data_);
				}

				reverse_iterator rbegin() noexcept
				{
					return std::rbegin(data_);
				}

				reverse_iterator rbegin() const noexcept
				{
					return std::rbegin(data_);
				}

				const_reverse_iterator crbegin() const noexcept
				{
					return std::crbegin(data_);
				}

				reverse_iterator rend() noexcept
				{
					return std::rend(data_);
				}

				reverse_iterator rend() const noexcept
				{
					return std::rend(data_);
				}

				const_reverse_iterator crend() const noexcept
				{
					return std::crend(data_);
				}

				size_type size() const noexcept
				{
					return std::size(data_);
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
					auto rd_it = std::crbegin(dimensions_);
					auto current_size {*rd_it};
					for(auto rdc_it = std::rbegin(dimensions_cache_); rdc_it != std::rend(dimensions_cache_); ++rdc_it)
					{
						*rdc_it = current_size;
						current_size *= *(++rd_it);
					}
				}
		};

		// Fixed in dimensions, variable in size
		template<class T, std::size_t Dimensions, class Allocator>
		class fixed_multidimensional_array
		{
			static_assert(Dimensions > 0);

			using storage_t = std::vector<T, Allocator>;
			using dimensions_t = std::array<std::size_t, Dimensions>;

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

				fixed_multidimensional_array(std::initializer_list<std::size_t> dimensions, const Allocator& alloc = Allocator{})
					: data_{alloc}
				{
					std::copy(std::begin(dimensions), std::end(dimensions), std::begin(dimensions_));
					fill_cache();
					data_.resize(dimensions_cache_.front() * dimensions_.front());
				}

				template<class T2, typename = std::enable_if_t<
		/* requires */	std::is_convertible_v<T2, T>
				>>
				fixed_multidimensional_array(const fixed_multidimensional_array<T2, Dimensions, Allocator>& other)
					: data_{other.data_}, dimensions_{other.dimensions_}, dimensions_cache_{other.dimensions_cache_} {}

				template<class T2, std::size_t... OtherDimensions>
				fixed_multidimensional_array(const constant_multidimensional_array<T2, OtherDimensions...>& other, const Allocator& alloc = Allocator{})
					: data_{alloc}, dimensions_{OtherDimensions...}
				{
					fill_cache();
					data_.resize(dimensions_cache_.front() * dimensions_.front());
					std::copy(std::begin(other), std::end(other), begin());
				}

				fixed_multidimensional_array(const fixed_multidimensional_array&) = default;
				fixed_multidimensional_array(fixed_multidimensional_array&&) noexcept = default;

				template<class T2, typename = std::enable_if_t<
		/* requires */	std::is_convertible_v<T2, T>
				>>
				fixed_multidimensional_array& operator=(const fixed_multidimensional_array<T2, Dimensions, Allocator>& other)
				{
					if (this != std::addressof(other))
					{
						data_ = other.data_;
						dimensions_ = other.dimensions_;
						dimensions_cache_ = other.dimensions_cache_;
					}

					return *this;
				}

				fixed_multidimensional_array& operator=(const fixed_multidimensional_array&) = default;
				fixed_multidimensional_array& operator=(fixed_multidimensional_array&&) noexcept = default;

				fixed_multidimensional_array& operator=(std::initializer_list<T> elements)
				{
					std::copy(std::begin(elements), std::end(elements), std::begin(data_));
					std::fill(std::begin(data_) + std::size(elements), std::end(data_), T{});
					return *this;
				}

				template<class T2, std::size_t... OtherDimensions>
				fixed_multidimensional_array& operator=(const constant_multidimensional_array<T2, OtherDimensions...>& other)
				{
					dimensions_ = {OtherDimensions...};
					fill_cache();
					data_.resize(dimensions_cache_.front() * dimensions_.front());
					std::copy(std::begin(other), std::end(other), begin());
					return *this;
				}

				const dimensions_t& dimensions() const noexcept
				{
					return dimensions_;
				}

				template<class... Elements, typename = std::enable_if_t<
		/* requires */ (sizeof...(Elements) > 0)
				>>
				void dimensions(Elements&&... elements)
				{
					static_assert((sizeof...(Elements) <= Dimensions));

					dimensions_ = {elements...};
					fill_cache();
					data_.resize(dimensions_cache_.front() * dimensions_.front());
				}

				template<class... Indexes>
				reference get(Indexes&&... indexes)
				{
					static_assert((sizeof...(Indexes) > 0) && (sizeof...(Indexes) <= Dimensions));
					return data_[index(std::forward<Indexes>(indexes)...)];
				}

				template<class... Indexes>
				const_reference get(Indexes&&... indexes) const
				{
					static_assert((sizeof...(Indexes) > 0) && (sizeof...(Indexes) <= Dimensions));
					return data_[index(std::forward<Indexes>(indexes)...)];
				}

				const_pointer data() const noexcept
				{
					return data_.data();
				}

				pointer data() noexcept
				{
					return data_.data();
				}

				iterator begin() noexcept
				{
					return std::begin(data_);
				}

				const_iterator begin() const noexcept
				{
					return std::begin(data_);
				}

				const_iterator cbegin() const noexcept
				{
					return std::cbegin(data_);
				}

				iterator end() noexcept
				{
					return std::end(data_);
				}

				const_iterator end() const noexcept
				{
					return std::end(data_);
				}

				const_iterator cend() const noexcept
				{
					return std::cend(data_);
				}

				reverse_iterator rbegin() noexcept
				{
					return std::rbegin(data_);
				}

				reverse_iterator rbegin() const noexcept
				{
					return std::rbegin(data_);
				}

				const_reverse_iterator crbegin() const noexcept
				{
					return std::crbegin(data_);
				}

				reverse_iterator rend() noexcept
				{
					return std::rend(data_);
				}

				reverse_iterator rend() const noexcept
				{
					return std::rend(data_);
				}

				const_reverse_iterator crend() const noexcept
				{
					return std::crend(data_);
				}

				size_type size() const noexcept
				{
					return std::size(data_);
				}
			private:
				storage_t data_;
				dimensions_t dimensions_ {};
				std::array<std::size_t, Dimensions - 1> dimensions_cache_ {};

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
					auto rd_it = std::crbegin(dimensions_);
					auto current_size {*rd_it};
					for(auto rdc_it = std::rbegin(dimensions_cache_); rdc_it != std::rend(dimensions_cache_); ++rdc_it)
					{
						*rdc_it = current_size;
						current_size *= *(++rd_it);
					}
				}
		};

		// Fixed in dimensions and size
		template<class T, std::size_t... Dimensions>
		class constant_multidimensional_array
		{
			static_assert((... && (Dimensions > 0)));

			static constexpr std::size_t size_ = (... * Dimensions);
			using storage_t = std::array<T, size_>;
			static constexpr auto dimensions_ = std::tuple{Dimensions...};
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

				constexpr constant_multidimensional_array() noexcept(std::is_nothrow_constructible_v<T>) {}

				template<class T2, typename = std::enable_if_t<
		/* requires */	std::is_convertible_v<T2, T>
				>>
				constexpr constant_multidimensional_array(const constant_multidimensional_array<T2>& other)
					: data_{other.data_} {}

				template<class... Elements, typename = std::enable_if_t<
		/* requires */	(... && std::is_convertible_v<Elements, T>)
				>>
				constexpr constant_multidimensional_array(Elements&&... elements) noexcept(std::is_nothrow_constructible_v<T>)
					: data_{static_cast<T>(std::forward<Elements>(elements))...} {}

				constexpr constant_multidimensional_array(const constant_multidimensional_array&) = default;
				constexpr constant_multidimensional_array(constant_multidimensional_array&&) noexcept = default;

				template<class T2, typename = std::enable_if_t<
		/* requires */	std::is_convertible_v<T2, T>
				>>
				constexpr constant_multidimensional_array& operator=(const constant_multidimensional_array<T2>& other)
				{
					if (this != std::addressof(other))
					{
						data_ = other.data_;
					}

					return *this;
				}

				constexpr constant_multidimensional_array& operator=(const constant_multidimensional_array&) = default;
				constexpr constant_multidimensional_array& operator=(constant_multidimensional_array&&) noexcept = default;

				constexpr const auto dimensions() const noexcept
				{
					return constant_multidimensional_array::dimensions_;
				}

				template<class... Indexes>
				constexpr reference get(Indexes&&... indexes)
				{
					static_assert(sizeof...(Indexes) == constant_multidimensional_array::dimensions_size);
					return data_[constant_multidimensional_array::index(std::forward<Indexes>(indexes)...)];
				}

				template<class... Indexes>
				constexpr const_reference get(Indexes&&... indexes) const
				{
					static_assert(sizeof...(Indexes) == constant_multidimensional_array::dimensions_size);
					return data_[constant_multidimensional_array::index(std::forward<Indexes>(indexes)...)];
				}

				constexpr const_pointer data() const noexcept
				{
					return data_.data();
				}

				constexpr pointer data() noexcept
				{
					return data_.data();
				}

				constexpr iterator begin() noexcept
				{
					return std::begin(data_);
				}

				constexpr const_iterator begin() const noexcept
				{
					return std::begin(data_);
				}

				constexpr const_iterator cbegin() const noexcept
				{
					return std::cbegin(data_);
				}

				constexpr iterator end() noexcept
				{
					return std::end(data_);
				}

				constexpr const_iterator end() const noexcept
				{
					return std::end(data_);
				}

				constexpr const_iterator cend() const noexcept
				{
					return std::cend(data_);
				}

				constexpr reverse_iterator rbegin() noexcept
				{
					return std::rbegin(data_);
				}

				constexpr reverse_iterator rbegin() const noexcept
				{
					return std::rbegin(data_);
				}

				constexpr const_reverse_iterator crbegin() const noexcept
				{
					return std::crbegin(data_);
				}

				constexpr reverse_iterator rend() noexcept
				{
					return std::rend(data_);
				}

				constexpr reverse_iterator rend() const noexcept
				{
					return std::rend(data_);
				}

				constexpr const_reverse_iterator crend() const noexcept
				{
					return std::crend(data_);
				}

				constexpr size_type size() const noexcept
				{
					return constant_multidimensional_array::size_;
				}
			private:
				storage_t data_ {};

				template<std::size_t Index = 0, class... Indexes>
				static constexpr std::size_t index(std::size_t current_index, Indexes&&... indexes) noexcept
				{
					return current_index * constant_multidimensional_array::dimension_multiplier<Index + 1>::value
						+ constant_multidimensional_array::index<Index + 1>(std::forward<Indexes>(indexes)...);
				}

				template<std::size_t TupleIndex>
				static constexpr std::size_t index(std::size_t current_index) noexcept
				{
					return current_index;
				}

				template<std::size_t Index>
				struct dimension_multiplier
				{
					static constexpr std::size_t value {std::get<Index>(constant_multidimensional_array::dimensions_) * dimension_multiplier<Index + 1>::value};
				};

				template<>
				struct dimension_multiplier<constant_multidimensional_array::dimensions_size>
				{
					static constexpr std::size_t value {1};
				};
		};
	}
}
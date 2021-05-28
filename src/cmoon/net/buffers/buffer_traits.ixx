export module cmoon.net.buffers.buffer_traits;

import <concepts>;
import <type_traits>;
import <cstddef>;
import <iterator>;

import cmoon.net.buffers.mutable_buffer;
import cmoon.net.buffers.const_buffer;

namespace cmoon::net
{
	template<class T>
	concept is_mutable_buffer_sequence_concept = std::destructible<T> &&
												 std::copy_constructible<T>;

	template<class T>
	concept is_iterator_reference_conv_const_buffer = std::convertible_to<typename std::iterator_traits<T>::reference, const_buffer> &&
													  std::bidirectional_iterator<T>;

	template<class T>
	concept is_const_buffer_sequence_concept = std::destructible<T> &&
											   std::copy_constructible<T> &&
		requires(T x, const T x1)
	{
		{ buffer_sequence_begin(x) } -> is_iterator_reference_conv_const_buffer<>;
		{ buffer_sequence_begin(x1) } -> is_iterator_reference_conv_const_buffer<>;
		{ buffer_sequence_end(x) } -> is_iterator_reference_conv_const_buffer<>;
		{ buffer_sequence_end(x1) } -> is_iterator_reference_conv_const_buffer<>;
	};

	template<class T>
	concept is_dynamic_buffer_concept = std::destructible<T> &&
										std::copy_constructible<T> &&
	requires(T x, const T x1, std::size_t pos, std::size_t n)
	{
		typename T::const_buffers_type;
		typename T::mutable_buffers_type;

		{ x1.size() } -> std::same_as<std::size_t>;
		{ x1.max_size() } -> std::same_as<std::size_t>;
		{ x1.capacity() } -> std::same_as<std::size_t>;
		{ x1.data(pos, n) } -> std::same_as<typename T::const_buffers_type>;
		{ x.data(pos, n) } -> std::same_as<typename T::mutable_buffers_type>;
		{ x.grow(n) };
		{ x.shrink(n) };
		{ x.consume(n) };
	} && 
		is_mutable_buffer_sequence_concept<typename T::mutable_buffers_type> &&
		is_const_buffer_sequence_concept<typename T::const_buffers_type>;

	export
	template<class T>
	struct is_mutable_buffer_sequence : std::bool_constant<is_mutable_buffer_sequence_concept<T>> {};

	export
	template<class T>
	inline constexpr auto is_mutable_buffer_sequence_v = is_mutable_buffer_sequence<T>::value;

	export
	template<class T>
	struct is_const_buffer_sequence : std::bool_constant<is_const_buffer_sequence_concept<T>> {};

	export
	template<class T>
	inline constexpr auto is_const_buffer_sequence_v = is_const_buffer_sequence<T>::value;

	export
	template<class T>
	struct is_dynamic_buffer : std::bool_constant<is_dynamic_buffer_concept<T>> {};

	export
	template<class T>
	inline constexpr auto is_dynamic_buffer_v = is_dynamic_buffer<T>::value;
}
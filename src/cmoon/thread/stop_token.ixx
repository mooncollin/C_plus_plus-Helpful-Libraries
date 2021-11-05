export module cmoon.thread.stop_token;

import <concepts>;
import <type_traits>;

import cmoon.concepts;

namespace cmoon
{
	template<template<class> class T>
	struct check_type_alias_exists {};

	export
	template<typename T>
	concept stoppable_token = std::copy_constructible<T> &&
							  std::move_constructible<T> &&
							  std::is_nothrow_copy_constructible_v<T> &&
							  std::is_nothrow_move_constructible_v<T> &&
							  std::equality_comparable<T> &&
		requires(const T& token)
	{
		{ token.stop_requested() } noexcept -> std::convertible_to<bool>;
		{ token.stop_possible() } noexcept -> std::convertible_to<bool>;
		typename check_type_alias_exists<T::template callback_type>;
	};

	export
	template<typename T, typename CB, typename Initializer = CB>
	concept stoppable_token_for = stoppable_token<T> &&
								  std::invocable<CB> &&
		requires
	{
		typename T::template callback_type<CB>;
	} &&
		std::constructible_from<CB, Initializer> &&
		std::constructible_from<typename T::template callback_type<CB>, T, Initializer> &&
		std::constructible_from<typename T::template callback_type<CB>, T&, Initializer> &&
		std::constructible_from<typename T::template callback_type<CB>, const T, Initializer> &&
		std::constructible_from<typename T::template callback_type<CB>, const T&, Initializer>;

	export
	template<typename T>
	concept unstoppable_token = stoppable_token<T> &&
		requires
	{
		{ T::stop_possible() } -> std::convertible_to<bool>;
	} && (!T::stop_possible());

	export
	struct never_stop_token
	{
		explicit constexpr never_stop_token() = default;
	};
}
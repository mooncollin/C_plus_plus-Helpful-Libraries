#ifndef COLLIN_TEST
#define COLLIN_TEST

#include <list>
#include <utility>
#include <sstream>
#include <string>
#include <stdexcept>
#include <type_traits>
#include <tuple>
#include <initializer_list>
#include <functional>
#include <iostream>
#include <string_view>
#include "collin_functional.hpp"
#include "collin_type_traits.hpp"

namespace collin
{
	template<class T, class F>
	void assert_equals(const T& first, const F& second, std::string_view message="")
	{
		if(first != second)
		{
			using namespace std::literals;
			throw std::runtime_error("Assertion Failed: "s + message.data());
		}
	}

	template<class T>
	void assert_test_data(const std::list<std::pair<T, T>>& data, std::string_view test="")
	{
		for(const auto&[result, expected] : data)
		{
			std::ostringstream error_text;

			error_text << test << "\n";

			if constexpr(is_streamable_v<decltype(std::cout), T>)
			{
				if constexpr(std::is_same_v<T, bool>)
				{
					error_text << std::boolalpha;
				}

				error_text << "Expected: "
					  	   << expected
					  	   << " Got: "
					  	   << result;
			}

			assert_equals(result, expected, error_text.str());
		}
	}

	template<class Function, class Expected, class... Args>
	std::list<std::pair<Expected, Expected>> make_test_data(Function&& func, const std::initializer_list<std::pair<std::tuple<Args...>, Expected>>& raw)
	{
		std::list<std::pair<Expected, Expected>> data;

		for(const auto& pair : raw)
		{
			data.emplace_back(std::apply(func, pair.first), pair.second);
		}

		return data;
	}
}

#endif
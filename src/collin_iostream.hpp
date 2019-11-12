#ifndef COLLIN_IOSTREAM
#define COLLIN_IOSTREAM

#include <iostream>
#include <string>
#include <string_view>

namespace collin
{
	template<class InputIterator>
	std::ostream& print(InputIterator begin, InputIterator end, std::string_view sep="", std::ostream& stream=std::cout)
	{
		while(begin != end)
		{
			stream << *begin << sep;
			begin++;
		}

		return stream;
	}

	template<class Container>
	std::ostream& print(const Container& container, std::string_view sep="", std::ostream& stream=std::cout)
	{
		return print(std::cbegin(container), std::cend(container), sep, stream);
	}

	template<>
	std::ostream& print<std::string>(const std::string& str, std::string_view sep, std::ostream& stream)
	{
		stream << str << sep;
		return stream;
	}

	std::ostream& print(std::string_view str, std::string_view sep="", std::ostream& stream=std::cout)
	{
		stream << str << sep;
		return stream;
	}

	std::ostream& println(const std::string& str, std::ostream& stream=std::cout)
	{
		return print(str, "\n", stream);
	}

	std::ostream& println(std::string_view str="", std::ostream& stream=std::cout)
	{
		return print(str, "\n", stream);
	}

}

#endif
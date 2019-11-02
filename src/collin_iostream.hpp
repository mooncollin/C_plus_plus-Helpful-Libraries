#ifndef COLLIN_IOSTREAM
#define COLLIN_IOSTREAM

#include <iostream>
#include <string>

namespace collin
{
	template<class InputIterator>
	std::ostream& print(InputIterator begin, InputIterator end, std::ostream& stream=std::cout, const std::string& sep="")
	{
		while(begin != end)
		{
			stream << *begin << sep;
			begin++;
		}

		return stream;
	}

	template<class Container>
	std::ostream& print(const Container& container, std::ostream& stream=std::cout, const std::string& sep="")
	{
		return print(std::cbegin(container), std::cend(container), stream, sep);
	}

	template<>
	std::ostream& print<std::string>(const std::string& str, std::ostream& stream, const std::string& sep)
	{
		stream << str << sep;
		return stream;
	}

	std::ostream& println(const std::string& str="", std::ostream& stream=std::cout)
	{
		return print(str, stream, "\n");
	}

}

#endif
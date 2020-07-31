#include "collin/ranges/ranges.hpp"
#include <iostream>

int main()
{

	for(int i : collin::ranges::iota_view{1, 10})
	{
		std::cout << i << ' ';
	}
	std::cout << '\n';
}
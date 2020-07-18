#include "collin/memory.hpp"
#include <vector>
#include <string>

int main()
{
    {
        collin::memory::Recycler<int> r;
        std::vector<int, collin::memory::Recycler<int>> v(r);
        std::vector<int, collin::memory::Recycler<int>> v2(r);

        std::vector<std::string, collin::memory::Recycler<std::string>> vstring;

        v.resize(1000);
        v.clear();
        v.shrink_to_fit();

        v2.resize(1000);
    }

    collin::memory::Recycler<double> r2;
    std::vector<double, decltype(r2)> v3(r2);
    std::vector<long, collin::memory::Recycler<long>> v4;
}
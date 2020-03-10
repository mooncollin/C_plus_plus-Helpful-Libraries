#include "collin_memory.hpp"
#include <vector>
#include <string>

int main()
{
    {
        collin::Recycler<int> r;
        std::vector<int, collin::Recycler<int>> v(r);
        std::vector<int, collin::Recycler<int>> v2(r);

        std::vector<std::string, collin::Recycler<std::string>> vstring;

        v.resize(1000);
        v.clear();
        v.shrink_to_fit();

        v2.resize(1000);
    }

    collin::Recycler<double> r2;
    std::vector<double, decltype(r2)> v3(r2);
    std::vector<long, collin::Recycler<long>> v4;
}
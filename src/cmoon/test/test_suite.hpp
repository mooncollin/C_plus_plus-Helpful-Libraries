#pragma once

#include <type_traits>
#include <vector>
#include <memory>

#include "cmoon/test/test_case.hpp"

namespace cmoon
{
	namespace test
	{
		class test_suite
		{
			using container = std::vector<std::unique_ptr<test_case>>;
			using iterator = typename container::iterator;

			public:
				template<std::derived_from<test_case> T, class... Args>
				void add_test_case(Args&&... args)
				{
					cases.push_back(std::make_unique<T>(std::forward<Args>(args)...));
				}

				void add_test_suite(test_suite&& suite)
				{
					for (auto& case_ : suite.cases)
					{
						cases.push_back(std::move(case_));
					}

					suite.clear();
				}

				std::vector<test_result> run_all()
				{
					std::vector<test_result> results;
					results.reserve(cases.size());
					for (auto& t_case : cases)
					{
						results.push_back(t_case->run());
					}

					return results;
				}

				iterator begin()
				{
					return std::begin(cases);
				}

				iterator end()
				{
					return std::end(cases);
				}

				void clear()
				{
					cases.clear();
				}
			private:
				container cases;
		};
	}
}
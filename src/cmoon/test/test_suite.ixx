module;

#include <type_traits>
#include <vector>
#include <memory>
#include <concepts>
#include <string>
#include <string_view>

export module cmoon.test.test_suite;

import cmoon.test.test_case;

namespace cmoon::test
{
	export
	class test_suite
	{
		using container = std::vector<std::unique_ptr<test_case>>;
		using iterator = typename container::iterator;

		public:
			test_suite(std::string_view name = "")
				: name_{name} {}

			test_suite(const test_suite&) = delete;
			test_suite& operator=(const test_suite&) = delete;

			test_suite(test_suite&&) = default;
			test_suite& operator=(test_suite&&) = default;

			template<std::derived_from<test_case> T, class... Args>
			void add_test_case(Args&&... args)
			{
				cases.push_back(std::make_unique<T>(std::forward<Args>(args)...));
			}

			std::string& name()
			{
				return name_;
			}

			const std::string& name() const
			{
				return name_;
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
			std::string name_;
			container cases;
	};
}
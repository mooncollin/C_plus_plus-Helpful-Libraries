export module cmoon.test.test_suite;

import <deque>;
import <memory>;
import <concepts>;
import <string>;
import <iterator>;
import <algorithm>;
import <ranges>;
import <cstddef>;

import cmoon.test.test_case;

namespace cmoon::test
{
	class test_suite;

	template<class It>
	class test_suite_iterator
	{
		public:
			using difference_type = std::iter_difference_t<It>;
			using value_type = test_case*;
			using iterator_category = std::forward_iterator_tag;

			test_suite_iterator() noexcept = default;

			value_type operator*() const noexcept
			{
				return ptr->get();
			}

			test_suite_iterator& operator++()
			{
				++ptr;
				return *this;
			}

			test_suite_iterator operator++(int)
			{
				auto copy {*this};
				++ptr;
				return copy;
			}

			[[nodiscard]] friend bool operator==(const test_suite_iterator&, const test_suite_iterator&) noexcept = default;
			[[nodiscard]] friend bool operator!=(const test_suite_iterator&, const test_suite_iterator&) noexcept = default;
		private:
			test_suite_iterator(It ptr) noexcept
				: ptr{ptr} {}

			It ptr;

			friend class test_suite;
	};

	export
	class test_suite
	{
		public:
			test_suite() noexcept = default;

			test_suite(std::string name) noexcept
				: name_{std::move(name)} {}

			test_suite(const test_suite&) = delete;
			test_suite& operator=(const test_suite&) = delete;

			test_suite(test_suite&&) noexcept = default;
			test_suite& operator=(test_suite&&) noexcept = default;

			template<std::derived_from<test_case> T, class... Args>
			void add_test_case(Args&&... args)
			{
				cases.push_back(std::make_unique<T>(std::forward<Args>(args)...));
			}

			const std::string& name() const noexcept
			{
				return name_;
			}

			void add_test_suite(test_suite&& suite)
			{
				std::ranges::move(suite.cases, std::back_inserter(cases));
				suite.clear();
			}

			template<std::output_iterator<test_result> Out>
			void run_all(Out out)
			{
				for (auto t_case : cases)
				{
					*out = t_case->run();
				}
			}

			[[nodiscard]] auto begin() const noexcept
			{
				return test_suite_iterator{std::cbegin(cases)};
			}

			[[nodiscard]] auto begin() noexcept
			{
				return test_suite_iterator{std::begin(cases)};
			}

			[[nodiscard]] auto end() noexcept
			{
				return test_suite_iterator{std::end(cases)};
			}

			[[nodiscard]] auto end() const noexcept
			{
				return test_suite_iterator{std::cend(cases)};
			}

			[[nodiscard]] std::size_t size() const noexcept
			{
				return std::ranges::size(cases);
			}

			void clear() noexcept
			{
				cases.clear();
			}
		private:
			std::string name_;
			std::deque<std::unique_ptr<test_case>> cases;
	};
}
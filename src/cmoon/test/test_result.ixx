export module cmoon.test.test_result;

import <exception>;
import <deque>;
import <utility>;
import <memory>;
import <type_traits>;
import <ranges>;

import cmoon.test.assert_exception;

namespace cmoon::test
{
	export
	class test_result
	{
		public:
			test_result() noexcept = default;

			void add_error(std::exception e)
			{
				errors_.push_back(std::move(e));
			}

			void add_failure(std::exception e)
			{
				failures_.push_back(std::move(e));
			}

			[[nodiscard]] auto errors() const noexcept
			{
				return std::views::all(errors_);
			}

			[[nodiscard]] auto failures() const noexcept
			{
				return std::views::all(failures_);
			}

			[[nodiscard]] bool passed() const noexcept
			{
				return std::ranges::empty(errors_) && std::ranges::empty(failures_);
			}

			[[nodiscard]] explicit operator bool() const noexcept
			{
				return passed();
			}
		private:
			std::deque<std::exception> errors_;
			std::deque<std::exception> failures_;
	};
}
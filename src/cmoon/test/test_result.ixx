export module cmoon.test.test_result;

import <exception>;
import <vector>;
import <utility>;
import <memory>;
import <type_traits>;

import cmoon.test.assert_exception;

namespace cmoon::test
{
	export
	template<class Allocator = std::allocator<std::exception>>
	class test_result
	{
		public:
			using allocator_type = typename std::allocator_traits<Allocator>::template rebind_alloc<std::exception>;

			test_result() noexcept(std::is_nothrow_default_constructible_v<std::vector<std::exception, allocator_type>>) = default;

			test_result(const Allocator& alloc)
				: errors_{alloc}, failures_{alloc} {}

			void add_error(std::exception e)
			{
				errors_.push_back(std::move(e));
			}

			void add_failure(std::exception e)
			{
				failures_.push_back(std::move(e));
			}

			const std::vector<std::exception, allocator_type>& errors() const noexcept
			{
				return errors_;
			}

			const std::vector<std::exception, allocator_type>& failures() const noexcept
			{
				return failures_;
			}

			[[nodiscard]] bool passed() const noexcept
			{
				return errors_.empty() && failures_.empty();
			}
		private:
			std::vector<std::exception, allocator_type> errors_;
			std::vector<std::exception, allocator_type> failures_;
	};
}
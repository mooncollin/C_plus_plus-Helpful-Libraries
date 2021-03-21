module;
#include <vector>
#include <stdexcept>

export module cmoon.test.test_result;

namespace cmoon::test
{
	export
	class test_result
	{
		public:
			void add_error(const std::exception& e)
			{
				errors_.push_back(e);
			}

			void add_error(std::exception&& e)
			{
				errors_.push_back(std::move(e));
			}

			void add_failure(const std::exception& e)
			{
				failures_.push_back(e);
			}

			void add_failure(std::exception&& e)
			{
				failures_.push_back(std::move(e));
			}

			const std::vector<std::exception>& errors() const
			{
				return errors_;
			}

			const std::vector<std::exception>& failures() const
			{
				return failures_;
			}

			bool passed() const noexcept
			{
				return errors_.empty() && failures_.empty();
			}
		private:
			std::vector<std::exception> errors_;
			std::vector<std::exception> failures_;
	};
}
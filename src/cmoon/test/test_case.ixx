module;
#include <string_view>
#include <string>
#include <stdexcept>

export module cmoon.test.test_case;
import cmoon.test.assert_exception;
import cmoon.test.test_result;

namespace cmoon::test
{
	export
	class test_case
	{
		public:
			test_case(std::string_view str = "")
				: name_{str} {}

			test_case(const test_case&) = default;
			test_case(test_case&&) = default;

			test_case& operator=(const test_case&) = default;
			test_case& operator=(test_case&&) = default;

			virtual void set_up() {}
			virtual void tear_down() {}
			virtual ~test_case() {}

			test_result run() noexcept
			{
				test_result result;

				try
				{
					set_up();
					operator()();
					tear_down();
				}
				catch (const assert_exception& e)
				{
					result.add_failure(e);
				}
				catch (const std::exception& e)
				{
					result.add_error(e);
				}

				return result;
			}

			virtual void operator()() = 0;

			[[nodiscard]] std::string& name() noexcept
			{
				return name_;
			}

			[[nodiscard]] const std::string& name() const noexcept
			{
				return name_;
			}
		private:
			std::string name_ {};
	};
}
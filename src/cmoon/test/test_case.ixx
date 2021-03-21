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
			test_case() = default;

			test_case(std::string_view str)
				: name_{str} {}

			virtual void set_up() {}
			virtual void tear_down() {}
			virtual ~test_case() {}

			test_result run()
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

			[[nodiscard]] const std::string& name() const noexcept
			{
				return name_;
			}

			void name(std::string_view str)
			{
				name_ = str;
			}

		private:
			std::string name_ {};
	};
}
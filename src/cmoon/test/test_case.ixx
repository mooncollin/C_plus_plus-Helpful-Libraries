export module cmoon.test.test_case;

import <string>;
import <stdexcept>;
import <exception>;
import <utility>;
import <memory>;

import cmoon.test.assert_exception;
import cmoon.test.test_result;

namespace cmoon::test
{
	export
	class test_case
	{
		public:
			test_case() noexcept = default;

			test_case(std::string str) noexcept
				: name_{std::move(str)} {}

			virtual void set_up() {}
			virtual void tear_down() {}
			virtual ~test_case() noexcept = default;

			template<class Allocator = std::allocator<std::exception>>
			test_result<Allocator> run(const Allocator& alloc = {})
			{
				test_result<Allocator> result{alloc};

				try
				{
					set_up();
					operator()();
					tear_down();
				}
				catch (assert_exception& e)
				{
					result.add_failure(std::move(e));
				}
				catch (std::exception& e)
				{
					result.add_error(std::move(e));
				}
				catch (...)
				{
					result.add_error(std::runtime_error{"Unknown Exception"});
				}

				return result;
			}

			virtual void operator()() = 0;

			void name(std::string n) noexcept
			{
				name_ = std::move(n);
			}

			[[nodiscard]] const std::string& name() const noexcept
			{
				return name_;
			}
		private:
			std::string name_;
	};
}
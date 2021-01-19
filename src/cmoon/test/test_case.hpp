#pragma once

#include <string_view>
#include <string>
#include <stdexcept>

#include "cmoon/test/assert_exception.hpp"
#include "cmoon/test/test_result.hpp"

namespace cmoon
{
	namespace test
	{
		class test_case
		{
			public:
				test_case() = default;

				test_case(std::string_view str)
					: name_{str} {}

				virtual void set_up() {}
				virtual ~test_case() {}

				test_result run()
				{
					test_result result;

					#ifndef _DEBUG
					try
					{
					#endif

						set_up();
						operator()();

					#ifndef _DEBUG
					}
					catch (const assert_exception& e)
					{
						result.add_failure(e);
					}
					catch (const std::exception& e)
					{
						result.add_error(e);
					}
					#endif

					return result;
				}

				virtual void operator()() = 0;

				const std::string& name() const noexcept
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
}
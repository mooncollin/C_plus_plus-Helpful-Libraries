export module cmoon.tests.scope.unique_resource;

import <cstdio>;

import cmoon.test;
import cmoon.scope;

namespace cmoon::tests::scope
{
	export
	class unique_resource_initialization_test : public cmoon::test::test_case
	{
		public:
			unique_resource_initialization_test()
				: cmoon::test::test_case {"unique_resource_initialization_test"} {}

			void operator()() override
			{
				auto file = cmoon::make_unique_resource_checked(
					std::fopen("potentially_nonexistent_file.txt", "r"),
					nullptr,
					[](std::FILE* fptr) { std::fclose(fptr); }
				);

				cmoon::test::assert_equal(file.get(), nullptr);

				constexpr int goal = 1;
				int value = 0;

				{
					auto test = cmoon::make_unique_resource_checked(
						value, // Dummy value
						10, // Dummy value
						[&value](int) { value++; }
					);

					cmoon::test::assert_equal(test.get(), value);
				}

				cmoon::test::assert_equal(value, goal);
			}
	};
}
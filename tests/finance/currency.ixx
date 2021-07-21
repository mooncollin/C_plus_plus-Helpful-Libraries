export module cmoon.tests.finance.currency;

import cmoon.test;
import cmoon.finance;

namespace cmoon::tests::finance
{
	export
	class currency_code_constructor_test : public cmoon::test::test_case
	{
		public:
			currency_code_constructor_test()
				: cmoon::test::test_case{"currency_code_constructor_test"} {}

			void operator()() override
			{
				constexpr cmoon::finance::currency_code c1{};

				cmoon::test::assert_equal(c1.code(), "\0\0\0");
			}
	};

	export
	class currency_constructor_test : public cmoon::test::test_case
	{
		public:
			currency_constructor_test()
				: cmoon::test::test_case{"currency_test"} {}

			void operator()() override
			{
				// Making sure I can access members
				static_assert(cmoon::finance::currencies::albanian_lek.currency_code().code() == "ALL");
				static_assert(cmoon::finance::currencies::albanian_lek.currency_number() == 8);
			}

		private:
			constexpr void static_information()
			{
				constexpr auto size = sizeof(cmoon::finance::currency);
			}
	};
}
export module cmoon.tests.finance.money;

import cmoon.test;
import cmoon.finance;

namespace cmoon::tests::finance
{
	export
	class money_constructor_test : public cmoon::test::test_case
	{
		public:
			money_constructor_test()
				: cmoon::test::test_case{"money_constructor_test"} {}

			void operator()() override
			{
			
			}

		private:
			constexpr void static_information()
			{
				constexpr auto size = sizeof(cmoon::finance::money<6>);
			}
	};
}
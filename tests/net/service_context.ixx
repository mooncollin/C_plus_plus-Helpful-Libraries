export module cmoon.tests.net.service_context;

import cmoon.test;
import cmoon.net;

namespace cmoon::tests::net
{
	export
	class service_context_make_service_test : public cmoon::test::test_case
	{
		class test_service
		{
			public:
				test_service(bool& b)
					: b{b} {}

				~test_service() noexcept
				{
					b = true;
				}
			private:
				bool& b;
		};

		public:
			service_context_make_service_test()
				: cmoon::test::test_case{"service_context_make_service_test"} {}

			void operator()() final
			{
				bool b {false};

				{
					cmoon::net::service_context ctx;
					ctx.make_service<test_service>(b);
				}

				cmoon::test::assert_true(b);
			}
	};
}
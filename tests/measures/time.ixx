export module cmoon.tests.measures.time;

import <sstream>;

import cmoon.test;
import cmoon.measures;
import cmoon.measures.time;

namespace cmoon::tests::measures
{
    export
	class time_string_test : public cmoon::test::test_case
    {
        public:
            time_string_test()
                : cmoon::test::test_case{"time_string_test"} {}

            void operator()() override
            {
                std::stringstream ss;

                ss << cmoon::measures::attoseconds{10};
		        cmoon::test::assert_equal(ss.str(), "10as");

		        ss.str("");
                ss << cmoon::measures::femtoseconds{10};
		        cmoon::test::assert_equal(ss.str(), "10fs");

		        ss.str("");
                ss << cmoon::measures::picoseconds{10};
		        cmoon::test::assert_equal(ss.str(), "10ps");

		        ss.str("");
                ss << cmoon::measures::nanoseconds{10};
		        cmoon::test::assert_equal(ss.str(), "10ns");

		        ss.str("");
                ss << cmoon::measures::microseconds{10};
		        cmoon::test::assert_equal(ss.str(), "10us");

		        ss.str("");
                ss << cmoon::measures::milliseconds{10};
		        cmoon::test::assert_equal(ss.str(), "10ms");

		        ss.str("");
                ss << cmoon::measures::centiseconds{10};
		        cmoon::test::assert_equal(ss.str(), "10cs");

		        ss.str("");
                ss << cmoon::measures::deciseconds{10};
		        cmoon::test::assert_equal(ss.str(), "10ds");

		        ss.str("");
                ss << cmoon::measures::seconds{10};
		        cmoon::test::assert_equal(ss.str(), "10s");

		        ss.str("");
                ss << cmoon::measures::decaseconds{10};
		        cmoon::test::assert_equal(ss.str(), "10das");

		        ss.str("");
                ss << cmoon::measures::hectoseconds{10};
		        cmoon::test::assert_equal(ss.str(), "10hs");

		        ss.str("");
                ss << cmoon::measures::kiloseconds{10};
		        cmoon::test::assert_equal(ss.str(), "10ks");

		        ss.str("");
                ss << cmoon::measures::megaseconds{10};
		        cmoon::test::assert_equal(ss.str(), "10Ms");

		        ss.str("");
                ss << cmoon::measures::gigaseconds{10};
		        cmoon::test::assert_equal(ss.str(), "10Gs");

		        ss.str("");
                ss << cmoon::measures::teraseconds{10};
		        cmoon::test::assert_equal(ss.str(), "10Ts");

		        ss.str("");
                ss << cmoon::measures::petaseconds{10};
		        cmoon::test::assert_equal(ss.str(), "10Ps");

		        ss.str("");
                ss << cmoon::measures::exaseconds{10};
		        cmoon::test::assert_equal(ss.str(), "10Es");

		        ss.str("");
                ss << cmoon::measures::minutes{10};
		        cmoon::test::assert_equal(ss.str(), "10min");

		        ss.str("");
                ss << cmoon::measures::hours{10};
		        cmoon::test::assert_equal(ss.str(), "10h");

		        ss.str("");
                ss << cmoon::measures::days{10};
		        cmoon::test::assert_equal(ss.str(), "10d");

		        ss.str("");
                ss << cmoon::measures::weeks{10};
		        cmoon::test::assert_equal(ss.str(), "10w");

		        ss.str("");
                ss << cmoon::measures::months{10};
		        cmoon::test::assert_equal(ss.str(), "10mo");

		        ss.str("");
                ss << cmoon::measures::years{10};
		        cmoon::test::assert_equal(ss.str(), "10y");
            }
    };

    export
    class time_values_test : public cmoon::test::test_case
    {
        public:
            time_values_test()
                : cmoon::test::test_case{"time_values_test"} {}

            void operator()() override
            {
				static_assert(cmoon::measures::seconds{1} == cmoon::measures::attoseconds{1000000000000000000});
                static_assert(cmoon::measures::seconds{1} == cmoon::measures::femtoseconds{1000000000000000});
                static_assert(cmoon::measures::seconds{1} == cmoon::measures::picoseconds{1000000000000});
                static_assert(cmoon::measures::seconds{1} == cmoon::measures::nanoseconds{1000000000});
                static_assert(cmoon::measures::seconds{1} == cmoon::measures::microseconds{1000000});
                static_assert(cmoon::measures::seconds{1} == cmoon::measures::milliseconds{1000});
                static_assert(cmoon::measures::seconds{1} == cmoon::measures::centiseconds{100});
                static_assert(cmoon::measures::seconds{1} == cmoon::measures::deciseconds{10});
                static_assert(cmoon::measures::seconds{1} == cmoon::measures::seconds{1});
                static_assert(cmoon::measures::seconds{10} == cmoon::measures::decaseconds{1});
                static_assert(cmoon::measures::seconds{1000} == cmoon::measures::kiloseconds{1});
                static_assert(cmoon::measures::seconds{1000000} == cmoon::measures::megaseconds{1});
                static_assert(cmoon::measures::seconds{1000000000} == cmoon::measures::gigaseconds{1});
                static_assert(cmoon::measures::seconds{1000000000000} == cmoon::measures::teraseconds{1});
                static_assert(cmoon::measures::seconds{1000000000000000} == cmoon::measures::petaseconds{1});
                static_assert(cmoon::measures::seconds{1000000000000000000} == cmoon::measures::exaseconds{1});

                static_assert(cmoon::measures::system_cast<cmoon::measures::seconds>(cmoon::measures::kiloseconds{1}) == cmoon::measures::seconds{1000});

                static_assert(cmoon::measures::seconds{1} * 10 == cmoon::measures::decaseconds{1});
                static_assert((cmoon::measures::seconds{1} + cmoon::measures::seconds{1} + cmoon::measures::seconds{1}) == cmoon::measures::seconds{3});
                static_assert((cmoon::measures::seconds{3} - cmoon::measures::seconds{2}) == cmoon::measures::seconds{1});
                static_assert(cmoon::measures::seconds{100} / 10 == cmoon::measures::seconds{10});

                static_assert(cmoon::measures::seconds{60} == cmoon::measures::minutes{1});
                static_assert(cmoon::measures::seconds{3600} == cmoon::measures::hours{1});
                static_assert(cmoon::measures::seconds{86400} == cmoon::measures::days{1});
                static_assert(cmoon::measures::seconds{604800} == cmoon::measures::weeks{1});
                static_assert(cmoon::measures::seconds{2629746} == cmoon::measures::months{1});
                static_assert(cmoon::measures::seconds{31556952} == cmoon::measures::years{1});

                static_assert(cmoon::measures::minutes{60} == cmoon::measures::hours{1});
                static_assert(cmoon::measures::hours{24} == cmoon::measures::days{1});
                static_assert(cmoon::measures::days{7} == cmoon::measures::weeks{1});
                static_assert(cmoon::measures::months{12} == cmoon::measures::years{1});

                static_assert(cmoon::measures::to_chrono(cmoon::measures::nanoseconds{10}) == std::chrono::nanoseconds{10});
                static_assert(cmoon::measures::to_chrono(cmoon::measures::microseconds{10}) == std::chrono::microseconds{10});
                static_assert(cmoon::measures::to_chrono(cmoon::measures::milliseconds{10}) == std::chrono::milliseconds{10});
                static_assert(cmoon::measures::to_chrono(cmoon::measures::seconds{10}) == std::chrono::seconds{10});
                static_assert(cmoon::measures::to_chrono(cmoon::measures::minutes{10}) == std::chrono::minutes{10});
                static_assert(cmoon::measures::to_chrono(cmoon::measures::hours{10}) == std::chrono::hours{10});
            }
    };
}
export module cmoon.tests.measures.mass;

import <sstream>;

import cmoon.test;
import cmoon.measures;
import cmoon.measures.mass;

namespace cmoon::tests::measures
{
    export
	class mass_string_test : public cmoon::test::test_case
    {
        public:
            mass_string_test()
                : cmoon::test::test_case{"mass_string_test"} {}

            void operator()() override
            {
                std::stringstream ss;

                ss << cmoon::measures::attograms{10};
                cmoon::test::assert_equal(ss.str(), "10ag");

                ss.str("");
                ss << cmoon::measures::femtograms{10};
                cmoon::test::assert_equal(ss.str(), "10fg");

                ss.str("");
                ss << cmoon::measures::picograms{10};
                cmoon::test::assert_equal(ss.str(), "10pg");

                ss.str("");
                ss << cmoon::measures::nanograms{10};
                cmoon::test::assert_equal(ss.str(), "10ng");

                ss.str("");
                ss << cmoon::measures::micrograms{10};
                cmoon::test::assert_equal(ss.str(), "10ug");

                ss.str("");
                ss << cmoon::measures::milligrams{10};
                cmoon::test::assert_equal(ss.str(), "10mg");

                ss.str("");
                ss << cmoon::measures::centigrams{10};
                cmoon::test::assert_equal(ss.str(), "10cg");

                ss.str("");
                ss << cmoon::measures::decigrams{10};
                cmoon::test::assert_equal(ss.str(), "10dg");

                ss.str("");
                ss << cmoon::measures::grams{10};
                cmoon::test::assert_equal(ss.str(), "10g");

                ss.str("");
                ss << cmoon::measures::decagrams{10};
                cmoon::test::assert_equal(ss.str(), "10dag");

                ss.str("");
                ss << cmoon::measures::hectograms{10};
                cmoon::test::assert_equal(ss.str(), "10hg");

                ss.str("");
                ss << cmoon::measures::kilograms{10};
                cmoon::test::assert_equal(ss.str(), "10kg");

                ss.str("");
                ss << cmoon::measures::megagrams{10};
                cmoon::test::assert_equal(ss.str(), "10Mg");

                ss.str("");
                ss << cmoon::measures::gigagrams{10};
                cmoon::test::assert_equal(ss.str(), "10Gg");

                ss.str("");
                ss << cmoon::measures::teragrams{10};
                cmoon::test::assert_equal(ss.str(), "10Tg");

                ss.str("");
                ss << cmoon::measures::petagrams{10};
                cmoon::test::assert_equal(ss.str(), "10Pg");

                ss.str("");
                ss << cmoon::measures::exagrams{10};
                cmoon::test::assert_equal(ss.str(), "10Eg");

                ss.str("");
                ss << cmoon::measures::grains{10};
                cmoon::test::assert_equal(ss.str(), "10gr");

                ss.str("");
                ss << cmoon::measures::drachm{10};
                cmoon::test::assert_equal(ss.str(), "10dr");

                ss.str("");
                ss << cmoon::measures::ounces{10};
                cmoon::test::assert_equal(ss.str(), "10oz");

                ss.str("");
                ss << cmoon::measures::pounds{10};
                cmoon::test::assert_equal(ss.str(), "10lb");

                ss.str("");
                ss << cmoon::measures::stones{10};
                cmoon::test::assert_equal(ss.str(), "10st");

                ss.str("");
                ss << cmoon::measures::quarters{10};
                cmoon::test::assert_equal(ss.str(), "10qr");

                ss.str("");
                ss << cmoon::measures::hundredweights{10};
                cmoon::test::assert_equal(ss.str(), "10cwt");

                ss.str("");
                ss << cmoon::measures::short_tons{10};
                cmoon::test::assert_equal(ss.str(), "10t");

                ss.str("");
                ss << cmoon::measures::tons{10};
                cmoon::test::assert_equal(ss.str(), "10t");
            }
    };

    export
    class mass_values_test : public cmoon::test::test_case
    {
        public:
            mass_values_test()
                : cmoon::test::test_case{"mass_values_test"} {}

            void operator()() override
            {
            }

            void static_asserts()
            {
                static_assert(cmoon::measures::grams{1} == cmoon::measures::attograms{1000000000000000000});
                static_assert(cmoon::measures::grams{1} == cmoon::measures::femtograms{1000000000000000});
                static_assert(cmoon::measures::grams{1} == cmoon::measures::picograms{1000000000000});
                static_assert(cmoon::measures::grams{1} == cmoon::measures::nanograms{1000000000});
                static_assert(cmoon::measures::grams{1} == cmoon::measures::micrograms{1000000});
                static_assert(cmoon::measures::grams{1} == cmoon::measures::milligrams{1000});
                static_assert(cmoon::measures::grams{1} == cmoon::measures::centigrams{100});
                static_assert(cmoon::measures::grams{1} == cmoon::measures::decigrams{10});
                static_assert(cmoon::measures::grams{1} == cmoon::measures::grams{1});
                static_assert(cmoon::measures::grams{10} == cmoon::measures::decagrams{1});
                static_assert(cmoon::measures::grams{1000} == cmoon::measures::kilograms{1});
                static_assert(cmoon::measures::grams{1000000} == cmoon::measures::megagrams{1});
                static_assert(cmoon::measures::grams{1000000000} == cmoon::measures::gigagrams{1});
                static_assert(cmoon::measures::grams{1000000000000} == cmoon::measures::teragrams{1});
                static_assert(cmoon::measures::grams{1000000000000000} == cmoon::measures::petagrams{1});
                static_assert(cmoon::measures::grams{1000000000000000000} == cmoon::measures::exagrams{1});

                static_assert(cmoon::measures::grains{7000} == cmoon::measures::pounds{1});
                static_assert(cmoon::measures::drachm{256} == cmoon::measures::pounds{1});
                static_assert(cmoon::measures::ounces{16} == cmoon::measures::pounds{1});
                static_assert(cmoon::measures::pounds{1} == cmoon::measures::pounds{1});
                static_assert(cmoon::measures::stones{1} == cmoon::measures::pounds{14});
                static_assert(cmoon::measures::quarters{1} == cmoon::measures::pounds{28});
                static_assert(cmoon::measures::hundredweights{1} == cmoon::measures::pounds{112});
                static_assert(cmoon::measures::short_tons{1} == cmoon::measures::pounds{2000});
                static_assert(cmoon::measures::tons{1} == cmoon::measures::pounds{2240});

                static_assert(cmoon::measures::system_cast<cmoon::measures::pounds>(cmoon::measures::short_tons{10}) == cmoon::measures::pounds{20000});
                static_assert(cmoon::measures::system_cast<cmoon::measures::grams>(cmoon::measures::pounds{1}) == cmoon::measures::grams{453});
                static_assert(cmoon::measures::system_cast<cmoon::measures::pounds>(cmoon::measures::kilograms{10}) == cmoon::measures::pounds{22});
            }
    };
}
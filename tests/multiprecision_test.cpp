#include <iostream>
#include <utility>

#include "cmoon/multiprecision/multiprecision.hpp"
#include "cmoon/multiprecision/big_int.hpp"
#include "cmoon/multiprecision/big_float.hpp"
#include "cmoon/test/assert.hpp"
#include "cmoon/test/test_case.hpp"
#include "cmoon/test/test_suite.hpp"
#include "cmoon/test/runner.hpp"

class big_int_construction_test : public cmoon::test::test_case
{
	public:
		big_int_construction_test()
			: cmoon::test::test_case{"big_int_construction_test"} {}

		void operator()() override
		{
		}

	private:
		void static_information()
		{
			using dynamic_signed = cmoon::multiprecision::big_int<0, cmoon::multiprecision::signed_type::signed_magnitude>;
			using dynamic_unsigned = cmoon::multiprecision::big_int<0, cmoon::multiprecision::signed_type::unsigned_magnitude>;

			using trivial_signed = cmoon::multiprecision::big_int<32, cmoon::multiprecision::signed_type::signed_magnitude>;
			using trivial_unsigned = cmoon::multiprecision::big_int<32, cmoon::multiprecision::signed_type::unsigned_magnitude>;

			using fixed_signed = cmoon::multiprecision::big_int<128, cmoon::multiprecision::signed_type::signed_magnitude>;
			using fixed_unsigned = cmoon::multiprecision::big_int<128, cmoon::multiprecision::signed_type::unsigned_magnitude>;

			using fixed_signed_large = cmoon::multiprecision::big_int<8192, cmoon::multiprecision::signed_type::signed_magnitude>;
			using fixed_unsigned_large = cmoon::multiprecision::big_int<8192, cmoon::multiprecision::signed_type::unsigned_magnitude>;

			constexpr auto dynamic_signed_size = sizeof(dynamic_signed);
			constexpr auto dynamic_unsigned_size = sizeof(dynamic_unsigned);

			constexpr auto trivial_signed_size = sizeof(trivial_signed);
			constexpr auto trivial_unsigned_size = sizeof(trivial_unsigned);

			constexpr auto fixed_signed_size = sizeof(fixed_signed);
			constexpr auto fixed_unsigned_size = sizeof(fixed_unsigned);

			constexpr auto fixed_signed_large_size = sizeof(fixed_signed_large);
			constexpr auto fixed_unsigned_large_size = sizeof(fixed_unsigned_large);
		}
};

class factorial_test : public cmoon::test::test_case
{
	public:
		factorial_test()
			: cmoon::test::test_case{"factorial_test"} {}

		void operator()() override
		{
			const auto t = cmoon::factorial(34);
			const auto t2 = cmoon::factorial(35);
		}
};

class big_float_construction_test : public cmoon::test::test_case
{
	public:
		big_float_construction_test()
			: cmoon::test::test_case{"big_float_construction_test"} {}

		void operator()() override
		{
			cmoon::multiprecision::big_float<0> f1{4294967296};
			cmoon::multiprecision::big_float<0> f2{25769803776};
			cmoon::multiprecision::big_float<0> f3{832.432984389};
			cmoon::multiprecision::big_float<800> f4;
			f4 /= f1;
			f4 += f1;
			f4 -= f1;
			f4 *= f1;

			std::cout << static_cast<long double>(f3) << '\n';
			std::cout << static_cast<long double>(f1 / f2) << '\n';
			std::cout << static_cast<long double>(f2 / f1) << '\n';
		}
};

int main()
{
	cmoon::test::test_suite suite;
	suite.add_test_case<big_int_construction_test>();
	suite.add_test_case<factorial_test>();
	suite.add_test_case<big_float_construction_test>();

	cmoon::test::text_test_runner runner{std::cout};

	return !runner.run(suite);
}
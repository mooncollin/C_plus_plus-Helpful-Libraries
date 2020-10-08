#include <iostream>
#include <utility>

#include "collin/multiprecision/multiprecision.hpp"
#include "collin/multiprecision/big_int.hpp"
#include "collin/multiprecision/big_float.hpp"
#include "collin/test.hpp"

class big_int_construction_test : public collin::test::test_case
{
	public:
		big_int_construction_test()
			: collin::test::test_case{"big_int_construction_test"} {}

		void operator()() override
		{
		}

	private:
		void static_information()
		{
			using dynamic_signed = collin::multiprecision::big_int<0, collin::multiprecision::signed_type::signed_magnitude>;
			using dynamic_unsigned = collin::multiprecision::big_int<0, collin::multiprecision::signed_type::unsigned_magnitude>;

			using trivial_signed = collin::multiprecision::big_int<32, collin::multiprecision::signed_type::signed_magnitude>;
			using trivial_unsigned = collin::multiprecision::big_int<32, collin::multiprecision::signed_type::unsigned_magnitude>;

			using fixed_signed = collin::multiprecision::big_int<128, collin::multiprecision::signed_type::signed_magnitude>;
			using fixed_unsigned = collin::multiprecision::big_int<128, collin::multiprecision::signed_type::unsigned_magnitude>;

			using fixed_signed_large = collin::multiprecision::big_int<8192, collin::multiprecision::signed_type::signed_magnitude>;
			using fixed_unsigned_large = collin::multiprecision::big_int<8192, collin::multiprecision::signed_type::unsigned_magnitude>;

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

class factorial_test : public collin::test::test_case
{
	public:
		factorial_test()
			: collin::test::test_case{"factorial_test"} {}

		void operator()() override
		{
			auto t = collin::multiprecision::factorial(34);
			auto t2 = collin::multiprecision::factorial(35);
		}
};

class big_float_construction_test : public collin::test::test_case
{
	public:
		big_float_construction_test()
			: collin::test::test_case{"big_float_construction_test"} {}

		void operator()() override
		{
			collin::multiprecision::big_float<0> f1{4294967296};
			collin::multiprecision::big_float<0> f2{25769803776};
			collin::multiprecision::big_float<0> f3{832.432984389};
			collin::multiprecision::big_float<800> f4;
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
	collin::test::test_suite suite;
	suite.add_test_case<big_int_construction_test>();
	suite.add_test_case<factorial_test>();
	suite.add_test_case<big_float_construction_test>();

	collin::test::text_test_runner runner{std::cout};

	return !runner.run(suite);
}
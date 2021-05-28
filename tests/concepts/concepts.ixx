export module cmoon.tests.concepts;

import <utility>;
import <string>;
import <functional>;
import <iostream>;
import <type_traits>;
import <memory>;
import <list>;

import cmoon.test;
import cmoon.concepts;

import cmoon.tests;

namespace cmoon::tests::concepts
{
	export
	class object_test : public cmoon::test::test_case
	{
		public:
			object_test()
				: cmoon::test::test_case{"object_test"} {}

			void operator()() override
			{
				cmoon::test::assert_true(cmoon::object<int>);
				cmoon::test::assert_true(cmoon::object<double>);
				cmoon::test::assert_true(cmoon::object<std::string>);
				cmoon::test::assert_true(cmoon::object<int*>);
				cmoon::test::assert_true(cmoon::object<double*>);
				cmoon::test::assert_true(cmoon::object<std::string*>);

				cmoon::test::assert_true(cmoon::object<const int>);
				cmoon::test::assert_true(cmoon::object<const double>);
				cmoon::test::assert_true(cmoon::object<const std::string>);
				cmoon::test::assert_true(cmoon::object<const int*>);
				cmoon::test::assert_true(cmoon::object<const double*>);
				cmoon::test::assert_true(cmoon::object<const std::string*>);

				cmoon::test::assert_false(cmoon::object<int&>);
				cmoon::test::assert_false(cmoon::object<double&>);
				cmoon::test::assert_false(cmoon::object<std::string&>);
				cmoon::test::assert_false(cmoon::object<const int&>);
				cmoon::test::assert_false(cmoon::object<const double&>);
				cmoon::test::assert_false(cmoon::object<const std::string&>);
				cmoon::test::assert_false(cmoon::object<void>);
				cmoon::test::assert_false(cmoon::object<int()>);
			}
	};

	export
	class array_test : public cmoon::test::test_case
	{
		public:
			array_test()
				: cmoon::test::test_case{"array_test"} {}

			void operator()() override
			{
				cmoon::test::assert_false(cmoon::array<int>);
				cmoon::test::assert_false(cmoon::array<double>);
				cmoon::test::assert_false(cmoon::array<std::string>);
				cmoon::test::assert_false(cmoon::array<const int>);
				cmoon::test::assert_false(cmoon::array<const double>);
				cmoon::test::assert_false(cmoon::array<const std::string>);
				cmoon::test::assert_false(cmoon::array<int&>);
				cmoon::test::assert_false(cmoon::array<double&>);
				cmoon::test::assert_false(cmoon::array<std::string&>);
				cmoon::test::assert_false(cmoon::array<const int&>);
				cmoon::test::assert_false(cmoon::array<const double&>);
				cmoon::test::assert_false(cmoon::array<const std::string&>);

				cmoon::test::assert_true(cmoon::array<int[]>);
				cmoon::test::assert_true(cmoon::array<double[]>);
				cmoon::test::assert_true(cmoon::array<std::string[]>);
				cmoon::test::assert_true(cmoon::array<const int[]>);
				cmoon::test::assert_true(cmoon::array<const double[]>);
				cmoon::test::assert_true(cmoon::array<const std::string[]>);

				cmoon::test::assert_false(cmoon::array<int(&)[]>);
				cmoon::test::assert_false(cmoon::array<double(&)[]>);
				cmoon::test::assert_false(cmoon::array<std::string(&)[]>);
				cmoon::test::assert_false(cmoon::array<const int(&)[]>);
				cmoon::test::assert_false(cmoon::array<const double(&)[]>);
				cmoon::test::assert_false(cmoon::array<const std::string(&)[]>);
			}
	};

	export
	class supplier_test : public cmoon::test::test_case
	{
		public:
			supplier_test()
				: cmoon::test::test_case{"supplier_test"} {}

			void operator()() override
			{
				using int_supplier = int();
				using double_supplier = double();
				using supplies_nothing = void();

				cmoon::test::assert_true(cmoon::supplier<int_supplier, int>);
				cmoon::test::assert_true(cmoon::supplier<double_supplier, double>);

				cmoon::test::assert_false(cmoon::supplier<supplies_nothing, int>);
				cmoon::test::assert_false(cmoon::supplier<supplies_nothing, double>);

				cmoon::test::assert_false(cmoon::supplier<int(int), int>);
			}
	};

	export
	class unary_operator_test : public cmoon::test::test_case
	{
		public:
			unary_operator_test()
				: cmoon::test::test_case{"unary_operator_test"} {}

			void operator()() override
			{
				cmoon::test::assert_true(cmoon::unary_operator<std::identity, int>);
				cmoon::test::assert_true(cmoon::unary_operator<std::identity, double>);
				cmoon::test::assert_true(cmoon::unary_operator<std::identity, const int>);
				cmoon::test::assert_true(cmoon::unary_operator<std::identity, const double>);
				cmoon::test::assert_false(cmoon::unary_operator<std::plus<int>, int>);
			}
	};

	export
	class consumer_test : public cmoon::test::test_case
	{
		public:
			consumer_test()
				: cmoon::test::test_case{"consumer_test"} {}

			void operator()() override
			{
				cmoon::test::assert_true(cmoon::consumer<void(int), int>);
				cmoon::test::assert_true(cmoon::consumer<void(double), double>);

				cmoon::test::assert_false(cmoon::consumer<int(int), int>);
				cmoon::test::assert_false(cmoon::consumer<int(int, int), int>);
			}
	};

	export
	class function_object_test : public cmoon::test::test_case
	{
		public:
			function_object_test()
				: cmoon::test::test_case{"function_object_test"} {}

			void operator()() override
			{
				struct s { void operator()() {} };
				struct s2 { void operator()(int) {} };

				cmoon::test::assert_true(cmoon::function_object<s>);
				cmoon::test::assert_true(cmoon::function_object<s2, int>);
				cmoon::test::assert_true(cmoon::function_object<decltype([]{})>);

				cmoon::test::assert_false(cmoon::function_object<s2>);
				cmoon::test::assert_false(cmoon::function_object<void()>);
			}
	};

	export
	class arithmetic_test : public cmoon::test::test_case
	{
		public:
			arithmetic_test()
				: cmoon::test::test_case{"arithmetic_test"} {}

			void operator()() override
			{
				cmoon::test::assert_true(cmoon::arithmetic<char>);
				cmoon::test::assert_true(cmoon::arithmetic<bool>);
				cmoon::test::assert_true(cmoon::arithmetic<short>);
				cmoon::test::assert_true(cmoon::arithmetic<int>);
				cmoon::test::assert_true(cmoon::arithmetic<long int>);
				cmoon::test::assert_true(cmoon::arithmetic<long long int>);
				cmoon::test::assert_true(cmoon::arithmetic<long>);
				cmoon::test::assert_true(cmoon::arithmetic<long long>);
				cmoon::test::assert_true(cmoon::arithmetic<double>);
				cmoon::test::assert_true(cmoon::arithmetic<long double>);
				cmoon::test::assert_true(cmoon::arithmetic<unsigned char>);
				cmoon::test::assert_true(cmoon::arithmetic<unsigned short>);
				cmoon::test::assert_true(cmoon::arithmetic<unsigned int>);
				cmoon::test::assert_true(cmoon::arithmetic<unsigned long int>);
				cmoon::test::assert_true(cmoon::arithmetic<unsigned long long int>);
				cmoon::test::assert_true(cmoon::arithmetic<unsigned long>);
				cmoon::test::assert_true(cmoon::arithmetic<unsigned long long>);

				cmoon::test::assert_true(cmoon::arithmetic<const char>);
				cmoon::test::assert_true(cmoon::arithmetic<const bool>);
				cmoon::test::assert_true(cmoon::arithmetic<const short>);
				cmoon::test::assert_true(cmoon::arithmetic<const int>);
				cmoon::test::assert_true(cmoon::arithmetic<const long int>);
				cmoon::test::assert_true(cmoon::arithmetic<const long long int>);
				cmoon::test::assert_true(cmoon::arithmetic<const long>);
				cmoon::test::assert_true(cmoon::arithmetic<const long long>);
				cmoon::test::assert_true(cmoon::arithmetic<const double>);
				cmoon::test::assert_true(cmoon::arithmetic<const long double>);
				cmoon::test::assert_true(cmoon::arithmetic<const unsigned char>);
				cmoon::test::assert_true(cmoon::arithmetic<const unsigned short>);
				cmoon::test::assert_true(cmoon::arithmetic<const unsigned int>);
				cmoon::test::assert_true(cmoon::arithmetic<const unsigned long int>);
				cmoon::test::assert_true(cmoon::arithmetic<const unsigned long long int>);
				cmoon::test::assert_true(cmoon::arithmetic<const unsigned long>);
				cmoon::test::assert_true(cmoon::arithmetic<const unsigned long long>);

				cmoon::test::assert_false(cmoon::arithmetic<char&>);
				cmoon::test::assert_false(cmoon::arithmetic<bool&>);
				cmoon::test::assert_false(cmoon::arithmetic<short&>);
				cmoon::test::assert_false(cmoon::arithmetic<int&>);
				cmoon::test::assert_false(cmoon::arithmetic<long int&>);
				cmoon::test::assert_false(cmoon::arithmetic<long long int&>);
				cmoon::test::assert_false(cmoon::arithmetic<long&>);
				cmoon::test::assert_false(cmoon::arithmetic<long long&>);
				cmoon::test::assert_false(cmoon::arithmetic<double&>);
				cmoon::test::assert_false(cmoon::arithmetic<long double&>);
				cmoon::test::assert_false(cmoon::arithmetic<unsigned char&>);
				cmoon::test::assert_false(cmoon::arithmetic<unsigned short&>);
				cmoon::test::assert_false(cmoon::arithmetic<unsigned int&>);
				cmoon::test::assert_false(cmoon::arithmetic<unsigned long int&>);
				cmoon::test::assert_false(cmoon::arithmetic<unsigned long long int&>);
				cmoon::test::assert_false(cmoon::arithmetic<unsigned long&>);
				cmoon::test::assert_false(cmoon::arithmetic<unsigned long long&>);

				cmoon::test::assert_false(cmoon::arithmetic<const char&>);
				cmoon::test::assert_false(cmoon::arithmetic<const bool&>);
				cmoon::test::assert_false(cmoon::arithmetic<const short&>);
				cmoon::test::assert_false(cmoon::arithmetic<const int&>);
				cmoon::test::assert_false(cmoon::arithmetic<const long int&>);
				cmoon::test::assert_false(cmoon::arithmetic<const long long int&>);
				cmoon::test::assert_false(cmoon::arithmetic<const long&>);
				cmoon::test::assert_false(cmoon::arithmetic<const long long&>);
				cmoon::test::assert_false(cmoon::arithmetic<const double&>);
				cmoon::test::assert_false(cmoon::arithmetic<const long double&>);
				cmoon::test::assert_false(cmoon::arithmetic<const unsigned char&>);
				cmoon::test::assert_false(cmoon::arithmetic<const unsigned short&>);
				cmoon::test::assert_false(cmoon::arithmetic<const unsigned int&>);
				cmoon::test::assert_false(cmoon::arithmetic<const unsigned long int&>);
				cmoon::test::assert_false(cmoon::arithmetic<const unsigned long long int&>);
				cmoon::test::assert_false(cmoon::arithmetic<const unsigned long&>);
				cmoon::test::assert_false(cmoon::arithmetic<const unsigned long long&>);

				cmoon::test::assert_false(cmoon::arithmetic<std::string>);
				cmoon::test::assert_false(cmoon::arithmetic<const std::string>);
				cmoon::test::assert_false(cmoon::arithmetic<std::string&>);
				cmoon::test::assert_false(cmoon::arithmetic<const std::string&>);
			}
	};

	export
	class signed_arithmetic_test : public cmoon::test::test_case
	{
		public:
			signed_arithmetic_test()
				: cmoon::test::test_case{"signed_arithmetic_test"} {}

			void operator()() override
			{
				cmoon::test::assert_true(cmoon::signed_arithmetic<char>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<bool>);
				cmoon::test::assert_true(cmoon::signed_arithmetic<short>);
				cmoon::test::assert_true(cmoon::signed_arithmetic<int>);
				cmoon::test::assert_true(cmoon::signed_arithmetic<long int>);
				cmoon::test::assert_true(cmoon::signed_arithmetic<long long int>);
				cmoon::test::assert_true(cmoon::signed_arithmetic<long>);
				cmoon::test::assert_true(cmoon::signed_arithmetic<long long>);
				cmoon::test::assert_true(cmoon::signed_arithmetic<double>);
				cmoon::test::assert_true(cmoon::signed_arithmetic<long double>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<unsigned char>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<unsigned short>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<unsigned int>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<unsigned long int>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<unsigned long long int>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<unsigned long>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<unsigned long long>);

				cmoon::test::assert_true(cmoon::signed_arithmetic<const char>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<const bool>);
				cmoon::test::assert_true(cmoon::signed_arithmetic<const short>);
				cmoon::test::assert_true(cmoon::signed_arithmetic<const int>);
				cmoon::test::assert_true(cmoon::signed_arithmetic<const long int>);
				cmoon::test::assert_true(cmoon::signed_arithmetic<const long long int>);
				cmoon::test::assert_true(cmoon::signed_arithmetic<const long>);
				cmoon::test::assert_true(cmoon::signed_arithmetic<const long long>);
				cmoon::test::assert_true(cmoon::signed_arithmetic<const double>);
				cmoon::test::assert_true(cmoon::signed_arithmetic<const long double>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<const unsigned char>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<const unsigned short>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<const unsigned int>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<const unsigned long int>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<const unsigned long long int>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<const unsigned long>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<const unsigned long long>);

				cmoon::test::assert_false(cmoon::signed_arithmetic<char&>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<bool&>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<short&>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<int&>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<long int&>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<long long int&>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<long&>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<long long&>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<double&>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<long double&>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<unsigned char&>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<unsigned short&>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<unsigned int&>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<unsigned long int&>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<unsigned long long int&>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<unsigned long&>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<unsigned long long&>);

				cmoon::test::assert_false(cmoon::signed_arithmetic<const char&>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<const bool&>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<const short&>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<const int&>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<const long int&>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<const long long int&>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<const long&>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<const long long&>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<const double&>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<const long double&>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<const unsigned char&>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<const unsigned short&>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<const unsigned int&>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<const unsigned long int&>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<const unsigned long long int&>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<const unsigned long&>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<const unsigned long long&>);

				cmoon::test::assert_false(cmoon::signed_arithmetic<std::string>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<const std::string>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<std::string&>);
				cmoon::test::assert_false(cmoon::signed_arithmetic<const std::string&>);
			}
	};

	export
	class unsigned_arithmetic_test : public cmoon::test::test_case
	{
		public:
			unsigned_arithmetic_test()
				: cmoon::test::test_case{"unsigned_arithmetic_test"} {}

			void operator()() override
			{
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<char>);
				cmoon::test::assert_true(cmoon::unsigned_arithmetic<bool>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<short>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<int>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<long int>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<long long int>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<long>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<long long>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<double>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<long double>);
				cmoon::test::assert_true(cmoon::unsigned_arithmetic<unsigned char>);
				cmoon::test::assert_true(cmoon::unsigned_arithmetic<unsigned short>);
				cmoon::test::assert_true(cmoon::unsigned_arithmetic<unsigned int>);
				cmoon::test::assert_true(cmoon::unsigned_arithmetic<unsigned long int>);
				cmoon::test::assert_true(cmoon::unsigned_arithmetic<unsigned long long int>);
				cmoon::test::assert_true(cmoon::unsigned_arithmetic<unsigned long>);
				cmoon::test::assert_true(cmoon::unsigned_arithmetic<unsigned long long>);

				cmoon::test::assert_false(cmoon::unsigned_arithmetic<const char>);
				cmoon::test::assert_true(cmoon::unsigned_arithmetic<const bool>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<const short>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<const int>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<const long int>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<const long long int>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<const long>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<const long long>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<const double>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<const long double>);
				cmoon::test::assert_true(cmoon::unsigned_arithmetic<const unsigned char>);
				cmoon::test::assert_true(cmoon::unsigned_arithmetic<const unsigned short>);
				cmoon::test::assert_true(cmoon::unsigned_arithmetic<const unsigned int>);
				cmoon::test::assert_true(cmoon::unsigned_arithmetic<const unsigned long int>);
				cmoon::test::assert_true(cmoon::unsigned_arithmetic<const unsigned long long int>);
				cmoon::test::assert_true(cmoon::unsigned_arithmetic<const unsigned long>);
				cmoon::test::assert_true(cmoon::unsigned_arithmetic<const unsigned long long>);

				cmoon::test::assert_false(cmoon::unsigned_arithmetic<char&>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<bool&>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<short&>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<int&>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<long int&>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<long long int&>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<long&>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<long long&>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<double&>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<long double&>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<unsigned char&>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<unsigned short&>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<unsigned int&>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<unsigned long int&>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<unsigned long long int&>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<unsigned long&>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<unsigned long long&>);

				cmoon::test::assert_false(cmoon::unsigned_arithmetic<const char&>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<const bool&>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<const short&>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<const int&>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<const long int&>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<const long long int&>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<const long&>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<const long long&>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<const double&>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<const long double&>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<const unsigned char&>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<const unsigned short&>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<const unsigned int&>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<const unsigned long int&>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<const unsigned long long int&>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<const unsigned long&>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<const unsigned long long&>);

				cmoon::test::assert_false(cmoon::unsigned_arithmetic<std::string>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<const std::string>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<std::string&>);
				cmoon::test::assert_false(cmoon::unsigned_arithmetic<const std::string&>);
			}
	};

	export
	class complete_type_test : public cmoon::test::test_case
	{
		public:
			complete_type_test()
				: cmoon::test::test_case{"complete_type_test"} {}

			void operator()() override
			{
				class f;
				
				cmoon::test::assert_true(cmoon::complete_type<int>);
				cmoon::test::assert_true(cmoon::complete_type<double>);
				cmoon::test::assert_true(cmoon::complete_type<const int>);
				cmoon::test::assert_true(cmoon::complete_type<const double>);
				cmoon::test::assert_true(cmoon::complete_type<std::string>);
				cmoon::test::assert_true(cmoon::complete_type<const std::string>);

				cmoon::test::assert_true(cmoon::complete_type<int&>);
				cmoon::test::assert_true(cmoon::complete_type<double&>);
				cmoon::test::assert_true(cmoon::complete_type<const int&>);
				cmoon::test::assert_true(cmoon::complete_type<const double&>);
				cmoon::test::assert_true(cmoon::complete_type<std::string&>);
				cmoon::test::assert_true(cmoon::complete_type<const std::string&>);

				cmoon::test::assert_false(cmoon::complete_type<f>);
			}
	};

	export
	class had_size_test : public cmoon::test::test_case
	{
		public:
			had_size_test()
				: cmoon::test::test_case{"had_size_test"} {}

			void operator()() override
			{
				cmoon::test::assert_true(cmoon::has_size<std::string>);
				cmoon::test::assert_true(cmoon::has_size<const std::string>);
				cmoon::test::assert_true(cmoon::has_size<std::string&>);
				cmoon::test::assert_true(cmoon::has_size<const std::string&>);

				cmoon::test::assert_false(cmoon::has_size<int>);
				cmoon::test::assert_false(cmoon::has_size<const int>);
				cmoon::test::assert_false(cmoon::has_size<int&>);
				cmoon::test::assert_false(cmoon::has_size<const int&>);
			}
	};

	export
	class has_type_test : public cmoon::test::test_case
	{
		public:
			has_type_test()
				: cmoon::test::test_case{"has_type_test"} {}

			void operator()() override
			{
				cmoon::test::assert_true(cmoon::has_type<std::type_identity<int>>);
				cmoon::test::assert_false(cmoon::has_type<int>);
			}
	};

	export
	class stream_writable_test : public cmoon::test::test_case
	{
		public:
			stream_writable_test()
				: cmoon::test::test_case{"stream_writable_test"} {}

			void operator()() override
			{
				struct s {};

				cmoon::test::assert_true(cmoon::stream_writable<std::string, std::ostream>);
				cmoon::test::assert_false(cmoon::stream_writable<s, std::ostream>);
			}
	};

	export
	class allocator_test : public cmoon::test::test_case
	{
		public:
			allocator_test()
				: cmoon::test::test_case{"allocator_test"} {}
			
			void operator()() override
			{
				cmoon::test::assert_true(cmoon::allocator<std::allocator<int>>);
				cmoon::test::assert_false(cmoon::allocator<int>);
			}
	};

	export
	class string_literal_test : public cmoon::test::test_case
	{
		public:
			string_literal_test()
				: cmoon::test::test_case{"string_literal_test"} {}

			void operator()() override
			{
				const char s1[] = "Hello!";
				const auto s2 = std::string{"Hello!"};

				cmoon::test::assert_true(cmoon::string_literal<decltype(s1)>);
				cmoon::test::assert_false(cmoon::string_literal<decltype(s2)>);
			}
	};

	export
	class char_literal_test : public cmoon::test::test_case
	{
		public:
			char_literal_test()
				: cmoon::test::test_case{"char_literal_test"} {}

			void operator()() override
			{
				cmoon::test::assert_true(cmoon::char_literal<char>);
				cmoon::test::assert_true(cmoon::char_literal<unsigned char>);
				cmoon::test::assert_true(cmoon::char_literal<const char>);
				cmoon::test::assert_true(cmoon::char_literal<const unsigned char>);
				cmoon::test::assert_true(cmoon::char_literal<signed char>);
				cmoon::test::assert_true(cmoon::char_literal<const signed char>);
				cmoon::test::assert_true(cmoon::char_literal<wchar_t>);
				cmoon::test::assert_true(cmoon::char_literal<const wchar_t>);
				cmoon::test::assert_true(cmoon::char_literal<char16_t>);
				cmoon::test::assert_true(cmoon::char_literal<const char16_t>);
				cmoon::test::assert_true(cmoon::char_literal<char32_t>);
				cmoon::test::assert_true(cmoon::char_literal<const char32_t>);
				cmoon::test::assert_true(cmoon::char_literal<char8_t>);
				cmoon::test::assert_true(cmoon::char_literal<const char8_t>);
				
				cmoon::test::assert_false(cmoon::char_literal<char&>);
				cmoon::test::assert_false(cmoon::char_literal<const char&>);
				cmoon::test::assert_false(cmoon::char_literal<unsigned char&>);
				cmoon::test::assert_false(cmoon::char_literal<const unsigned char&>);
				cmoon::test::assert_false(cmoon::char_literal<wchar_t&>);
				cmoon::test::assert_false(cmoon::char_literal<const wchar_t&>);
				cmoon::test::assert_false(cmoon::char_literal<char16_t&>);
				cmoon::test::assert_false(cmoon::char_literal<const char16_t&>);
				cmoon::test::assert_false(cmoon::char_literal<char32_t&>);
				cmoon::test::assert_false(cmoon::char_literal<const char32_t&>);
				cmoon::test::assert_false(cmoon::char_literal<char8_t&>);
				cmoon::test::assert_false(cmoon::char_literal<const char8_t&>);

				cmoon::test::assert_false(cmoon::char_literal<bool>);
				cmoon::test::assert_false(cmoon::char_literal<short>);
				cmoon::test::assert_false(cmoon::char_literal<int>);
				cmoon::test::assert_false(cmoon::char_literal<long>);
				cmoon::test::assert_false(cmoon::char_literal<double>);
			}
	};

	export
	class nothrow_comparable_with_test : public cmoon::test::test_case
	{
		public:
			nothrow_comparable_with_test()
				: cmoon::test::test_case{"nothrow_comparable_with_test"} {}

			void operator()() override
			{
				cmoon::test::assert_true(cmoon::nothrow_comparable_with<int, int>);
				cmoon::test::assert_false(cmoon::nothrow_comparable_with<std::list<int>, std::list<int>>);
			}
	};

	export
	class formattable_test : public cmoon::test::test_case
	{
		public:
			formattable_test()
				: cmoon::test::test_case{"formattable_test"} {}

			void operator()() override
			{
				struct s {};

				cmoon::test::assert_true(cmoon::formattable<bool>);
				cmoon::test::assert_true(cmoon::formattable<short>);
				cmoon::test::assert_true(cmoon::formattable<int>);
				cmoon::test::assert_true(cmoon::formattable<long>);
				cmoon::test::assert_true(cmoon::formattable<double>);
				cmoon::test::assert_true(cmoon::formattable<std::string>);
				cmoon::test::assert_true(cmoon::formattable<std::string_view>);

				cmoon::test::assert_false(cmoon::formattable<s>);
			}
	};

	export
	class specialization_of_test : public cmoon::test::test_case
	{
		public:
			specialization_of_test()
				: cmoon::test::test_case{"specialization_of_test"} {}

			void operator()() override
			{
				cmoon::test::assert_true(cmoon::specialization_of<std::string, std::basic_string>);

				cmoon::test::assert_false(cmoon::specialization_of<int, std::bool_constant>);
			}
	};
}

namespace cmoon::tests
{
	export
	template<>
	cmoon::test::test_suite get_test_suite<library::concepts>()
	{
		cmoon::test::test_suite suite{"Concepts library tests"};
		suite.add_test_case<concepts::object_test>();
		suite.add_test_case<concepts::array_test>();
		suite.add_test_case<concepts::supplier_test>();
		suite.add_test_case<concepts::unary_operator_test>();
		suite.add_test_case<concepts::consumer_test>();
		suite.add_test_case<concepts::function_object_test>();
		suite.add_test_case<concepts::arithmetic_test>();
		suite.add_test_case<concepts::signed_arithmetic_test>();
		suite.add_test_case<concepts::unsigned_arithmetic_test>();
		suite.add_test_case<concepts::complete_type_test>();
		suite.add_test_case<concepts::had_size_test>();
		suite.add_test_case<concepts::has_type_test>();
		suite.add_test_case<concepts::stream_writable_test>();
		suite.add_test_case<concepts::allocator_test>();
		suite.add_test_case<concepts::string_literal_test>();
		suite.add_test_case<concepts::char_literal_test>();
		suite.add_test_case<concepts::nothrow_comparable_with_test>();
		suite.add_test_case<concepts::formattable_test>();
		suite.add_test_case<concepts::specialization_of_test>();

		return std::move(suite);
	}
}
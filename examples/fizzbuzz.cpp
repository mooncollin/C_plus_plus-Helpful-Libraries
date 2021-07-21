import <iostream>;
import <typeinfo>;
import <type_traits>;

import cmoon.meta;

struct Fizz {};
struct Buzz {};
struct FizzBuzz {};

template<auto N>
struct Number {};

template<auto N>
struct fizz_buzz_maker
{
	static constexpr Number<N> value{};
};

template<auto N>
	requires(N % 5 == 0 && N % 3 == 0)
struct fizz_buzz_maker<N>
{
	static constexpr FizzBuzz value{};
};

template<auto N>
	requires(N % 3 == 0 && N % 5 != 0)
struct fizz_buzz_maker<N>
{
	static constexpr Fizz value{};
};

template<auto N>
	requires(N % 5 == 0 && N % 3 != 0)
struct fizz_buzz_maker<N>
{
	static constexpr Buzz value{};
};

int main()
{
	using integer_seq = cmoon::meta::iota<1, 101>;
	using fizz_buzz_values = integer_seq::transform<fizz_buzz_maker>;

	fizz_buzz_values::for_each([](auto v) {
		std::cout << typeid(v).name() << '\n';
	});
}
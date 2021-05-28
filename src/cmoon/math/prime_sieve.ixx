export module cmoon.math.prime_sieve;

import <cmath>;
import <vector>;
import <cstddef>;
import <cstdint>;
import <algorithm>;
import <stdexcept>;
import <format>;

namespace cmoon
{
	[[nodiscard]] std::size_t estimate_number_primes(std::uintmax_t upper_bound)
	{
		if (upper_bound < 17)
		{
			return 6;
		}

		return static_cast<std::size_t>(upper_bound / (std::log(upper_bound) - 1.5));
	}

	void sieve_of_eratosthenes(std::vector<bool>& composite) noexcept
	{
		const auto len {std::size(composite)};
		std::size_t d1 {8};
		std::size_t d2 {8};
		std::size_t p1 {3};
		std::size_t p2 {7};
		std::size_t s {7};
		std::size_t s2 {3};
		std::size_t n {0};
		bool toggle {false};

		while (s < len)
		{
			if (!composite[n++])
			{
				for (auto k {s}; k < len; k += p1 + p2)
				{
					composite[k] = true;
					composite[k + s2] = true;
				}
			}

			toggle = !toggle;
			if (toggle)
			{
				s += d2;
				d2 += 16;
				p1 += 2;
				p2 += 2;
				s2 = p2;
			}
			else
			{
				s += d1;
				d2 += 8;
				p1 += 2;
				p2 += 6;
				s2 = p1;
			}
		}
	}

	class prime_sieve
	{
		public:
			using value_type = std::uintmax_t;

			prime_sieve(value_type up_to)
				: prime_list(estimate_number_primes(up_to))
			{
				prime_list.resize(fill_prime_list(up_to));
			}

			[[nodiscard]] value_type operator[](std::size_t i) const noexcept
			{
				return prime_list[i];
			}

			[[nodiscard]] value_type at(std::size_t i) const
			{
				return prime_list.at(i);
			}

			[[nodiscard]] bool is_prime(value_type cand) const noexcept
			{
				return std::binary_search(std::begin(prime_list), std::end(prime_list), cand);
			}

			[[nodiscard]] value_type next_prime(value_type n) const
			{
				const auto next = std::upper_bound(std::begin(prime_list), std::end(prime_list), n);
				if (next == std::end(prime_list))
				{
					throw std::out_of_range{std::format("no prime computed after {}", n)};
				}

				return *next;
			}

			auto begin() noexcept
			{
				return std::begin(prime_list);
			}

			auto begin() const noexcept
			{
				return std::begin(prime_list);
			}

			auto cbegin() const noexcept
			{
				return std::cbegin(prime_list);
			}

			auto end() noexcept
			{
				return std::end(prime_list);
			}

			auto end() const noexcept
			{
				return std::end(prime_list);
			}

			auto cend() const noexcept
			{
				return std::cend(prime_list);
			}

			auto rbegin() noexcept
			{
				return std::rbegin(prime_list);
			}

			auto rbegin() const noexcept
			{
				return std::rbegin(prime_list);
			}

			auto crbegin() const noexcept
			{
				return std::crbegin(prime_list);
			}

			auto rend() noexcept
			{
				return std::rend(prime_list);
			}

			auto rend() const noexcept
			{
				return std::rend(prime_list);
			}

			auto crend() const noexcept
			{
				return std::crend(prime_list);
			}
		private:
			std::vector<value_type> prime_list;

			std::size_t fill_prime_list(value_type up_to)
			{
				std::vector<bool> composite(up_to / 3);

				sieve_of_eratosthenes(composite);
				
				bool toggle {false};
				std::size_t p {5};
				std::size_t i {0};
				std::size_t j {2};

				prime_list[0] = 2;
				prime_list[1] = 3;
				
				while (p <= up_to)
				{
					if (!composite[i++])
					{
						prime_list[j++] = p;
					}

					toggle = !toggle;
					p += toggle ? 2 : 4;
				}

				return j;
			}
	};
}
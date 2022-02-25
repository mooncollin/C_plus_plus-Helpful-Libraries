export module cmoon.test.test_suite;

import <type_traits>;
import <vector>;
import <memory>;
import <concepts>;
import <string>;
import <iterator>;
import <cstddef>;
import <exception>;

import cmoon.test.test_case;

namespace cmoon::test
{
	export
	template<class Allocator = std::allocator<test_case>>
	class test_suite
	{
		using container = std::vector<test_case*, typename std::allocator_traits<Allocator>::template rebind_alloc<test_case*>>;

		template<class Allocator2>
		friend class test_suite;

		public:
			using allocator_type = typename std::allocator_traits<Allocator>::template rebind_alloc<test_case>;

			test_suite() noexcept(std::is_nothrow_default_constructible_v<container>) = default;

			test_suite(std::string name) noexcept(std::is_nothrow_default_constructible_v<container>)
				: name_{std::move(name)} {}

			test_suite(const Allocator& alloc) noexcept(std::is_nothrow_constructible_v<container, Allocator>)
				: cases{alloc} {}

			test_suite(std::string name, const Allocator& alloc) noexcept(std::is_nothrow_constructible_v<container, Allocator>)
				: name_{std::move(name)}, cases{alloc} {}

			test_suite(const test_suite&) = delete;
			test_suite& operator=(const test_suite&) = delete;

			test_suite(test_suite&&) noexcept = default;
			test_suite& operator=(test_suite&&) noexcept = default;

			~test_suite() noexcept
			{
				deallocate_test_cases();
			}

			template<std::derived_from<test_case> T, class... Args>
			void add_test_case(Args&&... args)
			{
				using traits = typename std::allocator_traits<allocator_type>::template rebind_traits<T>;
				typename traits::allocator_type alloc {get_allocator()};
				T* ptr {traits::allocate(alloc, 1)};
				try
				{
					traits::construct(alloc, ptr, std::forward<Args>(args)...);
				}
				catch (...)
				{
					traits::deallocate(alloc, ptr, 1);
					throw std::current_exception();
				}

				try
				{
					cases.push_back(ptr);
				}
				catch (...)
				{
					traits::destroy(alloc, ptr);
					traits::deallocate(alloc, ptr, 1);
					throw std::current_exception();
				}
			}

			void name(std::string n) noexcept
			{
				name_ = std::move(n);
			}

			const std::string& name() const noexcept
			{
				return name_;
			}

			void add_test_suite(test_suite&& suite)
			{
				cases.insert(std::end(cases), std::begin(suite), std::end(suite));
				suite.cases.clear();
			}

			template<std::output_iterator<test_result> Out>
			void run_all(Out out)
			{
				for (auto t_case : cases)
				{
					*out = t_case->run();
				}
			}

			[[nodiscard]] auto begin() const noexcept
			{
				return std::begin(cases);
			}

			[[nodiscard]] auto end() const noexcept
			{
				return std::end(cases);
			}

			[[nodiscard]] std::size_t size() const noexcept
			{
				return std::size(cases);
			}

			void clear() noexcept
			{
				deallocate_test_cases();
				cases.clear();
			}

			allocator_type get_allocator() const noexcept
			{
				return cases.get_allocator();
			}
		private:
			std::string name_;
			container cases;

			void deallocate_test_cases() noexcept
			{
				using traits = typename std::allocator_traits<allocator_type>;
				allocator_type alloc {get_allocator()};

				for (auto ptr : cases)
				{
					traits::destroy(alloc, ptr);
					traits::deallocate(alloc, ptr, 1);
				}
			}
	};
}
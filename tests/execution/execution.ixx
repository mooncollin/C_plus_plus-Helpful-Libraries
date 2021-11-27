export module cmoon.tests.execution;
export import cmoon.tests.execution.set_value;
export import cmoon.tests.execution.set_done;
export import cmoon.tests.execution.set_error;
export import cmoon.tests.execution.start;
//export import cmoon.tests.execution.connect;
export import cmoon.tests.execution.schedule;
//export import cmoon.tests.execution.just;
//export import cmoon.tests.execution.just_error;
//export import cmoon.tests.execution.just_done;
//export import cmoon.tests.execution.then;
//export import cmoon.tests.execution.upon_error;
//export import cmoon.tests.execution.let_value;
//export import cmoon.tests.execution.bulk;
//export import cmoon.tests.execution.split;
export import cmoon.tests.execution.when_all;
export import cmoon.tests.execution.done_as_optional;
export import cmoon.tests.execution.ensure_started;
//export import cmoon.tests.execution.sync_wait;
//export import cmoon.tests.execution.execute;
//export import cmoon.tests.execution.async;

import <utility>;
import <exception>;
import <atomic>;

import cmoon.meta;
import cmoon.test;
import cmoon.execution;

import cmoon.tests;

namespace cmoon::tests::execution
{
	struct void_receiver
	{
		friend void tag_invoke(cmoon::execution::set_done_t, void_receiver&&) noexcept {}

		template<class E>
		friend void tag_invoke(cmoon::execution::set_error_t, void_receiver&&, E&&) noexcept {}

		friend void tag_invoke(cmoon::execution::set_value_t, void_receiver&&) {}
	};

	static_assert(cmoon::execution::receiver<void_receiver>);

	export
	class connect_test : public cmoon::test::test_case
	{
		struct connect_s : public cmoon::execution::sender_base
		{
			template<class R>
			struct op_state
			{
				friend void tag_invoke(cmoon::execution::start_t, op_state&) noexcept {}
			};

			template<class R>
			friend op_state<R> tag_invoke(cmoon::execution::connect_t, connect_s& s, R&&)
			{
				s.connected = true;
				return op_state<R>{};
			}

			bool connected {false};
		};

		static_assert(cmoon::execution::operation_state<connect_s::op_state<void_receiver>>);
		static_assert(cmoon::execution::sender<connect_s>);

		public:
			connect_test()
				: cmoon::test::test_case{"connect_test"} {}

			void operator()() override
			{
				connect_s c;

				auto o = cmoon::execution::connect(c, void_receiver{});

				cmoon::test::assert_true(c.connected);
			}
	};

	export
	class just_set_value_test : public cmoon::test::test_case
	{
		struct just_r
		{
			template<class... Ts>
			friend void tag_invoke(cmoon::execution::set_value_t, just_r&& r, Ts&&...)
			{
				r.value_set = true;
			}

			template<class E>
			friend void tag_invoke(cmoon::execution::set_error_t, just_r&& r, E&&) noexcept
			{
				r.error_set = true;
			}

			friend void tag_invoke(cmoon::execution::set_done_t, just_r&& r) noexcept
			{
				r.done_set = true;
			}

			bool value_set {false};
			bool error_set {false};
			bool done_set {false};
		};

		static_assert(cmoon::execution::receiver<just_r>, "just_r must be a receiver for this test to work");

		public:
			just_set_value_test()
				: cmoon::test::test_case{"just_set_value_test"} {}

			void operator()() override
			{
				just_r r;
				auto j = cmoon::execution::just(1, 2, 3);

				cmoon::execution::start(cmoon::execution::connect(std::move(j), r));

				cmoon::test::assert_true(r.value_set);
				cmoon::test::assert_false(r.error_set);
				cmoon::test::assert_false(r.done_set);
			}
	};

	export
	class just_set_error_test : public cmoon::test::test_case
	{
		struct just_r
		{
			template<class... Ts>
			friend void tag_invoke(cmoon::execution::set_value_t, just_r&&, Ts&&...)
			{
				throw std::exception{};
			}

			template<class E>
			friend void tag_invoke(cmoon::execution::set_error_t, just_r&& r, E&&) noexcept
			{
				r.error_set = true;
			}

			friend void tag_invoke(cmoon::execution::set_done_t, just_r&& r) noexcept
			{
				r.done_set = true;
			}

			bool value_set {false};
			bool error_set {false};
			bool done_set {false};
		};

		public:
			just_set_error_test()
				: cmoon::test::test_case{"just_set_error_test"} {}

			void operator()() override
			{
				just_r r;
				auto j = cmoon::execution::just(1, 2, 3);

				cmoon::execution::start(cmoon::execution::connect(std::move(j), r));

				cmoon::test::assert_false(r.value_set);
				cmoon::test::assert_true(r.error_set);
				cmoon::test::assert_false(r.done_set);
			}
	};

	export
	class just_error_test : public cmoon::test::test_case
	{
		struct just_r
		{
			template<class... Ts>
			friend void tag_invoke(cmoon::execution::set_value_t, just_r&& r, Ts&&...)
			{
				r.value_set = true;
			}

			template<class E>
			friend void tag_invoke(cmoon::execution::set_error_t, just_r&& r, E&&) noexcept
			{
				r.error_set = true;
			}

			friend void tag_invoke(cmoon::execution::set_done_t, just_r&& r) noexcept
			{
				r.done_set = true;
			}

			bool value_set {false};
			bool error_set {false};
			bool done_set {false};
		};

		public:
			just_error_test()
				: cmoon::test::test_case{"just_error_test"} {}

			void operator()() override
			{
				just_r r;
				auto j = cmoon::execution::just_error(std::exception{});

				cmoon::execution::start(cmoon::execution::connect(std::move(j), r));

				cmoon::test::assert_false(r.value_set);
				cmoon::test::assert_true(r.error_set);
				cmoon::test::assert_false(r.done_set);
			}
	};

	export
	class just_done_test : public cmoon::test::test_case
	{
		struct just_r
		{
			template<class... Ts>
			friend void tag_invoke(cmoon::execution::set_value_t, just_r&& r, Ts&&...)
			{
				r.value_set = true;
			}

			template<class E>
			friend void tag_invoke(cmoon::execution::set_error_t, just_r&& r, E&&) noexcept
			{
				r.error_set = true;
			}

			friend void tag_invoke(cmoon::execution::set_done_t, just_r&& r) noexcept
			{
				r.done_set = true;
			}

			bool value_set {false};
			bool error_set {false};
			bool done_set {false};
		};

		public:
			just_done_test()
				: cmoon::test::test_case{"just_done_test"} {}

			void operator()() override
			{
				just_r r;
				auto j = cmoon::execution::just_done();

				cmoon::execution::start(cmoon::execution::connect(std::move(j), r));

				cmoon::test::assert_false(r.value_set);
				cmoon::test::assert_false(r.error_set);
				cmoon::test::assert_true(r.done_set);
			}
	};

	export
	class then_test : public cmoon::test::test_case
	{
		struct then_r
		{
			constexpr friend void tag_invoke(cmoon::execution::set_value_t, then_r&& r, int v)
			{
				r.value = v;
			}

			template<class E>
			constexpr friend void tag_invoke(cmoon::execution::set_error_t, then_r&&, E&&) noexcept {}

			constexpr friend void tag_invoke(cmoon::execution::set_done_t, then_r&&) noexcept {}

			int value {0};
		};

		static_assert(cmoon::execution::receiver<then_r>);

		public:
			then_test()
				: cmoon::test::test_case{"then_test"} {}

			void operator()() override
			{
				constexpr int expected {12};

				constexpr auto f = [](int i1, int i2, int i3) noexcept {
					return i1 + i2 + i3;
				};

				constexpr auto f2 = [](int i) noexcept {
					return i * 2;
				};

				then_r r;

				auto s = cmoon::execution::just(1, 2, 3) |
						 cmoon::execution::then(f) |
						 cmoon::execution::then(f2);

				cmoon::execution::start(cmoon::execution::connect(std::move(s), r));

				cmoon::test::assert_equal(r.value, expected);
			}
	};

	export
	class upon_error_test : public cmoon::test::test_case
	{
		public:
			upon_error_test()
				: cmoon::test::test_case{"upon_error_test"} {}

			void operator()() override
			{
				constexpr int expected {5};
				int value {0};

				auto s = cmoon::execution::just() |
						 cmoon::execution::then([] { 
							throw std::exception{};
						 }) |
						 cmoon::execution::upon_error([&value, expected](auto&&) {
							value = expected;
						 });

				cmoon::execution::start_detached(std::move(s));

				cmoon::test::assert_equal(value, expected);
			}
	};

	export
	class let_value_test : public cmoon::test::test_case
	{
		public:
			let_value_test()
				: cmoon::test::test_case{"let_value_test"} {}

			void operator()() override
			{
				constexpr int expected {5};
				int value {0};

				auto s = cmoon::execution::just() |
						 cmoon::execution::let_value([&value, expected] {
							return cmoon::execution::just() |
								   cmoon::execution::then([&value, expected] {
										value = expected;
								   });
						 });

				cmoon::execution::start_detached(std::move(s));

				cmoon::test::assert_equal(value, expected);
			}
	};

	export
	class bulk_test : public cmoon::test::test_case
	{
		public:
			bulk_test()
				: cmoon::test::test_case{"bulk_test"} {}

			void operator()() override
			{
				constexpr int expected {5};
				int value {0};

				auto s = cmoon::execution::just() |
						 cmoon::execution::bulk([&value](auto) { ++value; }, expected);

				cmoon::execution::start_detached(std::move(s));

				cmoon::test::assert_equal(value, expected);
			}
	};

	export
	class split_single_test : public cmoon::test::test_case
	{
		struct dummy_r
		{
			friend void tag_invoke(cmoon::execution::set_value_t, dummy_r&& r, int v) noexcept
			{
				*r.v = v;
				r.v->notify_one();
			}

			template<class E>
			friend void tag_invoke(cmoon::execution::set_error_t, dummy_r&&, E&&) noexcept
			{
			}

			friend void tag_invoke(cmoon::execution::set_done_t, dummy_r&&) noexcept
			{
			}

			std::atomic<int>* v;
		};

		public:
			split_single_test()
				: cmoon::test::test_case{"split_single_test"} {}

			void operator()() override
			{
				constexpr int expected {5};
				std::atomic<int> value {0};
				dummy_r r{&value};
				auto s = cmoon::execution::split(
							cmoon::execution::just(expected)
						 );

				cmoon::execution::start(
					cmoon::execution::connect(
						s,
						std::move(r)
					)
				);

				value.wait(0);
				cmoon::test::assert_equal(value, expected);
			}
	};

	export
	class split_multi_test : public cmoon::test::test_case
	{
		struct dummy_r
		{
			friend void tag_invoke(cmoon::execution::set_value_t, dummy_r&& r, int v) noexcept
			{
				*r.v = v;
				r.v->notify_one();
			}

			template<class E>
			friend void tag_invoke(cmoon::execution::set_error_t, dummy_r&&, E&&) noexcept
			{
			}

			friend void tag_invoke(cmoon::execution::set_done_t, dummy_r&&) noexcept
			{
			}

			std::atomic<int>* v;
		};

		public:
			split_multi_test()
				: cmoon::test::test_case{"split_multi_test"} {}

			void operator()() override
			{
				constexpr int expected {5};
				std::atomic<int> value {0};
				std::atomic<int> value2 {0};
				dummy_r r{&value};
				dummy_r r2{&value2};
				auto s = cmoon::execution::split(
							cmoon::execution::just(expected)
						 );

				cmoon::execution::start(
					cmoon::execution::connect(
						s,
						std::move(r)
					)
				);

				value.wait(0);
				cmoon::test::assert_equal(value, expected);

				cmoon::execution::start(
					cmoon::execution::connect(
						s,
						std::move(r2)
					)
				);

				value2.wait(0);
				cmoon::test::assert_equal(value2, expected);
			}
	};

	export
	class split_cancel_test : public cmoon::test::test_case
	{
		struct dummy_r
		{
			friend void tag_invoke(cmoon::execution::set_value_t, dummy_r&& r, int v) noexcept
			{
				*r.v = v;
				r.v->notify_one();
			}

			template<class E>
			friend void tag_invoke(cmoon::execution::set_error_t, dummy_r&&, E&&) noexcept
			{
			}

			friend void tag_invoke(cmoon::execution::set_done_t, dummy_r&&) noexcept
			{
			}

			std::atomic<int>* v;
		};

		public:
			split_cancel_test()
				: cmoon::test::test_case{"split_cancel_test"} {}

			void operator()() override
			{
				std::atomic<int> value {0};
				dummy_r r{&value};
				auto s = cmoon::execution::split(
							cmoon::execution::just(1)
						);

				{
					auto op = cmoon::execution::connect(s, std::move(r));
				}

				cmoon::test::assert_equal(value, 0);
			}
	};

	export
	class sync_wait_value_test : public cmoon::test::test_case
	{
		public:
			sync_wait_value_test()
				: cmoon::test::test_case{"sync_wait_value_test"} {}

			void operator()() override
			{
				constexpr int expected {5};
				const auto [value] = cmoon::execution::sync_wait(cmoon::execution::just(expected)).value();
				cmoon::test::assert_equal(value, expected);
			}
	};

	export
	class sync_wait_error_test : public cmoon::test::test_case
	{
		struct sender_s
		{
			template<template<class...> class Tuple, template<class...> class Variant>
			using value_types = Variant<Tuple<int>>;

			template<template<class...> class Variant>
			using error_types = Variant<std::exception_ptr, std::exception>;

			static constexpr bool sends_done {false};

			template<class R>
			struct op
			{
				R r_;
				friend void tag_invoke(cmoon::execution::start_t, op& o) noexcept
				{
					cmoon::execution::set_error(std::move(o.r_), std::exception{});
				}
			};

			template<class R>
			friend auto tag_invoke(cmoon::execution::connect_t, sync_wait_error_test::sender_s&& s, R&& r)
			{
				return op<R>{std::forward<R>(r)};
			}
		};

		public:
			sync_wait_error_test()
				: cmoon::test::test_case{"sync_wait_error_test"} {}

			void operator()() override
			{
				auto fn = [] {
					cmoon::execution::sync_wait(sender_s{});
				};

				cmoon::test::assert_throws(std::exception{}, fn);
			}
	};

	export
	class sync_wait_done_test : public cmoon::test::test_case
	{
		struct sender_s
		{
			template<template<class...> class Tuple, template<class...> class Variant>
			using value_types = Variant<Tuple<int>>;

			template<template<class...> class Variant>
			using error_types = Variant<std::exception_ptr>;

			static constexpr bool sends_done {false};

			template<class R>
			struct op
			{
				R r_;
				friend void tag_invoke(cmoon::execution::start_t, op& o) noexcept
				{
					cmoon::execution::set_done(std::move(o.r_));
				}
			};

			template<class R>
			friend auto tag_invoke(cmoon::execution::connect_t, sync_wait_done_test::sender_s&& s, R&& r)
			{
				return op<R>{std::forward<R>(r)};
			}
		};

		public:
			sync_wait_done_test()
				: cmoon::test::test_case{"sync_wait_done_test"} {}

			void operator()() override
			{
				auto value = cmoon::execution::sync_wait(sender_s{});

				cmoon::test::assert_false(value.has_value());
			}
	};

	export
	class execute_test : public cmoon::test::test_case
	{
		public:
			struct dummy_sender : public cmoon::execution::sender_base
			{
				template<class R>
				struct op
				{
					R r_;

					friend void tag_invoke(cmoon::execution::start_t, op& o) noexcept
					{
						cmoon::execution::set_value(std::move(o.r_));
					}
				};

				template<class R>
				friend auto tag_invoke(cmoon::execution::connect_t, dummy_sender&&, R&& r)
				{
					return op<R>{std::forward<R>(r)};
				}
			};

			struct executor_s
			{
				friend dummy_sender tag_invoke(cmoon::execution::schedule_t, executor_s&)
				{
					return dummy_sender{};
				}

				[[nodiscard]] constexpr friend bool operator==(const executor_s&, const executor_s&) noexcept = default;
			};

			static_assert(cmoon::execution::scheduler<executor_s>);

			execute_test()
				: cmoon::test::test_case{"execute_test"} {}

			void operator()() override
			{
				constexpr int expected {5};
				std::atomic<int> value {0};

				executor_s sch;

				cmoon::execution::execute(sch, [&value, expected] {
					value = expected;
					value.notify_one();
				});

				value.wait(0);
				cmoon::test::assert_equal(value, expected);
			}
	};

	export
	class async_test : public cmoon::test::test_case
	{
		public:
			struct dummy_sender : public cmoon::execution::sender_base
			{
				template<class R>
				struct op
				{
					R r_;

					friend void tag_invoke(cmoon::execution::start_t, op& o) noexcept
					{
						cmoon::execution::set_value(std::move(o.r_));
					}
				};

				template<class R>
				friend auto tag_invoke(cmoon::execution::connect_t, dummy_sender&&, R&& r)
				{
					return op<R>{std::forward<R>(r)};
				}
			};

			struct executor_s
			{
				friend dummy_sender tag_invoke(cmoon::execution::schedule_t, executor_s&)
				{
					return dummy_sender{};
				}

				[[nodiscard]] constexpr friend bool operator==(const executor_s&, const executor_s&) noexcept = default;
			};

			async_test()
				: cmoon::test::test_case{"async_test"} {}

			void operator()() override
			{
				constexpr int expected {8};

				auto r = cmoon::execution::async(executor_s{}, std::plus<>{}, 3, 5);
				cmoon::test::assert_equal(r.get(), expected);
			}
	};
}

namespace cmoon::tests
{
	export
	template<>
	cmoon::test::test_suite get_test_suite<library::execution>()
	{
		cmoon::test::test_suite suite{"execution library tests"};
		suite.add_test_case<execution::set_value_test>();
		suite.add_test_case<execution::set_done_test>();
		suite.add_test_case<execution::set_error_test>();
		suite.add_test_case<execution::start_test>();
		suite.add_test_case<execution::connect_test>();
		suite.add_test_case<execution::schedule_test>();
		suite.add_test_case<execution::just_set_value_test>();
		suite.add_test_case<execution::just_set_error_test>();
		suite.add_test_case<execution::just_error_test>();
		suite.add_test_case<execution::just_done_test>();
		suite.add_test_case<execution::then_test>();
		suite.add_test_case<execution::upon_error_test>();
		suite.add_test_case<execution::let_value_test>();
		suite.add_test_case<execution::bulk_test>();
		suite.add_test_case<execution::split_single_test>();
		suite.add_test_case<execution::split_multi_test>();
		suite.add_test_case<execution::split_cancel_test>();
		suite.add_test_case<execution::when_all_single_value_test>();
		suite.add_test_case<execution::when_all_single_error_test>();
		suite.add_test_case<execution::when_all_single_done_test>();
		suite.add_test_case<execution::when_all_value_test>();
		suite.add_test_case<execution::when_all_error_test>();
		suite.add_test_case<execution::when_all_done_test>();
		suite.add_test_case<execution::when_all_error_and_done_test>();
		suite.add_test_case<execution::done_as_optional_test>();
		suite.add_test_case<execution::ensure_started_value_test>();
		suite.add_test_case<execution::sync_wait_value_test>();
		suite.add_test_case<execution::sync_wait_error_test>();
		suite.add_test_case<execution::sync_wait_done_test>();
		suite.add_test_case<execution::execute_test>();
		suite.add_test_case<execution::async_test>();

		return std::move(suite);
	}
}
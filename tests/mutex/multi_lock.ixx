export module cmoon.tests.mutex.multi_lock;

import <mutex>;

import cmoon.test;
import cmoon.mutex;

namespace cmoon::tests::mutex
{
	export
	class multi_lock_default_constructor : public cmoon::test::test_case
	{
		public:
			multi_lock_default_constructor()
				: cmoon::test::test_case{"multi_lock_default_constructor"} {}

			void operator()() override
			{
				cmoon::multi_lock<std::mutex> l;
			}
	};

	export
	class multi_lock_regular_constructor : public cmoon::test::test_case
	{
		public:
			multi_lock_regular_constructor()
				: cmoon::test::test_case{"multi_lock_regular_constructor"} {}

			void operator()() override
			{
				std::mutex m1, m2, m3, m4, m5, m6, m7;

				cmoon::multi_lock l {m1};
				cmoon::test::assert_true(l.owns_lock());
			
				cmoon::multi_lock l2 {m2, m3};
				cmoon::test::assert_true(l2.owns_lock());

				cmoon::multi_lock l3 {m4, m5, m6, m7};
				cmoon::test::assert_true(l3.owns_lock());

				l.unlock();
				cmoon::test::assert_false(l.owns_lock());

				l2.unlock();
				cmoon::test::assert_false(l2.owns_lock());

				l3.unlock();
				cmoon::test::assert_false(l3.owns_lock());

				l.lock();
				cmoon::test::assert_true(l.owns_lock());

				l2.lock();
				cmoon::test::assert_true(l2.owns_lock());

				l3.lock();
				cmoon::test::assert_true(l3.owns_lock());
			}
	};
}
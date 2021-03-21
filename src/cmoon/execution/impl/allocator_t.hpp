#pragma once

#include <type_traits>

#include "cmoon/execution/impl/executor.hpp"

namespace cmoon
{
	namespace execution
	{
		template<typename ProtoAllocator>
		struct allocator_t
		{
			public:
				template<class T>
				static constexpr bool is_applicable_property_v = executor<T>;

				static constexpr bool is_requirable = true;
				static constexpr bool is_preferable = true;

				template<class Executor>
				static constexpr auto static_query_v = Executor::query(allocator_t{});

				constexpr ProtoAllocator value() const
				{
					return a_;
				}
			private:
				ProtoAllocator a_;
		};

		template<>
		struct allocator_t<void>
		{
			template<class T>
			static constexpr bool is_applicable_property_v = executor<T>;

			static constexpr bool is_requirable = true;
			static constexpr bool is_preferable = true;

			template<class Executor>
			static constexpr auto static_query_v = Executor::query(allocator_t{});

			template<typename OtherProtoAllocator>
			allocator_t<OtherProtoAllocator> operator()(const OtherProtoAllocator& a) const
			{
				return {a};
			}
		};

		constexpr allocator_t<void> allocator;
	}
}
export module cmoon.execution.allocator_t;

import <memory>;
import <type_traits>;

import cmoon.execution.executor;

namespace cmoon::execution
{
	export
	template<typename ProtoAllocator>
	struct allocator_t
	{
		public:
			template<class T>
			static constexpr bool is_applicable_property_v {executor<T>};

			static constexpr bool is_requirable {true};
			static constexpr bool is_preferable {true};

			template<class Executor>
			static constexpr auto static_query_v {Executor::query(allocator_t)};

			constexpr ProtoAllocator value() const
			{
				return a_;
			}
		private:
			constexpr allocator_t(const ProtoAllocator& a)
				: a_{a} {}

			ProtoAllocator a_;

			template<class OtherProtoAllocator>
			friend struct allocator_t;
	};

	export
	template<>
	struct allocator_t<void>
	{
		public:
			template<class T>
			static constexpr bool is_applicable_property_v {executor<T>};

			static constexpr bool is_requirable {true};
			static constexpr bool is_preferable {true};

			template<class Executor>
			static constexpr auto static_query_v {Executor::query(allocator_t)};

			template<typename OtherProtoAllocator>
				requires(!std::is_void_v<OtherProtoAllocator>)
			allocator_t<OtherProtoAllocator> operator()(const OtherProtoAllocator& a) const
			{
				return {a};
			}
	};

	export
	inline constexpr allocator_t<void> allocator;
}
export module cmoon.executors.executor_properties;

import <memory>;

import cmoon.property;
import cmoon.execution;

namespace cmoon::executors
{
	export
	template<template<class, class, class, class> class Base,
			 class Blocking,
			 class Relationship,
			 class Outstanding,
			 class AllocatorType>
	class executor_properties
	{
		public:
			Base<cmoon::execution::blocking_t::never_t,
				 Relationship,
				 Outstanding,
				 AllocatorType>
			require(const cmoon::execution::blocking_t::never_t) const
			{
				return {*this};
			}

			Base<cmoon::execution::blocking_t::possibly_t,
				 Relationship,
				 Outstanding,
				 AllocatorType>
			require(const cmoon::execution::blocking_t::possibly_t) const
			{
				return {*this};
			}

			Base<cmoon::execution::blocking_t::always_t,
				 Relationship,
				 Outstanding,
				 AllocatorType>
			require(const cmoon::execution::blocking_t::always_t) const
			{
				return {*this};
			}

			Base<Blocking,
				 cmoon::execution::relationship_t::continuation_t,
				 Outstanding,
				 AllocatorType>
			require(const cmoon::execution::relationship_t::continuation_t) const
			{
				return {*this};
			}

			Base<Blocking,
				 cmoon::execution::relationship_t::fork_t,
				 Outstanding,
				 AllocatorType>
			require(const cmoon::execution::relationship_t::fork_t) const
			{
				return {*this};
			}

			Base<Blocking,
				 Relationship,
				 cmoon::execution::outstanding_work_t::tracked_t,
				 AllocatorType>
			require(const cmoon::execution::outstanding_work_t::tracked_t) const
			{
				return {*this};
			}

			Base<Blocking,
				 Relationship,
				 cmoon::execution::outstanding_work_t::untracked_t,
				 AllocatorType>
			require(const cmoon::execution::outstanding_work_t::untracked_t) const
			{
				return {*this};
			}

			template<class ProtoAllocator>
			Base<Blocking,
				 Relationship,
				 Outstanding,
				 ProtoAllocator>
			require(const cmoon::execution::allocator_t<ProtoAllocator>& other) const
			{
				return {*this, other.value()};
			}

			auto require(const cmoon::execution::allocator_t<void>&) const
			{
				return require(cmoon::execution::allocator(std::allocator<void>{}));
			}

			[[nodiscard]] static constexpr cmoon::execution::bulk_guarentee_t query(const cmoon::execution::bulk_guarentee_t) noexcept
			{
				return cmoon::execution::bulk_guarentee.parallel;
			}

			[[nodiscard]] static constexpr cmoon::execution::mapping_t query(const cmoon::execution::mapping_t) noexcept
			{
				return cmoon::execution::mapping.thread;
			}

			[[nodiscard]] static constexpr cmoon::execution::blocking_t query(const cmoon::execution::blocking_t) noexcept
			{
				return Blocking{};
			}

			[[nodiscard]] static constexpr cmoon::execution::relationship_t query(const cmoon::execution::relationship_t) noexcept
			{
				return Relationship{};
			}

			[[nodiscard]] static constexpr cmoon::execution::outstanding_work_t query(const cmoon::execution::outstanding_work_t) noexcept
			{
				return Outstanding{};
			}

			auto query(const cmoon::execution::allocator_t<void>&) const noexcept
			{
				return std::allocator<void>{};
			}

			template<class ProtoAllocator>
			auto query(const cmoon::execution::allocator_t<ProtoAllocator>&) const noexcept
			{
				return alloc;
			}
		protected:
			explicit executor_properties_base(const AllocatorType& a = AllocatorType{})
				: alloc{a} {}

			mutable AllocatorType alloc;
	};
}
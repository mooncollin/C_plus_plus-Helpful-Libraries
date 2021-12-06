export module cmoon.intrusive_queue;

import <utility>;
import <memory>;

namespace cmoon
{
	export
	template<auto Next>
	class intrusive_queue;

	export
	template<class Item, Item* Item::*Next>
	class intrusive_queue<Next>
	{
		public:
			constexpr intrusive_queue() noexcept = default;

			constexpr intrusive_queue(intrusive_queue&& other) noexcept
				: head_{std::exchange(other.head_, nullptr)},
				  tail_{std::exchange(other.tail_, nullptr)} {}

			constexpr intrusive_queue& operator=(intrusive_queue&& other) noexcept
			{
				if (this != std::addressof(other))
				{
					head_ = std::exchange(other.head_, nullptr);
					tail_ = std::exchange(other.tail_, nullptr);
				}

				return *this;
			}

			[[nodiscard]] constexpr bool empty() const noexcept
			{
				return head_ == nullptr;
			}

			[[nodiscard]] constexpr Item* pop_front() noexcept
			{
				auto item {std::exchange(head_, head_->*Next)};
				if (head_ == nullptr)
				{
					tail_ = nullptr;
				}
				return item;
			}

			constexpr void push_front(Item* item) noexcept
			{
				item->*Next = head_;
				head_ = item;
				if (tail_ == nullptr)
				{
					tail_ = item;
				}
			}

			constexpr void push_back(Item* item) noexcept
			{
				item->*Next = nullptr;
				if (tail_ == nullptr)
				{
					head_ = item;
				}
				else
				{
					tail_->*Next = item;
				}

				tail_ = item;
			}

			constexpr void append(intrusive_queue&& other) noexcept
			{
				if (other.empty())
				{
					return;
				}

				auto other_head {std::exchange(other.head_, nullptr)};
				if (empty())
				{
					head_ = other_head;
				}
				else
				{
					tail_->*Next = other_head;
				}
				tail_ = std::exchange(other.tail_, nullptr);
			}

			constexpr void prepend(intrusive_queue&& other) noexcept
			{
				if (other.empty())
				{
					return;
				}

				other.tail_->*Next = head_;
				head_ = other.head_;
				if (tail_ == nullptr)
				{
					tail_ = other.tail_;
				}

				other.tail_ = nullptr;
				other.head_ = nullptr;
			}
		private:
			Item* head_ {nullptr};
			Item* tail_ {nullptr};
	};
}
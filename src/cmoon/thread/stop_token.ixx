export module cmoon.thread.stop_token;

import <concepts>;
import <type_traits>;
import <thread>;

import cmoon.concepts;

namespace cmoon
{
	template<template<class> class T>
	struct check_type_alias_exists {};

	export
	template<typename T>
	concept stoppable_token = std::copy_constructible<T> &&
							  std::move_constructible<T> &&
							  std::is_nothrow_copy_constructible_v<T> &&
							  std::is_nothrow_move_constructible_v<T> &&
							  std::equality_comparable<T> &&
		requires(const T& token)
	{
		{ token.stop_requested() } noexcept -> std::convertible_to<bool>;
		{ token.stop_possible() } noexcept -> std::convertible_to<bool>;
		typename check_type_alias_exists<T::template callback_type>;
	};

	export
	template<typename T, typename CB, typename Initializer = CB>
	concept stoppable_token_for = stoppable_token<T> &&
								  std::invocable<CB> &&
		requires
	{
		typename T::template callback_type<CB>;
	} &&
		std::constructible_from<CB, Initializer> &&
		std::constructible_from<typename T::template callback_type<CB>, T, Initializer> &&
		std::constructible_from<typename T::template callback_type<CB>, T&, Initializer> &&
		std::constructible_from<typename T::template callback_type<CB>, const T, Initializer> &&
		std::constructible_from<typename T::template callback_type<CB>, const T&, Initializer>;

	export
	template<typename T>
	concept unstoppable_token = stoppable_token<T> &&
		requires
	{
		{ T::stop_possible() } -> std::convertible_to<bool>;
	} && (!T::stop_possible());

	export
	struct never_stop_token
	{
		template<class F>
		struct callback_type
		{
			explicit callback_type(never_stop_token, F&&) noexcept {}
		};

		[[nodiscard]] static constexpr bool stop_possible() noexcept
		{
			return false;
		}

		static constexpr bool stop_requested() noexcept
		{
			return false;
		}
	};

	class in_place_stop_source;

	struct in_place_stop_callback_base
	{
		public:
			virtual void execute() noexcept = 0;
		protected:
			explicit in_place_stop_callback_base(in_place_stop_source* source) noexcept
				: source_{source} {}

			void register_callback() noexcept;

			in_place_stop_source* source_;
			in_place_stop_callback_base* next_ {nullptr};
			in_place_stop_callback_base** prev_ptr_ {nullptr};
			bool* removed_during_callback_ {nullptr};
			std::atomic<bool> callback_completed_ {false};

			friend in_place_stop_source;
	};

	class in_place_stop_token;

	template <class Callback>
	class in_place_stop_callback;

	export
	class in_place_stop_source
	{
		public:
			in_place_stop_source() noexcept = default;
			~in_place_stop_source() noexcept = default;
			in_place_stop_source(in_place_stop_source&&) = delete;

			in_place_stop_token get_token() noexcept;

			bool request_stop() noexcept
			{
				if (!try_lock_unless_stop_requested(true))
				{
					return true;
				}

				notifying_thread_ = std::this_thread::get_id();

				while (callbacks_ != nullptr)
				{
					auto* callback = callbacks_;
					callback->prev_ptr_ = nullptr;
					callbacks_ = callback->next_;
					if (callbacks_ != nullptr)
					{
						callbacks_->prev_ptr_ = std::addressof(callbacks_);
					}

					state_.store(stop_requested_flag, std::memory_order_release);

					bool removed_during_callback {false};
					callback->removed_during_callback_ = std::addressof(removed_during_callback);

					callback->execute();

					if (!removed_during_callback)
					{
						callback->removed_during_callback_ = nullptr;
						callback->callback_completed_.store(true, std::memory_order_release);
						callback->callback_completed_.notify_all();
					}

					lock();
				}

				state_.store(stop_requested_flag, std::memory_order_release);
				return false;
			}

			bool stop_requested() const noexcept
			{
				return (state_.load(std::memory_order_acquire) & stop_requested_flag) != 0;
			}
		private:
			friend class in_place_stop_token;
			friend struct in_place_stop_callback_base;

			template <class Callback>
			friend class in_place_stop_callback;

			void remove_callback(in_place_stop_callback_base* callback) noexcept
			{
				auto old_state = lock();

				if (callback->prev_ptr_ != nullptr)
				{
					*callback->prev_ptr_ = callback->next_;
					if (callback->next_ != nullptr)
					{
						callback->next_->prev_ptr_ = callback->prev_ptr_;
					}

					unlock(old_state);
				}
				else
				{
					auto notifying_thread = notifying_thread_;
					unlock(old_state);

					if (std::this_thread::get_id() == notifying_thread)
					{
						if (callback->removed_during_callback_ != nullptr)
						{
							*callback->removed_during_callback_ = true;
						}
					}
					else
					{
						callback->callback_completed_.wait(false);
					}
				}
			}

			std::uint8_t lock() noexcept
			{
				auto old_state = state_.load(std::memory_order_relaxed);
				do
				{
					while ((old_state & locked_flag) != 0)
					{
						state_.wait(old_state);
						old_state = state_.load(std::memory_order_relaxed);
					}
				}
				while (!state_.compare_exchange_weak(
					old_state,
					old_state | locked_flag,
					std::memory_order_acquire,
					std::memory_order_relaxed
				));

				return old_state;
			}

			void unlock(std::uint8_t old_state) noexcept
			{
				(void) state_.store(old_state, std::memory_order_release);
				state_.notify_all();
			}

			bool try_lock_unless_stop_requested(bool set_stop_requested) noexcept
			{
				auto old_state = state_.load(std::memory_order_relaxed);
				do
				{
					while (true)
					{
						if ((old_state & stop_requested_flag) != 0)
						{
							return false;
						}
						else if (old_state == no_flag)
						{
							break;
						}
						else
						{
							state_.wait(old_state);
							old_state = state_.load(std::memory_order_relaxed);
						}
					}
				}
				while (!state_.compare_exchange_weak(
					old_state,
					set_stop_requested ? (locked_flag | stop_requested_flag) : locked_flag,
					std::memory_order_acq_rel,
					std::memory_order_relaxed
				));

				return true;
			}
			bool try_add_callback(in_place_stop_callback_base* callback) noexcept
			{
				if (!try_lock_unless_stop_requested(false))
				{
					return false;
				}

				callback->next_ = callbacks_;
				callback->prev_ptr_ = std::addressof(callbacks_);
				if (callbacks_ != nullptr)
				{
					callbacks_->prev_ptr_ = std::addressof(callback->next_);
				}
				callbacks_ = callback;

				unlock(0);
				
				return true;
			}

			static constexpr std::uint8_t no_flag {0};
			static constexpr std::uint8_t stop_requested_flag {1};
			static constexpr std::uint8_t locked_flag {2};

			std::atomic<std::uint8_t> state_{no_flag};
			in_place_stop_callback_base* callbacks_ {nullptr};
			std::thread::id notifying_thread_;
	};

	export
	class in_place_stop_token
	{
		public:
			template<class F>
			using callback_type = in_place_stop_callback<F>;

			in_place_stop_token() noexcept = default;
			in_place_stop_token(const in_place_stop_token&) noexcept = default;

			in_place_stop_token(in_place_stop_token&& other) noexcept
				: source_{std::exchange(other.source_, nullptr)} {}

			in_place_stop_token& operator=(const in_place_stop_token&) noexcept = default;

			in_place_stop_token& operator=(in_place_stop_token&& other) noexcept
			{
				source_ = std::exchange(other.source_, nullptr);
				return *this;
			}

			bool stop_requested() const noexcept
			{
				return source_ != nullptr && source_->stop_requested();
			}

			bool stop_possible() const noexcept
			{
				return source_ != nullptr;
			}

			void swap(in_place_stop_token& other) noexcept
			{
				std::swap(source_, other.source_);
			}

			bool operator==(const in_place_stop_token&) const noexcept = default;
		private:
			friend in_place_stop_source;

			template<class F>
			friend class in_place_stop_callback;

			explicit in_place_stop_token(in_place_stop_source* source) noexcept
				: source_{source} {}

			in_place_stop_source* source_ {nullptr};
	};

	inline in_place_stop_token in_place_stop_source::get_token() noexcept
	{
		return in_place_stop_token{this};
	}

	export
	template<class F>
	class in_place_stop_callback : in_place_stop_callback_base
	{
		public:
			template<class T>
				requires(std::constructible_from<F, T>)
			explicit in_place_stop_callback(in_place_stop_token token, T&& func) noexcept(std::is_nothrow_constructible_v<F, T>)
				: in_place_stop_callback_base(token.source_, &in_place_stop_callback::execute_impl),
				  func_{std::forward<T>(func)}
				{
					register_callback();
				}

			~in_place_stop_callback() noexcept
			{
				if (source_ != nullptr)
				{
					source_->remove_callback(this);
				}
			}

			void execute() noexcept final
			{
				std::invoke(std::move(func_));
			}
		private:
			[[no_unique_address]] F func_;
	};

	inline void in_place_stop_callback_base::register_callback() noexcept
	{
		if (source_ != nullptr)
		{
			if (!source_->try_add_callback(this))
			{
				source_ = nullptr;
				execute();
			}
		}
	}
}
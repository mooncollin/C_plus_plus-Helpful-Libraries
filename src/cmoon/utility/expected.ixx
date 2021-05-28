export module cmoon.utility.expected;

import <type_traits>;
import <concepts>;
import <utility>;
import <initializer_list>;
import <exception>;

namespace cmoon
{
	export
	struct unexpect_t
	{
		explicit unexpect_t() = default;
	};

	export
	inline constexpr unexpect_t unexpect{};

	export
	template<class E>
	class unexpected;

	export
	template<class E>
	unexpected(E) -> unexpected<E>;

	export
	template<class E>
	class bad_expected_access;

	export
	template<>
	class bad_expected_access<void>;

	export
	template<class T, class E>
		requires (std::is_nothrow_copy_constructible<E> &&
				  std::is_nothrow_move_constructible<E> &&
				  std::is_nothrow_is_copy_assignable_v<E> &&
				  std::is_nothrow_move_assignable<E>)
	class expected
	{
		public:
			using value_type = T;
			using error_type = E;
			using unexpected_type = unexpected<E>;
		private:
			struct expected_type_c_t {};
			static constexpr expected_type_c_t expected_type_c {};

			struct unexpected_type_c_t {};
			static constexpr unexpected_type_c_t unexpected_type_c {};

			template<class T2>
			struct union_type
			{
				union u
				{
					constexpr u() = default;

					template<class... Args>
					constexpr u(const expected_type_c_t, Args&&... args)
						: val{std::forward<Args>(args)...} {}

					template<class... Args>
					constexpr u(const unexpected_type_c_t, Args&&... args)
						: unex{std::forward<Args>(args)...} {}

					T2 val{};
					unexpected_type unex;
				};

				using type = u;
			};

			template<>
			struct union_type<void>
			{
				union u
				{
					template<class... Args>
					constexpr u(const unexpected_type_c_t, Args&&... args)
						: unex{std::forward<Args>(args)...} {}

					unexpected_type unex{};
				};

				using type = u;
			};

			using union_type_t = typename union_type<value_type>::type;
		public:

			template<class U>
			using rebind = expected<U, error_type>;

			constexpr expected() requires (std::default_initializable<T> || std::is_void_v<T>)
				: has_val{true} {}

			constexpr expected(const expected& other) requires (std::copy_constructible<T> || std::is_void_v<T>) && std::copy_constructible<E>
				: has_val{other.has_val}
			{
				if (!std::is_void_v<T> && has_val)
				{
					storage.val = *other;
				}
				else
				{
					storage.unex = unexpected(other.error());
				}
			}

			constexpr expected(expected&& other) noexcept((std::is_nothrow_move_constructible_v<T> || std::is_void_v<T>) &&
														  std::is_nothrow_move_constructible_v<E>)
				requires(std::move_constructible<T> && std::move_constructible<E>)
				: has_val{other.has_val}
			{
				if (!std::is_void_v<T> && has_val)
				{
					storage.val = std::move(*other);
				}
				else
				{
					storage.unex = unexpected(std::move(other.error()));
				}
			}

			template<class U, class G>
			requires ((std::is_void_v<T> && std::is_void_v<E>) ||
					  (
						std::constructible_from<T, const U&> &&
						!std::constructible_from<T, expected<U, G>&> &&
						!std::constructible_from<T, expected<U, G>&&> &&
						!std::constructible_from<T, const expected<U, G>&> &&
						!std::constructible_from<T, const expected<U, G>&&> &&
						!std::convertible_to<expected<U, G>&, T> &&
						!std::convertible_to<expected<U, G>&&, T> &&
						!std::convertible_to<const expected<U, G>&, T> &&
						!std::convertible_to<const expected<U, G>&&, T> &&
						std::constructible_from<E, const G&> &&
						!std::constructible_from<unexpected<E>, expected<U, G>&> &&
						!std::constructible_from<unexpected<E>, expected<U, G>&&> &&
						!std::constructible_from<unexpected<E>, const expected<U, G>&> &&
						!std::constructible_from<unexpected<E>, const expected<U, G>&&> &&
						!std::convertible_to<expected<U, G>&, unexpected<E>> &&
						!std::convertible_to<expected<U, G>&&, unexpected<E>> &&
						!std::convertible_to<const expected<U, G>&, unexpected<E>> &&
						!std::convertible_to<const expected<U, G>&&, unexpected<E>>
					  ))
			explicit((!std::is_void_v<T> && !std::is_void_v<U> && !std::convertible_to<const U&, T>) ||
					  !std::convertible_to<const G&, E>)
			constexpr expected(const expected<U, G>& other)
				: has_val{other.has_val}
			{
				if (!std::is_void_v<T> && has_val)
				{
					storage.val = *other;
				}
				else
				{
					storage.unex = unexpected(other.error());
				}
			}

			template<class U, class G>
			requires ((std::is_void_v<T> && std::is_void_v<E>) ||
					  (
						std::constructible_from<T, U&&> &&
						!std::constructible_from<T, expected<U, G>&> &&
						!std::constructible_from<T, expected<U, G>&&> &&
						!std::constructible_from<T, const expected<U, G>&> &&
						!std::constructible_from<T, const expected<U, G>&&> &&
						!std::convertible_to<expected<U, G>&, T> &&
						!std::convertible_to<expected<U, G>&&, T> &&
						!std::convertible_to<const expected<U, G>&, T> &&
						!std::convertible_to<const expected<U, G>&&, T> &&
						std::constructible_from<E, G&&> &&
						!std::constructible_from<unexpected<E>, expected<U, G>&> &&
						!std::constructible_from<unexpected<E>, expected<U, G>&&> &&
						!std::constructible_from<unexpected<E>, const expected<U, G>&> &&
						!std::constructible_from<unexpected<E>, const expected<U, G>&&> &&
						!std::convertible_to<expected<U, G>&, unexpected<E>> &&
						!std::convertible_to<expected<U, G>&&, unexpected<E>> &&
						!std::convertible_to<const expected<U, G>&, unexpected<E>> &&
						!std::convertible_to<const expected<U, G>&&, unexpected<E>>
					  ))
			explicit((!std::is_void_v<T> && !std::is_void_v<U> && !std::convertible_to<U&&, T>) ||
					  !std::convertible_to<G&&, E>)
			constexpr expected(expected<U, G>&& other)
				: has_val{other.has_val}
			{
				if constexpr (!std::is_void_v<T>)
				{
					if (has_val)
					{
						storage.val = std::move(*other);
					}
					else
					{
						storage.unex = unexpected(std::move(other.error()));
					}
				}
			}

			template<class U = T>
			requires(!std::is_void_v<T> &&
					 std::constructible_from<T, U&&> &&
					 !std::same_as<std::remove_cvref_t<U>, std::in_place_t> &&
					 !std::same_as<expected<T, E>, std::remove_cvref_t<U>> &&
					 !std::same_as<unexpected<E>, std::remove_cvref_t<U>>)
			explicit(!std::convertible_to<U&&, T>)
			constexpr expected(U&& v)
				: has_val{true}, storage{expected_type_c, std::forward<U>(v)} {}

			template<class G = E>
				requires (std::constructible_from<E, const G&>)
			explicit(!std::convertible_to<const G&, E>)
			constexpr expected(const unexpected<G>& e)
				: has_val{false}, storage{unexpected_type_c, unexpected<E>(e)} {}

			template<class G = E>
			requires(std::constructible_from<E, G&&>)
			explicit(!std::convertible_to<G&&, E>)
			constexpr expected(unexpected<G>&& e)
				: has_val{false}, storage{unexpected_type_c, std::move(e)} {}

			template<class... Args>
			requires(std::is_void_v<T> && sizeof...(Args) == 0)
			explicit constexpr expected(std::in_place_t, Args&&...)
				: has_val{true} {}

			template<class... Args>
			requires(!std::is_void_v<T>&& std::constructible_from<T, Args...>)
			explicit constexpr expected(std::in_place_t, Args&&... args)
				: has_val{true}, storage{expected_type_c, std::forward<Args>(args)...} {}

			template<class U, class... Args>
				requires(!std::is_void_v<T> && std::constructible_from<T, std::initializer_list<U>, Args...>)
			explicit constexpr expected(std::in_place_t, std::initializer_list<U> il, Args&&... args)
				: has_val{true}, storage{expected_type_c, il, std::forward<Args>(args)...} {}

			template<class... Args>
				requires(std::constructible_from<E, Args...>)
			explicit constexpr expected(unexpect_t, Args&&... args)
				: has_val{false}, storage{unexpected_type_c, unexpected<E>{std::forward<Args>(args)...}} {}

			template<class U, class... Args>
			requires(std::constructible_from<E, std::initializer_list<U>, Args...>)
			explicit constexpr expected(unexpect_t, std::initializer_list<U> il, Args&&... args)
				: has_val{false}, storage{unexpected_type_c, unexpected<E>{il, std::forward<Args>(args)...}} {}

			// Both T and E are trivially destructible
			~expected() requires((std::is_void_v<T> || std::is_trivially_destructible_v<T>) && std::is_trivially_destructible_v<E>) = default;

			// Either T or E are trivially destructible
			~expected()
			{
				if (*this)
				{
					if constexpr (!std::is_void_v<T> && !std::is_trivially_destructible_v<T>)
					{
						storage.val.~T();
					}
				}
				else
				{
					if constexpr (!std::is_trivially_destructible<E>)
					{
						storage.unex.~unexpected<E>();
					}
				}
			}

			expected& operator=(const expected& other) requires (std::is_void_v<T> && std::is_copy_assignable_v<E> && std::is_copy_constructible_v<E>) ||
																(!std::is_void_v<T> && std::is_copy_assignable_v<T> && std::is_copy_constructible_v<T> &&
																 std::is_copy_assignable_v<E> && std::is_copy_constructible_v<E> &&
																 (std::is_nothrow_move_constructible_v<E> || std::is_nothrow_move_constructible_v<T>))
			{
				if (other)
				{
					if (*this)
					{
						if constexpr (!std::is_void_v<T>)
						{
							storage.val = *other;
						}
					}
					else
					{
						if constexpr (std::is_void_v<T>)
						{
							storage.unex.~unexpected<E>();
							has_val = true;
						}
						else if constexpr (std::is_nothrow_copy_constructible_v<T>)
						{
							storage.unex.~unexpected<E>();
							storage.val = *other;
						}
						else if constexpr (std::is_nothrow_move_constructible_v<T>)
						{
							auto tmp = *other;
							storage.unex.~unexpected<E>();
							storage.val = std::move(tmp);
						}
						else
						{
							unexpected_type tmp = unexpected{error()}; // Can throw
							storage.unex.~unexpected<E>();
							try
							{
								storage.val = *other;
								has_val = true;
							}
							catch (...)
							{
								storage.unex = std::move(tmp);
								throw std::current_exception();
							}
						}
					}
				}
				else
				{
					if (*this)
					{
						if constexpr (std::is_void_v<T>)
						{
							try
							{
								storage.unex = unexpected_type{other.error()};
								has_val = false;
							}
							catch (...)
							{
								// Do nothing
							}
						}
						else if constexpr (std::is_nothrow_copy_constructible_v<E>)
						{
							storage.val.~T();
							storage.unex = unexpected_type{other.error()};
							has_val = false;
						}
						else if constexpr (std::is_nothrow_move_constructible_v<E>)
						{
							auto tmp = unexpected_type{other.error()}; // Can throw
							storage.val.~T();
							storage.unex = std::move(tmp);
							has_val = false;
						}
						else
						{
							T tmp = *(*this);
							storage.val.~T();
							try
							{
								storage.unex = unexpected_type{other.error()};
								has_val = false;
							}
							catch (...)
							{
								storage.val = std::move(tmp);
								throw std::current_exception();
							}
						}
					}
				}

				return *this;
			}

			expected& operator=(expected&& other) noexcept(std::is_nothrow_move_assignable_v<T> && std::is_nothrow_move_constructible_v<T>)
			requires((std::is_void_v<T>&& std::is_nothrow_move_constructible_v<E>&& std::is_nothrow_move_assignable_v<E>) ||
				(!std::is_void_v<T> && std::move_constructible<T> and std::is_move_assignable_v<T> && std::is_nothrow_move_constructible_v<E> &&
				 std::is_nothrow_move_assignable_v<E>))
			{
				if (other)
				{
					if (*this)
					{
						if constexpr (!std::is_void_v<T>)
						{
							storage.val = std::move(*other);
						}
					}
					else
					{
						if constexpr(std::is_void_v<T>)
						{
							storage.unex.~unexpected_type();
						}
						else if constexpr (std::is_nothrow_move_constructible_v<T>)
						{
							storage.unex.~unexpected_type();
							storage.val = *std::move(other);
						}
						else
						{
							unexpected_type tmp = unexpected{error()};
							storage.unex.~unexpected_type();
							try
							{
								storage.val = *std::move(other);
								has_val = true;
							}
							catch (...)
							{
								storage.unex = std::move(tmp);
								throw std::current_exception();
							}
						}
					}
				}
				else
				{
					if constexpr (std::is_void_v<T>)
					{
						try
						{
							storage.unex = unexpected{std::move(other.error())};
							has_val = false;
						}
						catch (...)
						{
							// Do nothing
						}
					}
					else if constexpr (std::is_nothrow_move_constructible_v<E>)
					{
						storage.val.~T();
						storage.unex = unexpected{std::move(other.error())};
					}
					else
					{
						T tmp = std::move(*(*this));
						storage.val.~T();
						try
						{
							storage.unex = unexpected{std::move(other.error())};
							has_val = false;
						}
						catch (...)
						{
							storage.val = std::move(tmp);
							throw std::current_exception();
						}
					}
				}

				return *this;
			}

			template<class U = T>
			requires(!std::is_void_v<T> &&
					 !std::same_as<expected<T, E>, std::remove_cvref_t<U>> &&
					 !std::conjunction_v<std::is_scalar<T>, std::same_as<T, std::decay_t<U>>> &&
					 std::constructible_from<T, U> &&
					 std::assignable_to<T&, U> &&
					 std::is_nothrow_move_constructible_v<E>)
			expected& operator=(U&& v)
			{
				if (*this)
				{
					storage.val = std::forward<U>(v);
				}
				else
				{
					if constexpr (std::is_nothrow_constructible_v<T, U>)
					{
						storage.unex.~unexpected_type();
						storage.val = std::forward<U>(v);
						has_val = true;
					}
					else
					{
						unexpected_type tmp = unexpected{std::move(error())};
						storage.unex.~unexpected_type();
						try
						{
							storage.val = std::forward<U>(v);
							has_val = true;
						}
						catch (...)
						{
							storage.unex = std::move(tmp);
							throw std::current_exception();
						}
					}
				}

				return *this;
			}

			template<class G = E>
			requires(std::is_nothrow_copy_constructible_v<E> &&
					 std::is_is_copy_assignable_v_v<E>)
			expected& operator=(const unexpected<G>& e)
			{
				if (*this)
				{
					storage.unex = unexpected{e.error()};
				}
				else
				{
					if constexpr (!std::is_void_v<T>)
					{
						storage.val.~T();
					}
					storage.unex = unexpected{e.error()};
					has_val = false;
				}

				return *this;
			}

			template<class G = E>
			requires(std::is_nothrow_move_constructible_v<E> &&
					 std::is_move_assignable_v<E>)
			expected& operator=(unexpected<G>&& e)
			{
				if (*this)
				{
					if constexpr (!std::is_void_v<T>)
					{
						storage.val.~T();
					}
					storage.unex = unexpected{std::move(e.error())};
					has_val = false;
				}
				else
				{
					storage.unex = unexpected{std::move(e.error())};
				}

				return *this;
			}

			void emplace() requires std::is_void_v<T>
			{
				if (!*this)
				{
					storage.unex.~unexpected_type();
					has_val = true;
				}
			}

			template<class... Args>
			requires (!std::is_void_v<T> && std::constructible_from<T, Args...>)
			T& emplace(Args&&... args)
			{
				if (*this)
				{
					storage.val = T{std::forward<Args>(args)...};
				}
				else if constexpr (std::is_nothrow_constructible_v<T, Args...>)
				{
					storage.unex.~unexpected_type();
					storage.val = T{std::forward<Args>(args)...};
					has_val = true;
				}
				else if constexpr (std::is_nothrow_move_constructible_v<T>)
				{
					auto tmp = T{std::forward<Args>(args)...};
					storage.unex.~unexpected_type();
					storage.val = std::move(tmp);
					has_val = true;
				}
				else
				{
					unexpected_type tmp = unexpected{std::move(error())};
					storage.unex.~unexpected_type();
					try
					{
						storage.val = T{std::forward<Args>(args)...};
						has_val = true;
					}
					catch (...)
					{
						storage.unex = std::move(tmp);
						throw std::current_exception();
					}
				}

				return *(*this);
			}

			template<class U, class... Args>
			requires(!std::is_void_v<T> && std::is_constructible_v<T, std::initializer_list<U>, Args...>)
			T& emplace(std::initializer_list<U> il, Args&&... args)
			{
				if (*this)
				{
					storage.val = T{il, std::forward<Args>(args)...};
				}
				else if constexpr (std::is_nothrow_constructible_v<T, std::initializer_list<U>, Args...>)
				{
					storage.unex.~unexpected_type();
					storage.val = T{il, std::forward<Args>(args)...};
					has_val = true;
				}
				else if constexpr (std::is_nothrow_move_constructible_v<T>)
				{
					T tmp{il, std::forward<Args>(args)...};
					storage.unex.~unexpected_type();
					storage.val = std::move(tmp);
					has_val = true;
				}
				else
				{
					unexpected_type tmp = unexpected{std::move(error())};
					storage.unex.~unexpected_type();
					try
					{
						storage.val = T{il, std::forward<Args>(args)...};
						has_val = true;
					}
					catch (...)
					{
						storage.unex = std::move(tmp);
						throw std::current_exception();
					}
				}

				return *(*this);
			}

			void swap(expected& other) // noexcept()
			requires(std::swappable<T> &&
					 std::swappable<E> &&
					 (std::is_void_v<T> ||
					  (!std::is_void_v<T> && (std::move_constructible<T> || std::move_constructible<E>))
					 )
					)
			{
				if (other)
				{
					if (*this)
					{
						if constexpr (!std::is_void_v<T>)
						{
							using std::swap;
							swap(storage.val, other.storage.val);
						}
					}
					else
					{
						other.swap(*this);
					}
				}
				else
				{
					if (*this)
					{
						if constexpr (std::is_void_v<T>)
						{
							try
							{
								storage.unex = unexpected{std::move(other)};
								has_val = false;
								other.unex.~unexpected_type();
								other.has_val = true;
							}
							catch (...)
							{
								throw std::current_exception();
							}
						}
						else if constexpr (std::is_nothrow_move_constructible_v<E>)
						{
							unexpected_type tmp = std::move(other.storage.unex);
							other.storage.unex.~unexpected_type();

							try
							{
								other.storage.val = std::move(*(*this));
								storage.val.~T();
								storage.unex = std::move(tmp);
								other.has_val = true;
								has_val = false;
							}
							catch (...)
							{
								other.storage.unex = std::move(tmp);
								throw std::current_exception();
							}
						}
						else if constexpr (std::is_nothrow_move_constructible_v<T>)
						{
							auto tmp = std::move(storage.val);
							storage.val.~T();
							try
							{
								storage.unex = unexpected{std::move(other.error())};
								other.storage.unex.~unexpected_type();
								other.val = std::move(tmp);
								other.has_val = true;
								has_val = false;
							}
							catch (...)
							{
								storage.val = std::move(tmp);
								throw std::current_exception();
							}
						}
					}
					else
					{
						using std::swap;
						swap(storage.unex, other.storage.unex);
					}
				}
			}

			constexpr const T* operator->() const
				requires !std::is_void_v<T>
			{
				return std::addressof(storage.val);
			}

			constexpr T* operator->()
				requires !std::is_void_v<T>
			{
				return std::addressof(storage.val);
			}

			constexpr const T& operator*() const&
				requires !std::is_void_v<T>
			{
				return storage.val;
			}

			constexpr T& operator*() &
				requires !std::is_void_v<T>
			{
				return storage.val;
			}


			constexpr const T&& operator*() const&&
				requires !std::is_void_v<T>
			{
				return std::move(storage.val);
			}

			constexpr T&& operator*() &&
				requires !std::is_void_v<T>
			{
				return std::move(storage.val);
			}

			[[nodiscard]] explicit constexpr operator bool() const noexcept
			{
				return has_val;
			}

			constexpr bool has_value() const noexcept
			{
				return has_val;
			}

			constexpr void value() const
				requires std::is_void_v<T>
			{
				if (!*this)
				{
					throw bad_expected_access{error()};
				}
			}

			constexpr const T& value() const&
				requires !std::is_void_v<T>
			{
				if (*this)
				{
					return storage.val;
				}

				throw bad_expected_access{error()};
			}

			constexpr T& value() &
				requires !std::is_void_v<T>
			{
				if (*this)
				{
					return storage.val;
				}

				throw bad_expected_access{error()};
			}

			constexpr const T&& value() const&&
				requires !std::is_void_v<T>
			{
				if (*this)
				{
					return std::move(storage.val);
				}

				throw bad_expected_access{error()};
			}

			constexpr T&& value() &&
				requires !std::is_void_v<T>
			{
				if (*this)
				{
					return std::move(storage.val);
				}

				throw bad_expected_access{error()};
			}

			constexpr const E& error() const&
			{
				return storage.unex.value();
			}

			constexpr E& error() &
			{
				return storage.unex.value();
			}

			constexpr const E&& error() const&&
			{
				return std::move(storage.unex.value());
			}

			constexpr E&& error() &&
			{
				return std::move(storage.unex.value());
			}

			template<class U>
			constexpr T value_or(U&& v) const&
				requires (std::copy_constructible<T> ? std::convertible_to<U, T> : true)
			{
				if (*this)
				{
					return **this;
				}

				return static_cast<T>(std::forward<U>(v));
			}

			template<class U>
			constexpr T value_or(U&& v) &&
				requires (std::move_constructible<T> ? std::convertible_to<U, T> : true)
			{
				if (*this)
				{
					return std::move(**this);
				}

				return static_cast<T>(std::forward<U>(v));
			}

			template<class T2, class E2>
				requires(requires (const expected& x, const expected<T2, E2>& y) {
					{ *x == *y } -> std::convertible_to<bool>;
					{ x.error() == y.error() } -> std::convertible_to<bool>;
				})
			friend constexpr bool operator==(const expected& x, const expected<T2, E2>& y)
			{
				if (static_cast<bool>(x) != static_cast<bool>(y))
				{
					return false;
				}

				if (!x)
				{
					return x.error() == y.error();
				}

				if constexpr (std::is_void_v<T> && std::is_void_v<T2>)
				{
					return true;
				}
				else
				{
					return *x == *y;
				}
			}

			template<class T2, class E2>
				requires(requires (const expected& x, const expected<T2, E2>& y) {
					{ *x != *y } -> std::convertible_to<bool>;
					{ x.error() != y.error() } -> std::convertible_to<bool>;
				})
			friend constexpr bool operator!=(const expected& x, const expected<T2, E2>& y)
			{
				if (static_cast<bool>(x) != static_cast<bool>(y))
				{
					return true;
				}

				if (!static_cast<bool>(x))
				{
					return x.error() != y.error();
				}

				if constexpr (std::is_void_v<T> && std::is_void_v<T2>)
				{
					return true;
				}
				else
				{
					return *x != *y;
				}
			}

			template<class T2>
				requires !std::is_void_v<T> && !std::is_void_v<T2> &&
					requires(const expected& x, const T2& v) {
						{ *x == v } -> std::convertible_to<bool>;
					}
			friend constexpr bool operator==(const expected& x, const T2& v)
			{
				if (x)
				{
					return *x == v;
				}

				return false;
			}

			template<class T2>
				requires !std::is_void_v<T> && !std::is_void_v<T2> &&
					requires(const expected& x, const T2& v) {
						{ *x == v } -> std::convertible_to<bool>;
					}
			friend constexpr bool operator==(const T2& v, const expected& x)
			{
				return x == v;
			}

			template<class T2>
				requires !std::is_void_v<T> && !std::is_void_v<T2> &&
					requires(const expected& x, const T2& v) {
						{ *x != v } -> std::convertible_to<bool>;
					}
			friend constexpr bool operator!=(const expected& x, const T2& v)
			{
				if (x)
				{
					return *x != v;
				}

				return false;
			}

			template<class T2>
				requires(!std::is_void_v<T> && !std::is_void_v<T2> &&
					requires(const expected& x, const T2& v) {
						{ *x != v } -> std::convertible_to<bool>;
					})
			friend constexpr bool operator!=(const T2& v, const expected& x)
			{
				return x != v;
			}

			template<class E2>
				requires(requires(const expected& x, const unexpected<E2>& e) {
					{ unexpected{x.error()} == e } -> std::convertible_to<bool>;
				})
			friend constexpr bool operator==(const expected& x, const unexpected<E2>& e)
			{
				if (x)
				{
					return false;
				}

				return unexpected{ x.error() } == e;
			}

			template<class E2>
				requires(requires(const expected& x, const unexpected<E2>& e) {
					{ unexpected{x.error()} == e } -> std::convertible_to<bool>;
				})
			friend constexpr bool operator==(const unexpected<E2>& e, const expected& x)
			{
				return x == e;
			}

			template<class E2>
				requires(requires(const expected& x, const unexpected<E2>& e) {
					{ unexpected{x.error()} != e } -> std::convertible_to<bool>;
				})
			friend constexpr bool operator!=(const expected& x, const unexpected<E2>& e)
			{
				if (x)
				{
					return true;
				}

				unexpected{ x.error() } != e;
			}

			template<class E2>
				requires(requires(const expected& x, const unexpected<E2>& e) {
					{ unexpected{x.error()} != e } -> std::convertible_to<bool>;
				})
			friend constexpr bool operator!=(const unexpected<E2>& e, const expected& x)
			{
				return x != e;
			}
		private:
			bool has_val;
			union_type_t storage;

	};

	export
	template<class E>
	class unexpected
	{
		public:
			constexpr unexpected(const unexpected&) = default;
			constexpr unexpected(unexpected&&) = default;

			template<class... Args>
				requires(std::constructible_from<E, Args...>)
			constexpr explicit unexpected(std::in_place_t, Args&&... args)
				: val{std::forward<Args>(args)...} {}

			template<class U, class... Args>
				requires(std::constructible_from<E, std::initializer_list<U>, Args...>)
			constexpr explicit unexpected(std::in_place_t, std::initializer_list<U> il, Args&&... args)
				: val{il, std::forward<Args>(args)...} {}

			template<class Err = E>
				requires(std::constructible_from<E, Err> &&
						 !std::same_as<std::remove_cvref_t<Err>, std::in_place_t> &&
						 !std::same_as<std::remove_cvref_t<Err>, unexpected>)
			constexpr explicit unexpected(Err&& e)
				: val{std::forward<Err>(e)} {}

			template<class Err>
				requires(std::constructible_from<E, const Err&> &&
						 !std::constructible_from<E, unexpected<Err>&> &&
						 !std::constructible_from<E, unexpected<Err>> &&
						 !std::constructible_from<E, const unexpected<Err>&> &&
						 !std::constructible_from<E, const unexpected<Err>> &&
						 !std::convertible_to<unexpected<Err>&, E> &&
						 !std::convertible_to<unexpected<Err>, E> &&
						 !std::convertible_to<const unexpected<Err>&, E> &&
						 !std::convertible_to<const unexpected<Err>, E>)
			constexpr explicit(!std::convertible_to<const Err&, E>) unexpected(const unexpected<Err>& e)
				: val{e.val} {}

			template<class Err>
				requires(std::constructible_from<E, Err> &&
						 !std::constructible_from<E, unexpected<Err>&> &&
						 !std::constructible_from<E, unexpected<Err>> &&
						 !std::constructible_from<E, const unexpected<Err>&> &&
						 !std::constructible_from<E, const unexpected<Err>> &&
						 !std::convertible_to<unexpected<Err>&, E> &&
						 !std::convertible_to<unexpected<Err>, E> &&
						 !std::convertible_to<const unexpected<Err>&, E> &&
						 !std::convertible_to<const unexpected<Err>, E>)
			constexpr explicit(!std::convertible_to<Err&, E>) unexpected(unexpected<Err>&& e)
				: val{std::move(e.val)} {}

			constexpr unexpected& operator=(const unexpected&) = default;
			constexpr unexpected& operator=(unexpected&&) = default;

			template<class Err = E>
				requires(std::is_assignable_v<E, const Err&>)
			constexpr unexpected& operator=(const unexpected<Err>& e)
			{
				val = e.val;
				return *this;
			}

			template<class Err = E>
				requires(std::is_assignable_v<E, Err>)
			constexpr unexpected& operator=(unexpected<Err>&& e)
			{
				val = std::move(e.val);
				return *this;
			}

			constexpr const E& value() const& noexcept
			{
				return val;
			}

			constexpr E& value() & noexcept
			{
				return val;
			}

			constexpr const E&& value() const&& noexcept
			{
				return std::move(val);
			}

			constexpr E&& value() && noexcept
			{
				return std::move(val);
			}

			void swap(unexpected& other) noexcept(std::is_nothrow_swappable_v<E>)
				requires(std::swappable<E>)
			{
				using std::swap;
				swap(val, other.val);
			}

			template<class E2>
			friend constexpr bool operator==(const unexpected& x, const unexpected<E2>& y)
			{
				return x.value() == y.value();
			}

			template<class E2>
			friend constexpr bool operator!=(const unexpected& x, const unexpected<E2>& y)
			{
				return x.value() != y.value();
			}

			friend void swap(unexpected& x, unexpected& y) noexcept(noexcept(x.swap(y)))
				requires(std::swappable<E>)
			{
				x.swap(y);
			}
		private:
			E val;
	};

	export
	template<class E>
	class bad_expected_access : public bad_expected_access<void>
	{
		public:
			explicit bad_expected_access(E e)
				: val{e} {}

			virtual const char* what() const noexcept override
			{
				"bad expected access";
			}

			E& error() &
			{
				return val;
			}

			const E& error() const&
			{
				return val;
			}

			E&& error() &&
			{
				return std::move(val);
			}

			const E&& error() const&&
			{
				return std::move(val);
			}
		private:
			E val;
	};
}

namespace std
{
	export
	template<class T, class E>
		requires ((std::is_void_v<T> || std::move_constructible<T>) &&
				 std::swappable<T> &&
				 std::move_constructible<E> &&
				 std::swappable<E>)
	void swap(cmoon::expected<T, E>& x, cmoon::expected<T, E>& y) noexcept(noexcept(x.swap(y)))
	{
		x.swap(y);
	}
}
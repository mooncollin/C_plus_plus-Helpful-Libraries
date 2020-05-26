#pragma once

#include <memory>
#include <map>
#include <type_traits>
#include <array>
#include <string>
#include <iostream>

namespace collin
{
	namespace memory
	{
		template<class T, class Allocator = std::allocator<T>>
		class Recycler
		{
			public:
				using value_type = T;
				using pointer = T*;
				using const_pointer = const pointer;
				using reference = value_type&;
				using const_reference = const reference;
				using size_type = std::size_t;
				using difference_type = std::ptrdiff_t;

				template<typename U>
				struct rebind
				{
					using other = Recycler<U>;
				};

				Recycler()
				{
					initializeMemory();
				}

				Recycler(const Recycler&) = default;
				Recycler(Recycler&&) = default;
				Recycler& operator=(const Recycler&) = default;
				Recycler& operator=(Recycler&&) = default;
				
				template<typename U>
				Recycler(const Recycler<U>& other)
				{
					initializeMemory();
				}

				Recycler(size_type amount_until_release)
					: _until_release(amount_until_release)
				{
					initializeMemory();
				}

				Recycler(size_type amount_until_release, const Allocator& alloc)
					: _until_release(amount_until_release), alloc(alloc)
				{
					initializeMemory();
				}

				pointer allocate(std::size_t n)
				{
					const auto location = storedMemory->find(n);
					if(location != std::end(*storedMemory))
					{
						auto ptr = location->second;
						storedMemory->erase(location);
						_currently_held -= n;
						return ptr;
					}

					return alloc.allocate(n);
				}

				void deallocate(pointer p, size_type n)
				{
					if(n + _currently_held >= _until_release
						|| n <= sizeof(typename memory_storage::allocator_type::value_type))
					{
						alloc.deallocate(p, n);
						return;
					}

					storedMemory->emplace(n, p);
					_currently_held += n;
				}

				void force_deallocate(pointer p, size_type n)
				{
					alloc.deallocate(p, n);
				}

				size_type until_release() const noexcept
				{
					return _until_release;
				}

				void until_release(size_type n)
				{
					_until_release = n;
					if(_currently_held >= _until_release)
					{
						free(_until_release);
					}
				}

				size_type currently_held() const noexcept
				{
					return _currently_held;
				}

				void currently_held(size_type n)
				{
					_currently_held = n;
				}

				void free(size_type n)
				{
					if(n >= _currently_held)
					{
						for(auto [n, ptr] : storedMemory)
						{
							force_deallocate(ptr, n);
						}
					}
					else
					{
						while(_currently_held >= n)
						{
							const auto key = _currently_held - n;
							const auto location = storedMemory.lower_bound(key);
							if(location != std::end(storedMemory))
							{
								force_deallocate(*location, key);
							}
							else
							{
								break;
							}
						}
					}
				}
			private:
				using memory_storage = std::multimap<size_type, pointer>;
				size_type _until_release = 1024 * 500;
				size_type _currently_held = 0;
				Allocator alloc;
				std::shared_ptr<memory_storage> storedMemory;

				void initializeMemory()
				{
					storedMemory = std::shared_ptr<memory_storage>(new memory_storage(), [&](memory_storage* storage) {
						for (auto [n, ptr] : *storage)
						{
							alloc.deallocate(ptr, n);
						}

						delete storage;
					});
				}
		};

		template<std::size_t N>
		class bitset
		{
			public:
				constexpr bitset() noexcept
					: bits_{} {}

				constexpr bitset(unsigned long val) noexcept
					: bits_{} 
				{
					for (long long int i = N - 1; i > 0; i--)
					{
						bits_[i] = val & 1;
						val >>= 1;
					}
				}

				template<class CharT, class Traits, class Alloc>
				explicit bitset(const std::basic_string<CharT, Traits, Alloc>& str,
					typename std::basic_string<CharT, Traits, Alloc>::size_type pos = 0,
					typename std::basic_string<CharT, Traits, Alloc>::size_type n = std::basic_string<CharT, Traits, Alloc>::npos)
					: bits_{}
				{
					for (std::size_t i = 0; i < n && i < N; i++)
					{
						bits_[i] = str[pos + i] == CharT('1');
					}
				}

				constexpr bool operator[](std::size_t pos) const noexcept
				{
					return bits_[pos];
				}

				constexpr bool& operator[](std::size_t pos) noexcept
				{
					return bits_[pos];
				}

				constexpr std::size_t count() const noexcept
				{
					std::size_t count_ {0};
					for (auto b : bits_)
					{
						count_ += b;
					}

					return count_;
				}

				constexpr std::size_t size() const noexcept
				{
					return N;
				}

				constexpr bool test(std::size_t pos) const noexcept
				{
					return bits_[pos];
				}

				constexpr bool any() const noexcept
				{
					for (auto b : bits_)
					{
						if (b)
						{
							return true;
						}
					}

					return false;
				}

				constexpr bool none() const noexcept
				{
					return !any();
				}

				constexpr bool all() const noexcept
				{
					return count() == N;
				}

				bitset& set(std::size_t pos, bool val = true) noexcept
				{
					bits_[pos] = val;
					return *this;
				}

				bitset& set() noexcept
				{
					for (auto& b : bits_)
					{
						b = true;
					}

					return *this;
				}
				
				bitset& reset() noexcept
				{
					for (auto& b : bits_)
					{
						b = false;
					}
					
					return *this;
				}

				bitset& reset(std::size_t pos) noexcept
				{
					bits_[pos] = false;

					return *this;
				}

				bitset& flip() noexcept
				{
					for (auto& b : bits_)
					{
						b = !b;
					}

					return *this;
				}

				bitset& flip(std::size_t pos) noexcept
				{
					bits_[pos] = !bits_[pos];

					return *this;
				}

				template<class CharT = char, class Traits = std::char_traits<CharT>, class Alloc = std::allocator<CharT>>
				std::basic_string<CharT, Traits, Alloc> to_string(CharT zero = CharT('0'), CharT one = CharT('1')) const
				{
					std::basic_string<CharT, Traits, Alloc> str;
					str.resize(N);
					
					for (std::size_t i = 0; i < N; i++)
					{
						str[i] = bits_[i] ? one : zero;
					}

					return str;
				}

				constexpr unsigned long to_ulong() const noexcept
				{
					unsigned long value {0};

					for (std::size_t i = 0; i < N; i++)
					{
						value |= (bits_[i] << (N - i - 1));
					}

					return value;
				}

				constexpr unsigned long long to_ullong() const noexcept
				{
					unsigned long long value {0};

					for (std::size_t i = 0; i < N; i++)
					{
						value |= (bits_[i] << (N - i - 1));
					}

					return value;
				}

				bitset& operator&=(const bitset& rhs) noexcept
				{
					for (std::size_t i = 0; i < N; i++)
					{
						bits_[i] &= rhs.bits_[i];
					}

					return *this;
				}

				bitset& operator|=(const bitset& rhs) noexcept
				{
					for (std::size_t i = 0; i < N; i++)
					{
						bits_[i] |= rhs.bits_[i];
					}

					return *this;
				}

				bitset& operator^=(const bitset& rhs) noexcept
				{
					for (std::size_t i = 0; i < N; i++)
					{
						bits_[i] ^= rhs.bits_[i];
					}

					return *this;
				}

				bitset& operator<<=(std::size_t pos) noexcept
				{
					for (std::size_t i = 0; i < N - pos; i++)
					{
						bits_[i] = bits_[i + pos];
					}

					for (; pos < N; pos++)
					{
						bits_[pos] = false;
					}

					return *this;
				}

				bitset& operator>>=(std::size_t pos) noexcept
				{
					for (std::size_t i = 0; i < N - pos; i++)
					{
						bits_[i + pos] = bits_[i];
						bits_[i] = false;
					}

					return *this;
				}

				bitset operator~() const noexcept
				{
					bitset b = *this;
					b.flip();
					return b;
				}

				bitset operator<<(std::size_t pos) const noexcept
				{
					bitset b = *this;
					b <<= pos;
					return b;
				}

				bitset operator>>(std::size_t pos) const noexcept
				{
					bitset b = *this;
					b >>= pos;
					return b;
				}

				bool operator==(const bitset& rhs) const noexcept
				{
					for (std::size_t i = 0; i < N; i++)
					{
						if (!(bits_[i] == rhs.bits_[i]))
						{
							return false;
						}
					}

					return true;
				}

				bool operator!=(const bitset& rhs) const noexcept
				{
					return !(*this == rhs);
				}
			private:
				std::array<bool, N> bits_;
		};

		template<std::size_t N>
		bitset<N> operator& (const bitset<N>& lhs, const bitset<N>& rhs) noexcept
		{
			bitset<N> b = lhs;
			b &= rhs;
			return b;
		}

		template<std::size_t N>
		bitset<N> operator| (const bitset<N>& lhs, const bitset<N>& rhs) noexcept
		{
			bitset<N> b = lhs;
			b |= rhs;
			return b;
		}

		template<std::size_t N>
		bitset<N> operator^ (const bitset<N>& lhs, const bitset<N>& rhs) noexcept
		{
			bitset<N> b = lhs;
			b ^= rhs;
			return b;
		}

		template<class CharT, class Traits, std::size_t N>
		std::basic_istream<CharT, Traits>& operator>>(std::basic_istream<CharT, Traits>& is, bitset<N>& rhs)
		{
			CharT c;
			for (std::size_t i = 0; i < N && is >> c; i++)
			{
				rhs[i] = c == CharT('1');
			}

			return is;
		}

		template<class CharT, class Traits, std::size_t N>
		std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& is, const bitset<N>& rhs)
		{
			return is << rhs.to_string();
		}
	}
}
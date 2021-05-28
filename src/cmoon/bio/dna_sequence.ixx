export module cmoon.bio.dna_sequence;

import <vector>;
import <memory>;
import <initializer_list>;
import <iostream>;
import <sstream>;
import <algorithm>;
import <format>;

import cmoon.bio.nucleotide;
import cmoon.bio.rna_sequence;

namespace cmoon::bio
{
	export
	template<class Allocator = std::allocator<dna_nucleotide>>
	class dna_sequence
	{
		using storage_t = std::vector<dna_nucleotide>;

		public:
			using value_type = typename storage_t::value_type;
			using allocator_type = typename storage_t::allocator_type;
			using size_type = typename storage_t::size_type;
			using difference_type = typename storage_t::difference_type;
			using reference = typename storage_t::reference;
			using const_reference = typename storage_t::const_reference;
			using pointer = typename storage_t::pointer;
			using const_pointer = typename storage_t::const_pointer;
			using iterator = typename storage_t::iterator;
			using const_iterator = typename storage_t::const_iterator;
			using reverse_iterator = typename storage_t::reverse_iterator;
			using const_reverse_iterator = typename storage_t::const_reverse_iterator;

			dna_sequence(const Allocator& alloc = Allocator())
				: sequence_{alloc} {}

			dna_sequence(const dna_sequence&) = default;
			dna_sequence(dna_sequence&&) noexcept = default;

			dna_sequence& operator=(const dna_sequence&) = default;
			dna_sequence& operator=(dna_sequence&&) noexcept = default;

			dna_sequence& operator=(std::initializer_list<dna_nucleotide> list)
			{
				sequence_ = list;
				return *this;
			}
				
			template<std::input_iterator I>
			dna_sequence(I first, I last, const Allocator& alloc = Allocator())
				: sequence_{first, first, alloc} {}

			dna_sequence(std::initializer_list<dna_nucleotide> init, const Allocator& alloc = Allocator())
				: sequence_{init, alloc} {}

			void assign(size_type count, const_reference value)
			{
				sequence_.assign(count, value);
			}

			template<std::input_iterator I>
			void assign(I first, I last)
			{
				sequence_.assign(first, last);
			}

			void assign(std::initializer_list<value_type> ilist)
			{
				sequence_.assign(ilist);
			}

			[[nodiscard]] const_reference operator[](size_type n) const noexcept
			{
				return sequence_[n];
			}

			[[nodiscard]] reference operator[](size_type n) noexcept
			{
				return sequence_[n];
			}

			[[nodiscard]] const_reference at(size_type n) const
			{
				return sequence_.at(n);
			}

			[[nodiscard]] reference at(size_type n)
			{
				return sequence_.at(n);
			}

			reference front()
			{
				return sequence_.front();
			}

			const_reference front() const
			{
				return sequence_.front();
			}

			reference back()
			{
				return sequence_.back();
			}

			const_reference back() const
			{
				return sequence_.back();
			}

			[[nodiscard]] pointer data() noexcept
			{
				return sequence_.data();
			}

			[[nodiscard]] const_pointer data() const noexcept
			{
				return sequence_.data();
			}

			iterator begin() noexcept
			{
				return std::begin(sequence_);
			}

			const_iterator begin() const noexcept
			{
				return std::begin(sequence_);
			}

			const_iterator cbegin() const noexcept
			{
				return std::cbegin(sequence_);
			}

			iterator end() noexcept
			{
				return std::end(sequence_);
			}

			const_iterator end() const noexcept
			{
				return std::end(sequence_);
			}

			const_iterator cend() const noexcept
			{
				return std::cend(sequence_);
			}

			reverse_iterator rbegin() noexcept
			{
				return std::rbegin(sequence_);
			}

			const_reverse_iterator rbegin() const noexcept
			{
				return std::rbegin(sequence_);
			}

			const_reverse_iterator crbegin() const noexcept
			{
				return std::crbegin(sequence_);
			}

			reverse_iterator rend() noexcept
			{
				return std::rend(sequence_);
			}

			const_reverse_iterator rend() const noexcept
			{
				return std::rend(sequence_);
			}

			const_reverse_iterator crend() const noexcept
			{
				return std::crend(sequence_);
			}

			[[nodiscard]] bool empty() const noexcept
			{
				return sequence_.empty();
			}

			[[nodiscard]] size_type size() const noexcept
			{
				return std::size(sequence_);
			}

			void clear() noexcept
			{
				sequence_.clear();
			}

			iterator insert(const_iterator pos, const_reference value)
			{
				return sequence_.insert(pos, value);
			}

			iterator insert(const_iterator pos, value_type&& value)
			{
				return sequence_.insert(pos, std::move(value));
			}

			iterator insert(const_iterator pos, size_type count, const_reference value)
			{
				return sequence_.insert(pos, count, value);
			}

			template<std::input_iterator I>
			iterator insert(const_iterator pos, I first, I last)
			{
				return sequence_.insert(pos, first, last);
			}

			iterator insert(const_iterator pos, std::initializer_list<value_type> ilist)
			{
				return sequence_.insert(pos, ilist);
			}

			iterator erase(const_iterator pos)
			{
				return sequence_.erase(pos);
			}

			iterator erase(const_iterator first, const_iterator last)
			{
				return sequence_.erase(first, last);
			}

			void push_back(const_reference value)
			{
				sequence_.push_back(value);
			}

			void pop_back()
			{
				sequence_.pop_back();
			}

			void swap(dna_sequence& other) noexcept(noexcept(std::declval<storage_t>().swap(std::declval<storage_t>())))
			{
				sequence_.swap(other.sequence_);
			}

			allocator_type get_allocator() const noexcept
			{
				return sequence_.get_allocator();
			}

			storage_t& sequence()
			{
				return sequence_;
			}

			const storage_t& sequence() const
			{
				return sequence_;
			}

			[[nodiscard]] auto to_rna_sequence() const
			{
				rna_sequence<typename std::allocator_traits<Allocator>::template rebind_alloc<rna_nucleotide>> r;
				r.sequence().resize(size());
				std::transform(begin(), end(), std::begin(r),
					[](const auto n) { return nucleotide_convert(n); });
				return r;
			}

			[[nodiscard]] dna_sequence compliment() const
			{
				dna_sequence d;
				d.sequence().resize(size());
				std::transform(begin(), end(), std::begin(d),
					[](const auto n) { return nucleotide_compliment(n); });
				return d;
			}

			[[nodiscard]] auto to_rna_compliment() const
			{
				rna_sequence<typename std::allocator_traits<Allocator>::template rebind_alloc<rna_nucleotide>> r;
				r.sequence().resize(size());
				std::transform(std::begin(), std::end(), std::begin(r),
					[](const auto n) { return nucleotide_convert(nucleotide_compliment(n)); });
				return r;
			}
		private:
			storage_t sequence_;
	};
	
	export
	template<class CharT, class Traits, class Allocator>
	std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const dna_sequence<Allocator>& s)
	{
		for (const auto n : s.sequence())
		{
			os << n;
		}

		return os;
	}

	export
	template<class CharT, class Traits, class Allocator>
	std::basic_istream<CharT, Traits>& operator>>(std::basic_istream<CharT, Traits>& is, dna_sequence<Allocator>& s)
	{
		CharT ch;
		s.clear();
		is >> std::ws;
		while (is.get(ch))
		{
			switch (ch)
			{
				case static_cast<char>(dna_nucleotide::guanine):
					s.sequence().push_back(dna_nucleotide::guanine);
					break;
				case static_cast<char>(dna_nucleotide::thymine):
					s.sequence().push_back(dna_nucleotide::thymine);
					break;
				case static_cast<char>(dna_nucleotide::adenine):
					s.sequence().push_back(dna_nucleotide::adenine);
					break;
				case static_cast<char>(dna_nucleotide::cytosine):
					s.sequence().push_back(dna_nucleotide::cytosine);
					break;
				default:
					if (!std::isspace(ch, std::locale{}))
					{
						is.setstate(std::ios::failbit);
					}
					is.unget();
					return is;
			}
		}

		return is;
	}
}

namespace std
{
	export
	template<class Allocator>
	struct formatter<cmoon::bio::dna_sequence<Allocator>> : public formatter<std::string_view>
	{
		private:
			using base = formatter<std::string_view>;
		public:
			template<class FormatContext>
			auto format(const cmoon::bio::dna_sequence<Allocator>& d, FormatContext& ctx)
			{
				return base::format({d.data(), d.size()}, ctx);
			}
	};
}
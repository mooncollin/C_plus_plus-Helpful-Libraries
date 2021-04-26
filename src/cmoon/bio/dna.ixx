export module cmoon.bio.dna;

import <vector>;
import <memory>;
import <initializer_list>;
import <iostream>;
import <sstream>;
import <algorithm>;

import cmoon.format;

import cmoon.bio.nucleotide;
import cmoon.bio.rna;

namespace cmoon
{
	namespace bio
	{
		template<class Allocator>
		class rna;

		export
		template<class Allocator = std::allocator<dna_nucleotide>>
		class dna
		{
			using storage_t = std::vector<dna_nucleotide>;

			public:
				dna(const Allocator& alloc = Allocator())
					: sequence_{alloc} {}

				dna(const dna&) = default;
				dna(dna&&) noexcept = default;

				dna& operator=(const dna&) = default;
				dna& operator=(dna&&) noexcept = default;

				dna& operator=(std::initializer_list<dna_nucleotide> list)
				{
					sequence_ = list;
					return *this;
				}
				
				template<class InputIt>
				dna(InputIt first, InputIt last, const Allocator& alloc = Allocator())
					: sequence_{first, first, alloc} {}

				dna(std::initializer_list<dna_nucleotide> init, const Allocator& alloc = Allocator())
					: sequence_{init, alloc} {}

				storage_t& sequence()
				{
					return sequence_;
				}

				const storage_t& sequence() const
				{
					return sequence_;
				}

				[[nodiscard]] auto to_rna() const
				{
					rna<std::allocator_traits<Allocator>::template rebind_alloc<rna_nucleotide>> r;
					r.sequence().resize(sequence_.size());
					std::transform(std::begin(sequence_), std::end(sequence_), std::begin(r.sequence()),
						[](const auto n) { return nucleotide_convert(n); });
					return r;
				}

				[[nodiscard]] dna compliment() const
				{
					dna d;
					d.sequence().resize(sequence_.size());
					std::transform(std::begin(sequence_), std::end(sequence_), std::begin(d.sequence()),
						[](const auto n) { return nucleotide_compliment(n); });
					return d;
				}

				[[nodiscard]] auto to_rna_compliment() const
				{
					rna<std::allocator_traits<Allocator>::template rebind_alloc<rna_nucleotide>> r;
					r.sequence().resize(sequence_.size());
					std::transform(std::begin(sequence_), std::end(sequence_), std::begin(r.sequence()),
						[](const auto n) { return nucleotide_convert(nucleotide_compliment(n)); });
					return r;
				}
			private:
				storage_t sequence_;
		};

		template<class CharT, class Traits, class Allocator>
		std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const dna<Allocator>& s)
		{
			for (const auto n : s.sequence())
			{
				os << n;
			}

			return os;
		}

		template<class CharT, class Traits, class Allocator>
		std::basic_istream<CharT, Traits>& operator>>(std::basic_istream<CharT, Traits>& is, dna<Allocator>& s)
		{
			CharT ch;
			s.sequence().clear();
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
}

namespace cmoon
{
	export
	template<typename CharT, class Allocator>
	struct formatter<dna<Allocator>> : public formatter<std::basic_string_view<CharT, CharT>>
	{
		template<class OutputIt>
		auto format(const dna<Allocator>& d, basic_format_context<OutputIt, CharT>& ctx)
		{
			std::basic_stringstream<CharT> ss;
			ss << d;
			return formatter<std::basic_string_view<CharT, CharT>>::format(ss.str(), ctx);
		}
	};
}
#pragma once

#include <vector>
#include <memory>
#include <initializer_list>
#include <iostream>
#include <locale>
#include <algorithm>
#include <cmath>

#include "cmoon/bio/nucleotide.hpp"
#include "cmoon/bio/dna.hpp"

namespace cmoon
{
	namespace bio
	{
		template<class Allocator = std::allocator<rna_nucleotide>>
		class rna
		{
			using storage_t = std::vector<rna_nucleotide, Allocator>;

			public:
				rna(const Allocator& alloc = Allocator())
					: sequence_{alloc} {}

				rna(const rna&) = default;
				rna(rna&&) noexcept = default;

				rna& operator=(const rna&) = default;
				rna& operator=(rna&&) noexcept = default;

				rna& operator=(std::initializer_list<rna_nucleotide> list)
				{
					sequence_ = list;
					return *this;
				}
				
				template<class InputIt>
				rna(InputIt first, InputIt last, const Allocator& alloc = Allocator())
					: sequence_{first, first, alloc} {}

				rna(std::initializer_list<rna_nucleotide> init, const Allocator& alloc = Allocator())
					: sequence_{init, alloc} {}

				storage_t& sequence()
				{
					return sequence_;
				}

				const storage_t& sequence() const
				{
					return sequence_;
				}

				[[nodiscard]] auto to_dna() const
				{
					dna<std::allocator_traits<Allocator>::template rebind_alloc<dna_nucleotide>> d;
					d.sequence().resize(sequence_.size());
					std::transform(std::begin(sequence_), std::end(sequence_), std::begin(d.sequence()),
						[](const auto n) { return nucleotide_convert(n); });
					return d;
				}

				[[nodiscard]] rna compliment() const
				{
					rna r;
					r.sequence().resize(sequence_.size());
					std::transform(std::begin(sequence_), std::end(sequence_), std::begin(r.sequence()),
						[](const auto n) { return nucleotide_compliment(n); });
					return r;
				}

				[[nodiscard]] auto to_dna_compliment() const
				{
					dna<std::allocator_traits<Allocator>::template rebind_alloc<rna_nucleotide>> d;
					d.sequence().resize(sequence_.size());
					std::transform(std::begin(sequence_), std::end(sequence_), std::begin(d.sequence()),
						[](const auto n) { return nucleotide_convert(nucleotide_compliment(n)); });
					return d;
				}

				[[nodiscard]] codon get_codon(std::size_t index, bool reverse = false) const
				{
					if (reverse)
					{
						return codon{sequence_[index], sequence_[index - 1], sequence_[index - 2]};
					}

					return codon{sequence_[index], sequence_[index + 1], sequence_[index + 2]};
				}
			private:
				storage_t sequence_;
		};

		template<class Allocator>
		std::ostream& operator<<(std::ostream& os, const rna<Allocator>& s)
		{
			for (const auto n : s.sequence())
			{
				os << static_cast<char>(n);
			}

			return os;
		}

		template<class Allocator>
		std::istream& operator>>(std::istream& is, rna<Allocator>& s)
		{
			char ch;
			s.sequence().clear();
			is >> std::ws;
			while (is.get(ch))
			{
				switch (ch)
				{
					case static_cast<char>(rna_nucleotide::guanine):
						s.sequence().push_back(rna_nucleotide::guanine);
						break;
					case static_cast<char>(rna_nucleotide::uracil):
						s.sequence().push_back(rna_nucleotide::uracil);
						break;
					case static_cast<char>(rna_nucleotide::adenine):
						s.sequence().push_back(rna_nucleotide::adenine);
						break;
					case static_cast<char>(rna_nucleotide::cytosine):
						s.sequence().push_back(rna_nucleotide::cytosine);
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

		template<class Allocator, class OutputIterator>
		void amino_acid_sequence(const rna<Allocator>& r, OutputIterator out, int reading_frame = 1) noexcept
		{
			const auto size = r.sequence().size();
			if (reading_frame > 0)
			{
				for (std::size_t lead {reading_frame - 1}; lead + 3 < size; lead += 3)
				{
					*out = r.get_codon(lead).to_amino_acid();
					++out;
				}
			}
			else if (reading_frame < 0)
			{
				for (std::size_t lead {-reading_frame}; lead + 3 < size; lead += 3)
				{
					*out = r.get_codon(size - lead, true).to_amino_acid();
					++out;
				}
			}
		}
	}
}
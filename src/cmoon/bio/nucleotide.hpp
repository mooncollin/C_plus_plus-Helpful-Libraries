#pragma once

#include <iostream>

namespace cmoon
{
	namespace bio
	{
		enum class rna_nucleotide : char
		{
			guanine = 'G',
			uracil = 'U',
			adenine = 'A',
			cytosine = 'C'
		};

		enum class dna_nucleotide : char
		{
			guanine = 'G',
			thymine = 'T',
			adenine = 'A',
			cytosine = 'C'
		};

		enum class amino_acid : char
		{
			 alanine = 'A',
			 cysteine = 'C',
			 aspartic_acid = 'D',
			 gluatmic_acid = 'E',
			 phenylalanine = 'F',
			 glycine = 'G',
			 histidine = 'H',
			 isoleucine = 'I',
			 lysine = 'K',
			 leucine = 'L',
			 methionine = 'M',
			 asparagine = 'N',
			 proline = 'P',
			 glutamine = 'Q',
			 arginine = 'R',
			 serine = 'S',
			 threonine = 'T',
			 valine = 'V',
			 tryptophan = 'W',
			 tyrosine = 'Y',
			 stop = '-',
		};

		[[nodiscard]] constexpr dna_nucleotide nucleotide_convert(const rna_nucleotide n) noexcept
		{
			switch (n)
			{
				case rna_nucleotide::guanine:
					return dna_nucleotide::guanine;
				case rna_nucleotide::uracil:
					return dna_nucleotide::thymine;
				case rna_nucleotide::adenine:
					return dna_nucleotide::adenine;
				case rna_nucleotide::cytosine:
					return dna_nucleotide::cytosine;
			}
		}

		[[nodiscard]] constexpr rna_nucleotide nucleotide_convert(const dna_nucleotide n) noexcept
		{
			switch (n)
			{
				case dna_nucleotide::guanine:
					return rna_nucleotide::guanine;
				case dna_nucleotide::thymine:
					return rna_nucleotide::uracil;
				case dna_nucleotide::adenine:
					return rna_nucleotide::adenine;
				case dna_nucleotide::cytosine:
					return rna_nucleotide::cytosine;
			}
		}

		[[nodiscard]] constexpr rna_nucleotide nucleotide_compliment(const rna_nucleotide n) noexcept
		{
			switch (n)
			{
				case rna_nucleotide::guanine:
					return rna_nucleotide::cytosine;
				case rna_nucleotide::cytosine:
					return rna_nucleotide::guanine;
				case rna_nucleotide::uracil:
					return rna_nucleotide::adenine;
				case rna_nucleotide::adenine:
					return rna_nucleotide::uracil;
			}
		}

		[[nodiscard]] constexpr dna_nucleotide nucleotide_compliment(const dna_nucleotide n) noexcept
		{
			switch (n)
			{
				case dna_nucleotide::guanine:
					return dna_nucleotide::cytosine;
				case dna_nucleotide::cytosine:
					return dna_nucleotide::guanine;
				case dna_nucleotide::thymine:
					return dna_nucleotide::adenine;
				case dna_nucleotide::adenine:
					return dna_nucleotide::thymine;
			}
		}

		struct codon
		{
			constexpr codon(rna_nucleotide f, rna_nucleotide s, rna_nucleotide t)
				: first{f}, second{s}, third{t} {}

			constexpr codon(dna_nucleotide f, dna_nucleotide s, dna_nucleotide t)
				: first{nucleotide_convert(f)}, second{nucleotide_convert(s)}, third{nucleotide_convert(t)} {}

			[[nodiscard]] constexpr amino_acid to_amino_acid() const noexcept
			{
				switch (first)
				{
					case rna_nucleotide::uracil:
						switch (second)
						{
							case rna_nucleotide::uracil:
								switch (third)
								{
									case rna_nucleotide::uracil:
									case rna_nucleotide::cytosine:
										return amino_acid::phenylalanine;
									case rna_nucleotide::adenine:
									case rna_nucleotide::guanine:
										return amino_acid::leucine;
								}
							case rna_nucleotide::cytosine:
								return amino_acid::serine;
							case rna_nucleotide::adenine:
								switch (third)
								{
									case rna_nucleotide::uracil:
									case rna_nucleotide::cytosine:
										return amino_acid::tyrosine;
									case rna_nucleotide::adenine:
									case rna_nucleotide::guanine:
										return amino_acid::stop;
								}
							case rna_nucleotide::guanine:
								switch (third)
								{
									case rna_nucleotide::uracil:
									case rna_nucleotide::cytosine:
										return amino_acid::cysteine;
									case rna_nucleotide::adenine:
										return amino_acid::stop;
									case rna_nucleotide::guanine:
										return amino_acid::tryptophan;
								}
						}
					case rna_nucleotide::cytosine:
						switch (second)
						{
							case rna_nucleotide::uracil:
								return amino_acid::leucine;
							case rna_nucleotide::cytosine:
								return amino_acid::proline;
							case rna_nucleotide::adenine:
								switch (third)
								{
									case rna_nucleotide::uracil:
									case rna_nucleotide::cytosine:
										return amino_acid::histidine;
									case rna_nucleotide::adenine:
									case rna_nucleotide::guanine:
										return amino_acid::glutamine;
								}
							case rna_nucleotide::guanine:
								return amino_acid::arginine;
						}
					case rna_nucleotide::adenine:
						switch (second)
						{
							case rna_nucleotide::uracil:
								switch (third)
								{
									case rna_nucleotide::uracil:
									case rna_nucleotide::cytosine:
									case rna_nucleotide::adenine:
										return amino_acid::isoleucine;
									case rna_nucleotide::guanine:
										return amino_acid::methionine;
								}
							case rna_nucleotide::cytosine:
								return amino_acid::threonine;
							case rna_nucleotide::adenine:
								switch (third)
								{
									case rna_nucleotide::uracil:
									case rna_nucleotide::cytosine:
										return amino_acid::asparagine;
									case rna_nucleotide::adenine:
									case rna_nucleotide::guanine:
										return amino_acid::lysine;
								}
							case rna_nucleotide::guanine:
								switch (third)
								{
									case rna_nucleotide::uracil:
									case rna_nucleotide::cytosine:
										return amino_acid::serine;
									case rna_nucleotide::adenine:
									case rna_nucleotide::guanine:
										return amino_acid::arginine;
								}
						}
					case rna_nucleotide::guanine:
						switch (second)
						{
							case rna_nucleotide::uracil:
								return amino_acid::valine;
							case rna_nucleotide::cytosine:
								return amino_acid::alanine;
							case rna_nucleotide::adenine:
								switch (third)
								{
									case rna_nucleotide::uracil:
									case rna_nucleotide::cytosine:
										return amino_acid::aspartic_acid;
									case rna_nucleotide::adenine:
									case rna_nucleotide::guanine:
										return amino_acid::gluatmic_acid;
								}
							case rna_nucleotide::guanine:
								return amino_acid::glycine;
						}
				}
			}

			rna_nucleotide first;
			rna_nucleotide second;
			rna_nucleotide third;
		};

		std::ostream& operator<<(std::ostream& os, const rna_nucleotide n)
		{
			return os << static_cast<char>(n);
		}

		std::ostream& operator<<(std::ostream& os, const dna_nucleotide n)
		{
			return os << static_cast<char>(n);
		}

		std::ostream& operator<<(std::ostream& os, const amino_acid a)
		{
			return os << static_cast<char>(a);
		}
	}
}
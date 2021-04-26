export module cmoon.bio.nucleotide;

import <iostream>;
import <concepts>;
import <type_traits>;

import cmoon.format;

namespace cmoon::bio
{
	export
	enum class rna_nucleotide : char
	{
		guanine = 'G',
		uracil = 'U',
		adenine = 'A',
		cytosine = 'C'
	};

	export
	enum class dna_nucleotide : char
	{
		guanine = 'G',
		thymine = 'T',
		adenine = 'A',
		cytosine = 'C'
	};

	export
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

	export
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

	export
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

	export
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

	export
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

	export
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

	export
	template<class CharT, class Traits>
	std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const rna_nucleotide n)
	{
		return os << static_cast<std::underlying_t<rna_nucleotide>>(n);
	}

	export
	template<class CharT, class Traits>
	std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const dna_nucleotide n)
	{
		return os << static_cast<std::underlying_t<dna_nucleotide>>(n);
	}

	export
	template<class CharT, class Traits>
	std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const amino_acid a)
	{
		return os << static_cast<std::underlying_t<amino_acid>>(a);
	}
}

namespace cmoon
{
	export
	template<typename CharT>
	struct formatter<rna_nucleotide> : public formatter<std::underlying_t<rna_nucleotide>, CharT>
	{
		template<class OutputIt>
		auto format(const rna_nucleotide& n, basic_format_context<OutputIt, CharT>& ctx)
		{
			return formatter<std::underlying_t<rna_nucleotide>, CharT>::format(static_cast<std::underlying_t<rna_nucleotide>>(n), ctx);
		}
	};

	export
	template<typename CharT>
	struct formatter<dna_nucleotide> : public formatter<std::underlying_t<dna_nucleotide>, CharT>
	{
		template<class OutputIt>
		auto format(const dna_nucleotide& n, basic_format_context<OutputIt, CharT>& ctx)
		{
			return formatter<std::underlying_t<dna_nucleotide>, CharT>::format(static_cast<std::underlying_t<dna_nucleotide>>(n), ctx);
		}
	};

	export
	template<typename CharT>
	struct formatter<amino_acid> : public formatter<std::underlying_t<amino_acid>, CharT>
	{
		template<class OutputIt>
		auto format(const amino_acid& n, basic_format_context<OutputIt, CharT>& ctx)
		{
			return formatter<std::underlying_t<amino_acid>, CharT>::format(static_cast<std::underlying_t<amino_acid>>(n), ctx);
		}
	};
}
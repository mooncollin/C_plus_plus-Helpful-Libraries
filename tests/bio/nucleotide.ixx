export module cmoon.tests.bio.nucleotide;

import <sstream>;
import <format>;

import cmoon.concepts;
import cmoon.test;
import cmoon.bio;

namespace cmoon::tests::bio
{
	export
	class rna_nucleotide_test : public cmoon::test::test_case
	{
		public:
			rna_nucleotide_test()
				: cmoon::test::test_case{"rna_nucleotide_test"} {}

			void operator()() override
			{
				cmoon::test::assert_equal(static_cast<char>(cmoon::bio::rna_nucleotide::guanine), 'G');
				cmoon::test::assert_equal(static_cast<char>(cmoon::bio::rna_nucleotide::uracil), 'U');
				cmoon::test::assert_equal(static_cast<char>(cmoon::bio::rna_nucleotide::adenine), 'A');
				cmoon::test::assert_equal(static_cast<char>(cmoon::bio::rna_nucleotide::cytosine), 'C');

				std::stringstream ss;
				ss << cmoon::bio::rna_nucleotide::guanine;
				cmoon::test::assert_equal(ss.str(), "G");

				ss.str("");
				ss << cmoon::bio::rna_nucleotide::uracil;
				cmoon::test::assert_equal(ss.str(), "U");

				ss.str("");
				ss << cmoon::bio::rna_nucleotide::adenine;
				cmoon::test::assert_equal(ss.str(), "A");

				ss.str("");
				ss << cmoon::bio::rna_nucleotide::cytosine;
				cmoon::test::assert_equal(ss.str(), "C");

				// TODO: MSVC c++20 modules do not expose std::formatter specializations yet
				//cmoon::test::assert_equal(std::format("{}", cmoon::bio::rna_nucleotide::guanine), "G");
				//cmoon::test::assert_equal(std::format("{}", cmoon::bio::rna_nucleotide::uracil), "U");
				//cmoon::test::assert_equal(std::format("{}", cmoon::bio::rna_nucleotide::adenine), "A");
				//cmoon::test::assert_equal(std::format("{}", cmoon::bio::rna_nucleotide::cytosine), "C");
			}
	};

	export
	class dna_nucleotide_test : public cmoon::test::test_case
	{
		public:
			dna_nucleotide_test()
				: cmoon::test::test_case{"dna_nucleotide_test"} {}

			void operator()() override
			{
				cmoon::test::assert_equal(static_cast<char>(cmoon::bio::dna_nucleotide::guanine), 'G');
				cmoon::test::assert_equal(static_cast<char>(cmoon::bio::dna_nucleotide::thymine), 'T');
				cmoon::test::assert_equal(static_cast<char>(cmoon::bio::dna_nucleotide::adenine), 'A');
				cmoon::test::assert_equal(static_cast<char>(cmoon::bio::dna_nucleotide::cytosine), 'C');

				std::stringstream ss;
				ss << cmoon::bio::dna_nucleotide::guanine;
				cmoon::test::assert_equal(ss.str(), "G");

				ss.str("");
				ss << cmoon::bio::dna_nucleotide::thymine;
				cmoon::test::assert_equal(ss.str(), "T");

				ss.str("");
				ss << cmoon::bio::dna_nucleotide::adenine;
				cmoon::test::assert_equal(ss.str(), "A");

				ss.str("");
				ss << cmoon::bio::dna_nucleotide::cytosine;
				cmoon::test::assert_equal(ss.str(), "C");

				// TODO: MSVC c++20 modules do not expose std::formatter specializations yet
				//cmoon::test::assert_equal(std::format("{}", cmoon::bio::dna_nucleotide::guanine), "G");
				//cmoon::test::assert_equal(std::format("{}", cmoon::bio::dna_nucleotide::thymine), "T");
				//cmoon::test::assert_equal(std::format("{}", cmoon::bio::dna_nucleotide::adenine), "A");
				//cmoon::test::assert_equal(std::format("{}", cmoon::bio::dna_nucleotide::cytosine), "C");
			}
	};

	export
	class amino_acid_test : public cmoon::test::test_case
	{
		public:
			amino_acid_test()
				: cmoon::test::test_case{"amino_acid_test"} {}

			void operator()() override
			{
				cmoon::test::assert_equal(static_cast<char>(cmoon::bio::amino_acid::alanine), 'A');
				cmoon::test::assert_equal(static_cast<char>(cmoon::bio::amino_acid::cysteine), 'C');
				cmoon::test::assert_equal(static_cast<char>(cmoon::bio::amino_acid::aspartic_acid), 'D');
				cmoon::test::assert_equal(static_cast<char>(cmoon::bio::amino_acid::gluatmic_acid), 'E');
				cmoon::test::assert_equal(static_cast<char>(cmoon::bio::amino_acid::phenylalanine), 'F');
				cmoon::test::assert_equal(static_cast<char>(cmoon::bio::amino_acid::histidine), 'H');
				cmoon::test::assert_equal(static_cast<char>(cmoon::bio::amino_acid::isoleucine), 'I');
				cmoon::test::assert_equal(static_cast<char>(cmoon::bio::amino_acid::lysine), 'K');
				cmoon::test::assert_equal(static_cast<char>(cmoon::bio::amino_acid::leucine), 'L');
				cmoon::test::assert_equal(static_cast<char>(cmoon::bio::amino_acid::methionine), 'M');
				cmoon::test::assert_equal(static_cast<char>(cmoon::bio::amino_acid::asparagine), 'N');
				cmoon::test::assert_equal(static_cast<char>(cmoon::bio::amino_acid::proline), 'P');
				cmoon::test::assert_equal(static_cast<char>(cmoon::bio::amino_acid::glutamine), 'Q');
				cmoon::test::assert_equal(static_cast<char>(cmoon::bio::amino_acid::arginine), 'R');
				cmoon::test::assert_equal(static_cast<char>(cmoon::bio::amino_acid::serine), 'S');
				cmoon::test::assert_equal(static_cast<char>(cmoon::bio::amino_acid::threonine), 'T');
				cmoon::test::assert_equal(static_cast<char>(cmoon::bio::amino_acid::valine), 'V');
				cmoon::test::assert_equal(static_cast<char>(cmoon::bio::amino_acid::tryptophan), 'W');
				cmoon::test::assert_equal(static_cast<char>(cmoon::bio::amino_acid::tyrosine), 'Y');
				cmoon::test::assert_equal(static_cast<char>(cmoon::bio::amino_acid::stop), '-');

				std::stringstream ss;
				ss << cmoon::bio::amino_acid::alanine;
				cmoon::test::assert_equal(ss.str(), "A");

				ss.str("");
				ss << cmoon::bio::amino_acid::cysteine;
				cmoon::test::assert_equal(ss.str(), "C");

				ss.str("");
				ss << cmoon::bio::amino_acid::aspartic_acid;
				cmoon::test::assert_equal(ss.str(), "D");

				ss.str("");
				ss << cmoon::bio::amino_acid::gluatmic_acid;
				cmoon::test::assert_equal(ss.str(), "E");

				ss.str("");
				ss << cmoon::bio::amino_acid::phenylalanine;
				cmoon::test::assert_equal(ss.str(), "F");

				ss.str("");
				ss << cmoon::bio::amino_acid::glycine;
				cmoon::test::assert_equal(ss.str(), "G");

				ss.str("");
				ss << cmoon::bio::amino_acid::histidine;
				cmoon::test::assert_equal(ss.str(), "H");

				ss.str("");
				ss << cmoon::bio::amino_acid::isoleucine;
				cmoon::test::assert_equal(ss.str(), "I");

				ss.str("");
				ss << cmoon::bio::amino_acid::lysine;
				cmoon::test::assert_equal(ss.str(), "K");

				ss.str("");
				ss << cmoon::bio::amino_acid::leucine;
				cmoon::test::assert_equal(ss.str(), "L");

				ss.str("");
				ss << cmoon::bio::amino_acid::methionine;
				cmoon::test::assert_equal(ss.str(), "M");

				ss.str("");
				ss << cmoon::bio::amino_acid::asparagine;
				cmoon::test::assert_equal(ss.str(), "N");

				ss.str("");
				ss << cmoon::bio::amino_acid::proline;
				cmoon::test::assert_equal(ss.str(), "P");

				ss.str("");
				ss << cmoon::bio::amino_acid::glutamine;
				cmoon::test::assert_equal(ss.str(), "Q");

				ss.str("");
				ss << cmoon::bio::amino_acid::arginine;
				cmoon::test::assert_equal(ss.str(), "R");

				ss.str("");
				ss << cmoon::bio::amino_acid::serine;
				cmoon::test::assert_equal(ss.str(), "S");

				ss.str("");
				ss << cmoon::bio::amino_acid::threonine;
				cmoon::test::assert_equal(ss.str(), "T");

				ss.str("");
				ss << cmoon::bio::amino_acid::valine;
				cmoon::test::assert_equal(ss.str(), "V");

				ss.str("");
				ss << cmoon::bio::amino_acid::tryptophan;
				cmoon::test::assert_equal(ss.str(), "W");

				ss.str("");
				ss << cmoon::bio::amino_acid::tyrosine;
				cmoon::test::assert_equal(ss.str(), "Y");

				ss.str("");
				ss << cmoon::bio::amino_acid::stop;
				cmoon::test::assert_equal(ss.str(), "-");

				// TODO: MSVC c++20 modules do not expose std::formatter specializations yet
				//cmoon::test::assert_equal(std::format("{}", cmoon::bio::amino_acid::alanine), "A");
				//cmoon::test::assert_equal(std::format("{}", cmoon::bio::amino_acid::cysteine), "C");
				//cmoon::test::assert_equal(std::format("{}", cmoon::bio::amino_acid::aspartic_acid), "D");
				//cmoon::test::assert_equal(std::format("{}", cmoon::bio::amino_acid::gluatmic_acid), "E");
				//cmoon::test::assert_equal(std::format("{}", cmoon::bio::amino_acid::phenylalanine), "F");
				//cmoon::test::assert_equal(std::format("{}", cmoon::bio::amino_acid::glycine), "G");
				//cmoon::test::assert_equal(std::format("{}", cmoon::bio::amino_acid::histidine), "H");
				//cmoon::test::assert_equal(std::format("{}", cmoon::bio::amino_acid::isoleucine), "I");
				//cmoon::test::assert_equal(std::format("{}", cmoon::bio::amino_acid::lysine), "K");
				//cmoon::test::assert_equal(std::format("{}", cmoon::bio::amino_acid::leucine), "L");
				//cmoon::test::assert_equal(std::format("{}", cmoon::bio::amino_acid::methionine), "M");
				//cmoon::test::assert_equal(std::format("{}", cmoon::bio::amino_acid::asparagine), "N");
				//cmoon::test::assert_equal(std::format("{}", cmoon::bio::amino_acid::proline), "P");
				//cmoon::test::assert_equal(std::format("{}", cmoon::bio::amino_acid::glutamine), "Q");
				//cmoon::test::assert_equal(std::format("{}", cmoon::bio::amino_acid::arginine), "R");
				//cmoon::test::assert_equal(std::format("{}", cmoon::bio::amino_acid::serine), "S");
				//cmoon::test::assert_equal(std::format("{}", cmoon::bio::amino_acid::threonine), "T");
				//cmoon::test::assert_equal(std::format("{}", cmoon::bio::amino_acid::valine), "V");
				//cmoon::test::assert_equal(std::format("{}", cmoon::bio::amino_acid::tryptophan), "W");
				//cmoon::test::assert_equal(std::format("{}", cmoon::bio::amino_acid::tyrosine), "Y");
				//cmoon::test::assert_equal(std::format("{}", cmoon::bio::amino_acid::stop), "-");
			}
	};

	export
	class nucleotide_convert_test : public cmoon::test::test_case
	{
		public:
			nucleotide_convert_test()
				: cmoon::test::test_case{"nucleotide_convert_test"} {}

			void operator()() override
			{
				cmoon::test::assert_equal(cmoon::bio::nucleotide_convert(cmoon::bio::rna_nucleotide::guanine), cmoon::bio::dna_nucleotide::guanine);
				cmoon::test::assert_equal(cmoon::bio::nucleotide_convert(cmoon::bio::rna_nucleotide::uracil), cmoon::bio::dna_nucleotide::thymine);
				cmoon::test::assert_equal(cmoon::bio::nucleotide_convert(cmoon::bio::rna_nucleotide::adenine), cmoon::bio::dna_nucleotide::adenine);
				cmoon::test::assert_equal(cmoon::bio::nucleotide_convert(cmoon::bio::rna_nucleotide::cytosine), cmoon::bio::dna_nucleotide::cytosine);

				static_assert(cmoon::bio::nucleotide_convert(cmoon::bio::rna_nucleotide::guanine) == cmoon::bio::dna_nucleotide::guanine);
				static_assert(cmoon::bio::nucleotide_convert(cmoon::bio::rna_nucleotide::uracil) == cmoon::bio::dna_nucleotide::thymine);
				static_assert(cmoon::bio::nucleotide_convert(cmoon::bio::rna_nucleotide::adenine) == cmoon::bio::dna_nucleotide::adenine);
				static_assert(cmoon::bio::nucleotide_convert(cmoon::bio::rna_nucleotide::cytosine) == cmoon::bio::dna_nucleotide::cytosine);

				cmoon::test::assert_equal(cmoon::bio::nucleotide_convert(cmoon::bio::dna_nucleotide::guanine), cmoon::bio::rna_nucleotide::guanine);
				cmoon::test::assert_equal(cmoon::bio::nucleotide_convert(cmoon::bio::dna_nucleotide::thymine), cmoon::bio::rna_nucleotide::uracil);
				cmoon::test::assert_equal(cmoon::bio::nucleotide_convert(cmoon::bio::dna_nucleotide::adenine), cmoon::bio::rna_nucleotide::adenine);
				cmoon::test::assert_equal(cmoon::bio::nucleotide_convert(cmoon::bio::dna_nucleotide::cytosine), cmoon::bio::rna_nucleotide::cytosine);

				static_assert(cmoon::bio::nucleotide_convert(cmoon::bio::dna_nucleotide::guanine) == cmoon::bio::rna_nucleotide::guanine);
				static_assert(cmoon::bio::nucleotide_convert(cmoon::bio::dna_nucleotide::thymine) == cmoon::bio::rna_nucleotide::uracil);
				static_assert(cmoon::bio::nucleotide_convert(cmoon::bio::dna_nucleotide::adenine) == cmoon::bio::rna_nucleotide::adenine);
				static_assert(cmoon::bio::nucleotide_convert(cmoon::bio::dna_nucleotide::cytosine) == cmoon::bio::rna_nucleotide::cytosine);
			}
	};

	export
	class nucleotide_compliment_test : public cmoon::test::test_case
	{
		public:
			nucleotide_compliment_test()
				: cmoon::test::test_case{"nucleotide_compliment_test"} {}

			void operator()() override
			{
				cmoon::test::assert_equal(cmoon::bio::nucleotide_compliment(cmoon::bio::rna_nucleotide::guanine), cmoon::bio::rna_nucleotide::cytosine);
				cmoon::test::assert_equal(cmoon::bio::nucleotide_compliment(cmoon::bio::rna_nucleotide::cytosine), cmoon::bio::rna_nucleotide::guanine);
				cmoon::test::assert_equal(cmoon::bio::nucleotide_compliment(cmoon::bio::rna_nucleotide::uracil), cmoon::bio::rna_nucleotide::adenine);
				cmoon::test::assert_equal(cmoon::bio::nucleotide_compliment(cmoon::bio::rna_nucleotide::adenine), cmoon::bio::rna_nucleotide::uracil);

				static_assert(cmoon::bio::nucleotide_compliment(cmoon::bio::rna_nucleotide::guanine) == cmoon::bio::rna_nucleotide::cytosine);
				static_assert(cmoon::bio::nucleotide_compliment(cmoon::bio::rna_nucleotide::cytosine) == cmoon::bio::rna_nucleotide::guanine);
				static_assert(cmoon::bio::nucleotide_compliment(cmoon::bio::rna_nucleotide::uracil) == cmoon::bio::rna_nucleotide::adenine);
				static_assert(cmoon::bio::nucleotide_compliment(cmoon::bio::rna_nucleotide::adenine) == cmoon::bio::rna_nucleotide::uracil);

				cmoon::test::assert_equal(cmoon::bio::nucleotide_compliment(cmoon::bio::dna_nucleotide::guanine), cmoon::bio::dna_nucleotide::cytosine);
				cmoon::test::assert_equal(cmoon::bio::nucleotide_compliment(cmoon::bio::dna_nucleotide::cytosine), cmoon::bio::dna_nucleotide::guanine);
				cmoon::test::assert_equal(cmoon::bio::nucleotide_compliment(cmoon::bio::dna_nucleotide::thymine), cmoon::bio::dna_nucleotide::adenine);
				cmoon::test::assert_equal(cmoon::bio::nucleotide_compliment(cmoon::bio::dna_nucleotide::adenine), cmoon::bio::dna_nucleotide::thymine);

				static_assert(cmoon::bio::nucleotide_compliment(cmoon::bio::dna_nucleotide::guanine) == cmoon::bio::dna_nucleotide::cytosine);
				static_assert(cmoon::bio::nucleotide_compliment(cmoon::bio::dna_nucleotide::cytosine) == cmoon::bio::dna_nucleotide::guanine);
				static_assert(cmoon::bio::nucleotide_compliment(cmoon::bio::dna_nucleotide::thymine) == cmoon::bio::dna_nucleotide::adenine);
				static_assert(cmoon::bio::nucleotide_compliment(cmoon::bio::dna_nucleotide::adenine) == cmoon::bio::dna_nucleotide::thymine);
			}
	};

	export
	class codon_constructor_test : public cmoon::test::test_case
	{
		public:
			codon_constructor_test()
				: cmoon::test::test_case{"codon_constructor_test"} {}

			void operator()() override
			{
				constexpr cmoon::bio::codon c1{cmoon::bio::rna_nucleotide::guanine,
											   cmoon::bio::rna_nucleotide::adenine,
											   cmoon::bio::rna_nucleotide::cytosine};

				constexpr cmoon::bio::codon c2{cmoon::bio::rna_nucleotide::uracil,
											   cmoon::bio::rna_nucleotide::uracil,
											   cmoon::bio::rna_nucleotide::uracil};

				constexpr cmoon::bio::codon c3{cmoon::bio::dna_nucleotide::guanine,
											   cmoon::bio::dna_nucleotide::adenine,
											   cmoon::bio::dna_nucleotide::cytosine};

				constexpr cmoon::bio::codon c4{cmoon::bio::dna_nucleotide::thymine,
											   cmoon::bio::dna_nucleotide::thymine,
											   cmoon::bio::dna_nucleotide::thymine};

				cmoon::test::assert_equal(c1.first, cmoon::bio::rna_nucleotide::guanine);
				cmoon::test::assert_equal(c1.second, cmoon::bio::rna_nucleotide::adenine);
				cmoon::test::assert_equal(c1.third, cmoon::bio::rna_nucleotide::cytosine);

				cmoon::test::assert_equal(c2.first, cmoon::bio::rna_nucleotide::uracil);
				cmoon::test::assert_equal(c2.second, cmoon::bio::rna_nucleotide::uracil);
				cmoon::test::assert_equal(c2.third, cmoon::bio::rna_nucleotide::uracil);

				cmoon::test::assert_equal(c3.first, cmoon::bio::rna_nucleotide::guanine);
				cmoon::test::assert_equal(c3.second, cmoon::bio::rna_nucleotide::adenine);
				cmoon::test::assert_equal(c3.third, cmoon::bio::rna_nucleotide::cytosine);

				cmoon::test::assert_equal(c4.first, cmoon::bio::rna_nucleotide::uracil);
				cmoon::test::assert_equal(c4.second, cmoon::bio::rna_nucleotide::uracil);
				cmoon::test::assert_equal(c4.third, cmoon::bio::rna_nucleotide::uracil);
			}
	};

	export
	class codon_amino_acid_test : public cmoon::test::test_case
	{
		public:
			codon_amino_acid_test()
				: cmoon::test::test_case{"codon_amino_acid_test"} {}

			void operator()() override
			{
				constexpr cmoon::bio::codon phenylalanine1{cmoon::bio::rna_nucleotide::uracil,
														   cmoon::bio::rna_nucleotide::uracil,
														   cmoon::bio::rna_nucleotide::uracil};

				constexpr cmoon::bio::codon phenylalanine2{cmoon::bio::rna_nucleotide::uracil,
														   cmoon::bio::rna_nucleotide::uracil,
														   cmoon::bio::rna_nucleotide::cytosine};

				constexpr cmoon::bio::codon leucine1{cmoon::bio::rna_nucleotide::uracil,
													 cmoon::bio::rna_nucleotide::uracil,
													 cmoon::bio::rna_nucleotide::adenine};

				constexpr cmoon::bio::codon leucine2{cmoon::bio::rna_nucleotide::uracil,
													 cmoon::bio::rna_nucleotide::uracil,
													 cmoon::bio::rna_nucleotide::guanine};

				constexpr cmoon::bio::codon leucine3{cmoon::bio::rna_nucleotide::cytosine,
													 cmoon::bio::rna_nucleotide::uracil,
													 cmoon::bio::rna_nucleotide::uracil};

				constexpr cmoon::bio::codon leucine4{cmoon::bio::rna_nucleotide::cytosine,
													 cmoon::bio::rna_nucleotide::uracil,
													 cmoon::bio::rna_nucleotide::cytosine};

				constexpr cmoon::bio::codon leucine5{cmoon::bio::rna_nucleotide::cytosine,
													 cmoon::bio::rna_nucleotide::uracil,
													 cmoon::bio::rna_nucleotide::adenine};

				constexpr cmoon::bio::codon leucine6{cmoon::bio::rna_nucleotide::cytosine,
													 cmoon::bio::rna_nucleotide::uracil,
													 cmoon::bio::rna_nucleotide::guanine};

				constexpr cmoon::bio::codon isoleucine1{cmoon::bio::rna_nucleotide::adenine,
														cmoon::bio::rna_nucleotide::uracil,
														cmoon::bio::rna_nucleotide::uracil};

				constexpr cmoon::bio::codon isoleucine2{cmoon::bio::rna_nucleotide::adenine,
														cmoon::bio::rna_nucleotide::uracil,
														cmoon::bio::rna_nucleotide::cytosine};

				constexpr cmoon::bio::codon isoleucine3{cmoon::bio::rna_nucleotide::adenine,
														cmoon::bio::rna_nucleotide::uracil,
														cmoon::bio::rna_nucleotide::adenine};

				constexpr cmoon::bio::codon methionine1{cmoon::bio::rna_nucleotide::adenine,
														cmoon::bio::rna_nucleotide::uracil,
														cmoon::bio::rna_nucleotide::guanine};

				constexpr cmoon::bio::codon valine1{cmoon::bio::rna_nucleotide::guanine,
													cmoon::bio::rna_nucleotide::uracil,
													cmoon::bio::rna_nucleotide::uracil};

				constexpr cmoon::bio::codon valine2{cmoon::bio::rna_nucleotide::guanine,
													cmoon::bio::rna_nucleotide::uracil,
													cmoon::bio::rna_nucleotide::cytosine};

				constexpr cmoon::bio::codon valine3{cmoon::bio::rna_nucleotide::guanine,
													cmoon::bio::rna_nucleotide::uracil,
													cmoon::bio::rna_nucleotide::adenine};

				constexpr cmoon::bio::codon valine4{cmoon::bio::rna_nucleotide::guanine,
													cmoon::bio::rna_nucleotide::uracil,
													cmoon::bio::rna_nucleotide::guanine};

				constexpr cmoon::bio::codon serine1{cmoon::bio::rna_nucleotide::uracil,
													cmoon::bio::rna_nucleotide::cytosine,
													cmoon::bio::rna_nucleotide::uracil};

				constexpr cmoon::bio::codon serine2{cmoon::bio::rna_nucleotide::uracil,
													cmoon::bio::rna_nucleotide::cytosine,
													cmoon::bio::rna_nucleotide::cytosine};

				constexpr cmoon::bio::codon serine3{cmoon::bio::rna_nucleotide::uracil,
													cmoon::bio::rna_nucleotide::cytosine,
													cmoon::bio::rna_nucleotide::adenine};

				constexpr cmoon::bio::codon serine4{cmoon::bio::rna_nucleotide::uracil,
													cmoon::bio::rna_nucleotide::cytosine,
													cmoon::bio::rna_nucleotide::guanine};

				constexpr cmoon::bio::codon serine5{cmoon::bio::rna_nucleotide::adenine,
													cmoon::bio::rna_nucleotide::guanine,
													cmoon::bio::rna_nucleotide::uracil};

				constexpr cmoon::bio::codon serine6{cmoon::bio::rna_nucleotide::adenine,
													cmoon::bio::rna_nucleotide::guanine,
													cmoon::bio::rna_nucleotide::cytosine};

				constexpr cmoon::bio::codon proline1{cmoon::bio::rna_nucleotide::cytosine,
													 cmoon::bio::rna_nucleotide::cytosine,
													 cmoon::bio::rna_nucleotide::uracil};

				constexpr cmoon::bio::codon proline2{cmoon::bio::rna_nucleotide::cytosine,
													 cmoon::bio::rna_nucleotide::cytosine,
													 cmoon::bio::rna_nucleotide::cytosine};

				constexpr cmoon::bio::codon proline3{cmoon::bio::rna_nucleotide::cytosine,
													 cmoon::bio::rna_nucleotide::cytosine,
													 cmoon::bio::rna_nucleotide::adenine};

				constexpr cmoon::bio::codon proline4{cmoon::bio::rna_nucleotide::cytosine,
													 cmoon::bio::rna_nucleotide::cytosine,
													 cmoon::bio::rna_nucleotide::guanine};

				constexpr cmoon::bio::codon threonine1{cmoon::bio::rna_nucleotide::adenine,
													   cmoon::bio::rna_nucleotide::cytosine,
													   cmoon::bio::rna_nucleotide::uracil};

				constexpr cmoon::bio::codon threonine2{cmoon::bio::rna_nucleotide::adenine,
													   cmoon::bio::rna_nucleotide::cytosine,
													   cmoon::bio::rna_nucleotide::cytosine};

				constexpr cmoon::bio::codon threonine3{cmoon::bio::rna_nucleotide::adenine,
													   cmoon::bio::rna_nucleotide::cytosine,
													   cmoon::bio::rna_nucleotide::adenine};

				constexpr cmoon::bio::codon threonine4{cmoon::bio::rna_nucleotide::adenine,
													   cmoon::bio::rna_nucleotide::cytosine,
													   cmoon::bio::rna_nucleotide::guanine};

				constexpr cmoon::bio::codon alanine1{cmoon::bio::rna_nucleotide::guanine,
													 cmoon::bio::rna_nucleotide::cytosine,
													 cmoon::bio::rna_nucleotide::uracil};

				constexpr cmoon::bio::codon alanine2{cmoon::bio::rna_nucleotide::guanine,
													 cmoon::bio::rna_nucleotide::cytosine,
													 cmoon::bio::rna_nucleotide::cytosine};

				constexpr cmoon::bio::codon alanine3{cmoon::bio::rna_nucleotide::guanine,
													 cmoon::bio::rna_nucleotide::cytosine,
													 cmoon::bio::rna_nucleotide::adenine};

				constexpr cmoon::bio::codon alanine4{cmoon::bio::rna_nucleotide::guanine,
													 cmoon::bio::rna_nucleotide::cytosine,
													 cmoon::bio::rna_nucleotide::guanine};

				constexpr cmoon::bio::codon tyrosine1{cmoon::bio::rna_nucleotide::uracil,
													  cmoon::bio::rna_nucleotide::adenine,
													  cmoon::bio::rna_nucleotide::uracil};

				constexpr cmoon::bio::codon tyrosine2{cmoon::bio::rna_nucleotide::uracil,
													  cmoon::bio::rna_nucleotide::adenine,
													  cmoon::bio::rna_nucleotide::cytosine};

				constexpr cmoon::bio::codon stop1{cmoon::bio::rna_nucleotide::uracil,
												  cmoon::bio::rna_nucleotide::adenine,
												  cmoon::bio::rna_nucleotide::adenine};

				constexpr cmoon::bio::codon stop2{cmoon::bio::rna_nucleotide::uracil,
												  cmoon::bio::rna_nucleotide::adenine,
												  cmoon::bio::rna_nucleotide::guanine};

				constexpr cmoon::bio::codon stop3{cmoon::bio::rna_nucleotide::uracil,
												  cmoon::bio::rna_nucleotide::guanine,
												  cmoon::bio::rna_nucleotide::adenine};

				constexpr cmoon::bio::codon histidine1{cmoon::bio::rna_nucleotide::cytosine,
													   cmoon::bio::rna_nucleotide::adenine,
													   cmoon::bio::rna_nucleotide::uracil};

				constexpr cmoon::bio::codon histidine2{cmoon::bio::rna_nucleotide::cytosine,
													   cmoon::bio::rna_nucleotide::adenine,
													   cmoon::bio::rna_nucleotide::cytosine};

				constexpr cmoon::bio::codon glutamine1{cmoon::bio::rna_nucleotide::cytosine,
													   cmoon::bio::rna_nucleotide::adenine,
													   cmoon::bio::rna_nucleotide::adenine};

				constexpr cmoon::bio::codon glutamine2{cmoon::bio::rna_nucleotide::cytosine,
													   cmoon::bio::rna_nucleotide::adenine,
													   cmoon::bio::rna_nucleotide::guanine};

				constexpr cmoon::bio::codon asparagine1{cmoon::bio::rna_nucleotide::adenine,
													    cmoon::bio::rna_nucleotide::adenine,
													    cmoon::bio::rna_nucleotide::uracil};

				constexpr cmoon::bio::codon asparagine2{cmoon::bio::rna_nucleotide::adenine,
													    cmoon::bio::rna_nucleotide::adenine,
													    cmoon::bio::rna_nucleotide::cytosine};

				constexpr cmoon::bio::codon lysine1{cmoon::bio::rna_nucleotide::adenine,
													cmoon::bio::rna_nucleotide::adenine,
													cmoon::bio::rna_nucleotide::adenine};

				constexpr cmoon::bio::codon lysine2{cmoon::bio::rna_nucleotide::adenine,
													cmoon::bio::rna_nucleotide::adenine,
													cmoon::bio::rna_nucleotide::guanine};

				constexpr cmoon::bio::codon aspartic_acid1{cmoon::bio::rna_nucleotide::guanine,
														   cmoon::bio::rna_nucleotide::adenine,
														   cmoon::bio::rna_nucleotide::uracil};

				constexpr cmoon::bio::codon aspartic_acid2{cmoon::bio::rna_nucleotide::guanine,
														   cmoon::bio::rna_nucleotide::adenine,
														   cmoon::bio::rna_nucleotide::cytosine};

				constexpr cmoon::bio::codon gluatmic_acid1{cmoon::bio::rna_nucleotide::guanine,
														   cmoon::bio::rna_nucleotide::adenine,
														   cmoon::bio::rna_nucleotide::adenine};

				constexpr cmoon::bio::codon gluatmic_acid2{cmoon::bio::rna_nucleotide::guanine,
														   cmoon::bio::rna_nucleotide::adenine,
														   cmoon::bio::rna_nucleotide::guanine};

				constexpr cmoon::bio::codon cysteine1{cmoon::bio::rna_nucleotide::uracil,
													  cmoon::bio::rna_nucleotide::guanine,
													  cmoon::bio::rna_nucleotide::uracil};

				constexpr cmoon::bio::codon cysteine2{cmoon::bio::rna_nucleotide::uracil,
													  cmoon::bio::rna_nucleotide::guanine,
													  cmoon::bio::rna_nucleotide::cytosine};

				constexpr cmoon::bio::codon tryptophan1{cmoon::bio::rna_nucleotide::uracil,
														cmoon::bio::rna_nucleotide::guanine,
														cmoon::bio::rna_nucleotide::guanine};

				constexpr cmoon::bio::codon arginine1{cmoon::bio::rna_nucleotide::cytosine,
													  cmoon::bio::rna_nucleotide::guanine,
													  cmoon::bio::rna_nucleotide::uracil};

				constexpr cmoon::bio::codon arginine2{cmoon::bio::rna_nucleotide::cytosine,
													  cmoon::bio::rna_nucleotide::guanine,
													  cmoon::bio::rna_nucleotide::cytosine};

				constexpr cmoon::bio::codon arginine3{cmoon::bio::rna_nucleotide::cytosine,
													  cmoon::bio::rna_nucleotide::guanine,
													  cmoon::bio::rna_nucleotide::adenine};

				constexpr cmoon::bio::codon arginine4{cmoon::bio::rna_nucleotide::cytosine,
													  cmoon::bio::rna_nucleotide::guanine,
													  cmoon::bio::rna_nucleotide::guanine};

				constexpr cmoon::bio::codon arginine5{cmoon::bio::rna_nucleotide::adenine,
													  cmoon::bio::rna_nucleotide::guanine,
													  cmoon::bio::rna_nucleotide::adenine};

				constexpr cmoon::bio::codon arginine6{cmoon::bio::rna_nucleotide::adenine,
													  cmoon::bio::rna_nucleotide::guanine,
													  cmoon::bio::rna_nucleotide::guanine};

				constexpr cmoon::bio::codon glycine1{cmoon::bio::rna_nucleotide::guanine,
													 cmoon::bio::rna_nucleotide::guanine,
													 cmoon::bio::rna_nucleotide::uracil};

				constexpr cmoon::bio::codon glycine2{cmoon::bio::rna_nucleotide::guanine,
													 cmoon::bio::rna_nucleotide::guanine,
													 cmoon::bio::rna_nucleotide::cytosine};

				constexpr cmoon::bio::codon glycine3{cmoon::bio::rna_nucleotide::guanine,
													 cmoon::bio::rna_nucleotide::guanine,
													 cmoon::bio::rna_nucleotide::adenine};

				constexpr cmoon::bio::codon glycine4{cmoon::bio::rna_nucleotide::guanine,
													 cmoon::bio::rna_nucleotide::guanine,
													 cmoon::bio::rna_nucleotide::guanine};

				cmoon::test::assert_equal(phenylalanine1.to_amino_acid(), cmoon::bio::amino_acid::phenylalanine);
				cmoon::test::assert_equal(phenylalanine2.to_amino_acid(), cmoon::bio::amino_acid::phenylalanine);
				cmoon::test::assert_equal(leucine1.to_amino_acid(), cmoon::bio::amino_acid::leucine);
				cmoon::test::assert_equal(leucine2.to_amino_acid(), cmoon::bio::amino_acid::leucine);
				cmoon::test::assert_equal(leucine3.to_amino_acid(), cmoon::bio::amino_acid::leucine);
				cmoon::test::assert_equal(leucine4.to_amino_acid(), cmoon::bio::amino_acid::leucine);
				cmoon::test::assert_equal(leucine5.to_amino_acid(), cmoon::bio::amino_acid::leucine);
				cmoon::test::assert_equal(leucine6.to_amino_acid(), cmoon::bio::amino_acid::leucine);
				cmoon::test::assert_equal(isoleucine1.to_amino_acid(), cmoon::bio::amino_acid::isoleucine);
				cmoon::test::assert_equal(isoleucine2.to_amino_acid(), cmoon::bio::amino_acid::isoleucine);
				cmoon::test::assert_equal(isoleucine3.to_amino_acid(), cmoon::bio::amino_acid::isoleucine);
				cmoon::test::assert_equal(methionine1.to_amino_acid(), cmoon::bio::amino_acid::methionine);
				cmoon::test::assert_equal(valine1.to_amino_acid(), cmoon::bio::amino_acid::valine);
				cmoon::test::assert_equal(valine2.to_amino_acid(), cmoon::bio::amino_acid::valine);
				cmoon::test::assert_equal(valine3.to_amino_acid(), cmoon::bio::amino_acid::valine);
				cmoon::test::assert_equal(valine4.to_amino_acid(), cmoon::bio::amino_acid::valine);
				cmoon::test::assert_equal(serine1.to_amino_acid(), cmoon::bio::amino_acid::serine);
				cmoon::test::assert_equal(serine2.to_amino_acid(), cmoon::bio::amino_acid::serine);
				cmoon::test::assert_equal(serine3.to_amino_acid(), cmoon::bio::amino_acid::serine);
				cmoon::test::assert_equal(serine4.to_amino_acid(), cmoon::bio::amino_acid::serine);
				cmoon::test::assert_equal(serine5.to_amino_acid(), cmoon::bio::amino_acid::serine);
				cmoon::test::assert_equal(serine6.to_amino_acid(), cmoon::bio::amino_acid::serine);
				cmoon::test::assert_equal(proline1.to_amino_acid(), cmoon::bio::amino_acid::proline);
				cmoon::test::assert_equal(proline2.to_amino_acid(), cmoon::bio::amino_acid::proline);
				cmoon::test::assert_equal(proline3.to_amino_acid(), cmoon::bio::amino_acid::proline);
				cmoon::test::assert_equal(proline4.to_amino_acid(), cmoon::bio::amino_acid::proline);
				cmoon::test::assert_equal(threonine1.to_amino_acid(), cmoon::bio::amino_acid::threonine);
				cmoon::test::assert_equal(threonine2.to_amino_acid(), cmoon::bio::amino_acid::threonine);
				cmoon::test::assert_equal(threonine3.to_amino_acid(), cmoon::bio::amino_acid::threonine);
				cmoon::test::assert_equal(threonine4.to_amino_acid(), cmoon::bio::amino_acid::threonine);
				cmoon::test::assert_equal(alanine1.to_amino_acid(), cmoon::bio::amino_acid::alanine);
				cmoon::test::assert_equal(alanine2.to_amino_acid(), cmoon::bio::amino_acid::alanine);
				cmoon::test::assert_equal(alanine3.to_amino_acid(), cmoon::bio::amino_acid::alanine);
				cmoon::test::assert_equal(alanine4.to_amino_acid(), cmoon::bio::amino_acid::alanine);
				cmoon::test::assert_equal(tyrosine1.to_amino_acid(), cmoon::bio::amino_acid::tyrosine);
				cmoon::test::assert_equal(tyrosine2.to_amino_acid(), cmoon::bio::amino_acid::tyrosine);
				cmoon::test::assert_equal(stop1.to_amino_acid(), cmoon::bio::amino_acid::stop);
				cmoon::test::assert_equal(stop2.to_amino_acid(), cmoon::bio::amino_acid::stop);
				cmoon::test::assert_equal(stop3.to_amino_acid(), cmoon::bio::amino_acid::stop);
				cmoon::test::assert_equal(histidine1.to_amino_acid(), cmoon::bio::amino_acid::histidine);
				cmoon::test::assert_equal(histidine2.to_amino_acid(), cmoon::bio::amino_acid::histidine);
				cmoon::test::assert_equal(glutamine1.to_amino_acid(), cmoon::bio::amino_acid::glutamine);
				cmoon::test::assert_equal(glutamine2.to_amino_acid(), cmoon::bio::amino_acid::glutamine);
				cmoon::test::assert_equal(asparagine1.to_amino_acid(), cmoon::bio::amino_acid::asparagine);
				cmoon::test::assert_equal(asparagine2.to_amino_acid(), cmoon::bio::amino_acid::asparagine);
				cmoon::test::assert_equal(lysine1.to_amino_acid(), cmoon::bio::amino_acid::lysine);
				cmoon::test::assert_equal(lysine2.to_amino_acid(), cmoon::bio::amino_acid::lysine);
				cmoon::test::assert_equal(aspartic_acid1.to_amino_acid(), cmoon::bio::amino_acid::aspartic_acid);
				cmoon::test::assert_equal(aspartic_acid2.to_amino_acid(), cmoon::bio::amino_acid::aspartic_acid);
				cmoon::test::assert_equal(gluatmic_acid1.to_amino_acid(), cmoon::bio::amino_acid::gluatmic_acid);
				cmoon::test::assert_equal(gluatmic_acid2.to_amino_acid(), cmoon::bio::amino_acid::gluatmic_acid);
				cmoon::test::assert_equal(cysteine1.to_amino_acid(), cmoon::bio::amino_acid::cysteine);
				cmoon::test::assert_equal(cysteine2.to_amino_acid(), cmoon::bio::amino_acid::cysteine);
				cmoon::test::assert_equal(tryptophan1.to_amino_acid(), cmoon::bio::amino_acid::tryptophan);
				cmoon::test::assert_equal(arginine1.to_amino_acid(), cmoon::bio::amino_acid::arginine);
				cmoon::test::assert_equal(arginine2.to_amino_acid(), cmoon::bio::amino_acid::arginine);
				cmoon::test::assert_equal(arginine3.to_amino_acid(), cmoon::bio::amino_acid::arginine);
				cmoon::test::assert_equal(arginine4.to_amino_acid(), cmoon::bio::amino_acid::arginine);
				cmoon::test::assert_equal(arginine5.to_amino_acid(), cmoon::bio::amino_acid::arginine);
				cmoon::test::assert_equal(arginine6.to_amino_acid(), cmoon::bio::amino_acid::arginine);
				cmoon::test::assert_equal(glycine1.to_amino_acid(), cmoon::bio::amino_acid::glycine);
				cmoon::test::assert_equal(glycine2.to_amino_acid(), cmoon::bio::amino_acid::glycine);
				cmoon::test::assert_equal(glycine3.to_amino_acid(), cmoon::bio::amino_acid::glycine);
				cmoon::test::assert_equal(glycine4.to_amino_acid(), cmoon::bio::amino_acid::glycine);
			}
	};
}
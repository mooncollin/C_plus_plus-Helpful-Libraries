export module cmoon.tests.bio;
export import cmoon.tests.bio.nucleotide;
export import cmoon.tests.bio.rna_sequence;
export import cmoon.tests.bio.dna_sequence;

import <utility>;

import cmoon.test;

import cmoon.tests;

namespace cmoon::tests
{
	export
	template<>
	cmoon::test::test_suite get_test_suite<library::bio>()
	{
		cmoon::test::test_suite suite{"Bio library tests"};
		suite.add_test_case<bio::rna_nucleotide_test>();
		suite.add_test_case<bio::dna_nucleotide_test>();
		suite.add_test_case<bio::nucleotide_convert_test>();
		suite.add_test_case<bio::nucleotide_compliment_test>();
		suite.add_test_case<bio::amino_acid_test>();
		suite.add_test_case<bio::codon_constructor_test>();
		suite.add_test_case<bio::codon_amino_acid_test>();
		suite.add_test_case<bio::rna_constructor_test>();
		suite.add_test_case<bio::rna_to_dna_test>();
		suite.add_test_case<bio::dna_constructor_test>();
		suite.add_test_case<bio::dna_to_rna_test>();

		return std::move(suite);
	}
}
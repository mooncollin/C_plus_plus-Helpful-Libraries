export module cmoon.tests.bio.rna_sequence;

import <sstream>;
import <format>;

import cmoon.concepts;
import cmoon.test;
import cmoon.bio;

namespace cmoon::tests::bio
{
	export
	class rna_constructor_test : public cmoon::test::test_case
	{
		public:
			rna_constructor_test()
				: cmoon::test::test_case{"rna_constructor_test"} {}

			void operator()() override
			{
				cmoon::bio::rna_sequence rs {cmoon::bio::rna_nucleotide::guanine, 
											 cmoon::bio::rna_nucleotide::guanine,
											 cmoon::bio::rna_nucleotide::adenine,
											 cmoon::bio::rna_nucleotide::guanine,
											 cmoon::bio::rna_nucleotide::cytosine,
											 cmoon::bio::rna_nucleotide::uracil};

				cmoon::test::assert_equal(rs[0], cmoon::bio::rna_nucleotide::guanine);
				cmoon::test::assert_equal(rs[1], cmoon::bio::rna_nucleotide::guanine);
				cmoon::test::assert_equal(rs[2], cmoon::bio::rna_nucleotide::adenine);
				cmoon::test::assert_equal(rs[3], cmoon::bio::rna_nucleotide::guanine);
				cmoon::test::assert_equal(rs[4], cmoon::bio::rna_nucleotide::cytosine);
				cmoon::test::assert_equal(rs[5], cmoon::bio::rna_nucleotide::uracil);
			}
	};

	export
	class rna_to_dna_test : public cmoon::test::test_case
	{
		public:
			rna_to_dna_test()
				: cmoon::test::test_case{"rna_to_dna_test"} {}

			void operator()() override
			{
				cmoon::bio::rna_sequence rs {cmoon::bio::rna_nucleotide::guanine,
											 cmoon::bio::rna_nucleotide::guanine,
											 cmoon::bio::rna_nucleotide::adenine,
											 cmoon::bio::rna_nucleotide::guanine,
											 cmoon::bio::rna_nucleotide::cytosine,
											 cmoon::bio::rna_nucleotide::uracil};

				const auto ds = rs.to_dna_sequence();

				cmoon::test::assert_equal(ds[0], cmoon::bio::dna_nucleotide::guanine);
				cmoon::test::assert_equal(ds[1], cmoon::bio::dna_nucleotide::guanine);
				cmoon::test::assert_equal(ds[2], cmoon::bio::dna_nucleotide::adenine);
				cmoon::test::assert_equal(ds[3], cmoon::bio::dna_nucleotide::guanine);
				cmoon::test::assert_equal(ds[4], cmoon::bio::dna_nucleotide::cytosine);
				cmoon::test::assert_equal(ds[5], cmoon::bio::dna_nucleotide::thymine);
			}
	};
}
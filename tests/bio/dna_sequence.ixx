export module cmoon.tests.bio.dna_sequence;

import cmoon.test;
import cmoon.bio;

namespace cmoon::tests::bio
{
	export
	class dna_constructor_test : public cmoon::test::test_case
	{
		public:
			dna_constructor_test()
				: cmoon::test::test_case{"dna_constructor_test"} {}

			void operator()() override
			{
				cmoon::bio::dna_sequence ds {cmoon::bio::dna_nucleotide::guanine, 
											 cmoon::bio::dna_nucleotide::guanine,
											 cmoon::bio::dna_nucleotide::adenine,
											 cmoon::bio::dna_nucleotide::guanine,
											 cmoon::bio::dna_nucleotide::cytosine,
											 cmoon::bio::dna_nucleotide::thymine};

				cmoon::test::assert_equal(ds[0], cmoon::bio::dna_nucleotide::guanine);
				cmoon::test::assert_equal(ds[1], cmoon::bio::dna_nucleotide::guanine);
				cmoon::test::assert_equal(ds[2], cmoon::bio::dna_nucleotide::adenine);
				cmoon::test::assert_equal(ds[3], cmoon::bio::dna_nucleotide::guanine);
				cmoon::test::assert_equal(ds[4], cmoon::bio::dna_nucleotide::cytosine);
				cmoon::test::assert_equal(ds[5], cmoon::bio::dna_nucleotide::thymine);
			}
	};

	export
	class dna_to_rna_test : public cmoon::test::test_case
	{
		public:
			dna_to_rna_test()
				: cmoon::test::test_case {"dna_to_rna_test"} {}

			void operator()() override
			{
				cmoon::bio::dna_sequence ds {cmoon::bio::dna_nucleotide::guanine, 
											 cmoon::bio::dna_nucleotide::guanine,
											 cmoon::bio::dna_nucleotide::adenine,
											 cmoon::bio::dna_nucleotide::guanine,
											 cmoon::bio::dna_nucleotide::cytosine,
											 cmoon::bio::dna_nucleotide::thymine};

				const auto rs = ds.to_rna_sequence();

				cmoon::test::assert_equal(rs[0], cmoon::bio::rna_nucleotide::guanine);
				cmoon::test::assert_equal(rs[1], cmoon::bio::rna_nucleotide::guanine);
				cmoon::test::assert_equal(rs[2], cmoon::bio::rna_nucleotide::adenine);
				cmoon::test::assert_equal(rs[3], cmoon::bio::rna_nucleotide::guanine);
				cmoon::test::assert_equal(rs[4], cmoon::bio::rna_nucleotide::cytosine);
				cmoon::test::assert_equal(rs[5], cmoon::bio::rna_nucleotide::uracil);
			}
	};
}
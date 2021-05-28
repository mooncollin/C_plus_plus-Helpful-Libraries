import <iostream>;

import cmoon.test;
import cmoon.bio;











int main()
{
	cmoon::test::test_suite suite;
	suite.add_test_case<nucleotide_convert_test>();
	suite.add_test_case<nucleotide_compliment_test>();
	suite.add_test_case<rna_constructor_test>();
	suite.add_test_case<dna_constructor_test>();
	suite.add_test_case<rna_to_dna_test>();
	suite.add_test_case<dna_to_rna_test>();

	cmoon::test::text_test_runner runner{std::cout};

	return !runner.run(suite);
}
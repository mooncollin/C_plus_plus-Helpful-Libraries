#include <iostream>

#include "collin/bio/rna.hpp"
#include "collin/bio/dna.hpp"
#include "collin/bio/nucleotide.hpp"

int main()
{
	collin::bio::rna rs {collin::bio::rna_nucleotide::guanine, 
						 collin::bio::rna_nucleotide::guanine,
						 collin::bio::rna_nucleotide::adenine,
						 collin::bio::rna_nucleotide::guanine,
						 collin::bio::rna_nucleotide::cytosine,
						 collin::bio::rna_nucleotide::uracil};

	std::cout << rs << '\n';
	std::cout << rs.to_dna() << '\n';
	std::cout << rs.to_dna().to_rna() << '\n';
}
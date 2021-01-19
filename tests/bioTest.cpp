#include <iostream>

#include "cmoon/bio/rna.hpp"
#include "cmoon/bio/dna.hpp"
#include "cmoon/bio/nucleotide.hpp"

int main()
{
	cmoon::bio::rna rs {cmoon::bio::rna_nucleotide::guanine, 
						 cmoon::bio::rna_nucleotide::guanine,
						 cmoon::bio::rna_nucleotide::adenine,
						 cmoon::bio::rna_nucleotide::guanine,
						 cmoon::bio::rna_nucleotide::cytosine,
						 cmoon::bio::rna_nucleotide::uracil};

	std::cout << rs << '\n';
	std::cout << rs.to_dna() << '\n';
	std::cout << rs.to_dna().to_rna() << '\n';
}
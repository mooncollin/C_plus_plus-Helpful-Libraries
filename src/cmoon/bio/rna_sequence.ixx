export module cmoon.bio.rna_sequence;

import <memory>;
import <vector>;
import <iterator>;
import <iostream>;

import cmoon.bio.nucleotide;

namespace cmoon::bio
{
	export
	template<class Allocator = std::allocator<rna_nucleotide>>
	class rna_sequence
	{
		using storage_t = std::vector<rna_nucleotide, Allocator>;

		public:
			using value_type = typename storage_t::value_type;
			using allocator_type = typename storage_t::allocator_type;
			using size_type = typename storage_t::size_type;
			using difference_type = typename storage_t::difference_type;
			using reference = typename storage_t::reference;
			using const_reference = typename storage_t::const_reference;
			using pointer = typename storage_t::pointer;
			using const_pointer = typename storage_t::const_pointer;
			using iterator = typename storage_t::iterator;
			using const_iterator = typename storage_t::const_iterator;
			using reverse_iterator = typename storage_t::reverse_iterator;
			using const_reverse_iterator = typename storage_t::const_reverse_iterator;

			rna_sequence(const allocator_type& = allocator_type{});

			rna_sequence(const rna_sequence&) = default;
			rna_sequence(rna_sequence&&) noexcept = default;

			rna_sequence& operator=(const rna_sequence&) = default;
			rna_sequence& operator=(rna_sequence&&) noexcept = default;

			rna_sequence& operator=(std::initializer_list<value_type>);

			template<std::input_iterator I>
			rna_sequence(I, I, const allocator_type& = allocator_type{});

			rna_sequence(std::initializer_list<value_type>, const allocator_type& = allocator_type{});

			void assign(size_type, const_reference);

			template<std::input_iterator I>
			void assign(I, I);

			void assign(std::initializer_list<value_type>);

			allocator_type get_allocator() const noexcept;

			[[nodiscard]] const_reference operator[](size_type) const noexcept;
			[[nodiscard]] reference operator[](size_type) noexcept;
			[[nodiscard]] const_reference at(size_type) const;
			[[nodiscard]] reference at(std::size_t);
			reference front();
			const_reference front() const;
			reference back();
			const_reference back() const;
			[[nodiscard]] pointer data() noexcept;
			[[nodiscard]] const_pointer data() const noexcept;
			iterator begin() noexcept;
			const_iterator begin() const noexcept;
			const_iterator cbegin() const noexcept;
			iterator end() noexcept;
			const_iterator end() const noexcept;
			const_iterator cend() const noexcept;
			reverse_iterator rbegin() noexcept;
			const_reverse_iterator rbegin() const noexcept;
			const_reverse_iterator crbegin() const noexcept;
			reverse_iterator rend() noexcept;
			const_reverse_iterator rend() const noexcept;
			const_reverse_iterator crend() const noexcept;
			[[nodiscard]] bool empty() const noexcept;
			[[nodiscard]] size_type size() const noexcept;
			void clear() noexcept;
			iterator insert(const_iterator pos, const_reference value);
			iterator insert(const_iterator pos, value_type&& value);
			iterator insert(const_iterator pos, size_type count, const_reference value);

			template<std::input_iterator I>
			iterator insert(const_iterator pos, I first, I last);

			iterator insert(const_iterator pos, std::initializer_list<value_type>);

			iterator erase(const_iterator pos);
			iterator erase(const_iterator first, const_iterator last);
			void push_back(const_reference value);
			void pop_back();
			void swap(rna_sequence&) noexcept(noexcept(std::declval<storage_t>().swap(std::declval<storage_t>())));

			[[nodiscard]] storage_t& sequence() noexcept;
			[[nodiscard]] const storage_t& sequence() const noexcept;

			[[nodiscard]] auto to_dna_sequence() const;
			[[nodiscard]] rna_sequence compliment() const;
			[[nodiscard]] auto dna_compliment() const;
			[[nodiscard]] codon get_codon(size_type, bool reverse = false) const noexcept;
		private:
			storage_t sequence_;
	};

	template<class CharT, class Traits, class Allocator>
	std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const rna_sequence<Allocator>& s);

	template<class CharT, class Traits, class Allocator>
	std::basic_istream<CharT, Traits>& operator>>(std::basic_istream<CharT, Traits>& is, rna_sequence<Allocator>& s);

	template<class Allocator, std::output_iterator<amino_acid> OutputIterator>
	void amino_acid_sequence(const rna_sequence<Allocator>& r, OutputIterator out, int reading_frame = 1) noexcept;
}

namespace std
{
	export
	template<class Allocator>
	struct formatter<cmoon::bio::rna_sequence<Allocator>> : public formatter<std::string_view>;
}
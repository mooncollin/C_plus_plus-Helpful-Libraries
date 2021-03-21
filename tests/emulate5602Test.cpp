#include <iostream>

#include "cmoon/emulation/6502/cpu.hpp"
#include "cmoon/emulation/6502/memory.hpp"

#include "cmoon/test/assert.hpp"
#include "cmoon/test/runner.hpp"
#include "cmoon/test/test_case.hpp"
#include "cmoon/test/test_suite.hpp"

class M6502_test1 : public cmoon::test::test_case
{
	public:
		M6502_test1()
			: cmoon::test::test_case{"M6502_test1"} {}

		void operator()() override
		{
			mem[0xFFFC] = static_cast<std::uint8_t>(cmoon::emulate::CPU_6502::instruction::jsr);
			mem[0xFFFD] = 0x42;
			mem[0xFFFE] = 0x42;
			mem[0x4242] = static_cast<std::uint8_t>(cmoon::emulate::CPU_6502::instruction::lda_im);
			mem[0x4243] = 0x84;
			cpu.execute(9, mem);

			cmoon::test::assert_equal(cpu.A, 0x84);
		}
	private:
		cmoon::emulate::CPU_6502 cpu;
		cmoon::emulate::memory_6502 mem;
};

int main()
{
	
	cmoon::test::test_suite suite;
	suite.add_test_case<M6502_test1>();

	cmoon::test::text_test_runner runner{std::cout};

	return !runner.run(suite);
}
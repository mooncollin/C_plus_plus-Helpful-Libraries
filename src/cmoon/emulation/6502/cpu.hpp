#pragma once

#include <cstdint>
#include <cstddef>
#include <array>
#include <initializer_list>
#include <cassert>
#include <functional>

#include "cmoon/emulation/6502/memory.hpp"

namespace cmoon
{
	namespace emulate
	{
		struct CPU_6502
		{
			enum class instruction : std::uint8_t
			{
				lda_im = 0xA9,
				lda_zp = 0xA5,
				lda_zpx = 0xB5,
				lda_ab = 0xAD,
				jsr = 0x20
			};

			std::uint16_t PC {0xFFFC}; // stack pointer
			std::uint16_t SP {0x00FF}; // program counter

			std::uint8_t A {0}; // Register
			std::uint8_t X {0}; // Register
			std::uint8_t Y {0}; // Register

			std::uint8_t C : 1 {0}; // status flag
			std::uint8_t Z : 1 {0}; // status flag
			std::uint8_t I : 1 {0}; // status flag
			std::uint8_t D : 1 {0}; // status flag
			std::uint8_t B : 1 {0}; // status flag
			std::uint8_t V : 1 {0}; // status flag
			std::uint8_t N : 1 {0}; // status flag

			constexpr void reset()
			{
				PC = 0xFFFC;
				SP = 0x00FF;
				A = 0;
				X = 0;
				Y = 0;
				C = 0;
				Z = 0;
				I = 0;
				D = 0;
				B = 0;
				V = 0;
				N = 0;
			}

			constexpr void LDA_set_status(std::uint32_t& cycles) noexcept
			{
				Z = A == 0;
				N = (A & 0b10000000) > 0;
				--cycles;
			}

			constexpr void execute(std::uint32_t cycles, memory_6502& memory)
			{
				while (cycles > 0)
				{
					const auto instruction = fetch_instruction(cycles, memory);
					switch (instruction)
					{
						case instruction::lda_im:
							A = fetch_byte(cycles, memory, PC);
							LDA_set_status(cycles);
							break;
						case instruction::lda_zp:
							{
								const auto zero_page_address = fetch_byte(cycles, memory, PC);
								A = fetch_byte(cycles, memory, zero_page_address);
								LDA_set_status(cycles);
							}
							break;
						case instruction::lda_zpx:
							{
								const auto zero_page_address = static_cast<std::uint8_t>(fetch_byte(cycles, memory, PC) + X);
								--cycles;
								A = fetch_byte(cycles, memory, zero_page_address);
								LDA_set_status(cycles);
							}
							break;
						//case instruction::lda_ab:
						//	{
						//		const auto absolute_address = fetch_word(cycles, memory, PC);
						//		A = fetch_byte(cycles, memory, absolute_address);
						//		LDA_set_status(cycles);
						//	}
						//	break;
						case instruction::jsr:
							{
								const auto subroutine_address = fetch_subroutine(cycles, memory);
								write_word(cycles, memory, PC - 1, SP);
								PC = subroutine_address;
								--cycles;
							}
							break;
					}
				}
			}

			constexpr instruction fetch_instruction(std::uint32_t& cycles, const memory_6502& memory) noexcept
			{
				return static_cast<CPU_6502::instruction>(fetch_byte(cycles, memory, PC++));
			}

			constexpr std::uint16_t fetch_subroutine(std::uint32_t& cycles, const memory_6502& memory) noexcept
			{
				const auto sub = fetch_word(cycles, memory, PC);
				PC += 2;
				return sub;
			}

			constexpr std::uint8_t fetch_byte(std::uint32_t& cycles, const memory_6502& memory, const std::uint16_t address) const noexcept
			{
				const auto data = memory[address];
				--cycles;
				return data;
			}

			constexpr std::uint16_t fetch_word(std::uint32_t& cycles, const memory_6502& memory, const std::uint16_t address) const noexcept
			{
				std::uint16_t data = fetch_byte(cycles, memory, address);
				data |= fetch_byte(cycles, memory, address + 1) << 8;
				return data;
			}

			void write_byte(std::uint32_t& cycles, memory_6502& memory, const std::uint8_t data, const std::uint16_t address) const noexcept
			{
				memory[address] = data;
				--cycles;
			}

			void write_word(std::uint32_t& cycles, memory_6502& memory, const std::uint16_t data, const std::uint16_t address) const noexcept
			{
				write_byte(cycles, memory, data & 0xFF, address);
				write_byte(cycles, memory, data >> 8, address + 1);
			}
		};
	}
}
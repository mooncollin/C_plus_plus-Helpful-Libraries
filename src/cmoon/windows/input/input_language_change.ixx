module;

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

export module cmoon.windows.input.input_language_change;

namespace cmoon::windows
{
	export
	enum class input_language_change : unsigned int
	{
		backward = INPUTLANGCHANGE_BACKWARD,
		forward = INPUTLANGCHANGE_FORWARD,
		system_character_set = INPUTLANGCHANGE_SYSCHARSET
	};
}
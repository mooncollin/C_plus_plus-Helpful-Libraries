module;

#include <Windows.h>

export module cmoon.windows.language;

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
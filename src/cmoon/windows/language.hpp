#pragma once

#include <windows.h>
#include <winnt.h>

namespace cmoon
{
	namespace windows
	{
		enum class input_language_change : unsigned int
		{
			backward = INPUTLANGCHANGE_BACKWARD,
			forward = INPUTLANGCHANGE_FORWARD,
			system_character_set = INPUTLANGCHANGE_SYSCHARSET
		};
	}
}
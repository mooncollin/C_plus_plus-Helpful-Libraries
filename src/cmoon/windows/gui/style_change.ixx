module;

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

export module cmoon.windows.gui.style_change;

namespace cmoon::windows
{
	export
	enum class style_change
	{
		exstyle = GWL_EXSTYLE,
		style = GWL_STYLE
	};
}
module;

#include <WinUser.h>

export module cmoon.windows.gui.multiple_document_interface;

namespace cmoon::windows
{
	export
	enum class multiple_document_tiling_value : unsigned int
	{
		tile_horizontal = MDITILE_HORIZONTAL,
		tile_vertical = MDITILE_VERTICAL
	};

	export
	enum class multiple_document_cascade_value : unsigned int
	{
		tile_skip_disabled = MDITILE_SKIPDISABLED,
		tile_z_order = MDITILE_ZORDER
	};
}
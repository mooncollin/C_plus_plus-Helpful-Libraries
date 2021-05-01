export module cmoon.windows.input.keystroke_message;

import <cstdint>;

namespace cmoon::windows
{
	export
	struct keystroke_message
	{
		std::uint16_t repeat_count: 16;
		std::uint8_t scan_code: 8;
		bool is_extended_key: 1;
	private:
		char reserved : 4;
	public:
		bool alt_context : 1;
		bool is_down : 1;
		bool is_releasing : 1;
	};
}
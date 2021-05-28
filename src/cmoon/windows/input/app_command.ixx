module;

#include <Windows.h>

export module cmoon.windows.input.app_command;

namespace cmoon::windows
{
	export
	enum class app_command
	{
		browser_backward = APPCOMMAND_BROWSER_BACKWARD,
		browser_forward = APPCOMMAND_BROWSER_FORWARD,
		browser_refresh = APPCOMMAND_BROWSER_REFRESH,
		browser_stop = APPCOMMAND_BROWSER_STOP,
		browser_search = APPCOMMAND_BROWSER_SEARCH,
		browser_favorites = APPCOMMAND_BROWSER_FAVORITES,
		browser_home = APPCOMMAND_BROWSER_HOME,
		volume_mute = APPCOMMAND_VOLUME_MUTE,
		volume_down = APPCOMMAND_VOLUME_DOWN,
		volume_up = APPCOMMAND_VOLUME_UP,
		media_nexttrack = APPCOMMAND_MEDIA_NEXTTRACK,
		media_previoustrack = APPCOMMAND_MEDIA_PREVIOUSTRACK,
		media_stop = APPCOMMAND_MEDIA_STOP,
		media_play_pause = APPCOMMAND_MEDIA_PLAY_PAUSE,
		launch_mail = APPCOMMAND_LAUNCH_MAIL,
		launch_media_select = APPCOMMAND_LAUNCH_MEDIA_SELECT,
		launch_app1 = APPCOMMAND_LAUNCH_APP1,
		launch_app2 = APPCOMMAND_LAUNCH_APP2,
		bass_down = APPCOMMAND_BASS_DOWN,
		bass_boost = APPCOMMAND_BASS_BOOST,
		bass_up = APPCOMMAND_BASS_UP,
		treble_down = APPCOMMAND_TREBLE_DOWN,
		treble_up = APPCOMMAND_TREBLE_UP,
		microphone_volume_mute = APPCOMMAND_MICROPHONE_VOLUME_MUTE,
		microphone_volume_down = APPCOMMAND_MICROPHONE_VOLUME_DOWN,
		microphone_volume_up = APPCOMMAND_MICROPHONE_VOLUME_UP,
		help = APPCOMMAND_HELP,
		find = APPCOMMAND_FIND,
		ap_new = APPCOMMAND_NEW,
		open = APPCOMMAND_OPEN,
		close = APPCOMMAND_CLOSE,
		save = APPCOMMAND_SAVE,
		print = APPCOMMAND_PRINT,
		undo = APPCOMMAND_UNDO,
		redo = APPCOMMAND_REDO,
		copy = APPCOMMAND_COPY,
		cut = APPCOMMAND_CUT,
		paste = APPCOMMAND_PASTE,
		reply_to_mail = APPCOMMAND_REPLY_TO_MAIL,
		forward_mail = APPCOMMAND_FORWARD_MAIL,
		send_mail = APPCOMMAND_SEND_MAIL,
		spell_check = APPCOMMAND_SPELL_CHECK,
		dictate_or_command_control_toggle = APPCOMMAND_DICTATE_OR_COMMAND_CONTROL_TOGGLE,
		mic_on_off_toggle = APPCOMMAND_MIC_ON_OFF_TOGGLE,
		correction_list = APPCOMMAND_CORRECTION_LIST,
		media_play = APPCOMMAND_MEDIA_PLAY,
		media_pause = APPCOMMAND_MEDIA_PAUSE,
		media_record = APPCOMMAND_MEDIA_RECORD,
		media_fast_forward = APPCOMMAND_MEDIA_FAST_FORWARD,
		media_rewind = APPCOMMAND_MEDIA_REWIND,
		media_channel_up = APPCOMMAND_MEDIA_CHANNEL_UP,
		media_channel_down = APPCOMMAND_MEDIA_CHANNEL_DOWN,
		ap_delete = APPCOMMAND_DELETE,
		flip3d = APPCOMMAND_DWM_FLIP3D,
	};

	export
	[[nodiscard]] constexpr short get_appcommand_lparam(::LPARAM lp) noexcept
	{
		return GET_APPCOMMAND_LPARAM(lp);
	}
}
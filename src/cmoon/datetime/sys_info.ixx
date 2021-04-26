export module cmoon.datetime.sys_info;

import <chrono>;
import <string>;

import cmoon.datetime.sys_time;

namespace cmoon::datetime
{
	export
	struct sys_info
	{
		sys_seconds begin;
		sys_seconds end;
		std::chrono::seconds offset;
		std::chrono::minutes save;
		std::string abbrev;
	};
}
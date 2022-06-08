import <vector>;
import <iterator>;

import cmoon.windows.system_info;
import cmoon.windows.system_string;

int main()
{
	std::vector<cmoon::windows::string_type> drive_names;
	cmoon::windows::get_drive_names(std::back_inserter(drive_names));
}
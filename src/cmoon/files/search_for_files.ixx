export module cmoon.files.search_for_files;

import <filesystem>;
import <regex>;
import <vector>;
import <iterator>;

namespace cmoon
{
	export
    template<std::output_iterator<std::filesystem::directory_entry> Output>
    Output search_for_files(const std::filesystem::path& path, const std::regex& r, Output out)
    {
        for (const auto& p : std::filesystem::directory_iterator(path))
        {
            std::smatch match;
            const auto path_string = p.path().string();
            if (std::regex_match(path_string, match, r))
            {
                *out = p;
                ++out;
            }
        }

        return out;
    }

    export
    std::vector<std::filesystem::directory_entry> search_for_files(const std::filesystem::path& path, const std::regex& r)
    {
        std::vector<std::filesystem::directory_entry> entries;
        search_for_files(path, r, std::back_inserter(entries));
        return entries;
    }
}
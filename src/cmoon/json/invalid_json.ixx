export module cmoon.json.invalid_json;

import <string>;
import <stdexcept>;

namespace cmoon::json
{
	export
	class invalid_json : public std::runtime_error
	{
		public:
			invalid_json(const std::string& msg)
				: std::runtime_error(msg) {}
	};
}
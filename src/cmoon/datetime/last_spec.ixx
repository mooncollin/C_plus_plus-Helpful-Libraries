export module cmoon.datetime.last_spec;

namespace cmoon::datetime
{
	export
	struct last_spec
	{
		explicit constexpr last_spec() = default;
	};

	export
	inline constexpr last_spec last {};
}
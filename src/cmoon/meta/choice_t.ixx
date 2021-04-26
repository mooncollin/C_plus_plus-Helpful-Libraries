export module cmoon.meta.choice_t;

namespace cmoon::meta
{
	export
	template<class T>
	struct choice_t
	{
		T strategy {};
		bool no_throw {false};
	};
}
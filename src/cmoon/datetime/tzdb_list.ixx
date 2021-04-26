export module cmoon.datetime.tzdb_list;

import <forward_list>;
import <mutex>;

import cmoon.datetime.tzdb;
import cmoon.datetime.get_default_tzdb;

namespace cmoon::datetime
{
	export
	class tzdb_list
	{
		using container = std::forward_list<tzdb>;

		public:
			tzdb_list(const tzdb_list&) = delete;
			tzdb_list& operator=(const tzdb_list&) = delete;

			using const_iterator = typename container::const_iterator;

			const tzdb& front() const noexcept
			{
				std::scoped_lock l{m};
				return values.front();
			}

			const_iterator erase_after(const_iterator p)
			{
				std::scoped_lock l{m};
				return values.erase_after(p);
			}

			const_iterator begin() const noexcept
			{
				return values.begin();
			}

			const_iterator cbegin() const noexcept
			{
				return values.cbegin();
			}

			const_iterator end() const noexcept
			{
				return values.end();
			}

			const_iterator cend() const noexcept
			{
				return values.cend();
			}
		private:
			container values;
			mutable std::mutex m;

			tzdb_list()
			{
				values.push_front(get_default_tzdb());
			}

			friend tzdb_list& get_tzdb_list();
			friend const tzdb& reload_tzdb();
	};

	export
	tzdb_list& get_tzdb_list()
	{
		static tzdb_list l;
		return l;
	}

	export
	const tzdb& get_tzdb()
	{
		return get_tzdb_list().front();
	}

	export
	std::string remote_version()
	{
		return get_tzdb().version;
	}

	export
	const tzdb& reload_tzdb()
	{
		if (remote_version() == get_tzdb().version)
		{
			return get_tzdb();
		}

		auto& list = get_tzdb_list();
		std::scoped_lock l {list.m};

		// Add new tzdb to list
		return list.values.front();
	}
}
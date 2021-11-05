export module cmoon.execution.sender_to;

import <utility>;

import cmoon.execution.receiver;
import cmoon.execution.sender;
import cmoon.execution.connect;

namespace cmoon::execution
{
	export
	template<class S, class R>
	concept sender_to = sender<S> &&
						receiver<R> &&
		requires(S&& s, R&& r)
	{
		execution::connect(std::forward<S>(s), std::forward<R>(r));
	};
}
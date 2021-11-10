export module cmoon.execution.async;

import <future>;
import <concepts>;
import <functional>;
import <type_traits>;
import <memory>;

import cmoon.property;

import cmoon.execution.executor;
import cmoon.execution.execute;
import cmoon.execution.allocator_t;

namespace cmoon::execution
{
	export
	template<executor E, class F, class...Args>
	[[nodiscard]] auto async(const E& e, F&& f, Args&&... args)
	{
		auto f2 = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
		std::packaged_task<std::invoke_result_t<std::decay_t<decltype(f2)>>()> t{std::move(f2)};
		auto result = t.get_future();
		cmoon::execution::execute(e, std::move(t));
		return result;
	}
}
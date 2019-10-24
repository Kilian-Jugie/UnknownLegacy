#pragma once
#include <type_traits>

namespace ul {
	template<typename _T1, typename _T2>
	struct decay_same :
		std::is_same<typename std::decay<_T1>::type,
					 typename std::decay<_T2>::type>::type { };

	template<typename _T1, typename _T2>using decay_same_v = typename decay_same<_T1, _T2>::value;
}
#pragma once
#include <vector>

namespace ul {
	class ExitManager {
	public:
		void addExitCallback(void(*func)(void*), void* params = nullptr) {
			m_Callbacks.push_back(std::make_pair(func, params));
		}

		[[noreturn]]void exit() {
			for (auto it : m_Callbacks) {
				it.first(it.second);
			}
		}
	private:
		std::vector<std::pair<void(*)(void*), void*>> m_Callbacks;
	};
}
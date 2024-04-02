#pragma once

#include "Log.h"
#include <memory>

#define EZZOO_BIND(fn)  [this](auto &&...args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...);}

#define BIT(x) (1 << x)


#ifdef EZZOO_DEBUG
	#define EZZOO_ASSERT_ENABLE
#endif

#ifdef EZZOO_ASSERT_ENABLE
	#define EZZOO_CORE_ASSERT(x, ...)  { if(!(x)) { EZZOO_CORE_ERROR(__VA_ARGS__); __debugbreak(); } }
#else 
	#define EZZOO_CORE_ASSERT(x, y)
#endif

namespace Ezzoo {

	template <typename T>
	using Ref = std::shared_ptr<T>;

	template <typename T, typename ...ARGS>
	Ref<T> CreateRef(ARGS &&...args)
	{
		return std::make_shared<T>(std::forward<ARGS>(args)...);
	}


	template<typename T>
	using Scop = std::unique_ptr<T>;

	template <typename T, typename ...ARGS>
	Scop<T> CreateScope(ARGS&& ...args)
	{
		return std::make_unique<T>(std::forward<ARGS>(args)...);
	}
}
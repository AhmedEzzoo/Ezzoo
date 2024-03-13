#pragma once

#define EZZOO_BIND(fn)  [this](auto &&...args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...);}

#define BIT(x) (1 << x)

////////////////////////////////////////
/// general utility header-only-library
/// for convenience methods/types in C++
/// 2025 Julian Benzel
////////////////////////////////////////
/// utility macros
////////////////////////////////////////
#ifndef UTIL_MACROS_HPP
#define UTIL_MACROS_HPP
////////////////////////////////////////
/// automatic get/set methods
////////////////////////////////////////
#define UTIL_GET(type, name) \
inline type get_##name() const \
{ \
	return name; \
}

#define UTIL_SET(type, name) \
inline void set_##name(type t) \
{ \
	this->name = t; \
}

#define UTIL_GET_SET(type, name) \
UTIL_GET(type, name) \
UTIL_SET(type, name)
////////////////////////////////////////
#endif
////////////////////////////////////////
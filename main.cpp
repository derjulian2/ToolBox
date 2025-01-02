#include <iostream>
#include <cstdint>
#include <concepts>
#include <string>

// maybe better approach for LinAlgPP?

template <typename Tn>
concept Number = requires(Tn a)
{
	a = NULL;
};

template <Number Tn>
struct vec2
{
	vec2() : x(NULL), y(NULL) {}
	vec2(const Tn x, const Tn y) : x(x), y(y) {}

	Tn x = NULL;
	Tn y = NULL;

	std::string str()
	{
		return "( " + std::to_string(x) + ", " + std::to_string(y) + " )";
	}
};

template<Number Tn>
std::ostream& operator<<(std::ostream& os, const vec2<Tn>& vec2)
{
	os << "( " << vec2.x << ", " << vec2.y << " )";
	return os;
}

typedef vec2<uint8_t> vec2u8;
typedef vec2<uint16_t> vec2u16;
typedef vec2<uint32_t> vec2u32;
typedef vec2<uint64_t> vec2u64;
typedef vec2<int8_t> vec2i8;
typedef vec2<int16_t> vec2i16;
typedef vec2<int32_t> vec2i32;
typedef vec2<int64_t> vec2i64;
typedef vec2<int> vec2i;
typedef vec2<double> vec2d;
typedef vec2<float> vec2f;

#include "XMLParser.hpp"

int main()
{
	XML message;

	XMLNode& n = message.AddNode("food", "far");
	n.AddAttribute(XMLAttribute("isscheise", "ja"));
	XMLNode& d = n.AddNode(XMLNode("sars", "siltam"));
	d.AddNode(XMLNode("serrs", "sus"));

	std::cout << message;

	return 0;
}

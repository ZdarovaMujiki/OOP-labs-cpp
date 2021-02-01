#pragma once
#include <tuple>
#include <fstream>
#include <type_traits>

using namespace std;

template <int index, typename... Args>
typename enable_if<(index == sizeof...(Args)), void>::type print(ostream& os, tuple<Args...> const& t) {};

template <int index, typename... Args>
typename enable_if<(index < sizeof...(Args)), void>::type print(ostream& os, tuple<Args...> const& t)
{
	if (index != 0)
	{
		os << ", ";
	}
	os << get<index>(t);
	print<index + 1>(os, t);
}

template <typename... Args>
ostream& operator<<(ostream& os, tuple<Args...> const& t)
{
	print<0>(os, t);
	return os;
}
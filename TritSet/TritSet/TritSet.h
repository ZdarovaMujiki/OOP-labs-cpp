#include <cassert>
#include <unordered_map>
#include <iostream>
#include <cmath>

typedef unsigned int uint;
using namespace std;

enum Trit
{
	Unknown,	//00
	False,		//01
	True		//10
};

Trit operator &(const Trit& a, const Trit& b);
Trit operator |(const Trit& a, const Trit& b);
Trit operator !(const Trit& a);

class TritSet
{
private:
	uint tritAmount;
	uint intAmount;
	uint* data;

public:
	class proxy
	{
	private:
		reference_wrapper<TritSet> set;
		Trit value;
		uint index;
	public:
		proxy(TritSet& tritSet, uint i);
		proxy(const proxy&) = default;
		proxy(proxy&&) = default;
		proxy& operator=(TritSet::proxy&& that) noexcept;
		proxy& operator=(const TritSet::proxy& that);
		proxy& operator=(const Trit& trit);
		operator Trit() const;
	};
	TritSet();
	TritSet(int newLength);
	TritSet(const TritSet& set);
	~TritSet();
	proxy operator[](uint index);
	TritSet& operator=(TritSet set);

	TritSet operator&(const TritSet &b) const;
	TritSet operator|(const TritSet b) const;
	TritSet operator!() const;

	unordered_map< Trit, int> cardinality();
	size_t cardinality(Trit value);
	size_t newLength();
	size_t capacity();
	void trim(size_t lastIndex);
	void reAlloc(uint curInt);
	void shrink();

	void setTrit(uint index, Trit trit);
	Trit getTrit(uint index) const;
};
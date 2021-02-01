#include "TritSet.h"

Trit operator &(const Trit& a, const Trit& b)
{
	if (a == False || b == False)
	{
		return False;
	}
	if (a == Unknown || b == Unknown)
	{
		return Unknown;
	}
	return True;
}

Trit operator |(const Trit& a, const Trit& b)
{
	if (a == True || b == True)
	{
		return True;
	}
	if (a == Unknown || b == Unknown)
	{
		return Unknown;
	}
	return False;
}

Trit operator !(const Trit& a)
{
	if (a == Unknown)
	{
		return Unknown;
	}
	return static_cast<Trit>(a ^ 3);
}

TritSet::proxy::proxy(TritSet& tritSet, uint i) : set(tritSet), index(i), value(set.get().getTrit(i)) {}

TritSet::proxy& TritSet::proxy::operator=(TritSet::proxy&& that) noexcept
{
	value = that.value;
	set.get().setTrit(index, value);
	return *this;
}

TritSet::proxy& TritSet::proxy::operator=(const TritSet::proxy& that)
{
	value = that.value;
	set.get().setTrit(index, value);
	return *this;
}

TritSet::proxy& TritSet::proxy::operator=(const Trit& trit)
{
	value = trit;
	set.get().setTrit(index, trit);
	return *this;
}

TritSet::proxy::operator Trit() const
{
	return value;
}

TritSet::TritSet() : tritAmount(0), intAmount(0), data(nullptr) {}

TritSet::TritSet(int newLength) : tritAmount(0), intAmount((uint)ceil((double)newLength * 2 / (sizeof(uint) * 8)))
{
	assert(newLength >= 0);
	newLength > 0 ? data = new uint[intAmount]() : data = nullptr;
}

TritSet::TritSet(const TritSet& set) : tritAmount(set.tritAmount), intAmount(set.intAmount)
{	
	if (intAmount)
	{
		this->data = new uint[intAmount]();
		memcpy(this->data, set.data, sizeof(uint*) * intAmount);
	}
}

TritSet::~TritSet()
{
	delete[] data;
};

TritSet::proxy TritSet::operator[](uint index)
{
	return proxy(*this, index);
}

TritSet& TritSet::operator=(TritSet set)
{
	if (this == &set)
	{
		return *this;
	}
	delete[] this->data;

	this->tritAmount = set.tritAmount;
	this->intAmount = set.intAmount;
	if (this->intAmount)
	{
		this->data = new uint[this->intAmount]();
		memcpy(this->data, set.data, sizeof(uint*) * this->intAmount);
	}
	return *this;
}

TritSet TritSet::operator&(const TritSet& b) const
{
	TritSet set = (this->intAmount >= b.intAmount) ? *this : b;
	uint trits = (this->tritAmount >= b.tritAmount) ? this->tritAmount : b.tritAmount;
	for (uint i = 0; i < trits; i++)
	{
		set[i] = (*this).getTrit(i) & b.getTrit(i);
	}
	return set;
}

TritSet TritSet::operator|(const TritSet b) const
{
	TritSet set = (this->intAmount >= b.intAmount) ? *this : b;
	uint trits = (this->tritAmount >= b.tritAmount) ? this->tritAmount : b.tritAmount;
	for (uint i = 0; i < trits; i++)
	{
		set[i] = (*this).getTrit(i) | b.getTrit(i);
	}
	return set;
}

TritSet TritSet::operator!() const
{
	TritSet set = *this;
	for (uint i = 0; i < (*this).tritAmount; i++)
	{
		set[i] = !set.getTrit(i);
	}
	return set;
}

unordered_map< Trit, int> TritSet::cardinality()
{
	unordered_map <Trit, int> tritMap;
	int count[3] = { 0 };
	for (uint i = 0; i < tritAmount; i++)
	{
		count[static_cast<uint>((*this)[i])]++;
	}
	for (uint i = 0; i < 3; i++)
	{
		tritMap.insert({ static_cast<Trit>(i), count[i] });
	}
	return tritMap;
}

size_t TritSet::cardinality(Trit value)
{
	size_t count = 0;
	for (uint i = 0; i < tritAmount; i++)
	{
		if ((*this)[i] == value)
		{
			count++;
		}
	}
	return count;
}

size_t TritSet::newLength()
{
	int i = tritAmount - 1;
	while (i >= 0)
	{
		for (i; (i % (8 * sizeof(uint) / 2 - 1)) && (*this)[i] == Unknown; i--); // until last Trit in uint
		if ((*this)[i] != Unknown)
		{
			return i + 1;
		}
		else
		{
			i--;
		}

		if (!data[i * 2 / (8 * sizeof(uint))])
		{
			i = i * 2 / (8 * sizeof(uint)); // from Trit to uint
			for (i; !data[i] && i >= 0; i--);
			if (i == -1)
			{
				return 0;
			}

			i = i * (8 * sizeof(uint)) / 2 + (8 * sizeof(uint)) / 2 - 1; // from uint to last Trit in uint
		}
	}
	return 0;
}

size_t TritSet::capacity()
{
	return intAmount;
}

void TritSet::trim(size_t lastIndex)
{
	if (lastIndex < tritAmount)
	{
		uint i;
		for (i = lastIndex; i < tritAmount && (i % (8 * sizeof(uint) / 2)); i++)
		{
			(*this)[i] = Unknown;
		}
		i = i * 2 / (8 * sizeof(uint)); // from Trit to uint
		for (i; i < intAmount; i++)
		{
			data[i] = 0;
		}
		tritAmount = newLength();
	}
}

void TritSet::reAlloc(uint curInt)
{
	if (curInt >= intAmount)
	{
		if (intAmount)
		{
			uint* tmp = new uint[curInt + 1]();
			memcpy(tmp, data, sizeof(uint*) * intAmount);

			delete[] data;
			data = tmp;
		}
		else
		{
			data = new uint[curInt + 1]();
		}
		intAmount = curInt + 1;
	}
}

void TritSet::shrink()
{
	intAmount = (uint)ceil((double)tritAmount * 2 / (sizeof(uint) * 8));

	uint* tmp = new uint[intAmount];
	memcpy(tmp, data, sizeof(uint*) * intAmount);

	delete[] data;
	data = tmp;
}

void TritSet::setTrit(uint index, Trit value)
{
	uint curInt = (index * 2) / (8 * sizeof(int));
	uint curTrit = (index * 2) % (8 * sizeof(int));

	uint mask = ~(3 << (sizeof(uint) * 8 - curTrit - 2)); //1111001111111111
	uint val = value << (sizeof(uint) * 8 - curTrit - 2); //0000..0000000000

	if (index >= tritAmount)
	{
		if (value == Unknown)
		{
			return;
		}
		else
		{
			reAlloc(curInt);
			tritAmount = index + 1;
		}
	}
	else //no reallocation
	{
		if ((value == Unknown) && ((index + 1) == tritAmount))
		{
			data[curInt] = data[curInt] & mask | val;
			tritAmount = newLength();
		}
	}

	data[curInt] = data[curInt] & mask | val;
}

Trit TritSet::getTrit(uint index) const
{
	if (index >= tritAmount)
	{
		return Unknown;
	}
	uint curInt = (index * 2) / (8 * sizeof(int));
	uint curTrit = (index * 2) % (8 * sizeof(int));

	return static_cast<Trit>((data[curInt] >> (8 * sizeof(int) - curTrit - 2)) & 3);
}
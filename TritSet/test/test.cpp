#include "pch.h"
#include "../TritSet/TritSet.cpp"

TEST(fixed, fixed)
{
	TritSet set(100);
	set[0] = True;
	set[1] = set[0];
	EXPECT_TRUE(set[0] == True);
	EXPECT_TRUE(set[1] == True);
	set[2] = False;
	set[3] = set[2];
	EXPECT_TRUE(set[2] == False);
	EXPECT_TRUE(set[3] == False);
}
TEST(Trim, part1)
{
	TritSet set;

	for (uint i = 0; i < 100; i++)
	{
		set[i] = True;
	}
	for (uint i = 60; i < 80; i++)
	{
		set[i] = Unknown;
	}
	set.trim(80);
	for (uint i = 0; i < 60; i++)
	{
		EXPECT_TRUE(set[i] == True);
	}
	for (uint i = 60; i < 100; i++)
	{
		EXPECT_TRUE(set[i] == Unknown);
	}
	EXPECT_TRUE(set.newLength() == 60);
}
TEST(Trim, part2)
{
	TritSet set;

	for (uint i = 0; i < 80; i++)
	{
		set[i] = True;
	}

	set.trim(90);
	for (uint i = 0; i < 80; i++)
	{
		EXPECT_TRUE(set[i] == True);
	}
	EXPECT_TRUE(set.newLength() == 80);
}
TEST(Shrink, part1)
{
	TritSet set(100);

	for (uint i = 0; i < 70; i++)
	{
		set[i] = True;
	}
	set.shrink();

	for (uint i = 0; i < 70; i++)
	{
		EXPECT_TRUE(set[i] == True);
	}
	uint tritAmount = set.newLength();
	EXPECT_TRUE(tritAmount == 70);
}
TEST(Shrink, part2)
{
	TritSet set(100);

	for (uint i = 0; i < 70; i++)
	{
		set[i] = True;
	}
	uint n = set.newLength();
	for (uint i = 50; i < 70; i++)
	{
		set[i] = Unknown;
	}
	uint m = set.newLength();
	set.shrink();

	for (uint i = 0; i < 50; i++)
	{
		EXPECT_TRUE(set[i] == True);
	}
	for (uint i = 50; i < 70; i++)
	{
		EXPECT_TRUE(set[i] == Unknown);
	}
	EXPECT_TRUE(n == 70);
	EXPECT_TRUE(n - 20 == m);
	EXPECT_TRUE(m == set.newLength());
}
TEST(Cardinality, Cardinality)
{
	TritSet set;
	set[22] = False;
	set[19] = True;
	set[18] = True;
	set[16] = False;
	set[14] = True;
	set[13] = True;
	set[12] = True;
	set[9] = False;
	set[8] = False;
	set[7] = False;
	set[5] = True;
	set[4] = True;
	set[3] = False;
	set[2] = False;
	set[0] = True;

	EXPECT_TRUE(set.cardinality(False) == 7);
	EXPECT_TRUE(set.cardinality(Unknown) == 8);
	EXPECT_TRUE(set.cardinality(True) == 8);
}
TEST(Cardinality, Map_cardinality)
{
	TritSet set;
	set[0] = True;
	set[2] = False;
	set[3] = set[2];
	set[4] = True;
	set[5] = set[4];
	set[7] = False;
	set[8] = set[7];
	set[9] = set[8];
	set[12] = True;
	set[13] = set[12];
	set[14] = set[12];
	set[16] = False;
	set[18] = True;
	set[19] = set[18];
	set[22] = False;

	std::unordered_map<Trit, int> count = set.cardinality();

	EXPECT_TRUE(count[False] == 7);
	EXPECT_TRUE(count[Unknown] == 8); //1, 6, 10, 11, 15, 17, 20, 21
	EXPECT_TRUE(count[True] == 8);

}
TEST(Constructor, Constructor)
{
	TritSet set(1000);

	for (uint i = 0; i < 30; i++)
	{
		set[i] = static_cast<Trit>(i % 3);
	}
	for (uint i = 600; i < 800; i++)
	{
		set[i] = False;
	}
	for (uint i = 800; i < 1000; i++)
	{
		set[i] = True;
	}

	for (uint i = 0; i < 30; i++)
	{
		EXPECT_TRUE(set[i] == static_cast<Trit>(i % 3));
	}
	for (uint i = 30; i < 600; i++)
	{
		EXPECT_TRUE(set[i] == Unknown);
	}
	for (uint i = 600; i < 800; i++)
	{
		EXPECT_TRUE(set[i] == False);
	}
	for (uint i = 800; i < 1000; i++)
	{
		EXPECT_TRUE(set[i] == True);
	}
}
TEST(Constructor, Copy_Constructor)
{
	TritSet setA(100);
	setA[70] = True;
	setA[50] = Unknown;
	setA[24] = False;
	TritSet setB = setA;
	setB[70] = False;
	setB[50] = False;
	setB[24] = False;
	setB.trim(60);

	EXPECT_TRUE(setA[70] == True);
	EXPECT_TRUE(setA[50] == Unknown);
	EXPECT_TRUE(setA[24] == False);
	EXPECT_TRUE(setB[70] == Unknown);
	EXPECT_TRUE(setB[50] == False);
	EXPECT_TRUE(setB[24] == False);
	EXPECT_TRUE(setB.newLength() < setA.newLength());
	EXPECT_TRUE(setA.capacity() == setB.capacity());
}

TEST(Memory_reallocation, no_reallocation)
{
	TritSet set(30);
	uint cap = set.capacity();
	for (uint i = 0; i < 30; i++)
	{
		set[i] = static_cast<Trit>(i % 3);
	}
	EXPECT_TRUE(cap == set.capacity());
	set[31] = Unknown;
	set[40] = Unknown;
	set[100] = Unknown;
	EXPECT_TRUE(cap == set.capacity());
}

TEST(Memory_reallocation, reallocation)
{
	TritSet set;

	for (uint i = 0; i < 30; i++)
	{
		set[i] = static_cast<Trit>(i % 3);
	}
	set[70] = True;
	set[70] = Unknown;

	for (uint i = 0; i < 30; i++)
	{
		EXPECT_TRUE(set[i] == static_cast<Trit>(i % 3));
	}
	for (uint i = 30; i < 100; i++)
	{
		EXPECT_TRUE(set[i] == Unknown);
	}
}
TEST(trit_operator, AND)
{
	Trit F = False, U = Unknown, T = True;

	EXPECT_TRUE((F & F) == False);
	EXPECT_TRUE((F & U) == False);
	EXPECT_TRUE((F & T) == False);
	EXPECT_TRUE((U & U) == Unknown);
	EXPECT_TRUE((U & T) == Unknown);
	EXPECT_TRUE((T & T) == True);
}
TEST(trit_operator, OR)
{
	Trit F = False, U = Unknown, T = True;

	EXPECT_TRUE((T | T) == True);
	EXPECT_TRUE((T | U) == True);
	EXPECT_TRUE((T | F) == True);
	EXPECT_TRUE((U | U) == Unknown);
	EXPECT_TRUE((U | F) == Unknown);
	EXPECT_TRUE((F | F) == False);
}
TEST(trit_operator, NOT)
{
	Trit F = False, U = Unknown, T = True;
	EXPECT_TRUE(!F == True);
	EXPECT_TRUE(!U == Unknown);
	EXPECT_TRUE(!T == False);
}
TEST(trit_operator, assignment)
{
	Trit U, F, T;
	TritSet set(3);
	set[0] = Unknown;
	set[1] = False;
	set[2] = True;
	U = set[0];
	F = set[1];
	T = set[2];
	EXPECT_TRUE(U == Unknown);
	EXPECT_TRUE(F == False);
	EXPECT_TRUE(T == True);
	set[0] = T;
	set[1] = U;
	set[2] = F;
	EXPECT_TRUE(set[0] == True);
	EXPECT_TRUE(set[1] == Unknown);
	EXPECT_TRUE(set[2] == False);
}
TEST(TritSet_Operator, assignment)
{
	TritSet setA(100), setB;
	setA[88] = True;
	setA[70] = True;
	setA[50] = Unknown;
	setA[24] = False;
	uint n = setA.capacity(), m = setA.newLength();
	setB = setA;
	setB[70] = False;
	setB[50] = False;
	setB[24] = False;
	setB.trim(60);

	EXPECT_TRUE(setA[70] == True);
	EXPECT_TRUE(setA[50] == Unknown);
	EXPECT_TRUE(setA[24] == False);
	EXPECT_TRUE(setA[88] == True);

	EXPECT_TRUE(setB[88] == Unknown);
	EXPECT_TRUE(setB[70] == Unknown);
	EXPECT_TRUE(setB[50] == False);
	EXPECT_TRUE(setB[24] == False);

	EXPECT_TRUE(setA.capacity() == n);
	EXPECT_TRUE(setA.newLength() == m);
	EXPECT_TRUE(setB.newLength() < setA.newLength());
	EXPECT_TRUE(setA.capacity() == setB.capacity());
}
TEST(TritSet_Operator, AND)
{
	TritSet setA, setB, setC;
	setA[1] = False;
	setA[3] = False;
	setA[5] = True;

	setB[0] = True;
	setB[2] = False;
	setB[3] = False;
	setB[4] = True;
	setB[5] = True;
	setB[7] = False;
	setB[8] = False;
	setB[9] = False;
	uint capA = setA.capacity(), capB = setB.capacity();
	setC = setA & setB;

	EXPECT_TRUE(setA[0] == Unknown);
	EXPECT_TRUE(setA[1] == False);
	EXPECT_TRUE(setA[2] == Unknown);
	EXPECT_TRUE(setA[3] == False);
	EXPECT_TRUE(setA[4] == Unknown);
	EXPECT_TRUE(setA[5] == True);

	EXPECT_TRUE(setB[0] == True);
	EXPECT_TRUE(setB[1] == Unknown);
	EXPECT_TRUE(setB[2] == False);
	EXPECT_TRUE(setB[3] == False);
	EXPECT_TRUE(setB[4] == True);
	EXPECT_TRUE(setB[5] == True);
	EXPECT_TRUE(setB[6] == Unknown);
	EXPECT_TRUE(setB[7] == False);
	EXPECT_TRUE(setB[8] == False);
	EXPECT_TRUE(setB[9] == False);

	EXPECT_TRUE(setC[0] == Unknown);
	EXPECT_TRUE(setC[1] == False);
	EXPECT_TRUE(setC[2] == False);
	EXPECT_TRUE(setC[3] == False);
	EXPECT_TRUE(setC[4] == Unknown);
	EXPECT_TRUE(setC[5] == True);
	EXPECT_TRUE(setC[6] == Unknown);
	EXPECT_TRUE(setC[7] == False);
	EXPECT_TRUE(setC[8] == False);
	EXPECT_TRUE(setC[9] == False);

	EXPECT_TRUE(setA.capacity() == capA);
	EXPECT_TRUE(setB.capacity() == capB);
	EXPECT_TRUE(setC.capacity() == capB);
}
TEST(TritSet_Operator, OR)
{
	TritSet setA, setB, setC;
	setA[5] = True;
	setA[3] = False;
	setA[1] = False;

	setB[9] = False;
	setB[8] = False;
	setB[7] = False;
	setB[0] = True;
	setB[3] = False;
	setB[5] = True;
	setB[4] = True;
	setB[2] = False;
	uint capA = setA.capacity(), capB = setB.capacity();
	setC = setA | setB;

	EXPECT_TRUE(setA[0] == Unknown);
	EXPECT_TRUE(setA[1] == False);
	EXPECT_TRUE(setA[2] == Unknown);
	EXPECT_TRUE(setA[3] == False);
	EXPECT_TRUE(setA[4] == Unknown);
	EXPECT_TRUE(setA[5] == True);

	EXPECT_TRUE(setB[0] == True);
	EXPECT_TRUE(setB[1] == Unknown);
	EXPECT_TRUE(setB[2] == False);
	EXPECT_TRUE(setB[3] == False);
	EXPECT_TRUE(setB[4] == True);
	EXPECT_TRUE(setB[5] == True);
	EXPECT_TRUE(setB[6] == Unknown);
	EXPECT_TRUE(setB[7] == False);
	EXPECT_TRUE(setB[8] == False);
	EXPECT_TRUE(setB[9] == False);

	EXPECT_TRUE(setC[0] == True);
	EXPECT_TRUE(setC[1] == Unknown);
	EXPECT_TRUE(setC[2] == Unknown);
	EXPECT_TRUE(setC[3] == False);
	EXPECT_TRUE(setC[4] == True);
	EXPECT_TRUE(setC[5] == True);
	EXPECT_TRUE(setC[6] == Unknown);
	EXPECT_TRUE(setC[7] == Unknown);
	EXPECT_TRUE(setC[8] == Unknown);
	EXPECT_TRUE(setC[9] == Unknown);

	EXPECT_TRUE(setA.capacity() == capA);
	EXPECT_TRUE(setB.capacity() == capB);
	EXPECT_TRUE(setC.capacity() == capB);
}
TEST(TritSet_Operator, NOT)
{
	TritSet setA(100);
	setA[0] = True;
	setA[1] = False;
	setA[2] = True;
	setA[5] = False;
	TritSet setB = !setA;

	EXPECT_TRUE(setA.newLength() == 6);
	EXPECT_TRUE(setA.newLength() == setB.newLength());

	EXPECT_TRUE(setA[0] == True);
	EXPECT_TRUE(setA[1] == False);
	EXPECT_TRUE(setA[2] == True);
	EXPECT_TRUE(setA[3] == Unknown);
	EXPECT_TRUE(setA[4] == Unknown);
	EXPECT_TRUE(setA[5] == False);

	EXPECT_TRUE(setB[0] == False);
	EXPECT_TRUE(setB[1] == True);
	EXPECT_TRUE(setB[2] == False);
	EXPECT_TRUE(setB[3] == Unknown);
	EXPECT_TRUE(setB[4] == Unknown);
	EXPECT_TRUE(setB[5] == True);
}
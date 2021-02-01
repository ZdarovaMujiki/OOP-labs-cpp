#include "TritSet.h"

int main()
{
	TritSet set(100);
	set[0] = True;
	set[1] = set[2] = set[3] = set[0];
	cout << set[1] << set[2] << set[3];
	return 0;
}
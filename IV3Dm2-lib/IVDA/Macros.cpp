#include "Macros.h"

bool isBigEndian()
{
	int i = 1;
	return (((char*)&i)[0] == 0);
}

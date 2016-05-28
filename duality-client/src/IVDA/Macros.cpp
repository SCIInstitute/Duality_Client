#include "src/IVDA/Macros.h"

bool isBigEndian()
{
	int i = 1;
	return (((char*)&i)[0] == 0);
}

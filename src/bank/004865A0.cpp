#include <string.h>

// Leaf functions in bank/004865A0. Each one is a load, a store, or a constant return.

extern "C" {

void fn_00486B00()
{
	*(int*)0x22fb6c4 = 0;
	*(int*)0x22fb6cc = 0;
}

unsigned char fn_00486BE0()
{
	return *(unsigned char*)0x22fb6f7;
}

void fn_00487750(int value)
{
	*(int*)0x59c124 = value;
}

void fn_004872B0()
{
	memset((void*)0x22f9560, 0, 24);
}

}

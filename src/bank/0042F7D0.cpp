// Leaf functions in bank/0042F7D0, plus the new-game name copy and the mode 2 enter.

#include <string.h>

struct LevelHost {
	int fn_004D38B0(void);
};

extern "C" {

int fn_004300B0()
{
	return *(int*)0x5f9e70;
}

void fn_004302A0()
{
	*(int*)0x5f9e64 = 0;
}

void fn_00430B10()
{
	*(unsigned char*)0x5f9e74 = 0;
}

void fn_00431DF0()
{
	*(unsigned char*)0x5fc01c = 0;
}

void fn_00432E50(int value)
{
	*(int*)0x5931b4 = value;
}

int fn_00432710(void);
void fn_004119A0(int value);
void fn_00411CD0(void);

void fn_00412620(int kind, int flags);
void fn_00401020(int value);
int fn_00401040(void);
int fn_00401050(void);
int fn_004D6300(char* name);
int fn_004696D0(char* name);
void fn_00469080(int id, int unused);
void fn_0046B200(void);
void fn_0046B680(void);
void fn_00439800(void);

void fn_00430B80()
{
	fn_00412620(0x15, 0);
}

void fn_00431E70()
{
	fn_00401020(4);
}

void fn_00431E00(void)
{
	*(unsigned char*)0x5fc01c = 1;
	fn_004119A0(fn_00432710());
	fn_00411CD0();
}

void fn_00431A40(char* name, char* variant)
{
	int id;
	int slot;
	char* table;

	id = fn_004D6300(name);
	*(unsigned char*)0x60a308 = 0;
	*(int*)0x60a33c = 1;
	strcpy((char*)0x60a2c8, name);
	if (variant == 0)
	{
		if (id != -1)
		{
			table = (char*)(0x25388c0 + id * 0x70);
			for (slot = 0; slot < 7; slot = slot + 1)
			{
				if (*(int*)table != 0)
					break;
				table = table + 0x10;
			}
			if (slot < 7)
				variant = *(char**)table;
			else
				variant = (char*)0x60a2c8;
		}
		else
			variant = (char*)0x60a2c8;
	}
	strcpy((char*)0x60a2e8, variant);
	if (((LevelHost*)0x2493a38)->fn_004D38B0() != 0 || (id == 3 && _stricmp((char*)0x60a2e8, (char*)0x5930c0) == 0))
		*(unsigned char*)0x25443a8 = 1;
	else
		*(unsigned char*)0x25443a8 = 0;
	fn_00401020(2);
	if (fn_00401040() == 0)
		*(unsigned char*)0x5fbf18 = 1;
}

void fn_00431E20(void)
{
	int previous;
	unsigned int flags;

	previous = fn_00401050();
	fn_00469080(fn_004696D0((char*)0x58a144), 1);
	fn_0046B200();
	fn_0046B680();
	fn_00439800();
	flags = *(unsigned int*)0x5fbfd8 & 0x1c;
	flags = flags | ((previous == 0xe) + 1);
	*(unsigned int*)0x5fbfd8 = flags;
}

}

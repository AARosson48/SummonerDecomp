// bank/00401000. Original .cpp is not known yet. Names stay FUN_/DAT_
// until a string or a 100% match proves one. Not annotated FUNCTION.

int DAT_005B27E0;
int DAT_005B27E4;
int DAT_005B27E8;
unsigned char DAT_005B27EC;

unsigned char DAT_02491A5C;
int DAT_005A5740;
unsigned char DAT_005FBFD8;
int DAT_0065139C;
int DAT_005951C4;
char DAT_005958FC;
char DAT_00588B24[];
char DAT_0257ED80[];
int DAT_005FBFDC;
int DAT_005FBFE0;
char DAT_0257EF80[];

typedef void (*VoidFn)(void);

VoidFn DAT_00588A70[];

void FUN_00401190(void);
void FUN_004138A0(void);
void FUN_00414020(void);
void FUN_00501510(void);
void FUN_00500A90(void);
void FUN_005011E0(int* a, int* b, void* c);
void FUN_004413D0(void);
void FUN_004408A0(void);
void FUN_00431640(void);
void FUN_004328B0(void);
void FUN_0056C740(char* dst, char* fmt, int type, int count);

void FUN_00401000(void)
{
	DAT_005B27E0 = 0;
	DAT_005B27E4 = 0;
	DAT_005B27E8 = 0;
}

void FUN_00401020(int value)
{
	DAT_005B27EC = 1;
	DAT_005B27E8 = value;
}

int FUN_00401040(void)
{
	return DAT_005B27E0;
}

int FUN_00401050(void)
{
	return DAT_005B27E4;
}

int FUN_00401060(void)
{
	return DAT_005B27E8;
}

void FUN_00401190(void)
{
	DAT_005B27E4 = DAT_005B27E0;
	DAT_005B27E0 = DAT_005B27E8;
	DAT_005B27EC = 0;
}

int FUN_00401070(void)
{
	int cursor;
	int type;

	if (DAT_005B27EC)
	{
		do
		{
			if (DAT_00588A70[DAT_005B27E0 * 3 + 2])
				DAT_00588A70[DAT_005B27E0 * 3 + 2]();
			FUN_00401190();
			FUN_004138A0();
			FUN_00414020();
			if (DAT_00588A70[DAT_005B27E0 * 3])
				DAT_00588A70[DAT_005B27E0 * 3]();
		} while (DAT_005B27EC);
	}

	if (DAT_02491A5C == 1)
		DAT_005A5740 = 0;
	else
		FUN_00501510();

	FUN_00500A90();
	FUN_005011E0(&DAT_005FBFDC, &DAT_005FBFE0, DAT_0257EF80);

	if (DAT_005FBFD8 & 2)
	{
		for (cursor = (int)&DAT_005951C4; cursor < (int)&DAT_005958FC; cursor += 0x1C)
			*(int*)cursor = 0;
		FUN_004413D0();
	}

	if (DAT_0065139C == 1)
		FUN_004408A0();

	if (DAT_00588A70[DAT_005B27E0 * 3 + 1])
		DAT_00588A70[DAT_005B27E0 * 3 + 1]();

	if (DAT_005FBFD8 & 2)
	{
		type = 0;
		for (cursor = (int)&DAT_005951C4; cursor < (int)&DAT_005958FC; cursor += 0x1C)
		{
			if (*(int*)cursor > 0)
			{
				// DAT_00588B24 is "Packet type %d created %d time(s)\n"
				FUN_0056C740(DAT_0257ED80, DAT_00588B24, type, *(int*)cursor);
			}
			type++;
		}
	}

	FUN_00431640();
	FUN_004328B0();
	return DAT_005B27E0;
}

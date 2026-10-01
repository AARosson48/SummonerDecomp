// bank/00401000. Original .cpp is not known yet.
// Function names match config/dtk_symbols.txt so objdiff can pair them.
// Not annotated FUNCTION.
//
// fn_00401070 is the interface mode runner. The 15 modes are triples of
// callbacks at 0x588A70. Mode 2 plays "Title" (next to credits.tbl and
// geeks.bik). Mode 8 builds a screen with a "Load" button and a "Cancel"
// button. The panels in this bank are Assess-Bkgrnd (fn_004011D0 and the
// unlabeled span at 0x401380), Map-Bkgrnd (fn_00401EF0), and BuySell-Bkgrnd
// (fn_00404A20). Options-Bkgrnd and SaveLoad-Bkgrnd are other addresses.

extern "C" {

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

void fn_00401190(void);
void FUN_004138A0(void);
void FUN_00414020(void);
void FUN_00501510(void);
void FUN_00500A90(void);
void FUN_005011E0(int* a, int* b, void* c);
void FUN_004413D0(void);
void FUN_004408A0(void);
void FUN_00431640(void);
void FUN_004328B0(void);
int sprintf(char* dst, const char* fmt, ...);

void fn_00401000(void)
{
	DAT_005B27E0 = 0;
	DAT_005B27E4 = 0;
	DAT_005B27E8 = 0;
}

void fn_00401020(int value)
{
	DAT_005B27EC = 1;
	DAT_005B27E8 = value;
}

int fn_00401040(void)
{
	return DAT_005B27E0;
}

int fn_00401050(void)
{
	return DAT_005B27E4;
}

int fn_00401060(void)
{
	return DAT_005B27E8;
}

void fn_00401190(void)
{
	DAT_005B27E4 = DAT_005B27E0;
	DAT_005B27E0 = DAT_005B27E8;
	DAT_005B27EC = 0;
}

int fn_00401070(void)
{
	int cursor;
	int type;

	if (DAT_005B27EC)
	{
		do
		{
			if (DAT_00588A70[DAT_005B27E0 * 3 + 2])
				DAT_00588A70[DAT_005B27E0 * 3 + 2]();
			fn_00401190();
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
				sprintf(DAT_0257ED80, DAT_00588B24, type, *(int*)cursor);
			}
			type++;
		}
	}

	FUN_00431640();
	FUN_004328B0();
	return DAT_005B27E0;
}

// 0x402250 through 0x4044A0 sits between Map-Bkgrnd and the automap commands.
// The int at 0x60AD68 selects a scale: 0 and the default are 1.0, 1 is 1.25, 2 is 1.6.
// The int at 0x5B2A10 is a step, 0 through 6, dispatched by fn_00402680.
// fn_00402280 writes the label at 0x5B2A14. The large bodies stay symbols until their C is written.

void fn_00402280(int which);
void fn_004025A0(int index);
void fn_004025F0(void);
void fn_0042FFE0(int* out_a, int* out_b, int width, int height);
int fn_00430220(int id);
void fn_00503500(int entry, int* width, int* height);
void fn_00468D90(int id, float value, int a, int b, int c);
void fn_00416A10(void);
int fn_00413460(int a, int b, int c, int d, int e, int f);
int fn_00402760(int arg);
void fn_00402E90(void);
void fn_00403000(void);
void fn_00403100(void);
int fn_00403350(int arg);
void fn_00403510(void);
void fn_004035C0(void);

void fn_004023B0(void)
{
	int mode;
	float scale;
	int* obj;
	int out_a;
	int out_b;
	int scaled_a;
	int scaled_b;

	mode = *(int*)0x0060AD68;
	switch (mode)
	{
	case 1:
		scale = 1.25f;
		break;
	case 2:
		scale = 1.6f;
		break;
	default:
		scale = 1.0f;
		break;
	}
	obj = *(int**)0x005FCC88;
	fn_0042FFE0(&out_a, &out_b, obj[4], obj[6]);
	scaled_a = (int)(*(int*)0x005B29F4 * scale * (float)out_a + 0.5f);
	scaled_b = (int)(*(int*)0x005B29F8 * scale * (float)out_b + 0.5f);
	if (*(char*)0x005B2A0C)
	{
		*(int*)0x005B2B4C = (*(int*)(mode * 8 + 0x0057E910) / 2) - scaled_a;
		*(int*)0x005B2B50 = (*(int*)(mode * 8 + 0x0057E914) / 2) - scaled_b;
	}
	else
	{
		*(int*)0x005B2B4C = *(int*)0x005B29E8;
		*(int*)0x005B2B50 = *(int*)0x005B29EC;
	}
}

void fn_00402490(void)
{
	int count;
	int index;
	float scale;
	char* src;
	int* dst;

	switch (*(int*)0x0060AD68)
	{
	case 1:
		scale = 1.25f;
		break;
	case 2:
		scale = 1.6f;
		break;
	default:
		scale = 1.0f;
		break;
	}
	count = *(int*)0x005F9E64;
	src = (char*)0x005F9840;
	dst = (int*)0x005B2B84;
	for (index = 0; index < count; index++)
	{
		if (*(int*)(src + 4) != *(int*)0x005B2CB8)
		{
			dst[-1] = -1;
			dst[0] = -1;
		}
		else
		{
			int out_a;
			int out_b;

			fn_0042FFE0(&out_a, &out_b, *(int*)(src - 4), *(int*)src);
			dst[-1] = (int)(*(int*)0x005B29F4 * scale * (float)out_a + 0.5f);
			dst[0] = (int)(*(int*)0x005B29F8 * scale * (float)out_b + 0.5f);
		}
		src += 0x30;
		dst += 2;
	}
}

void fn_00402560(void)
{
	int* obj;

	obj = *(int**)0x005FCC88;
	fn_004025A0(fn_00430220(obj[0x68C / 4]));
	*(char*)0x005B2C89 = 1;
	*(char*)0x005B2A0C = 1;
	*(int*)0x005B29E8 = 0;
	*(int*)0x005B29EC = 0;
}

void fn_004025A0(int index)
{
	if (index < 0 || index >= *(int*)0x0060A340)
		return;
	*(int*)0x005B2CB8 = index;
	fn_00503500(*(int*)(index * 4 + 0x0060A344), (int*)0x005B29F4, (int*)0x005B29F8);
	fn_00402490();
	fn_00402280(-1);
	fn_004025F0();
}

void fn_004025F0(void)
{
	int index;
	int mask;

	index = *(int*)0x005B2CB8;
	mask = ~4;
	if (index > 0)
	{
		*(int*)0x005B2CD8 &= mask;
		*(int*)0x005B2CF0 &= mask;
	}
	else
	{
		*(int*)0x005B2CD8 |= 4;
		*(int*)0x005B2CF0 |= 4;
	}
	if (index >= *(int*)0x0060A340 - 1)
	{
		*(int*)0x005B2D08 |= 4;
		*(int*)0x005B2D20 |= 4;
	}
	else
	{
		*(int*)0x005B2D08 &= mask;
		*(int*)0x005B2D20 &= mask;
	}
}

int fn_00402680(int arg)
{
	int* rec;
	int step;

	for (rec = (int*)0x005B2CC0; rec < (int*)0x005B2D98; rec = (int*)((char*)rec + 0x18))
		*rec &= ~2;
	fn_004023B0();
	if (*(char*)0x005B2C89)
	{
		int* obj;
		int id;

		obj = *(int**)0x005FCC88;
		id = fn_00430220(obj[0x68C / 4]);
		if (id != *(int*)0x005B2CB8)
			fn_004025A0(id);
	}
	if (*(unsigned char*)0x005FCC98 & 0x10)
		return arg;
	step = *(int*)0x005B2A10;
	switch (step)
	{
	case 0:
		return fn_00402760(arg);
	case 1:
		fn_00402E90();
		return arg;
	case 2:
		fn_00403000();
		return arg;
	case 3:
		fn_00403100();
		return arg;
	case 4:
		return fn_00403350(arg);
	case 5:
		fn_00403510();
		return arg;
	case 6:
		fn_004035C0();
		return arg;
	default:
		return arg;
	}
}

void fn_00402250(void)
{
	fn_00402560();
	*(int*)0x005B2A10 = 0;
	*(int*)0x005B2B5C = -1;
	fn_004023B0();
	fn_00402490();
	fn_00402280(-1);
}

void fn_00402FB0(void)
{
	int index;

	index = *(int*)0x005B2CB8;
	if (index > 0)
	{
		fn_004025A0(index - 1);
		*(char*)0x005B2C89 = 0;
	}
}

void fn_00402FD0(void)
{
	int limit;
	int index;

	limit = *(int*)0x0060A340 - 1;
	index = *(int*)0x005B2CB8;
	if (index < limit)
	{
		fn_004025A0(index + 1);
		*(char*)0x005B2C89 = 0;
	}
}

void fn_00402F10(int arg)
{
	switch (arg - 2)
	{
	case 0:
		fn_00402FB0();
		fn_00468D90(0x27, 1.0f, 0, 0, 0);
		break;
	case 2:
		fn_00402FD0();
		fn_00468D90(0x27, 1.0f, 0, 0, 0);
		break;
	case 4:
		fn_00402560();
		fn_00468D90(0x23, 1.0f, 0, 0, 0);
		break;
	case 6:
		fn_00416A10();
		fn_00468D90(0x24, 1.0f, 0, 0, 0);
		break;
	default:
		break;
	}
}

int fn_00404460(void)
{
	return *(int*)0x005B2CD0;
}

int fn_00404470(int a, int b)
{
	return fn_00413460(
		a,
		b,
		*(int*)0x005B2CC8,
		*(int*)0x005B2CCC,
		*(int*)0x005B2CD0,
		*(int*)0x005B2CD4);
}

// Assess panel. Seven background records live at 0x5B2810, stride 0x18.
// The int at 0x5B28B8 is the list cursor. The int at 0x5B29D4 is the step.

struct ConsoleCmd
{
	void fn_005019E0(void);
	void fn_0050A180(const char* name, const char* help, void (*handler)(void));
	void fn_004D14F0(int id, int value);
};

struct ColorPair
{
	void fn_005067F0(int a, int b, int c, int d);
};

void fn_004013F0(int arg);
void fn_004016D0(void);
int fn_00401840(int arg);
void fn_00401B20(void);
void fn_00401BF0(void);
unsigned char fn_00432DA0(void);
void fn_00432660(int a, int b);
void fn_0041E380(void);
void fn_004326C0(void);
void fn_0050A5B0(int flags);
void fn_0050A5A0(char* text, int kind);
int fn_004302B0(void);
int fn_004D6340(int id, int path);

void fn_00401380(int arg)
{
	unsigned char flags;

	flags = *(unsigned char*)0x005FBFD8;
	*(int*)0x005B2800 = 0;
	if (flags & 1)
	{
		if (!fn_00432DA0())
		{
			*(int*)0x005B2800 |= 1;
			fn_00432660(0, 0);
		}
	}
	else
	{
		fn_0041E380();
	}
	fn_004013F0(arg);
	*(int*)0x005B28B8 = 0;
	fn_004016D0();
	*(int*)0x005B29D4 = 0;
	*(int*)0x005B2804 = -1;
}

void fn_004017E0(void)
{
	if (*(unsigned char*)0x005B2800 & 1)
		fn_004326C0();
}

int fn_004017F0(int arg)
{
	int* rec;
	int step;

	for (rec = (int*)0x005B2810; rec < (int*)0x005B28B8; rec = (int*)((char*)rec + 0x18))
		*rec &= ~2;
	step = *(int*)0x005B29D4;
	switch (step)
	{
	case 0:
		return fn_00401840(arg);
	case 1:
		fn_00401B20();
		return arg;
	case 2:
		fn_00401BF0();
		return arg;
	default:
		return arg;
	}
}

void fn_00401A20(void)
{
	int cursor;
	int step;

	cursor = *(int*)0x005B28B8;
	if (cursor <= 0)
		return;
	step = *(int*)(*(int*)0x0060AD68 * 4 + 0x0057E6A4);
	cursor -= step;
	if (cursor < 0)
		cursor = 0;
	*(int*)0x005B28B8 = cursor;
	fn_004016D0();
}

void fn_00401A50(void)
{
	int step;
	int cursor;
	int limit;
	int next;

	step = *(int*)(*(int*)0x0060AD68 * 4 + 0x0057E6A4);
	cursor = *(int*)0x005B28B8;
	limit = *(int*)0x005B28CC - step;
	if (cursor >= limit)
		return;
	next = cursor + step;
	if (next < limit)
		limit = next;
	*(int*)0x005B28B8 = limit;
	fn_004016D0();
}

void fn_00401AD0(void)
{
	int cursor;

	cursor = *(int*)0x005B28B8;
	if (cursor <= 0)
		return;
	*(int*)0x005B28B8 = cursor - 1;
	fn_004016D0();
}

void fn_00401AF0(void)
{
	int step;
	int limit;
	int cursor;

	step = *(int*)(*(int*)0x0060AD68 * 4 + 0x0057E6A4);
	limit = *(int*)0x005B28CC - step;
	cursor = *(int*)0x005B28B8;
	if (cursor >= limit)
		return;
	*(int*)0x005B28B8 = cursor + 1;
	fn_004016D0();
}

void fn_00401A80(int arg)
{
	switch (arg - 2)
	{
	case 0:
		fn_00401AD0();
		fn_00468D90(0x25, 1.0f, 0, 0, 0);
		break;
	case 2:
		fn_00401AF0();
		fn_00468D90(0x25, 1.0f, 0, 0, 0);
		break;
	case 4:
		fn_00416A10();
		fn_00468D90(0x24, 1.0f, 0, 0, 0);
		break;
	default:
		break;
	}
}

int fn_00401E50(int a, int b)
{
	return fn_00413460(
		a,
		b,
		*(int*)0x005B2818,
		*(int*)0x005B281C,
		*(int*)0x005B2820,
		*(int*)0x005B2824);
}

void fn_004011B0(void)
{
	((ConsoleCmd*)0x005B28BC)->fn_005019E0();
}

void fn_00401E80(void)
{
	((ConsoleCmd*)0x005B2B38)->fn_005019E0();
}

void fn_00401EA0(void)
{
	((ColorPair*)0x005B29E0)->fn_005067F0(0xFF, 0, 0, 0xFF);
	((ColorPair*)0x005B29E4)->fn_005067F0(0, 0xFF, 0, 0xFF);
}

void fn_004044D0(void);
void fn_004045C0(void);
void fn_004046B0(void);
void fn_004047A0(void);

void fn_004044A0(void)
{
	((ConsoleCmd*)0x005B2DA8)->fn_0050A180((char*)0x00588C14, (char*)0x00588C20, fn_004044D0);
}

void fn_004044D0(void)
{
	unsigned char arg;

	if (*(int*)0x025DB218)
	{
		fn_0050A5B0(0xC1);
		arg = *(unsigned char*)0x025DAE04;
		if (arg & 0x40)
			*(unsigned char*)0x00588BC4 = 1;
		else if (arg & 0x80)
			*(unsigned char*)0x00588BC4 = 0;
		else if (arg & 1)
			*(unsigned char*)0x00588BC4 ^= 1;
	}
	if (*(int*)0x025DB2E0)
	{
		sprintf((char*)0x0257ED80, (char*)0x00588C50, 0x00588C14, 0x00588CA0);
		fn_0050A5A0((char*)0x0257ED80, 0);
	}
	if (*(int*)0x025DADF0)
	{
		sprintf((char*)0x0257ED80, (char*)0x00588C34, 0x00588C14, *(unsigned char*)0x00588BC4 ? 0x00588C48 : 0x00588C40);
		fn_0050A5A0((char*)0x0257ED80, 0);
	}
}

void fn_00404590(void)
{
	((ConsoleCmd*)0x005B2A00)->fn_0050A180((char*)0x00588CAC, (char*)0x00588CB8, fn_004045C0);
}

void fn_004045C0(void)
{
	unsigned char arg;

	if (*(int*)0x025DB218)
	{
		fn_0050A5B0(0xC1);
		arg = *(unsigned char*)0x025DAE04;
		if (arg & 0x40)
			*(unsigned char*)0x005B2DB4 = 1;
		else if (arg & 0x80)
			*(unsigned char*)0x005B2DB4 = 0;
		else if (arg & 1)
			*(unsigned char*)0x005B2DB4 ^= 1;
	}
	if (*(int*)0x025DB2E0)
	{
		sprintf((char*)0x0257ED80, (char*)0x00588C50, 0x00588CAC, 0x00588CCC);
		fn_0050A5A0((char*)0x0257ED80, 0);
	}
	if (*(int*)0x025DADF0)
	{
		sprintf((char*)0x0257ED80, (char*)0x00588C34, 0x00588CAC, *(unsigned char*)0x005B2DB4 ? 0x00588C48 : 0x00588C40);
		fn_0050A5A0((char*)0x0257ED80, 0);
	}
}

void fn_00404680(void)
{
	((ConsoleCmd*)0x005B2B60)->fn_0050A180((char*)0x00588CD8, (char*)0x00588CEC, fn_004046B0);
}

void fn_004046B0(void)
{
	unsigned char arg;

	if (*(int*)0x025DB218)
	{
		fn_0050A5B0(0xC1);
		arg = *(unsigned char*)0x025DAE04;
		if (arg & 0x40)
			*(unsigned char*)0x00588BD0 = 1;
		else if (arg & 0x80)
			*(unsigned char*)0x00588BD0 = 0;
		else if (arg & 1)
			*(unsigned char*)0x00588BD0 ^= 1;
	}
	if (*(int*)0x025DB2E0)
	{
		sprintf((char*)0x0257ED80, (char*)0x00588C50, 0x00588CD8, 0x00588D08);
		fn_0050A5A0((char*)0x0257ED80, 0);
	}
	if (*(int*)0x025DADF0)
	{
		sprintf((char*)0x0257ED80, (char*)0x00588C34, 0x00588CD8, *(unsigned char*)0x00588BD0 ? 0x00588C48 : 0x00588C40);
		fn_0050A5A0((char*)0x0257ED80, 0);
	}
}

void fn_00404770(void)
{
	((ConsoleCmd*)0x005B2CA8)->fn_0050A180((char*)0x00588D1C, (char*)0x00588D30, fn_004047A0);
}

void fn_004047A0(void)
{
	int id;
	int value;

	if (*(int*)0x025DB2E0)
	{
		fn_0050A5A0((char*)0x00588DEC, 0);
		return;
	}
	if (*(int*)0x025DB218 == 0)
		return;
	if (fn_00401040() != 4)
	{
		fn_0050A5A0((char*)0x00588DC4, 0);
		return;
	}
	if (*(int*)0x005B2A10)
	{
		fn_0050A5A0((char*)0x00588D94, 0);
		return;
	}
	if (fn_004302B0())
	{
		fn_0050A5A0((char*)0x00588D74, 0);
		return;
	}
	id = *(int*)0x0059F120;
	value = fn_004D6340(id, 0x0060A2E8);
	((ConsoleCmd*)0x02493A38)->fn_004D14F0(id, value);
	fn_00402490();
	fn_00402280(-1);
	fn_0050A5A0((char*)0x00588D5C, 0);
}

void fn_00404860(void)
{
}

typedef void (__fastcall *StepFn)(void* item);

void __stdcall fn_00404880(char* item, int stride, int count, StepFn step)
{
	if (count <= 0)
		return;
	do
	{
		step(item);
		item += stride;
	} while (--count);
}

}

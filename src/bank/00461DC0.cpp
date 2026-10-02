// One pass of bank/00461DC0. Strings in this block: Sets Chance_to_drop_gold, chance_gold, kills off friendly summoned creature, kill_summon, Toggles No_resist, no_resist, Toggles Use_spell_ap, Spell_ap.

struct ConsoleCmd {
	void fn_0050A180(const char* name, const char* help, void (*handler)(void));
};

extern "C" {

void fn_00463050(void)
{
	((ConsoleCmd*)0x8eca30)->fn_0050A180((char*)0x59a09c, (char*)0x59a0a8, (void (*)(void))0x463080);
}

void fn_00463110(void)
{
	((ConsoleCmd*)0x8eca50)->fn_0050A180((char*)0x59a0d8, (char*)0x59a0e4, (void (*)(void))0x463140);
}

void fn_00463180(void)
{
	((ConsoleCmd*)0x940428)->fn_0050A180((char*)0x59a10c, (char*)0x59a118, (void (*)(void))0x4631b0);
}

void fn_00463270(void)
{
	((ConsoleCmd*)0x8eca40)->fn_0050A180((char*)0x59a138, (char*)0x59a144, (void (*)(void))0x4632a0);
}

void fn_00463360(void)
{
	((ConsoleCmd*)0x941730)->fn_0050A180((char*)0x59a170, (char*)0x59a17c, (void (*)(void))0x463390);
}

void fn_00463450(void)
{
	((ConsoleCmd*)0x941710)->fn_0050A180((char*)0x597834, (char*)0x59a19c, (void (*)(void))0x463480);
}

void fn_00464010(void)
{
	((ConsoleCmd*)0x941720)->fn_0050A180((char*)0x59a1e8, (char*)0x59a1f4, (void (*)(void))0x464040);
}

void* fn_0046B1A0(int id);

}

// The actor's link id is at +0x6e0 and its flag dword is at +0x6e4.
struct Obj62D20 {
	char pad[0x6e0];
	int link;
	int flags;

	int fn_00462D20(void);
	int fn_00462D40(void);
	int fn_00462D60(void);
};

int Obj62D20::fn_00462D20(void)
{
	unsigned char* other = (unsigned char*)fn_0046B1A0(link);
	if (other != 0 && (other[0x6e4] & 2))
		return (int)other;
	return 0;
}

int Obj62D20::fn_00462D40(void)
{
	unsigned char* other = (unsigned char*)fn_0046B1A0(link);
	if (other != 0 && (other[0x6e4] & 1))
		return (int)other;
	return 0;
}

int Obj62D20::fn_00462D60(void)
{
	return (flags >> 1) & 1;
}

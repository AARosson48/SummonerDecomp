// One pass of bank/004FCE40. Strings in this block: reinits worldmap, iw, Toggles Random_encounters_enabled, random_encounters, masad_v2, worldmap1, Barbarian Fighter#$npc037, Nar-Leaving-Masad.

struct ConsoleCmd {
	void fn_0050A180(const char* name, const char* help, void (*handler)(void));
};

extern "C" {

void fn_004FD5C0(void)
{
	((ConsoleCmd*)0x257ed50)->fn_0050A180((char*)0x5a4f3c, (char*)0x5a4f40, (void (*)(void))0x4fd5f0);
}

void fn_004FD600(void)
{
	((ConsoleCmd*)0x257ed10)->fn_0050A180((char*)0x5a4f54, (char*)0x5a4f68, (void (*)(void))0x4fd630);
}

}

// One pass of bank/00449B00. Strings in this block: dump info on all characters in the game, charinfo, dump info on all characters in the level, levelchars.

struct ConsoleCmd {
	void fn_0050A180(const char* name, const char* help, void (*handler)(void));
};

extern "C" {

void fn_00449BA0(void)
{
	((ConsoleCmd*)0x65e968)->fn_0050A180((char*)0x598e00, (char*)0x598e0c, (void (*)(void))0x4f13e0);
}

void fn_00449BD0(void)
{
	((ConsoleCmd*)0x8b3c38)->fn_0050A180((char*)0x598e34, (char*)0x598e40, (void (*)(void))0x449c00);
}

}

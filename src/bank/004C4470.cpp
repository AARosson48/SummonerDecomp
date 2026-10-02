// One pass of bank/004C4470. Strings in this block: Start battle with tentacle beasts, tentacles, Makes tentacles go in and out of water quickly -- no attacks, tentacle_test, Set Giant salamanka's current combat stage, gs_stage, j, Sets gsal in combat state.

struct ConsoleCmd {
	void fn_0050A180(const char* name, const char* help, void (*handler)(void));
};

extern "C" {

void fn_004C7C90(void)
{
	((ConsoleCmd*)0x248c788)->fn_0050A180((char*)0x59de34, (char*)0x59de40, (void (*)(void))0x4c7cc0);
}

void fn_004C7E60(void)
{
	((ConsoleCmd*)0x248c778)->fn_0050A180((char*)0x59de6c, (char*)0x59de7c, (void (*)(void))0x4c7e90);
}

void fn_004C8070(void)
{
	((ConsoleCmd*)0x248c7c0)->fn_0050A180((char*)0x59dec0, (char*)0x59decc, (void (*)(void))0x4c80a0);
}

void fn_004C80D0(void)
{
	((ConsoleCmd*)0x248c7b0)->fn_0050A180((char*)0x59def8, (char*)0x59df04, (void (*)(void))0x4c8100);
}

}

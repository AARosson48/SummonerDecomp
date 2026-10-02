// One pass of bank/004B86E0. Strings in this block: Set Pyrul's current combat stage, pyrul_stage, j, Sets pyrul in combat state, pyrul_combat, Set Ghost rider's current combat stage, ghost_stage, Unlinks ghost from mount.

struct ConsoleCmd {
	void fn_0050A180(const char* name, const char* help, void (*handler)(void));
};

extern "C" {

void fn_004B8820(void)
{
	((ConsoleCmd*)0x247fcf0)->fn_0050A180((char*)0x59d934, (char*)0x59d940, (void (*)(void))0x4b8850);
}

void fn_004B8890(void)
{
	((ConsoleCmd*)0x247fd18)->fn_0050A180((char*)0x59d980, (char*)0x59d990, (void (*)(void))0x4b88c0);
}

void fn_004B9E70(void)
{
	((ConsoleCmd*)0x2480928)->fn_0050A180((char*)0x59d9ec, (char*)0x59d9f8, (void (*)(void))0x4b9ea0);
}

void fn_004B9ED0(void)
{
	((ConsoleCmd*)0x2480910)->fn_0050A180((char*)0x59da20, (char*)0x59da30, (void (*)(void))0x4b9f00);
}

void fn_004B9FC0(void);
void fn_0050A5A0(char* text, int kind);

void fn_004B9F90(void)
{
	((ConsoleCmd*)0x2480300)->fn_0050A180((char*)0x59da4c, (char*)0x59da5c, fn_004B9FC0);
}

void fn_004B9FC0(void)
{
	*(unsigned char*)0x248031a = 1;
	fn_0050A5A0((char*)0x59da80, 0);
}

void fn_004BA020(void)
{
	((ConsoleCmd*)0x2480938)->fn_0050A180((char*)0x59daa4, (char*)0x59dab0, (void (*)(void))0x4ba050);
}

}

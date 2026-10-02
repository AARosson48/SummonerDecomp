// One pass of bank/004954A0. Strings in this block: Set phoenix rider's current combat stage, phoenix_stage, Unlinks phoenix to mount, phoenix_unlink, phoenix_solo, j, Sets phoenix rider in combat state, phoenix_combat.

struct ConsoleCmd {
	void fn_0050A180(const char* name, const char* help, void (*handler)(void));
};

extern "C" {

void fn_00497AC0(void)
{
	((ConsoleCmd*)0x24529f8)->fn_0050A180((char*)0x59c80c, (char*)0x59c81c, (void (*)(void))0x497af0);
}

void fn_00497B20(void)
{
	((ConsoleCmd*)0x2452a58)->fn_0050A180((char*)0x59c848, (char*)0x59c858, (void (*)(void))0x497b50);
}

void fn_00497C20(void)
{
	((ConsoleCmd*)0x2452a38)->fn_0050A180((char*)0x59c874, (char*)0x59c884, (void (*)(void))0x497c50);
}

void fn_00497CA0(void);
void fn_0050A5A0(char* text, int kind);

void fn_00497C70(void)
{
	((ConsoleCmd*)0x2452a48)->fn_0050A180((char*)0x59c8a0, (char*)0x59c8b0, fn_00497CA0);
}

void fn_00497CA0(void)
{
	*(unsigned char*)0x245242a = 1;
	fn_0050A5A0((char*)0x59c8d4, 0);
}

}

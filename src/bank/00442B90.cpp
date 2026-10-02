// One pass of bank/00442B90. Strings in this block: Toggles All_summons, All_summons, j.

struct ConsoleCmd {
	void fn_0050A180(const char* name, const char* help, void (*handler)(void));
};

extern "C" {

void fn_00444A20(void)
{
	((ConsoleCmd*)0x68e200)->fn_0050A180((char*)0x59814c, (char*)0x598158, (void (*)(void))0x444a50);
}

}

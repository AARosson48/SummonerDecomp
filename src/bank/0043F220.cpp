// One pass of bank/0043F220. Strings in this block: **, * , Load a monster layout file, load_mlayout, Save a monster layout file, save_mlayout, j, Toggle whether net rate info for a packet is displayed.

struct ConsoleCmd {
	void fn_0050A180(const char* name, const char* help, void (*handler)(void));
};

extern "C" {

void fn_00440170(void)
{
	((ConsoleCmd*)0x60f928)->fn_0050A180((char*)0x595044, (char*)0x595054, (void (*)(void))0x4401a0);
}

void fn_004402C0(void)
{
	((ConsoleCmd*)0x60f4f0)->fn_0050A180((char*)0x5950c4, (char*)0x5950d4, (void (*)(void))0x4402f0);
}

void fn_00442350(void)
{
	((ConsoleCmd*)0x65e0c0)->fn_0050A180((char*)0x595988, (char*)0x595994, (void (*)(void))0x442380);
}

}

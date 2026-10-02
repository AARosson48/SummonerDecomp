// One pass of bank/004DF6E0. Strings in this block: Remove redundant $npc navpoints, remove_nav.

struct ConsoleCmd {
	void fn_0050A180(const char* name, const char* help, void (*handler)(void));
};

extern "C" {

void fn_004E0040(void)
{
	((ConsoleCmd*)0x254a8a0)->fn_0050A180((char*)0x5a0908, (char*)0x5a0914, (void (*)(void))0x4e0070);
}

}

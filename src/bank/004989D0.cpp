// One pass of bank/004989D0. Strings in this block: Set Tiger rider's current combat stage, tiger_stage, Unlinks tiger to mount, tiger_unlink, j, Sets tiger in combat state, tiger_combat, tiger_solo.

struct ConsoleCmd {
	void fn_0050A180(const char* name, const char* help, void (*handler)(void));
};

extern "C" {

void fn_00499E90(void)
{
	((ConsoleCmd*)0x2452a80)->fn_0050A180((char*)0x59c980, (char*)0x59c98c, (void (*)(void))0x499ec0);
}

void fn_00499F00(void)
{
	((ConsoleCmd*)0x2452ca8)->fn_0050A180((char*)0x59c9d4, (char*)0x59c9e4, (void (*)(void))0x499f30);
}

void fn_00499FC0(void)
{
	((ConsoleCmd*)0x2452c98)->fn_0050A180((char*)0x59c9fc, (char*)0x59ca0c, (void (*)(void))0x499ff0);
}

void fn_0049A050(void)
{
	((ConsoleCmd*)0x2452a70)->fn_0050A180((char*)0x59ca4c, (char*)0x59ca58, (void (*)(void))0x49a080);
}

void fn_0049C840(void)
{
	((ConsoleCmd*)0x2452d40)->fn_0050A180((char*)0x59cb2c, (char*)0x59cb40, (void (*)(void))0x49c870);
}

}

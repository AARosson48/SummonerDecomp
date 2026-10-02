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

void fn_00499FF0(void);
void fn_0050A5A0(char* text, int kind);

void fn_00499FC0(void)
{
	((ConsoleCmd*)0x2452c98)->fn_0050A180((char*)0x59c9fc, (char*)0x59ca0c, fn_00499FF0);
}

void fn_00499FF0(void)
{
	*(unsigned char*)0x2452a9a = 1;
	fn_0050A5A0((char*)0x59ca28, 0);
}

void fn_0049A050(void)
{
	((ConsoleCmd*)0x2452a70)->fn_0050A180((char*)0x59ca4c, (char*)0x59ca58, (void (*)(void))0x49a080);
}

void fn_0049C840(void)
{
	((ConsoleCmd*)0x2452d40)->fn_0050A180((char*)0x59cb2c, (char*)0x59cb40, (void (*)(void))0x49c870);
}

// Index of the pointer at 0x2452c40 inside the 0x20-byte table at 0x2452b80.
// Six entries. Anything outside that range is -1.
int fn_0049A0A0(void)
{
	int ptr = *(int*)0x2452c40;
	int index;
	if (ptr == 0)
		return -1;
	index = (ptr - 0x2452b80) >> 5;
	if (index < 0 || index >= 6)
		return -1;
	return index;
}

}

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

// Two character pools. ecx is the pool object; the three pushes are the
// buffer, the byte count, and a flags dword.
struct Obj44DE0 {
	void fn_00544DE0(void* mem, int bytes, int flags);
};

extern "C" void fn_00444B20(void)
{
	((Obj44DE0*)0x68e1a8)->fn_00544DE0((void*)0x74469c, 0x14c00, 0);
}

extern "C" void fn_00444B70(void)
{
	((Obj44DE0*)0x68e1c0)->fn_00544DE0((void*)0x7dfeb8, 0x1400, 0);
}

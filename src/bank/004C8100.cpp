// One pass of bank/004C8100. Strings in this block: Set Luminar's current combat stage, stage, Toggles Dont_load_fx, dont_load_fx, Toggles Do_frame_callback, do_frame_callback, Saves the EAX properties to disk, eax_save_props.

struct ConsoleCmd {
	void fn_0050A180(const char* name, const char* help, void (*handler)(void));
};

extern "C" {

void fn_004C8D60(void)
{
	((ConsoleCmd*)0x248ce30)->fn_0050A180((char*)0x59c798, (char*)0x59dfd4, (void (*)(void))0x4c8d90);
}

void fn_004CA760(void)
{
	((ConsoleCmd*)0x2490c80)->fn_0050A180((char*)0x59e130, (char*)0x59e140, (void (*)(void))0x4ca790);
}

void fn_004CA850(void)
{
	((ConsoleCmd*)0x2490c68)->fn_0050A180((char*)0x59e168, (char*)0x59e17c, (void (*)(void))0x4ca880);
}

void fn_004CA940(void)
{
	((ConsoleCmd*)0x24902b8)->fn_0050A180((char*)0x59e1ac, (char*)0x59e1bc, (void (*)(void))0x4ca970);
}

}

struct Obj4902A8 {
	char fn_00501910(void);
	void fn_00501880(int count);
};

// If the object at 0x248ce28 reports ready, arm it for 0x1388 ticks.
extern "C" int fn_004CA5C0(void)
{
	if (((Obj4902A8*)0x248ce28)->fn_00501910() == 1) {
		((Obj4902A8*)0x248ce28)->fn_00501880(0x1388);
		return 1;
	}
	return 0;
}

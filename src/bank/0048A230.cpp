// One pass of bank/0048A230. Strings in this block: Sets Time_mult, time_mult, Sets Space_mult, space_mult, Sets Small_mesh_threshold, small_mesh_threshold.

struct ConsoleCmd {
	void fn_0050A180(const char* name, const char* help, void (*handler)(void));
};

extern "C" {

void fn_0048DC20(void)
{
	((ConsoleCmd*)0x23fc838)->fn_0050A180((char*)0x59c488, (char*)0x59c494, (void (*)(void))0x48dc50);
}

void fn_0048DCE0(void)
{
	((ConsoleCmd*)0x2432478)->fn_0050A180((char*)0x59c4b0, (char*)0x59c4bc, (void (*)(void))0x48dd10);
}

void fn_0048DDA0(void)
{
	((ConsoleCmd*)0x23f81d8)->fn_0050A180((char*)0x59c4d8, (char*)0x59c4f0, (void (*)(void))0x48ddd0);
}

}

// One pass of bank/004A1AC0. Strings in this block: 333@333@, Set the height level to draw obstacles relative to main player pos, obstacle_height_offset.

struct ConsoleCmd {
	void fn_0050A180(const char* name, const char* help, void (*handler)(void));
};

extern "C" {

void fn_004A3DE0(void)
{
	((ConsoleCmd*)0x245cd90)->fn_0050A180((char*)0x59cdac, (char*)0x59cdc4, (void (*)(void))0x4a3e10);
}

}

// One pass of bank/004A5750. Strings in this block: Sets avoidance area parameters, avoidance.

struct ConsoleCmd {
	void fn_0050A180(const char* name, const char* help, void (*handler)(void));
};

extern "C" {

void fn_004A67E0(void)
{
	((ConsoleCmd*)0x246b918)->fn_0050A180((char*)0x59ce98, (char*)0x59cea4, (void (*)(void))0x4a6810);
}

}

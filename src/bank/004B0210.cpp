// One pass of bank/004B0210. Strings in this block: Toggles Use_turn_anims, Turn_anims, Set the turn scaler, turn_scaler.

struct ConsoleCmd {
	void fn_0050A180(const char* name, const char* help, void (*handler)(void));
};

extern "C" {

void fn_004B1410(void)
{
	((ConsoleCmd*)0x247e890)->fn_0050A180((char*)0x59d714, (char*)0x59d720, (void (*)(void))0x4b1440);
}

void fn_004B1500(void)
{
	((ConsoleCmd*)0x247e880)->fn_0050A180((char*)0x59d748, (char*)0x59d754, (void (*)(void))0x4b1530);
}

}

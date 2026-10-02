// One pass of bank/00561900. Strings in this block: Toggles Dump_mesh_frames, dump_mesh_frames.

struct ConsoleCmd {
	void fn_0050A180(const char* name, const char* help, void (*handler)(void));
};

extern "C" {

void fn_00562A30(void)
{
	((ConsoleCmd*)0x2f91a60)->fn_0050A180((char*)0x5ae120, (char*)0x5ae104, (void (*)(void))0x562a60);
}

}

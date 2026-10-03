// Original: D:\projects\Summoner\pccode\levelscripts\conversation.cpp
// Allocates the conversation buffer on the object at +0x210.

struct ConvStore {
	int fn_00532420(int size, int flags);
};

struct ConvHost {
	int fn_004D7C20(void);
};

extern "C" void fn_004F13E0(void);
extern "C" void fn_00531CB0(const char* file, int line, const char* message);

int ConvHost::fn_004D7C20(void)
{
	if (((ConvStore*)((char*)this + 0x210))->fn_00532420(0x20000, 0)) {
		for (;;)
			fn_00531CB0((const char*)0x5a0250, 0x131, (const char*)0x5a028c);
	}
	fn_004F13E0();
	*(int*)((char*)this + 0x22c) = -1;
	return 0;
}

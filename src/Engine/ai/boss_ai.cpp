// Original: D:\projects\Summoner\pccode\Engine\ai\boss_ai.cpp
// Kind is actor+0x10. The byte table at 0x4A1714 selects the range.

extern "C" float fn_004A16A0(void* actor)
{
	unsigned int index = *(unsigned int*)((char*)actor + 0x10) - 0x51;
	if (index > 0x78)
		return *(float*)0x57e990;
	switch (((unsigned char*)0x4a1714)[index]) {
	case 0:
		return *(float*)0x581b68;
	case 1:
		return ((float*)0x59d7c8)[*(int*)((char*)actor + 0x90)];
	case 2:
		return *(float*)0x57ef18;
	case 3:
		return *(float*)0x5819a0;
	case 4:
		return *(float*)0x581918;
	case 5:
		return *(float*)0x580200;
	default:
		return *(float*)0x57e990;
	}
}

// Original: D:\projects\Summoner\pccode\Engine\objects\item_paging.cpp

extern "C" void fn_00531CB0(const char* file, int line, const char* message);

extern "C" int fn_0046CB50(void* actor, int* out_a, int* out_b)
{
	if (*(int*)0x5fbfd8 & 2) {
		if (*(int*)((char*)actor + 0xc) != 7) {
			*out_a = -1;
			*out_b = -1;
			return -1;
		}
		int** cursor = *(int***)0x5fc350;
		int slot = 0;
		while (cursor != (int**)0x5fc350) {
			if (cursor[3] == (int*)*(int*)((char*)actor + 0x80))
				break;
			cursor = (int**)*cursor;
			slot++;
			if (cursor == (int**)0x5fc350)
				break;
		}
		if (slot >= 4) {
			for (;;)
				fn_00531CB0("D:\\projects\\Summoner\\pccode\\Engine\\objects\\item_paging.cpp", 0x29f, "Unable to find paging slot for player\n");
		}
		*out_a = slot * 2;
		*out_b = slot * 2 + 1;
		return slot * 2 + 1;
	}
	int kind = *(int*)(*(int*)((char*)actor + 0x71c) + 0x10);
	if (kind > 3) {
		*out_a = -1;
		*out_b = -1;
		return kind;
	}
	switch (kind) {
	case 0:
		*out_a = 0;
		*out_b = 1;
		break;
	case 1:
		*out_a = 2;
		*out_b = 3;
		break;
	case 2:
		*out_a = 4;
		*out_b = 5;
		break;
	case 3:
		*out_a = 6;
		*out_b = 7;
		break;
	}
	return kind;
}

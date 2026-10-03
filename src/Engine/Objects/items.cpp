// Original: D:\projects\Summoner\pccode\Engine\Objects\items.cpp

extern "C" int printf(const char* fmt, ...);
extern "C" void fn_0046C8F0();
extern "C" int fn_0044B450(const char* name);
extern "C" int fn_0046C940(int item, int page);
extern "C" int fn_0046CB50(void* actor, int* out_a, int* out_b);
extern "C" void fn_00531CB0(const char* file, int line, const char* message);

static const char* items_file = "D:\\projects\\Summoner\\pccode\\Engine\\Objects\\items.cpp";

static int item_index(int* item)
{
	return (item[1] - 0x8cbec0) >> 8;
}

extern "C" int fn_0044D540()
{
	int* actor;
	printf("============================== ITEM LEVEL LOAD!\n");
	fn_0046C8F0();
	int backpack = fn_0044B450("Backpack");
	if (backpack < 0) {
		for (;;)
			fn_00531CB0(items_file, 0x924, "Cannot find backpack!");
	}
	if (!fn_0046C940(backpack, 0x11)) {
		for (;;)
			fn_00531CB0(items_file, 0x927, "Cannot page backpack!");
	}
	int bag = fn_0044B450("Leather Bag");
	if (bag < 0) {
		for (;;)
			fn_00531CB0(items_file, 0x92b, "Cannot find leather bag!");
	}
	if (!fn_0046C940(bag, 0x11)) {
		for (;;)
			fn_00531CB0(items_file, 0x92e, "Cannot page leather bag!");
	}
	int gold = fn_0044B450("Gold");
	if (gold < 0) {
		for (;;)
			fn_00531CB0(items_file, 0x932, "Cannot find gold!");
	}
	if (!fn_0046C940(gold, 0x11)) {
		for (;;)
			fn_00531CB0(items_file, 0x935, "Cannot page gold!");
	}
	for (actor = *(int**)0x8ecd78; actor != (int*)0x8eca60; actor = *(int**)((char*)actor + 0x318)) {
		int flags = *(int*)((char*)actor + 0x320);
		if (flags & 0x100)
			continue;
		int kind = *(int*)(*(int*)((char*)actor + 0x71c) + 0x10);
		if (kind >= 0 && kind <= 3)
			continue;
		int* slot = (int*)((char*)actor + 0x754);
		int left = 0xa;
		int step;
		do {
			if (*slot)
				fn_0046C940(item_index((int*)*slot), 0x11);
			slot++;
			step = left;
			left--;
		} while (step != 1);
		if (*(int*)(*(int*)((char*)actor + 0x71c) + 0x10) == 0x57)
			fn_0046C940(fn_0044B450("TigerAxe"), 0x11);
	}
	for (actor = *(int**)0x8ecd78; actor != (int*)0x8eca60; actor = *(int**)((char*)actor + 0x318)) {
		int flags = *(int*)((char*)actor + 0x320);
		int page_a;
		int page_b;
		if ((flags & 0x100) == 0) {
			fn_0046CB50(actor, &page_a, &page_b);
			if (page_a != -1) {
				int* left_hand = *(int**)((char*)actor + 0x768);
				if (left_hand)
					fn_0046C940(item_index(left_hand), page_a);
				int* right_hand = *(int**)((char*)actor + 0x76c);
				if (right_hand)
					fn_0046C940(item_index(right_hand), page_b);
			}
		} else {
			int* left_hand = *(int**)((char*)actor + 0x768);
			if (left_hand)
				fn_0046C940(item_index(left_hand), 8);
			if (*(int*)((char*)actor + 0x778))
				fn_0046C940(item_index(*(int**)((char*)actor + 0x768)), 9);
			if (*(int*)(*(int*)((char*)actor + 0x71c) + 0x10) == 0xce)
				fn_0046C940(*(int*)((char*)actor + 0x730), 9);
		}
	}
	for (actor = *(int**)0x8ed500; actor != (int*)0x8ed1e8; actor = *(int**)((char*)actor + 0x318)) {
		int page_a;
		int page_b;
		fn_0046CB50(actor, &page_b, &page_a);
		if (page_b != -1) {
			int* left_hand = *(int**)((char*)actor + 0x768);
			if (left_hand)
				fn_0046C940(item_index(left_hand), page_b);
			int* right_hand = *(int**)((char*)actor + 0x76c);
			if (right_hand)
				fn_0046C940(item_index(right_hand), page_a);
		}
	}
	return 0;
}

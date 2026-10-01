// Leaf functions in bank/004CFF80. Each one is a load, a store, or a constant return.
// The save header lives in this bank. fn_004D1DF0 writes SAVg; fn_004D29F0 reads it.

#include <string.h>

struct SaveBuf {
	char pad[0xc];
	unsigned char* data;
	int capacity;

	int fn_004CEFF0(int* cursor);
	int fn_004CF260(int* cursor);
	int fn_004D1DF0(char* description);
	int fn_004D29F0(char* area, char* area_sub, int* field, int* time, char* description, int* version);
	int fn_004D2090();
	int fn_004D2C90();
};

extern "C" int fn_004D6300(char* name);
extern "C" int fn_004D6340(int id, char* name);
extern "C" void fn_0046BBB0();
extern "C" int fn_004D5F50(char* name);
extern "C" int fn_0044B4A0(int hash);
extern "C" void fn_0046B980(int index, int quantity, int charge, int flag);
extern "C" int printf(const char* text);

extern "C" int fn_004D25A0()
{
	return 1;
}

int SaveBuf::fn_004D1DF0(char* description)
{
	int cursor;
	unsigned char* buf;
	unsigned int length;

	cursor = 0;
	buf = data;
	*(int*)(buf + cursor) = 0x67564153;
	cursor += 4;
	*(int*)(buf + cursor) = 0x27;
	cursor += 4;
	memset(buf + cursor, 0, 0x80);
	length = strlen((char*)0x60A2C8);
	memcpy(buf + cursor, (char*)0x60A2C8, length);
	cursor += 0x80;
	memset(buf + cursor, 0, 0x20);
	length = strlen((char*)0x60A2E8);
	memcpy(buf + cursor, (char*)0x60A2E8, length);
	cursor += 0x20;
	*(int*)(buf + cursor) = 0;
	cursor += 4;
	*(int*)(buf + cursor) = *(int*)0x60A654;
	cursor += 4;
	length = strlen(description);
	if (length > 0xff) {
		return 0;
	}
	memset(buf + cursor, 0, 0x100);
	memcpy(buf + cursor, description, length);
	cursor += 0x100;
	fn_004CEFF0(&cursor);
	*(int*)(buf + cursor) = cursor + 4;
	return cursor + 4 <= capacity;
}

int SaveBuf::fn_004D29F0(char* area, char* area_sub, int* field, int* time, char* description, int* version)
{
	unsigned char* buf;
	int cursor;
	int file_version;
	int id;
	int stored;

	buf = data;
	if (*(int*)buf != 0x67564153) {
		return -1;
	}
	file_version = *(int*)(buf + 4);
	*(int*)0x24E3B3C = file_version;
	if (file_version != 0x27) {
		return -1;
	}
	if (version) {
		*version = file_version;
	}
	strcpy(area, (char*)(buf + 8));
	strcpy(area_sub, (char*)(buf + 0x88));
	id = fn_004D6300(area_sub);
	if (id == -1) {
		return -1;
	}
	if (fn_004D6340(id, area_sub) < 0) {
		return -1;
	}
	cursor = 0xa8;
	*field = *(int*)(buf + cursor);
	cursor += 4;
	*time = *(int*)(buf + cursor);
	cursor += 4;
	strcpy(description, (char*)(buf + cursor));
	cursor += 0x100;
	if (!fn_004CF260(&cursor)) {
		return 0;
	}
	stored = *(int*)(buf + (cursor - 4));
	cursor += 4;
	return cursor == stored;
}

int SaveBuf::fn_004D2090()
{
	unsigned char* buf;
	int count;
	int cursor;
	int i;

	buf = *(unsigned char**)((char*)this + 0x101c);
	*(int*)buf = 0x54564e49;
	*(int*)(buf + 4) = *(int*)0x971868;
	count = *(int*)0x97186C;
	if (count < 0) {
		count = 0;
	} else if (count > 0x209) {
		count = 0x209;
	}
	*(int*)(buf + 8) = count;
	cursor = 0xc;
	for (i = 0; i < count; i++) {
		int* slot = (int*)(0x970824 + i * 8);
		int* object = (int*)slot[0];
		*(int*)(buf + cursor) = fn_004D5F50(*(char**)object[1]);
		cursor += 4;
		*(int*)(buf + cursor) = *(int*)((char*)slot - 4);
		cursor += 4;
		*(int*)(buf + cursor) = object[2];
		cursor += 4;
	}
	*(int*)(buf + cursor) = cursor + 4;
	return cursor + 4 <= *(int*)((char*)this + 0x1020);
}

int SaveBuf::fn_004D2C90()
{
	unsigned char* buf;
	int count;
	int cursor;
	int index;
	int hash;
	int quantity;
	int charge;
	int brown;
	int gray;
	int ring;
	int preservation;
	int found;

	fn_0046BBB0();
	buf = *(unsigned char**)((char*)this + 0x101c);
	if (*(int*)buf != 0x54564e49) {
		return 0;
	}
	*(int*)0x971868 = *(int*)(buf + 4);
	count = *(int*)(buf + 8);
	cursor = 0xc;
	for (index = 0; index < count; index++) {
		hash = *(int*)(buf + cursor);
		cursor += 4;
		quantity = *(int*)(buf + cursor);
		cursor += 4;
		charge = *(int*)(buf + cursor);
		cursor += 4;
		brown = fn_004D5F50((char*)0x59EBF4);
		gray = fn_004D5F50((char*)0x59EBE0);
		ring = fn_004D5F50((char*)0x59EBC8);
		preservation = fn_004D5F50((char*)0x59EBB4);
		if (hash == brown) {
			hash = gray;
		}
		if (hash == ring) {
			hash = preservation;
		}
		found = fn_0044B4A0(hash);
		if (found < 0) {
			printf("Couldn't find item type!!!\n");
			return 0;
		}
		fn_0046B980(found, quantity, charge, 1);
	}
	cursor += 4;
	return cursor == *(int*)(buf + (cursor - 4));
}

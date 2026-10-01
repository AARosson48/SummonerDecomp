// Leaf functions in bank/004181D0. Each one is a load, a store, or a constant return.
// fn_004191B0 builds MultiTrade. fn_0041BEE0 builds PXO-LevelSelect.
// See notes/multiplayer.md.

#include <string.h>

struct UiSlot {
	int flags;
	int id;
	int x;
	int y;
	int w;
	int h;
};

extern "C" {

unsigned char fn_00419160()
{
	return *(unsigned char*)0x5c1518;
}

float fn_004134A0(void);
int fn_005026E0(char* name, int pack, int mode);
void fn_00503500(int id, int* w, int* h);
void fn_00503E80(int value);

static void load_ui_slot(UiSlot* slot, char* name, float scale, int mark)
{
	char temp[96];
	int res;
	char* suffix;
	int lang;

	slot->flags = 0;
	slot->id = -1;
	slot->x = 0;
	slot->y = 0;
	slot->w = 0;
	slot->h = 0;
	if (name[0] != 0) {
		res = *(int*)0x5BD318;
		suffix = *(char**)(0x58BDA4 + res * 4);
		strcpy(temp, name);
		strcat(temp, suffix);
		slot->id = fn_005026E0(temp, *(int*)0x60AC10, -1);
		lang = *(int*)0x60AD68;
		slot->x = *(int*)(name + lang * 8 + 0x24);
		slot->y = *(int*)(name + lang * 8 + 0x28);
		fn_00503500(slot->id, &slot->w, &slot->h);
		slot->w = (int)((float)slot->w * scale + 0.5f);
		slot->h = (int)((float)slot->h * scale + 0.5f);
	}
	if (mark) {
		slot->flags |= 1;
	}
}

static void load_ui_bar(int* id, int* w, int* h, unsigned int table, float scale)
{
	int res;
	char* name;

	res = *(int*)0x5BD318;
	name = *(char**)(table + res * 4);
	*id = fn_005026E0(name, *(int*)0x60AC10, -1);
	fn_00503500(*id, w, h);
	*w = (int)((float)*w * scale + 0.5f);
	*h = (int)((float)*h * scale + 0.5f);
}

void fn_004191B0(void)
{
	float scale;
	int i;

	scale = fn_004134A0();
	for (i = 0; i < 28; i++) {
		fn_00503E80(0);
		load_ui_slot((UiSlot*)0x5C1598 + i, (char*)(0x58CDA0 + i * 0x40), scale, i == 0 || i == 21);
	}
	fn_00503E80(0);
	load_ui_bar((int*)0x5C1848, (int*)0x5C184C, (int*)0x5C1850, 0x58D4A0, scale);
}

void fn_004185A0(void)
{
	float scale;
	int i;
	int image;

	scale = fn_004134A0();
	for (i = 0; i < 7; i++) {
		fn_00503E80(0);
		load_ui_slot((UiSlot*)0x5C1470 + i, (char*)(0x58CBD8 + i * 0x40), scale, i == 0);
	}
	fn_00503E80(0);
	image = fn_005026E0(*(char**)(*(int*)0x5BD318 * 4 + 0x58CD98), *(int*)0x60AC10, -1);
	*(int*)0x5C1558 = image;
	fn_00503500(image, (int*)0x5C1550, (int*)0x5C1554);
	*(int*)0x5C1550 = (int)((float)*(int*)0x5C1550 * scale);
	*(int*)0x5C1554 = (int)((float)*(int*)0x5C1554 * scale);
}

void fn_0041A960(void)
{
	float scale;
	int i;
	int image;

	scale = fn_004134A0();
	for (i = 0; i < 19; i++) {
		fn_00503E80(0);
		load_ui_slot((UiSlot*)0x5C18C0 + i, (char*)(0x58D4B8 + i * 0x40), scale, i == 0);
	}
	image = fn_005026E0(*(char**)(*(int*)0x5BD318 * 4 + 0x58D978), *(int*)0x60AC10, -1);
	*(int*)0x5C1A88 = image;
	fn_00503500(image, (int*)0x5C1AA8, (int*)0x5C1AAC);
	*(int*)0x5C1AA8 = (int)((float)*(int*)0x5C1AA8 * scale);
	*(int*)0x5C1AAC = (int)((float)*(int*)0x5C1AAC * scale);
}

void fn_0041BEE0(void)
{
	float scale;
	int i;

	scale = fn_004134A0();
	for (i = 0; i < 15; i++) {
		load_ui_slot((UiSlot*)0x5C1AE8 + i, (char*)(0x58D980 + i * 0x40), scale, i == 0);
	}
	load_ui_bar((int*)0x5C2F60, (int*)0x5C1C5C, (int*)0x5C1C60, 0x58DD40, scale);
}

}

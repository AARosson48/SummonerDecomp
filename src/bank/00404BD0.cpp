// Leaf functions in bank/00404BD0. Each one is a load, a store, or a constant return.
// fn_004063F0 builds the PXO character-select screen. See notes/multiplayer.md.

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

unsigned char fn_004063A0()
{
	return 1;
}

float fn_004134A0(void);
int fn_005026E0(char* name, int pack, int mode);
void fn_00503500(int id, int* w, int* h);

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

void fn_004063F0(void)
{
	float scale;
	int i;

	scale = fn_004134A0();
	for (i = 0; i < 34; i++) {
		load_ui_slot((UiSlot*)0x5B6940 + i, (char*)(0x589828 + i * 0x40), scale, i == 0 || i == 19);
	}
	load_ui_bar((int*)0x5BC3D8, (int*)0x5BC398, (int*)0x5BC39C, 0x58A0A8, scale);
}

}

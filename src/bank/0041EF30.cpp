// Menus in bank/0041EF30. fn_0041EF30 builds Options. fn_00421590 builds the pause popup.

#include <string.h>

extern "C" {

float fn_004134A0(void);
int fn_005026E0(char* name, int pack, int mode);
void fn_00503500(int id, int* w, int* h);
void fn_00503E80(int value);

static void load_menu_slot(int* slot, char* src, float scale, int mark)
{
	char name[0x70];
	int image;
	int mode;

	slot[0] = 0;
	slot[1] = -1;
	slot[2] = 0;
	slot[3] = 0;
	slot[4] = 0;
	slot[5] = 0;
	if (src[0] == 0) {
		if (mark)
			slot[0] |= 1;
		return;
	}
	strcpy(name, src);
	strcat(name, *(char**)(*(int*)0x5BD318 * 4 + 0x58BDA4));
	image = fn_005026E0(name, *(int*)0x60AC10, -1);
	slot[1] = image;
	mode = *(int*)0x60AD68;
	slot[2] = *(int*)(src + mode * 8 + 0x24);
	slot[3] = *(int*)(src + mode * 8 + 0x28);
	fn_00503500(image, &slot[4], &slot[5]);
	slot[4] = (int)((float)slot[4] * scale + 0.5f);
	slot[5] = (int)((float)slot[5] * scale + 0.5f);
	if (mark)
		slot[0] |= 1;
}

void fn_0041EF30(void)
{
	float scale;
	int i;

	scale = fn_004134A0();
	for (i = 0; i < 75; i++) {
		load_menu_slot(
			(int*)(i * 0x18 + 0x5C3328),
			(char*)(i * 0x40 + 0x58E0D0),
			scale,
			i == 0 || i == 1 || i == 2 || i == 19 || i == 70);
	}
}

void fn_00421590(void)
{
	float scale;
	int i;

	scale = fn_004134A0();
	for (i = 0; i < 9; i++) {
		fn_00503E80(0);
		load_menu_slot((int*)(i * 0x18 + 0x5C3B80), (char*)(i * 0x40 + 0x58F4B8), scale, i == 0);
	}
}

}

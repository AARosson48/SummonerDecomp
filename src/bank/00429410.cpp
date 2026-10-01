// fn_0042B260 builds the skill screen from Skl-Bkgrnd. It lives in bank/00429410.

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

static void load_named(unsigned int table, int* id)
{
	*id = fn_005026E0(*(char**)(*(int*)0x5BD318 * 4 + table), *(int*)0x60AC10, -1);
}

void fn_0042B260(void)
{
	float scale;
	int i;
	int image;

	scale = fn_004134A0();
	for (i = 0; i < 21; i++) {
		fn_00503E80(0);
		load_menu_slot((int*)(i * 0x18 + 0x5F93C8), (char*)(i * 0x40 + 0x5807A0), scale, i == 0);
	}
	image = fn_005026E0(*(char**)(*(int*)0x5BD318 * 4 + 0x592900), *(int*)0x60AC10, -1);
	*(int*)0x5F93B8 = image;
	fn_00503500(image, (int*)0x5F95F4, (int*)0x5F95F8);
	*(int*)0x5F95F4 = (int)((float)*(int*)0x5F95F4 * scale);
	*(int*)0x5F95F8 = (int)((float)*(int*)0x5F95F8 * scale);
	load_named(0x592908, (int*)0x5F95D0);
	load_named(0x592910, (int*)0x5F95E8);
	fn_00503E80(0);
	image = fn_005026E0(*(char**)(*(int*)0x5BD318 * 4 + 0x592930), *(int*)0x60AC10, -1);
	*(int*)0x5F5AAC = image;
	fn_00503500(image, (int*)0x5F95C8, (int*)0x5F95CC);
	*(int*)0x5F95C8 = (int)((float)*(int*)0x5F95C8 * scale);
	*(int*)0x5F95CC = (int)((float)*(int*)0x5F95CC * scale);
}

}

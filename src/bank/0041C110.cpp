// Leaf functions in bank/0041C110. Each one is a load, a store, or a constant return.
// fn_0041D740 builds the title menu. fn_0041D8B0 plays the opening logos, then "Title".

#include <string.h>

extern "C" {

float fn_004134A0(void);
int fn_005026E0(char* name, int pack, int mode);
void fn_00503500(int id, int* w, int* h);
void fn_005172F0(char* path);
int fn_00521BA0(void);
int fn_004696D0(int mode, char* name);
void fn_00469080(int id);

static void load_menu_slot(int* slot, char* src, float scale, int mark, int dot_check)
{
	char name[0x70];
	int image;
	int mode;
	int scale_it;

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
	scale_it = 1;
	if (dot_check && strchr(name, '.') != 0)
		scale_it = 0;
	else
		strcat(name, *(char**)(*(int*)0x5BD318 * 4 + 0x58BDA4));
	image = fn_005026E0(name, *(int*)0x60AC10, -1);
	slot[1] = image;
	mode = *(int*)0x60AD68;
	slot[2] = *(int*)(src + mode * 8 + 0x24);
	slot[3] = *(int*)(src + mode * 8 + 0x28);
	fn_00503500(image, &slot[4], &slot[5]);
	if (scale_it) {
		slot[4] = (int)((float)slot[4] * scale + 0.5f);
		slot[5] = (int)((float)slot[5] * scale + 0.5f);
	}
	if (mark)
		slot[0] |= 1;
}

void fn_0041D740(void)
{
	float scale;
	int i;

	scale = fn_004134A0();
	for (i = 0; i < 13; i++)
		load_menu_slot((int*)(i * 0x18 + 0x5C2F98), (char*)(i * 0x40 + 0x58DD48), scale, i == 0, 1);
}

void fn_0041D8B0(void)
{
	*(int*)0x5C30D0 = 0;
	*(int*)0x5C30D4 = -1;
	if (*(unsigned char*)0x5930BD != 0) {
		fn_005172F0((char*)0x58E094);
		if (fn_00521BA0())
			fn_005172F0((char*)0x58E088);
		*(unsigned char*)0x5930BD = 0;
	}
	fn_00469080(fn_004696D0(1, (char*)0x58A144));
}

void fn_0041E420()
{
	*(int*)0x5c32d0 = 0;
	*(int*)0x5c328c = 0;
}

void fn_0041ED50()
{
	*(int*)0x5c328c = 0;
}

}

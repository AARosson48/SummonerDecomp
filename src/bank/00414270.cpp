// Leaf functions in bank/00414270. Each one is a load, a store, or a constant return.
// fn_00414DE0 builds the inventory screen from Inv-Bkgrnd.

#include <string.h>

extern "C" {

void fn_00414560()
{
	*(unsigned char*)0x5bd960 = 0;
	*(unsigned char*)0x5bd961 = 0;
	*(unsigned char*)0x5bd7b0 = 0;
	*(unsigned char*)0x5bd50c = 0;
}

void fn_004146A0()
{
	*(unsigned char*)0x5bd962 = 1;
}

void fn_004146B0()
{
	*(unsigned char*)0x5bd962 = 0;
}

void fn_00416674()
{
}

void fn_004326C0(void);
void fn_00412E50(int value);
void fn_00406360(int value);

void fn_00410760(int first, int second);
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

void fn_00414DE0(void)
{
	float scale;
	int i;
	int image;

	scale = fn_004134A0();
	for (i = 0; i < 45; i++) {
		fn_00503E80(0);
		load_menu_slot((int*)(i * 0x18 + 0x5C1020), (char*)(i * 0x40 + 0x58C080), scale, i == 0);
	}
	image = fn_005026E0(*(char**)(*(int*)0x5BD318 * 4 + 0x58CBC0), *(int*)0x60AC10, -1);
	*(int*)0x5C0FEC = image;
	fn_00503500(image, (int*)0x5C0FF0, (int*)0x5C0FF4);
	*(int*)0x5C0FF0 = (int)((float)*(int*)0x5C0FF0 * scale);
	*(int*)0x5C0FF4 = (int)((float)*(int*)0x5C0FF4 * scale);
}

void fn_00414DC0()
{
	fn_00410760(0x5bd980, 0x58cbc8);
}

void fn_004158B0()
{
	if (*(unsigned char*)0x5c100c & 1)
		fn_004326C0();
	if (*(unsigned char*)0x5c100c & 2)
		fn_00412E50(0);
	if (*(unsigned char*)0x5c100c & 4)
		fn_00406360(0);
}

}

// One pass of bank/00449B00. Strings in this block: dump info on all characters in the game, charinfo, dump info on all characters in the level, levelchars.

struct ConsoleCmd {
	void fn_0050A180(const char* name, const char* help, void (*handler)(void));
};

extern "C" int g_info_rows[];

extern "C" {

void fn_00449B60(void)
{
	int* row = g_info_rows;
	do {
		*row = 0;
		row = (int*)((char*)row + 0x3ec);
	} while ((int)row < 0x7dfeb8);
}

void fn_00449BA0(void)
{
	((ConsoleCmd*)0x65e968)->fn_0050A180((char*)0x598e00, (char*)0x598e0c, (void (*)(void))0x4f13e0);
}

void fn_00449BD0(void)
{
	((ConsoleCmd*)0x8b3c38)->fn_0050A180((char*)0x598e34, (char*)0x598e40, (void (*)(void))0x449c00);
}

char* fn_00446DE0(int id);
int fn_0044B450(int key);
int fn_0044B2D0(int id);
int fn_0044B3F0(int id);

// Character record is 0x3d4 bytes past the lookup result.
char* fn_00449B80(int id)
{
	char* row = fn_00446DE0(id);
	if (row == 0)
		return 0;
	return row + 0x3d4;
}

// Free list of 0x10-byte nodes from 0x8b50c8 up to 0x8b9d48.
// Each node stores the previous node. The head is the last node.
void fn_0044B280(void)
{
	int* node = (int*)0x8b50c8;
	int* prev = 0;
	do {
		*node = (int)prev;
		prev = node;
		node = (int*)((char*)node + 0x10);
	} while ((int)node < 0x8b9d48);
	*(int*)0x8b50b8 = (int)prev;
	*(int*)0x8cbebc = 0;
}

void fn_0044B2B0(int* node)
{
	*node = *(int*)0x8b50b8;
	*(int*)0x8b50b8 = (int)node;
	(*(int*)0x8cbebc)--;
}

int* fn_0044B380(void)
{
	int* node = *(int**)0x8b50b8;
	*(int*)0x8b50b8 = *node;
	*node = 0;
	node[3] = -1;
	(*(int*)0x8cbebc)++;
	return node;
}

int fn_0044B320(int key)
{
	int id = fn_0044B450(key);
	if (id != -1)
		return fn_0044B2D0(id);
	return 0;
}

int fn_0044B430(int key)
{
	int id = fn_0044B450(key);
	if (id != -1)
		return fn_0044B3F0(id);
	return 0;
}

int fn_0044B4F0(unsigned char* row)
{
	return row[2] & 1;
}

int fn_0044B670(int* row)
{
	unsigned char flags = *(unsigned char*)(*(int*)((char*)row + 4) + 0x5c);
	return ((~flags) >> 2) & 1;
}

int fn_0044B690(int* row)
{
	if (*(unsigned char*)0x5fbfd8 & 2)
		return 1;
	unsigned char flags = *(unsigned char*)(*(int*)((char*)row + 4) + 0x5c);
	return (~flags) & 1;
}

int fn_0044B6B0(unsigned char* row)
{
	if (*(unsigned char*)0x5fbfd8 & 2)
		return 1;
	return (~row[0x5c]) & 1;
}

int _stricmp(const char* a, const char* b);
int sprintf(char* dst, const char* fmt, ...);
char* strcat(char* dst, const char* src);
int fn_004D0030(void* table, const char* name, int kind);
int fn_005152A0(char* dst, int max_len, int value);
int fn_004CFCA0(const char* name);

// Kind 2 writes a ring blurb into 0x8ec7c4. The quest value is read from
// the table at 0x2493a38. 60000 is the charged threshold. Ring of Darkness
// tests got_laharah_spirit instead. Kind 0 and kind 1 format other item
// text, and the tail appends Joseph, Jekhar, Flece, and Rosalind.
char* fn_0044B7A0(int* rec)
{
	static const unsigned int rings[][3] = {
		{ 0x593e64, 0x5994ac, 0x598964 },
		{ 0x598a08, 0x598954, 0 },
		{ 0x593e78, 0x593e54, 0 },
		{ 0x5989f8, 0x598944, 0 },
		{ 0x5989e8, 0x598934, 0 },
		{ 0x598978, 0x598924, 0 },
		{ 0x5989c4, 0x598914, 0 },
		{ 0x5989a0, 0x598904, 0 }
	};
	int* item = *(int**)((char*)rec + 4);
	int kind = item[1];
	char* out = (char*)0x8ec7c4;
	char tmp[0x80];
	const char* name;
	int value;
	int i;

	*out = 0;
	if (kind != 2)
		return out;
	name = *(const char**)item;
	for (i = 0; i < 8; i++) {
		if (_stricmp(name, (const char*)rings[i][0]) != 0)
			continue;
		sprintf(tmp, *(const char**)0x25cd940);
		strcat(out, tmp);
		strcat(out, (const char*)rings[i][1]);
		strcat(out, *(const char**)0x25cd92c);
		value = fn_004D0030((void*)0x2493a38, (const char*)rings[i][1], 4);
		fn_005152A0(tmp, 0x7f, value);
		strcat(out, tmp);
		strcat(out, (const char*)0x588ba4);
		sprintf(tmp, *(const char**)0x25cd944);
		strcat(out, tmp);
		sprintf(tmp, *(const char**)0x25cd950);
		strcat(out, tmp);
		if (rings[i][2] != 0) {
			if (fn_004CFCA0((const char*)rings[i][2]) == 0)
				break;
		} else if (fn_004D0030((void*)0x2493a38, (const char*)rings[i][1], 4) < 0xea60) {
			break;
		}
		sprintf(tmp, *(const char**)0x25cd954);
		strcat(out, tmp);
		break;
	}
	return out;
}

}

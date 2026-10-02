// One pass of bank/00449B00. Strings in this block: dump info on all characters in the game, charinfo, dump info on all characters in the level, levelchars.

struct ConsoleCmd {
	void fn_0050A180(const char* name, const char* help, void (*handler)(void));
};

extern "C" {

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

}

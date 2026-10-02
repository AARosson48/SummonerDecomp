// Bank 004CC010. The level-script init clears a 10-slot table at
// 0x2491A70. Each slot is two dwords. -1 means the slot is empty.
// The short functions after that resolve a name, then forward it.

struct Obj76670 {
	unsigned char fn_00476670(void);
};

extern "C" {

int fn_004CDDF0(int key);
void fn_004CE140(int id, int arg);
void fn_004CE220(int id);
void fn_004CE340(int id, int arg);
bool fn_004CE460(int id, int arg);
bool fn_004CE570(int id);
void fn_004CE5D0(int id);
void fn_004CE640(int id);
void fn_004A78B0(int value);

bool fn_004CCC50(char* obj)
{
	unsigned char result;
	bool same;

	result = ((Obj76670*)(obj + 0x324))->fn_00476670();
	same = result == 1;
	return same;
}

int fn_004CED90(void)
{
	int index;
	int* slot;

	index = 0;
	slot = (int*)0x2491a70;
	do {
		if (*slot == -1)
			return index;
		slot = slot + 2;
		index = index + 1;
	} while ((int)slot < 0x2491ac0);
	return -1;
}

void fn_004CEDB0(void)
{
	int* slot;

	slot = (int*)0x2491a70;
	do {
		*slot = -1;
		slot = slot + 2;
	} while ((int)slot < 0x2491ac0);
}

bool fn_004CEDD0(int id)
{
	int* slot;
	bool hit;

	hit = 0;
	slot = (int*)0x2491a70;
	do {
		if (*slot == id) {
			hit = 1;
			break;
		}
		slot = slot + 2;
	} while ((int)slot < 0x2491ac0);
	return hit;
}

void fn_004CEE90(int id)
{
	int* slot;

	slot = (int*)0x2491a70;
	do {
		if (*slot == id) {
			fn_004A78B0(slot[1]);
			*slot = -1;
		}
		slot = slot + 2;
	} while ((int)slot < 0x2491ac0);
}

void fn_004CE1C0(int key, int arg)
{
	int id;

	id = fn_004CDDF0(key);
	if (id >= 0)
		fn_004CE140(id, arg);
}

void fn_004CE320(int key)
{
	int id;

	id = fn_004CDDF0(key);
	if (id >= 0)
		fn_004CE220(id);
}

void fn_004CE3A0(int key, int arg)
{
	int id;

	id = fn_004CDDF0(key);
	if (id >= 0)
		fn_004CE340(id, arg);
}

bool fn_004CE540(int key, int arg)
{
	int id;

	id = fn_004CDDF0(key);
	if (id >= 0)
		return fn_004CE460(id, arg);
	return 0;
}

bool fn_004CE600(int key)
{
	int id;

	id = fn_004CDDF0(key);
	if (id >= 0)
		return fn_004CE570(id);
	return 0;
}

void fn_004CE620(int key)
{
	int id;

	id = fn_004CDDF0(key);
	if (id >= 0)
		fn_004CE5D0(id);
}

void fn_004CE6D0(int key)
{
	int id;

	id = fn_004CDDF0(key);
	if (id >= 0)
		fn_004CE640(id);
}

}

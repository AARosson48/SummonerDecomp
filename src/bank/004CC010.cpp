// Bank 004CC010. The level-script init clears a 10-slot table at
// 0x2491A70. Each slot is two dwords. -1 means the slot is empty.
// The short functions after that resolve a name, then forward it.

struct Obj76670 {
	unsigned char fn_00476670(void);
};

// The text reader used by masad.tbl. Keys are the section lines.
struct ParseText {
	int fn_00516110(const char* key);
	int fn_005166B0(char* dst, int max_len, char open_ch, char close_ch);
	float fn_00516460(void);
	int fn_00516010(char* dst, int max_len);
	void fn_005161E0(const char* key);
	int fn_00516290(void);
	int fn_00516BD0(const char** table, int count);
};

// One $Trigger record. The next record starts 0x64 bytes later.
struct LevelTrigger {
	char name[0x20];
	char* level_name;
	char ref_name[0x20];
	int gap;
	int id_kind;
	int type;
	int index;
	int flags;
	int plane;
	int marker;
	float radius;
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
int fn_004D6300(char* name);
int fn_004D6340(int id, char* variant);
int fn_004DC570(char* name);
int fn_004DC600(char* name, float* pos, float* extra);
int fn_004774A0(char* name, float* pos, float* extra);
int fn_00477580(char* name);
void fn_004CD250(int sound, float* pos, int scaled);
int fn_005207D0(char* wav, float first, float second, float third, int zero_a, int zero_b);
void* fn_0046B1A0(int id);
void fn_004361D0(char* name);
int _stricmp(const char* a, const char* b);
int sscanf(const char* text, const char* fmt, ...);
int sprintf(char* dst, const char* fmt, ...);
char DAT_0257ED80[];

static void copy_text(char* dst, const char* src)
{
	while ((*dst++ = *src++) != 0)
		;
}

static void append_text(char* dst, const char* src)
{
	while (*dst != 0)
		dst = dst + 1;
	while ((*dst++ = *src++) != 0)
		;
}

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

// $Ambient lines. A line is a marker name, a wav path, three numbers,
// and an optional fourth number. The fourth is scaled by 1000.
void fn_004CC010(ParseText* text)
{
	char marker[0x80];
	char wav[0x80];
	char extra[0x80];
	float first;
	float second;
	float third;
	float fourth;
	float pos[3];
	float other[3];
	int sound;

	if (text->fn_00516110((char*)0x59e490) == 0)
		return;
	do {
		text->fn_005166B0(marker, 0x80, 0x22, 0x22);
		text->fn_005166B0(wav, 0x80, 0x22, 0x22);
		first = text->fn_00516460();
		second = text->fn_00516460();
		third = text->fn_00516460();
		fourth = 0;
		if (text->fn_00516010(extra, 0x80) == 0)
			continue;
		if (extra[0] != 0 && sscanf(extra, (char*)0x59e48c, &fourth) != 1)
			continue;
		if (fn_004DC600(marker, pos, other) != 1 && fn_004774A0(marker, pos, other) != 1) {
			sprintf(DAT_0257ED80, (char*)0x59e464, marker);
			continue;
		}
		sound = fn_005207D0(wav, first, second, third, 0, 0);
		if (sound != -1)
			fn_004CD250(sound, pos, (int)(fourth * 1000.0f));
	} while (text->fn_00516110((char*)0x59e490) != 0);
}

// $Trigger lines. +Id is load level, boss, or sound. +Type is spline or
// location. A load-level trigger names a level and a variant slot.
void fn_004CC190(ParseText* text)
{
	LevelTrigger* trig;
	char* ref;
	char script[0x100];
	char marker_name[0x20];
	int level_id;
	int slot;

	if (text->fn_00516110((char*)0x59838c) == 0)
		return;
	trig = (LevelTrigger*)0x2490dd0;
	ref = (char*)0x2490df4;
	do {
		*(int*)0x2490c74 = *(int*)0x2490c74 + 1;
		trig->flags = 1;
		text->fn_005166B0(trig->name, 0x20, 0x22, 0x22);
		if (text->fn_00516110((char*)0x59e534) != 0)
			trig->flags = trig->flags | 4;
		text->fn_005161E0((char*)0x59e52c);
		trig->id_kind = text->fn_00516BD0((const char**)0x59e0f0, 3);
		if (trig->id_kind == 0) {
			if (text->fn_00516110((char*)0x59e520) != 0) {
				text->fn_005166B0(script, 0x100, 0x22, 0x22);
			} else {
				copy_text(script, trig->name);
				if ((trig->flags & 4) != 0)
					append_text(script, (char*)0x59e51c);
			}
			if (_stricmp(trig->name, (char*)0x59e50c) != 0) {
				level_id = fn_004D6300(trig->name);
				slot = fn_004D6340(level_id, script);
				if (slot >= 0)
					trig->level_name = *(char**)(0x25388c0 + (level_id * 7 + slot) * 16);
				else
					trig->level_name = (char*)0x5bd0d0;
			} else {
				trig->level_name = (char*)0x5bd0d0;
			}
			if (text->fn_00516110((char*)0x59e500) != 0)
				text->fn_005166B0(ref, 0x20, 0x22, 0x22);
			else
				copy_text(ref, trig->name);
		}
		trig->index = 0;
		if (text->fn_00516110((char*)0x59e4f8) != 0)
			trig->index = text->fn_00516290();
		text->fn_005161E0((char*)0x59e4f0);
		trig->type = text->fn_00516BD0((const char**)0x59e0e8, 2);
		trig->plane = -1;
		if (text->fn_00516110((char*)0x59e4e8) == 1)
			trig->plane = text->fn_00516290();
		if (trig->type == 0) {
			if (text->fn_00516110((char*)0x59e49c) != 0)
				text->fn_005166B0(marker_name, 0x20, 0x22, 0x22);
			else
				copy_text(marker_name, trig->name);
			trig->marker = fn_00477580(marker_name);
			if (trig->marker == 0) {
				sprintf(DAT_0257ED80, (char*)0x59e4b8, trig->name);
				*(int*)0x2490c74 = *(int*)0x2490c74 - 1;
			} else {
				trig = (LevelTrigger*)((char*)trig + 0x64);
				ref = ref + 0x64;
			}
		} else if (trig->type == 1) {
			text->fn_005161E0((char*)0x59e4dc);
			text->fn_005166B0(marker_name, 0x20, 0x22, 0x22);
			trig->marker = fn_004DC570(marker_name);
			if (trig->marker == 0) {
				sprintf(DAT_0257ED80, (char*)0x59e4b8, trig->name);
				text->fn_005161E0((char*)0x59e4ac);
				trig->radius = text->fn_00516460();
				*(int*)0x2490c74 = *(int*)0x2490c74 - 1;
			} else {
				text->fn_005161E0((char*)0x59e4ac);
				trig->radius = text->fn_00516460();
				trig = (LevelTrigger*)((char*)trig + 0x64);
				ref = ref + 0x64;
			}
		}
	} while (text->fn_00516110((char*)0x59838c) != 0);
}

// Starts a cutscene unless cutscenes are suppressed. The party list at
// 0x5FC350 has to contain one member whose flags at +0x320 have neither
// bit 7 nor bit 8 set. 0x24902AC is replaced with fn_004CCDF0 until the
// cutscene returns.
void fn_004CCF00(char* name, void (*done)(void))
{
	int* node;
	void* member;
	unsigned int flags;

	node = *(int**)0x5fc350;
	while (node != (int*)0x5fc350) {
		member = fn_0046B1A0(node[3]);
		if (member != 0) {
			flags = *(unsigned int*)((char*)member + 0x320);
			if ((flags & 0x100) == 0 && (flags & 0x80) == 0)
				break;
		}
		node = (int*)node[0];
	}
	if (node == (int*)0x5fc350)
		return;
	if (*(unsigned char*)0x2493a16 != 0 || (*(unsigned char*)0x5fbfd8 & 2) != 0) {
		if (done != 0)
			done();
		return;
	}
	*(int*)0x2493a04 = *(int*)0x24902ac;
	*(int*)0x24902ac = 0x4ccdf0;
	*(int*)0x24902b0 = (int)done;
	fn_004361D0(name);
}

}

// One pass of bank/00461DC0. Strings in this block: Sets Chance_to_drop_gold, chance_gold, kills off friendly summoned creature, kill_summon, Toggles No_resist, no_resist, Toggles Use_spell_ap, Spell_ap.

struct ConsoleCmd {
	void fn_0050A180(const char* name, const char* help, void (*handler)(void));
};

extern "C" {

void fn_00463050(void)
{
	((ConsoleCmd*)0x8eca30)->fn_0050A180((char*)0x59a09c, (char*)0x59a0a8, (void (*)(void))0x463080);
}

void fn_00463110(void)
{
	((ConsoleCmd*)0x8eca50)->fn_0050A180((char*)0x59a0d8, (char*)0x59a0e4, (void (*)(void))0x463140);
}

void fn_00463180(void)
{
	((ConsoleCmd*)0x940428)->fn_0050A180((char*)0x59a10c, (char*)0x59a118, (void (*)(void))0x4631b0);
}

void fn_00463270(void)
{
	((ConsoleCmd*)0x8eca40)->fn_0050A180((char*)0x59a138, (char*)0x59a144, (void (*)(void))0x4632a0);
}

void fn_00463360(void)
{
	((ConsoleCmd*)0x941730)->fn_0050A180((char*)0x59a170, (char*)0x59a17c, (void (*)(void))0x463390);
}

void fn_00463450(void)
{
	((ConsoleCmd*)0x941710)->fn_0050A180((char*)0x597834, (char*)0x59a19c, (void (*)(void))0x463480);
}

void fn_00464010(void)
{
	((ConsoleCmd*)0x941720)->fn_0050A180((char*)0x59a1e8, (char*)0x59a1f4, (void (*)(void))0x464040);
}

void* fn_0046B1A0(int id);

}

// The actor's link id is at +0x6e0 and its flag dword is at +0x6e4.
struct Obj62D20 {
	char pad[0x6e0];
	int link;
	int flags;

	int fn_00462D20(void);
	int fn_00462D40(void);
	int fn_00462D60(void);
	unsigned char fn_00462D70(void);
};

int Obj62D20::fn_00462D20(void)
{
	unsigned char* other = (unsigned char*)fn_0046B1A0(link);
	if (other != 0 && (other[0x6e4] & 2))
		return (int)other;
	return 0;
}

int Obj62D20::fn_00462D40(void)
{
	unsigned char* other = (unsigned char*)fn_0046B1A0(link);
	if (other != 0 && (other[0x6e4] & 1))
		return (int)other;
	return 0;
}

int Obj62D20::fn_00462D60(void)
{
	return (flags >> 1) & 1;
}

unsigned char Obj62D20::fn_00462D70(void)
{
	return (unsigned char)(flags & 1);
}

struct Actor62440 {
	int pad[3];
	int field_c;

	float fn_00462440(int kind);
	int fn_00455950(int flag);
};

struct Obj73200 {
	int fn_00473200(int a, int b);
};

struct Obj75000 {
	int fn_00475000(void);
};

extern "C" float fn_005426E0(void);

// Kind 0x0a..0x55 selects a group. The byte at 0x4645A0 is kind minus 0x0a.
static const unsigned char fn_00464100_group[0x4c] = {
    0, 10, 10, 1, 2, 2, 2, 10, 3, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 4, 4, 4, 1, 1,
    1, 1, 1, 1, 1, 1, 10, 5, 6, 7, 1, 1, 1, 8, 8, 8,
    8, 8, 8, 8, 8, 1, 9, 9, 2, 2, 10, 1,
};

struct ChanceRecord {
	Actor62440* actor;
	int pad_4;
	Actor62440** other;
	char pad_c[0x30];
	int kind;
	int hits;

	float fn_00463B90(void);
};

struct Obj64100 {
	Actor62440* actor;
	char pad_4[0x10];
	int field_14;
	int field_18;
	char pad_1c[0x14];
	int field_30;

	void fn_004637D0(void);
	void fn_00463830(void);
	void fn_00464100(ChanceRecord* record, char mode);
};

static float scale_chance(float value, Actor62440* actor, Actor62440* other)
{
	if (actor->fn_00455950(0))
		value *= 0.3f;
	if (actor->fn_00455950(7))
		value += 0.2f;
	if (other->fn_00455950(7))
		value += 0.1f;
	if (other->fn_00455950(8))
		value *= 0.5f;
	if (actor->fn_00455950(0xc))
		value -= 0.2f;
	return value;
}

static float clamp_95(float value, Actor62440* actor)
{
	short stat = *(short*)((char*)actor + 0x600);
	value = value * (float)(0x5dc / (int)stat);
	if (value >= 0.95f)
		value = 0.95f;
	return value;
}

void Obj64100::fn_00464100(ChanceRecord* record, char mode)
{
	float part_a;
	float part_b;
	float part_c;
	float half;
	float sum;
	float share_a;
	float share_half;
	float roll;
	int group;
	Actor62440* other;

	record->hits = 0;
	if (mode == 0)
		field_18 = 0;

	part_a = actor->fn_00462440(9);
	if (part_a <= 0.0f)
		part_a = 0.0f;
	part_b = actor->fn_00462440(0xb);
	if (part_b <= 0.0f)
		part_b = 0.0f;
	part_c = 0.0f;

	other = *record->other;
	group = 10;
	if ((unsigned)(record->kind - 0x0a) <= 0x4b)
		group = fn_00464100_group[record->kind - 0x0a];

	if (group == 0) {
		part_a = 0.0f;
		part_b = 0.0f;
	} else if (group == 1) {
		part_c = scale_chance(record->fn_00463B90(), record->actor, other);
		if (part_c < 0.0f)
			part_c = 0.0f;
	} else if (group == 2) {
		part_c = record->actor->fn_00462440(record->kind);
		part_a = 0.0f;
	} else if (group == 3) {
		if (!((Obj73200*)((char*)other + 0x324))->fn_00473200(3, -1))
			part_b = 0.0f;
		part_a = 0.0f;
		part_c = clamp_95(record->actor->fn_00462440(record->kind), actor);
	} else if (group == 4) {
		part_a = 0.0f;
		part_b = 0.0f;
		part_c = clamp_95(record->actor->fn_00462440(record->kind), actor);
	} else if (group == 5) {
		part_c = record->actor->fn_00462440(0x21);
	} else if (group == 6) {
		part_c = record->actor->fn_00462440(0x20);
	} else if (group == 7) {
		part_c = record->actor->fn_00462440(0x1e);
	} else if (group == 8) {
		part_c = 1.0f;
		part_a = 0.0f;
		part_b = 0.0f;
	} else if (group == 9) {
		part_c = scale_chance(record->actor->fn_00462440(record->kind), record->actor, other);
		if (part_c >= 0.95f)
			part_c = 0.95f;
	}

	if (mode == 1)
		part_a = 0.0f;

	if (actor->fn_00455950(0xd) || ((Obj75000*)((char*)actor + 0x324))->fn_00475000()) {
		part_b = part_b - 0.2f;
		if (part_b < 0.0f)
			part_b = 0.0f;
	}

	half = part_b * 0.5f;
	if (*(float*)((char*)actor + 0x6a8) > 0.5f)
		half = 0.0f;

	sum = part_c + half + part_a;
	if (sum <= 0.0f) {
		share_a = 0.0f;
		share_half = 0.0f;
	} else {
		share_a = part_a / sum;
		share_half = half / sum;
	}

	roll = fn_005426E0();
	if (*(unsigned char*)0x94173d && record->actor->field_c == 7)
		roll = 1.0f;

	if (mode == 0) {
		if (roll < share_a)
			fn_004637D0();
		else if (roll < share_a + share_half)
			fn_00463830();
		else {
			field_14 = 2;
			record->hits += 1;
		}
	} else if (record->kind == 0x12 && roll < part_c)
		record->hits += 1;
	else if (roll >= share_half)
		record->hits += 1;

	if (record->kind == 0xd) {
		roll = fn_005426E0();
		if (share_half + share_a <= roll) {
			if (mode == 0) {
				field_14 = 2;
				field_30 = 0;
			}
			record->hits += 1;
		}
	}
}

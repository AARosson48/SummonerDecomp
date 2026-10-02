// Ring-gated random encounters. Writes the level at 0x60a2c8 and the variant
// at 0x60a2e8. A ring that has not been used yet saves the current level at
// 0x60a308 and forces a night or day map.

extern "C" {

int fn_004CFCA0(void* table, int kind, const char* name);
const char* fn_004FC2A0(float* out);
const char* fn_004FC350(float* out);
void fn_004FC400(float* out);
float fn_005426E0(void);
int fn_004D47B0(void);
void strcpy(char* dst, const char* src);
int _stricmp(const char* a, const char* b);

static int quest_set(int kind, const char* name)
{
	return fn_004CFCA0((void*)0x2493a38, kind, name);
}

static void save_current(void)
{
	strcpy((char*)0x60a308, (const char*)0x60a2c8);
}

static void put_level(const char* level)
{
	strcpy((char*)0x60a2c8, level);
}

static void put_variant(const char* variant)
{
	strcpy((char*)0x60a2e8, variant);
}

static void grassland(float first)
{
	const char* variant;
	if (first >= 0.1f) {
		put_level((const char*)0x5a4d00);
		variant = first >= 0.55f ? (const char*)0x5a4d00 : (const char*)0x5a4d78;
	} else {
		put_level((const char*)0x59ffa0);
		variant = (const char*)0x5a1a30;
	}
	put_variant(variant);
}

void fn_004FC4B0(void)
{
	float first;
	float second;
	float roll;
	const char* name;
	const char* variant;
	int region = 0;

	if (quest_set(1, (const char*)0x59ed74) && !quest_set(0x13, (const char*)0x5a347c)) {
		save_current();
		put_level((const char*)0x5a0000);
		put_variant((const char*)0x5a348c);
		return;
	}
	if (quest_set(0xe, (const char*)0x59ed9c) && !quest_set(0xa, (const char*)0x5a4650)) {
		save_current();
		put_level((const char*)0x5a0048);
		put_variant((const char*)0x5a4660);
		return;
	}
	if (quest_set(0xc, (const char*)0x59ed88) && !quest_set(0x12, (const char*)0x5a351c)) {
		save_current();
		put_level((const char*)0x5a0014);
		put_variant((const char*)0x5a352c);
		return;
	}

	name = fn_004FC2A0(&first);
	variant = fn_004FC350(&second);
	if (second < first) {
		name = variant;
		first = second;
		region = 1;
	}
	fn_004FC400(&second);
	if (second < first) {
		first = second;
		region = 2;
	}

	if (region == 0) {
		roll = fn_005426E0();
		if (_stricmp(name, (const char*)0x5a4e04) != 0 || roll >= 0.7f
			|| quest_set(*(int*)0x59f120, (const char*)0x592d9c)) {
			roll = fn_005426E0();
			if (roll < 0.33f) {
				put_level((const char*)0x5a4d4c);
				variant = (const char*)0x5a4de0;
			} else if (roll < 0.66f) {
				put_level((const char*)0x5a4d4c);
				variant = (const char*)0x5a4dcc;
			} else {
				put_level((const char*)0x5a0048);
				variant = (const char*)0x5a0048;
			}
		} else {
			save_current();
			put_level((const char*)0x5a0048);
			variant = (const char*)0x5a4df4;
		}
		put_variant(variant);
		return;
	}

	if (region == 1) {
		roll = fn_005426E0();
		if (roll < 0.25f) {
			put_level((const char*)0x5a0014);
			variant = (const char*)0x5a4db8;
		} else if (roll < 0.5f) {
			put_level((const char*)0x5a0014);
			variant = (const char*)0x5a4da4;
		} else if (roll >= 0.75f || fn_004D47B0() < 7) {
			put_level((const char*)0x5a0000);
			variant = (const char*)0x5a0000;
		} else {
			put_level((const char*)0x5a0000);
			variant = (const char*)0x5a4d90;
		}
		put_variant(variant);
		return;
	}

	roll = fn_005426E0();
	if (!(quest_set(0x23, (const char*)0x5a1a20) && !quest_set(0x2a, (const char*)0x5a1a10))
		&& quest_set(7, (const char*)0x5a19a8) && !quest_set(0x2a, (const char*)0x5a1998)) {
		if (fn_005426E0() < 0.5f) {
			put_level((const char*)0x59ffa0);
			put_variant((const char*)0x5a1a30);
		}
		grassland(roll);
		return;
	}
	if (fn_005426E0() >= 0.5f)
		grassland(roll);
	else {
		put_level((const char*)0x59ffa0);
		put_variant((const char*)0x5a1a30);
	}
}

}

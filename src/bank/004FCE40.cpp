// One pass of bank/004FCE40. Strings in this block: reinits worldmap, iw, Toggles Random_encounters_enabled, random_encounters, masad_v2, worldmap1, Barbarian Fighter#$npc037, Nar-Leaving-Masad.

struct ConsoleCmd {
	void fn_0050A180(const char* name, const char* help, void (*handler)(void));
};

extern "C" {

void fn_004FD5C0(void)
{
	((ConsoleCmd*)0x257ed50)->fn_0050A180((char*)0x5a4f3c, (char*)0x5a4f40, (void (*)(void))0x4fd5f0);
}

void fn_004FD600(void)
{
	((ConsoleCmd*)0x257ed10)->fn_0050A180((char*)0x5a4f54, (char*)0x5a4f68, (void (*)(void))0x4fd630);
}

void strcpy(char* dst, const char* src);
void strcat(char* dst, const char* src);
int fn_004D6300(char* name);
int fn_0044B450(char* name);
void fn_00500990(const char* action);
void fn_005009F0(void);
void fn_004FED30(void);

}

struct NodeFED20 {
	int pad0;
	int pad4;
	NodeFED20* next;
};

struct ObjFED20 {
	char pad[0x40];
	NodeFED20* head;
	void fn_004FED20(NodeFED20* node);
};

void ObjFED20::fn_004FED20(NodeFED20* node)
{
	node->next = head;
	head = node;
}

// _dlg.tbl. Retail also keeps the caller's ecx and passes it to the
// character lookups. The cleanup and the '{' jump table sit past this symbol.
struct ParseText {
	void fn_00515960(void);
	void fn_00515D20(const char* path, int mode);
	void fn_005159D0(void);
	int fn_00516110(const char* tag);
	int fn_005166B0(char* dst, int max_len, char open_ch, char close_ch);
	int fn_00516290(void);
	int fn_005160D0(const char* tag);
};

static int speaker_line(ParseText* file)
{
	static const int tags[] = {
		0x5a56ec, 0x5a56e0, 0x5a56d0, 0x5a56c0, 0x5a56b4, 0x5a56a4
	};
	int i;
	for (i = 0; i < 6; i++) {
		if (file->fn_00516110((const char*)tags[i]))
			return 1;
	}
	return 0;
}

extern "C" int __stdcall fn_004FEE50(char* dir)
{
	char path[0x200];
	char token[0x100];
	ParseText file;
	const int open_mode = 0x98967f;

	strcpy(path, dir);
	strcat(path, (const char*)0x5a5728);
	file.fn_00515960();
	file.fn_00515D20(path, open_mode);

	while (file.fn_00516110((const char*)0x593d54)) {
		if (!file.fn_005166B0(token, 0x100, 0x22, 0x22))
			break;
		if (file.fn_00516110((const char*)0x5a5718))
			file.fn_00516290();
		if (file.fn_00516110((const char*)0x5a5708))
			file.fn_00516290();
		if (file.fn_00516110((const char*)0x599454))
			fn_004FED30();
		if (!file.fn_00516110((const char*)0x5a5700))
			break;
		if (!file.fn_005166B0(token, 0x100, 0x22, 0x22))
			break;

		for (;;) {
			if (file.fn_00516110((const char*)0x5a56f8))
				continue;
			if (file.fn_00516110((const char*)0x5a07f8))
				continue;
			if (speaker_line(&file)) {
				file.fn_005160D0((const char*)0x593ab0);
				file.fn_005166B0(token, 0x100, 0x22, 0x22);
				continue;
			}
			if (file.fn_00516110((const char*)0x5a07ec) || file.fn_00516110((const char*)0x59e2f4)) {
				file.fn_005166B0(token, 0x100, 0x22, 0x22);
				fn_00500990(token);
				fn_005009F0();
				continue;
			}
			if (file.fn_00516110((const char*)0x5a5690) || file.fn_00516110((const char*)0x5a5658)) {
				file.fn_005166B0(token, 0x100, 0x22, 0x22);
				fn_00500990(token);
				file.fn_005166B0(token, 0x100, 0x22, 0x22);
				fn_0044B450(token);
				fn_005009F0();
				continue;
			}
			if (file.fn_00516110((const char*)0x5a567c) || file.fn_00516110((const char*)0x5a5648)) {
				file.fn_005166B0(token, 0x100, 0x22, 0x22);
				fn_00500990(token);
				file.fn_00516290();
				fn_005009F0();
				continue;
			}
			if (file.fn_00516110((const char*)0x5a5668) || file.fn_00516110((const char*)0x5a5638)) {
				file.fn_005166B0(token, 0x100, 0x22, 0x22);
				fn_00500990(token);
				file.fn_005166B0(token, 0x100, 0x22, 0x22);
				fn_004D6300(token);
				file.fn_005166B0(token, 0x100, 0x22, 0x22);
				fn_005009F0();
				continue;
			}
			if (file.fn_00516110((const char*)0x5a5630)) {
				file.fn_00516290();
				fn_00500990((const char*)0x581ce4);
				fn_005009F0();
				continue;
			}
			if (file.fn_00516110((const char*)0x5a5628)) {
				file.fn_00516290();
				fn_00500990((const char*)0x581cf0);
				fn_005009F0();
				continue;
			}
			if (file.fn_00516110((const char*)0x5a561c)) {
				file.fn_00516290();
				fn_00500990((const char*)0x581cfc);
				fn_005009F0();
				continue;
			}
			if (file.fn_00516110((const char*)0x5a5610)) {
				file.fn_005166B0(token, 0x100, 0x22, 0x22);
				fn_0044B450(token);
				fn_00500990((const char*)0x581d08);
				fn_005009F0();
				continue;
			}
			if (file.fn_00516110((const char*)0x5a5604)) {
				file.fn_005166B0(token, 0x100, 0x22, 0x22);
				fn_0044B450(token);
				fn_00500990((const char*)0x581d14);
				fn_005009F0();
				continue;
			}
			if (file.fn_00516110((const char*)0x5a55f8) || file.fn_00516110((const char*)0x5a55e8)
				|| file.fn_00516110((const char*)0x5a55d4) || file.fn_00516110((const char*)0x5a55bc)
				|| file.fn_00516110((const char*)0x5a55ac) || file.fn_00516110((const char*)0x5a559c)
				|| file.fn_00516110((const char*)0x5a558c) || file.fn_00516110((const char*)0x5a557c)
				|| file.fn_00516110((const char*)0x5a556c) || file.fn_00516110((const char*)0x5a5558)
				|| file.fn_00516110((const char*)0x5a5544)) {
				file.fn_005166B0(token, 0x100, 0x22, 0x22);
				continue;
			}
			if (file.fn_005160D0((const char*)0x581ce0))
				file.fn_005166B0(token, 0x100, 0x22, 0x22);
			break;
		}
	}
	file.fn_005159D0();
	return 0;
}

// Picks the random-encounter level into 0x60a2c8 and the variant into 0x60a2e8.
// Region 0 is Iceland or the hills, region 1 is the forest tome pages, region 2
// is grassland. The page-8 path returns before the region split.
int fn_004CFCA0(void* table, int kind, const char* name);
void fn_004CFD30(void* table, int kind, const char* name, int value);
int fn_004CE9A0(const char* item);
void fn_004FC2A0(float* out);
void fn_004FC350(float* out);
void fn_004FC400(float* out);
float fn_005426E0(void);
void strcat(char* dst, const char* src);

static int quest_set(int kind, const char* name)
{
	return fn_004CFCA0((void*)0x2493a38, kind, name);
}

static int owns_item(const char* item)
{
	return fn_004CE9A0(item) == 0;
}

static void copy_level(const char* level, const char* variant)
{
	strcpy((char*)0x60a2c8, level);
	strcpy((char*)0x60a2e8, variant);
}

static void add_suffix(const char* suffix)
{
	strcat((char*)0x60a2e8, suffix);
}

static int below(float roll, unsigned int constant)
{
	return roll < *(float*)constant;
}

extern "C" void fn_004FCE40(void)
{
	float first;
	float second;
	float roll;
	float roll_b;
	int region = 0;
	int page;

	fn_004FC2A0(&first);
	fn_004FC350(&second);
	if (second < first) {
		region = 1;
		first = second;
	}
	fn_004FC400(&second);
	if (second < first)
		region = 2;

	if (quest_set(0x12, (const char*)0x5a4f20) && owns_item((const char*)0x599954)
		&& !quest_set(0x22, (const char*)0x598964)) {
		copy_level((const char*)0x5a4f0c, (const char*)0x5a4ef4);
		return;
	}

	if (region == 0) {
		if (quest_set(0x17, (const char*)0x5a4ed8) && !quest_set(2, (const char*)0x5a4ed0)
			&& owns_item((const char*)0x5998cc)) {
			copy_level((const char*)0x59ffb4, (const char*)0x5a4ebc);
			fn_004CFD30((void*)0x2493a38, 2, (const char*)0x5a4ed0, 1);
			return;
		}
		roll = fn_005426E0();
		if (below(roll, 0x58021c)) {
			copy_level((const char*)0x5a0048, (const char*)0x5a4eac);
			return;
		}
		if (below(roll, 0x57e6b0)) {
			copy_level((const char*)0x59ffb4, (const char*)0x59ffb4);
			if (below(roll, 0x581cdc))
				add_suffix((const char*)0x5a4e28);
			else if (below(roll, 0x581c08))
				add_suffix((const char*)0x5a4e10);
			return;
		}
		copy_level((const char*)0x5a4f0c, (const char*)0x5a4f0c);
		if (below(roll, 0x580e04))
			add_suffix((const char*)0x5a4e10);
		return;
	}

	if (region == 1) {
		roll = fn_005426E0();
		roll_b = fn_005426E0();
		page = 0;
		while (page < 6 && !owns_item((const char*)(0x5a35f0 - page * 0x18)))
			page++;
		if (page == 6 && !quest_set(0x22, (const char*)0x598964)) {
			copy_level((const char*)0x5a0014, (const char*)0x5a365c);
			return;
		}
		if (below(roll, 0x57e6b0)) {
			copy_level((const char*)0x5a0014, (const char*)0x5a0014);
			if (below(roll_b, 0x57ef10))
				add_suffix((const char*)0x5a4ea8);
			else
				add_suffix((const char*)0x5a4ea0);
			return;
		}
		copy_level((const char*)0x5a0000, (const char*)0x5a0000);
		if (below(roll_b, 0x57ef10))
			add_suffix((const char*)0x5a4e10);
		else if (below(roll_b, 0x581ad8))
			add_suffix((const char*)0x5a4ea4);
		else if (below(roll_b, 0x5819a4))
			add_suffix((const char*)0x5a4ea8);
		else if (below(roll_b, 0x58020c))
			add_suffix((const char*)0x5a4ea0);
		else
			add_suffix((const char*)0x5a4e9c);
		return;
	}

	if (region != 2)
		return;
	roll = fn_005426E0();
	if (below(roll, 0x580214) && quest_set(0x17, (const char*)0x59ee28)
		&& quest_set(0x22, (const char*)0x598964)) {
		copy_level((const char*)0x59ffa0, (const char*)0x5a4e88);
		return;
	}
	if (below(roll, 0x57e6b0) && !quest_set(2, (const char*)0x5a4e80)) {
		fn_004CFD30((void*)0x2493a38, 2, (const char*)0x5a4e80, 1);
		copy_level((const char*)0x59ffa0, (const char*)0x5a4e6c);
		return;
	}
	copy_level((const char*)0x5a4d00, (const char*)0x5a4e54);
}

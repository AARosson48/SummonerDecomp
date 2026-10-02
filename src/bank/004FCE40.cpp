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

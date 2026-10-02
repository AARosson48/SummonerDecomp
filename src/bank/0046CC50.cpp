// Bank 0046CC50. This is the S3D loader and the player-file console
// commands that sit in front of it. The loader zeros a 0xE0-byte level,
// checks the four bytes S3DF, then reads meshes (stride 0x54, count at
// +0x20) and placed copies (stride 0x70, count at +0x28).

struct ConsoleCmd {
	void fn_00501A00(int ticks);
	void fn_00501A80(void);
	unsigned char fn_00501A90(void);
	void fn_0050A180(const char* name, const char* help, void (*handler)(void));
};

struct PathSet {
	void fn_00512770(void* root, int size, const char* name);
};

struct MeshPart {
	void fn_004BCDC0(void* file);
};

extern "C" {

void vfs_file_ctor(void* file);
void vfs_file_dtor(void* file);
int vfs_file_open_mode(void* file, const char* name, int path_id, unsigned int mode);
int vfs_file_read(void* file, void* dst, int count, int min_user, int unused);
int vfs_file_close(void* file);
int strncmp(const char* a, const char* b, unsigned int n);
int _stricmp(const char* a, const char* b);

void fn_0046D0B0(int id);
void fn_0046E1A0(void* level);
int fn_0046F6D0(void* level, void* file);
void fn_0046F990(void* level);
void fn_0046F660(void* level, void* file);
void fn_0046F100(void* level, void* file);
void fn_0046E910(void* level, void* mesh, void* file);
void fn_0046EBE0(void* level, void* placed, void* file);
void fn_00503E80(int flag);
void fn_005127F0(void* path);
void fn_0051CCD0(void);

// Zeros the level record. 0x38 dwords is 0xE0 bytes.
void fn_0046E1A0(void* level)
{
	int* word;
	int index;

	word = (int*)level;
	index = 0;
	do {
		word[index] = 0;
		index = index + 1;
	} while (index < 0x38);
}

void fn_0046DA20(void)
{
	((ConsoleCmd*)0xa59a84)->fn_00501A00(0x493e0);
}

// If the console command is armed, write the .plr for the current id.
void fn_0046DA30(void)
{
	int id;

	if (((ConsoleCmd*)0xa59a84)->fn_00501A90() == 0)
		return;
	id = *(int*)(*(int*)0x651598 + 0x28);
	fn_0046D0B0(id);
	((ConsoleCmd*)0xa59a84)->fn_00501A00(0x493e0);
}

void fn_0046DA60(void)
{
	int id;

	id = *(int*)(*(int*)0x651598 + 0x28);
	if (id != 0)
		fn_0046D0B0(id);
	((ConsoleCmd*)0xa59a84)->fn_00501A80();
}

void fn_0046DD90(void)
{
	((PathSet*)0x22e43c0)->fn_00512770((void*)0xa5a380, 0x186a000, (char*)0x59aa5c);
}

void fn_0046DDC0(void)
{
	((ConsoleCmd*)0x22e43e8)->fn_0050A180((char*)0x59aa64, (char*)0x59aa78, (void (*)(void))0x46ddf0);
}

void fn_0046DEB0(void)
{
	((ConsoleCmd*)0x22c4388)->fn_0050A180((char*)0x59aaa8, (char*)0x59aab8, (void (*)(void))0x46dee0);
}

void fn_0046DFA0(void)
{
	((ConsoleCmd*)0x22e43a8)->fn_0050A180((char*)0x59aae8, (char*)0x59aaf4, (void (*)(void))0x46dfd0);
}

void fn_0046E090(void)
{
	((ConsoleCmd*)0x22e4398)->fn_0050A180((char*)0x59ab14, (char*)0x5bd0d0, (void (*)(void))0x46e0c0);
}

void fn_0046E110(void)
{
	((ConsoleCmd*)0x22e4408)->fn_0050A180((char*)0x59ab20, (char*)0x5bd0d0, (void (*)(void))0x46e140);
}

// Opens one .s3d. A name equal to ionaext.s3d sets the byte at 0x22E4425.
// The first count the header stored, at +0x20, is the mesh count.
int fn_0046E1B0(void* level, char* name)
{
	char file[0x120];
	char magic[4];
	int count;
	int index;
	char* row;

	vfs_file_ctor(file);
	fn_0051CCD0();
	fn_0046E1A0(level);
	if (vfs_file_open_mode(file, name, 1, 0x98967f) < 0) {
		vfs_file_dtor(file);
		return 0;
	}
	*(unsigned char*)0x22e4425 = _stricmp(name, (char*)0x59ac4c) == 0;
	vfs_file_read(file, magic, 4, 0, 0);
	if (strncmp(magic, (char*)0x59ac44, 4) != 0) {
		vfs_file_close(file);
		vfs_file_dtor(file);
		return 0;
	}
	if (fn_0046F6D0(level, file) == 0) {
		vfs_file_close(file);
		vfs_file_dtor(file);
		return 0;
	}
	fn_0046F990(level);
	fn_0046F660(level, file);
	fn_00503E80(0);
	fn_0046F100(level, file);
	fn_00503E80(0);
	count = *(int*)((char*)level + 0x20);
	index = 0;
	row = *(char**)((char*)level + 0x24);
	while (index < count) {
		fn_0046E910(level, row, file);
		fn_00503E80(0);
		count = *(int*)((char*)level + 0x20);
		index = index + 1;
		row = row + 0x54;
	}
	fn_005127F0((void*)0x22e43c0);
	count = *(int*)((char*)level + 0x20);
	index = 0;
	row = *(char**)((char*)level + 0x24);
	while (index < count) {
		((MeshPart*)(row + 0x38))->fn_004BCDC0(file);
		count = *(int*)((char*)level + 0x20);
		index = index + 1;
		row = row + 0x54;
	}
	fn_005127F0(0);
	count = *(int*)((char*)level + 0x28);
	index = 0;
	row = *(char**)((char*)level + 0x2c);
	while (index < count) {
		fn_0046EBE0(level, row, file);
		fn_00503E80(0);
		count = *(int*)((char*)level + 0x28);
		index = index + 1;
		row = row + 0x70;
	}
	vfs_file_close(file);
	vfs_file_dtor(file);
	return 1;
}

}

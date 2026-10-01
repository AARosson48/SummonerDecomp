// Leaf functions in bank/0046F210, plus the S3D header read and the
// name searches that walk the meshes after it.

struct VfsFile {
	void io_dword(unsigned int* value, int version, int fallback);
	void io_float(float* value, int version, int fallback);
	void set_user(int value);
	int error(void);
};

extern "C" {

int fn_0046FF50(void* key, void* row);
int fn_0046FFE0(void* key, void* second, void* row);
int fn_00470190(void* key, void* row);
int fn_00470410(void* file, void* row);

void fn_00470BB0()
{
	*(unsigned char*)0x22e455c = 1;
}

int fn_0046FFA0(void* key, char* row, int count)
{
	int index;
	int found;

	found = 0;
	index = 0;
	if (count > 0) {
		do {
			found = fn_0046FF50(key, row);
			if (found)
				break;
			index = index + 1;
			row = row + 0x14;
		} while (index < count);
	}
	return found;
}

int fn_004701E0(void* key, char* row, unsigned int count)
{
	unsigned int index;
	int found;

	found = 0;
	index = 0;
	if (count != 0) {
		do {
			found = fn_00470190(key, row);
			if (found)
				break;
			index = index + 1;
			row = row + 0x18;
		} while (index < count);
	}
	return found;
}

int fn_00470150(void* key, void* second, char* row, unsigned int count)
{
	unsigned int index;
	int found;

	found = 0;
	index = 0;
	if (count != 0) {
		do {
			found = fn_0046FFE0(key, second, row);
			if (found)
				break;
			index = index + 1;
			row = row + 8;
		} while (index < count);
	}
	return found;
}

int fn_00470560(VfsFile* file, char* row, unsigned int count)
{
	unsigned int index;

	index = 0;
	if (count != 0) {
		do {
			if (fn_00470410(file, row))
				return -1;
			index = index + 1;
			row = row + 0x20;
		} while (index < count);
	}
	return file->error() != 0 ? -1 : 0;
}

// fn_0046F6D0 reads the S3D header after the four bytes S3DF.
// The first dword is the version. The rest are the counts and the color.
bool fn_0046F6D0(char* level, VfsFile* file)
{
	unsigned int* version;
	bool ok;

	version = (unsigned int*)(level + 4);
	file->io_dword(version, 0, 0);
	file->set_user(*(int*)version);
	file->io_dword((unsigned int*)(level + 0x20), 0, 0);
	file->io_dword((unsigned int*)(level + 0x28), 0, 0);
	file->io_dword((unsigned int*)(level + 0x38), 0, 0);
	file->io_dword((unsigned int*)(level + 0x30), 0, 0);
	file->io_dword((unsigned int*)(level + 0x40), 0, 0);
	file->io_dword((unsigned int*)(level + 0x48), 0, 0);
	file->io_dword((unsigned int*)(level + 0x50), 0, 0);
	file->io_dword((unsigned int*)(level + 0x60), 0, 0);
	file->io_dword((unsigned int*)(level + 0x74), 0, 0);
	file->io_dword((unsigned int*)(level + 0x78), 0, 0);
	file->io_dword((unsigned int*)(level + 0x6c), 0, 0);
	file->io_dword((unsigned int*)(level + 0x90), 0, 0);
	file->io_dword((unsigned int*)(level + 0x94), 0, 0);
	file->io_dword((unsigned int*)(level + 0x9c), 0, 0);
	file->io_dword((unsigned int*)(level + 0x98), 0, 0);
	file->io_dword((unsigned int*)(level + 0xa0), 0, 0);
	file->io_dword((unsigned int*)(level + 0xa4), 0, 0);
	file->io_dword((unsigned int*)(level + 0x84), 0, 0);
	file->io_dword((unsigned int*)(level + 0x58), 0, 0);
	file->io_float((float*)(level + 0xd4), 0, 0);
	file->io_float((float*)(level + 0xd8), 0, 0);
	file->io_float((float*)(level + 0xdc), 0, 0);
	ok = file->error() == 0;
	return ok;
}

}

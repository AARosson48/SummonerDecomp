// Original: D:\projects\Summoner\pccode\vsdk\geom\effect_mem.cpp
// Effect pools. Rows are 0x14 bytes at 0x2C8F060.

static const char* em_file = "D:\\projects\\Summoner\\pccode\\vsdk\\geom\\effect_mem.cpp";

struct EmRow {
	unsigned short count;
	unsigned short kind;
	int at_4;
	int flags;
	int buffer;
	char* name;
};

struct EmObj {
	int fn_00536BC0(void* arg3, int arg2);
};

struct EmItem {
	int pad;
	int handle;
	int fn_0051CA30(int arg2, void* arg3);
};

struct EmGroup {
	unsigned short count;
	unsigned char pad[10];
	EmItem* items;
	void fn_0051C9F0(int arg2, void* arg3);
};

extern "C" int fn_0051BF70(char* name);
extern "C" int fn_0051BE80(int value);
extern "C" int fn_00440510(int handle);
extern "C" void* fn_00512800();
extern "C" int fn_004404A0(char* name, int pool, int zero);
extern "C" int fn_004408A0();
extern "C" void fn_00440560();
extern "C" void fn_00440B20();
extern "C" void fn_00503DA0(int on);
extern "C" void fn_00536E50(void* effect, int value);
extern "C" void fn_00531CB0(const char* file, int line, const char* message);
extern "C" int printf(const char* fmt, ...);
extern "C" int _stricmp(const char* a, const char* b);
extern "C" char* strchr(const char* text, int ch);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" char* _strlwr(char* text);
extern "C" void* memset(void* dst, int value, unsigned int size);
extern "C" char* fn_0056D5B7(char* text, const char* delim);
extern "C" int vfs_read_line(void* file, char* dst, int mode);
extern "C" void fn_005140D0(void* file);
extern "C" int fn_00514630(void* file, const char* name, int mode, int path);
extern "C" int fn_00525FE0(void* file, void* dst, int size, int a);
extern "C" void fn_005140F0(void* file);
extern "C" int fn_00514A10(void* file);
extern "C" void fn_00514740(void* file);
extern "C" void fn_00544B80(int a, int b, int c, int d);
extern "C" char* fn_00544CD0(int pool, char* name);
extern "C" void fn_00564D50(int* slot, int arg);

extern "C" int fn_0051BFD0(int id);
extern "C" void* fn_0051C370(int id);
extern "C" int fn_0051C490(int id);
extern "C" int fn_0051C520(int id);
extern "C" int fn_0051C5C0(int id);
extern "C" int fn_0051C660(int index);
extern "C" int fn_0051C680(int index);
extern "C" int fn_0051C6A0(int id, int* out_size, int* out_count, char flag, int pool);
extern "C" void* fn_0051C720(int index);
extern "C" int fn_0051C800();
extern "C" int fn_0051C810(char* name);
extern "C" int fn_0051C8B0(int id, unsigned int* a, int* b, int* c, unsigned int* d);
extern "C" void* fn_0051C9C0(char* name, int* out);
extern "C" int fn_0051CA60(void* file, int* line_no, char* name, int* kind, int* count, char* names);
extern "C" void* fn_0051CCD0();
extern "C" void* fn_0051CD40();
extern "C" int fn_0051CF60();

int EmItem::fn_0051CA30(int arg2, void* arg3)
{
	fn_00564D50(&handle, arg2);
	return ((EmObj*)handle)->fn_00536BC0(arg3, arg2);
}

void EmGroup::fn_0051C9F0(int arg2, void* arg3)
{
	unsigned int i = 0;
	if (count <= 0)
		return;
	do {
		items[i].fn_0051CA30(arg2, arg3);
		i++;
	} while (i < count);
}

extern "C" {

#define em_row(id) ((EmRow*)(0x2c8f060 + (id) * 0x14))

int fn_0051C660(int index)
{
	return index * 0x7a140 + 0x2965f10;
}

int fn_0051C680(int index)
{
	return index * 0x3d0c0 + 0x277d910;
}

int fn_0051C800()
{
	return *(int*)0x2c9f998;
}

int fn_0051C810(char* name)
{
	unsigned int count = *(unsigned int*)0x2c9f998;
	unsigned int i = 0;
	if (count > 0) {
		char** slot = (char**)0x2c8f070;
		do {
			if (_stricmp(*slot, name) == 0)
				return (int)i;
			i++;
			slot = (char**)((char*)slot + 0x14);
		} while (i < *(unsigned int*)0x2c9f998);
	}
	return -1;
}

void* fn_0051C9C0(char* name, int* out)
{
	int id = fn_0051BF70(name);
	if (id < 0)
		return 0;
	if (out)
		*out = id;
	return fn_0051C720(id);
}

int fn_0051C520(int id)
{
	int cursor = *(int*)0x2c9f974;
	int end = cursor + 6;
	int i = cursor;
	if (cursor < end) {
		do {
			int slot = i & 5;
			if (*(int*)(0x2c9f270 + slot * 4) == -1) {
				*(int*)(0x2c9f270 + slot * 4) = id;
				*(int*)0x2c9f974 = (slot + 1) & 5;
				return slot;
			}
			i++;
		} while (i < end);
	}
	i = cursor;
	if (cursor >= end)
		return -1;
	do {
		int slot = i & 5;
		int held = *(int*)(0x2c9f270 + slot * 4);
		if (*(unsigned char*)(held + 0x2c9f1b0) == 0) {
			fn_0051C370(held);
			*(int*)(0x2c9f270 + slot * 4) = id;
			*(int*)0x2c9f974 = (slot + 1) & 5;
			return slot;
		}
		i++;
	} while (i < end);
	return -1;
}

int fn_0051C5C0(int id)
{
	int cursor = *(int*)0x2c9f970;
	int end = cursor + 6;
	int i = cursor;
	if (cursor < end) {
		do {
			int slot = i & 7;
			if (*(int*)(0x27288b8 + slot * 4) == -1) {
				*(int*)(0x27288b8 + slot * 4) = id;
				*(int*)0x2c9f970 = (slot + 1) & 5;
				return slot;
			}
			i++;
		} while (i < end);
	}
	i = cursor;
	if (cursor >= end)
		return -1;
	do {
		int slot = i & 7;
		int held = *(int*)(0x27288b8 + slot * 4);
		if (*(unsigned char*)(held + 0x2c9f1b0) == 0) {
			fn_0051C370(held);
			*(int*)(0x27288b8 + slot * 4) = id;
			*(int*)0x2c9f970 = (slot + 1) & 5;
			return slot;
		}
		i++;
	} while (i < end);
	return -1;
}

int fn_0051C6A0(int id, int* out_size, int* out_count, char flag, int pool)
{
	if (id < 0 || id >= *(int*)0x2c9f998) {
		*out_count = 0;
		*out_size = 0;
		return -1;
	}
	EmRow* row = em_row(id);
	*out_count = 0;
	*out_size = fn_0051BE80((row->flags & 0x7fffff) + row->at_4);
	if (!flag)
		return 0;
	if (fn_004404A0(row->name, pool, 0))
		return 0;
	return -1;
}

int fn_0051C490(int id)
{
	EmRow* row = em_row(id);
	if ((row->flags & 0x80000000) == 0) {
		if (fn_004408A0())
			return -1;
		if ((row->flags & 0x40000000) == 0) {
			unsigned short kind = row->kind;
			if (kind > 9)
				return -1;
			if (kind < 9)
				return -1;
			if (fn_0051BFD0(id))
				return -1;
			fn_00440560();
		}
		if (fn_004408A0() == 1) {
			do {
			} while (fn_004408A0() == 1);
		}
		fn_00440B20();
	}
	return 0;
}

void* fn_0051C370(int id)
{
	EmRow* row = em_row(id);
	row->flags &= 0x7fffffff;
	row->buffer = 0;
	if ((row->flags & 0x38000000) == 0x18000000) {
		int live = *(int*)0x2c9f96c;
		if (live) {
			*(int*)0x2c9f96c = live - 1;
			if (live == 1) {
				int* saved;
				for (saved = (int*)0x2c8e7b0; saved < (int*)0x2c8e7c8; saved++) {
					if (*saved >= 0)
						fn_0051C490(*saved);
				}
				for (saved = (int*)0x272b8dc; saved < (int*)0x272b8fc; saved++) {
					if (*saved >= 0)
						fn_0051C490(*saved);
				}
				fn_00503DA0(1);
				int index = 0;
				if (*(int*)0x2c9f99c > 0) {
					do {
						int which = *(int*)(0x2c8e7c8 + index * 4);
						int flags = em_row(which)->flags;
						if ((flags & 0x80000000) && (flags & 0x1000000)) {
							void* got = fn_0051C720(index);
							if (got)
								fn_00536E50(got, -1);
						}
						index++;
					} while (index < *(int*)0x2c9f99c);
				}
				fn_00503DA0(0);
				int left = *(int*)0x2c9f998;
				if (left > 0) {
					int* flags = (int*)0x2c8f068;
					int remain = left;
					int step;
					do {
						if (*flags & 0x1000000)
							*flags &= 0xfeffffff;
						flags += 5;
						step = remain;
						remain--;
					} while (step != 1);
				}
			}
		}
	}
	return row;
}

void* fn_0051C720(int index)
{
	int which = *(int*)(index + 0x2c8e7c8);
	EmRow* row = em_row(which);
	if (row->flags & 0x80000000) {
		if (*(unsigned char*)(index + 0x2722820) == 0xff) {
			int key = fn_0051BE80(*(int*)(0x27281d8 + index * 4));
			int count = row->count;
			int slot = 0;
			int* entry = (int*)row->buffer;
			if (count > 0) {
				do {
					if (*entry == key) {
						*(unsigned char*)(index + 0x2722820) = (unsigned char)slot;
						break;
					}
					slot++;
					entry += 2;
				} while (slot < count);
			}
		}
		unsigned char found = *(unsigned char*)(index + 0x2722820);
		if (found != 0xff) {
			int* table = (int*)row->buffer;
			void* result = (void*)table[found * 2 + 1];
			if (found < row->count && result && (int)result > (int)table) {
				*(int*)((char*)result + 0x1c) = (row->flags >> 0x1b) & 7;
				return result;
			}
		}
	} else if ((!*(int*)0x2c9f96c || (row->flags & 0x4000000) || (row->flags & 0x38000000) == 0x18000000) && !fn_0051C490(which)) {
		if (*(unsigned char*)(index + 0x2722820) == 0xff) {
			int key = fn_0051BE80(*(int*)(0x27281d8 + index * 4));
			int count = row->count;
			int slot = 0;
			int* entry = (int*)row->buffer;
			if (count > 0) {
				do {
					if (*entry == key) {
						*(unsigned char*)(index + 0x2722820) = (unsigned char)slot;
						break;
					}
					slot++;
					entry += 2;
				} while (slot < count);
			}
		}
		unsigned char found = *(unsigned char*)(index + 0x2722820);
		if (found != 0xff) {
			int* table = (int*)row->buffer;
			void* result = (void*)table[found * 2 + 1];
			if (found < row->count && result && (int)result > (int)table) {
				*(int*)((char*)result + 0x1c) = (row->flags >> 0x1b) & 7;
				return result;
			}
		}
	}
	return 0;
}

int fn_0051C8B0(int id, unsigned int* a, int* b, int* c, unsigned int* d)
{
	char file[0x120];
	if (id < *(int*)0x2c9f998) {
		fn_005140D0(file);
		if (!fn_00514630(file, em_row(id)->name, 1, 0x98967f)) {
			int header[5];
			if (fn_00525FE0(file, header, 0x14, 0) == 0x14) {
				EmRow* row = em_row(id);
				row->count = (unsigned short)header[0];
				*a = row->count;
				row->at_4 = header[1];
				*b = header[1];
				row->flags = (row->flags & ~0x7fffff) | (header[2] & 0x7fffff);
				*c = header[2] & 0x7fffff;
				row->kind = (unsigned short)(header[0] >> 16);
				*d = row->kind;
			}
		}
		fn_005140F0(file);
	}
	return id;
}

int fn_0051BFD0(int id)
{
	EmRow* row = em_row(id);
	if ((row->flags & 0x80000000) || fn_00440510((int)row->name))
		return 0;
	printf("*************** ADD STREAMING EFFECT *****************\n");
	unsigned int a;
	int b;
	int c;
	unsigned int d;
	fn_0051C8B0(id, &a, &b, &c, &d);
	int* pool;
	if ((row->flags & 0x38000000) != 0x8000000) {
		pool = (int*)fn_00512800();
		if (!pool) {
			int kind = (row->flags >> 0x1b) & 7;
			if (!kind)
				pool = (int*)0x22e43c0;
			else if (kind == 5)
				pool = (int*)0x7592c0;
			else
				pool = (int*)0x2c98a70;
		}
	} else {
		pool = (int*)0x2c98a70;
	}
	if (pool)
		printf("Going into pool %s\n", pool[3]);
	row->flags &= 0xff7fffff;
	int size = 0;
	int count = 0;
	if (fn_0051C6A0(id, &size, &count, 0, (int)pool))
		return -1;
	row->buffer = 0;
	int total = size + count;
	if (pool != (int*)0x2c98a70) {
		printf("AS 9\n");
		row->flags = (row->flags & 0xfdffffff) | 0x4000000;
	} else {
		printf("AS 0\n");
		if ((row->flags & 0x38000000) != 0x8000000) {
			row->flags &= 0xfbffffff;
			if (total > 0x7a140 || (row->flags & 0x38000000) == 0x18000000) {
				printf("AS 2\n");
				if ((row->flags & 0x38000000) != 0x18000000) {
					printf("AS 3\n");
					return -1;
				}
				row->flags &= 0xfdffffff;
				if (!*(int*)0x2c9f96c) {
					int i;
					for (i = 0; i < 0x18; i += 4) {
						int held = *(int*)(0x2c9f270 + i);
						*(int*)(0x2c8e7b0 + i) = held;
						if (held >= 0) {
							fn_0051C370(held);
							*(int*)(0x2c9f270 + i) = -1;
						}
					}
					for (i = 0; i < 0x20; i += 4) {
						int held = *(int*)(0x27288b8 + i);
						*(int*)(0x272b8dc + i) = held;
						if (held >= 0) {
							fn_0051C370(held);
							*(int*)(0x27288b8 + i) = -1;
						}
					}
					row->buffer = 0x277d910;
				} else {
					row->buffer = *(int*)0x2c98a78 + *(int*)0x2c98a70;
				}
				*(int*)0x2c9f96c += 1;
			} else if (total <= 0x3d0c0) {
				printf("AS 5\n");
				int slot = fn_0051C5C0(id);
				if (slot < 0) {
					slot = fn_0051C520(id);
					if (slot < 0)
						printf("AS AIEEEEEEEEEEE\n");
					else {
						printf("AS 7 (%x)\n", em_row(id));
						row->buffer = fn_0051C660(slot);
						row->flags |= 0x2000000;
					}
				} else {
					printf("AS 6 (%x)\n", em_row(id));
					row->buffer = fn_0051C680(slot);
					row->flags &= 0xfdffffff;
				}
			} else {
				printf("AS 4 (%x)\n", em_row(id));
				int slot = fn_0051C520(id);
				if (slot < 0)
					printf("AS 4b\n");
				else {
					row->buffer = fn_0051C660(slot);
					row->flags |= 0x2000000;
				}
			}
		} else {
			printf("AS 1\n");
			row->buffer = 0x2731450;
			if (total > 0x4c4c0) {
				for (;;)
					fn_00531CB0(em_file, 0x447, "Preload.rfx is too big for the current preload size! I refuse to run!\n");
			}
		}
		if (!row->buffer)
			return -1;
		*(int*)0x2c98a70 = row->buffer;
		*(int*)0x2c98a74 = 0;
		*(int*)0x2c98a78 = 0;
	}
	if (fn_0051C6A0(id, &size, &count, 1, (int)pool))
		return -1;
	*(unsigned char*)(id + 0x2c9f1b0) += 1;
	return 0;
}

int fn_0051CA60(void* file, int* line_no, char* name, int* kind, int* count, char* names)
{
	char line[0x400];
	*count = 0;
	if (!vfs_read_line(file, line, 0))
		return -1;
	do {
		char* semi = strchr(line, ';');
		if (semi)
			*semi = 0;
		(*line_no)++;
		char* word = fn_0056D5B7(line, " \r\n\t:,");
		char* tag = fn_0056D5B7(0, " \r\n\t:,");
		char* list = fn_0056D5B7(0, " \r\n\t:,");
		if (word && tag && list) {
			strcpy(name, word);
			int matched = 0;
			if (_stricmp(tag, "preload") == 0) {
				*kind = 1;
				matched = 1;
			} else if (_stricmp(tag, "cutscene") == 0) {
				*kind = 3;
				matched = 1;
			} else if (_stricmp(tag, "level") == 0) {
				*kind = 0;
				matched = 1;
			} else if (_stricmp(tag, "spell") == 0) {
				*kind = 2;
				matched = 1;
			} else if (_stricmp(tag, "character") == 0) {
				*kind = 5;
				matched = 1;
			} else if (_stricmp(tag, "summon") == 0) {
				*kind = 4;
				matched = 1;
			}
			if (matched) {
				int n = *count;
				strcpy(names + n * 0x29, list);
				while (n < 0x20) {
					*count = n + 1;
					char* more = fn_0056D5B7(0, " \r\n\t:,");
					if (!more)
						return 0;
					n = *count;
					strcpy(names + n * 0x29, more);
				}
				return -1;
			}
		}
	} while (vfs_read_line(file, line, 0));
	return -1;
}

void* fn_0051CCD0()
{
	int* flags = (int*)0x2c8f068;
	memset((void*)0x27288b8, 0xff, 0x20);
	memset((void*)0x2c9f270, 0xff, 0x18);
	int id = 0;
	do {
		int value = *flags & 0xff7fffff;
		*flags = value;
		if ((value & 0x38000000) != 0x8000000 && (value & 0x80000000)) {
			fn_0051C370(id);
			*(unsigned char*)(id + 0x2c9f1b0) = 0;
		}
		flags += 5;
		id++;
	} while ((int)flags < 0x2c8ff68);
	return fn_0051CD40();
}

static void em_link(char* cursor, char* end, int stride, int prev_at, int next_at, int* head)
{
	char* prev = 0;
	while (cursor < end) {
		if (prev) {
			*(char**)(cursor + next_at) = *(char**)(prev + next_at);
			*(char**)(cursor + prev_at) = prev;
			*(char**)(*(char**)(prev + next_at) + prev_at) = cursor;
			*(char**)(prev + next_at) = cursor;
		} else {
			*(char**)(cursor + prev_at) = cursor;
			*(char**)(cursor + next_at) = cursor;
			prev = cursor;
		}
		cursor += stride;
	}
	*head = (int)prev;
}

void* fn_0051CD40()
{
	memset((void*)0x2c9f978, 0, 0x1c);
	char* cursor = (char*)0x2c7d050;
	char* prev = 0;
	while (cursor < (char*)0x2c7ddd0) {
		char* node = cursor - 0x40;
		if (prev) {
			*(char**)(cursor + 4) = *(char**)(prev + 0x44);
			*(char**)cursor = prev;
			*(char**)(*(char**)(prev + 0x44) + 0x40) = node;
			*(char**)(prev + 0x44) = node;
		} else {
			*(char**)cursor = node;
			*(char**)(cursor + 4) = node;
			prev = node;
		}
		*(int*)(cursor - 0x28) = 0;
		*(int*)(cursor - 0x24) = 0;
		*(int*)(cursor - 0x20) = 0;
		*(int*)(cursor - 0x18) = 0;
		*(int*)node &= ~0x40;
		cursor += 0x48;
	}
	*(int*)0x2c9f978 = (int)prev;
	em_link((char*)0x2c7ddb0, (char*)0x2c8c1b0, 0x98, 0x90, 0x94, (int*)0x2c9f97c);
	em_link((char*)0x2730e50, (char*)0x2731450, 0x30, 0x28, 0x2c, (int*)0x2c9f980);
	em_link((char*)0x2c8ff60, (char*)0x2c90a60, 0x2c, 0x24, 0x28, (int*)0x2c9f984);
	em_link((char*)0x2c8c1b0, (char*)0x2c8e7b0, 0x4c, 0x44, 0x48, (int*)0x2c9f988);
	em_link((char*)0x2c42690, (char*)0x2c7d010, 0x28, 0x20, 0x24, (int*)0x2c9f98c);
	em_link((char*)0x27229d8, (char*)0x27281d8, 0xb0, 0xa8, 0xac, (int*)0x2c9f990);
	return (void*)0x27281d8;
}

int fn_0051CF60()
{
	char file[0x640];
	int i = 0;
	memset((void*)0x27288b8, 0xff, 0x20);
	memset((void*)0x2c9f270, 0xff, 0x18);
	memset((void*)0x2c9f1b0, 0, 0xc0);
	memset((void*)0x2c9f288, 0xff, 0x6e0);
	memset((void*)0x27281d8, 0, 0x6e0);
	do {
		*(int*)(0x2c8e980 + i * 4) = i;
		i++;
	} while (i < 0x1b8);
	fn_00544B80(0x2c7dd98, 0x27288d8, 0x3000, 0);
	fn_0051CD40();
	*(int*)0x2c9f998 = 0;
	fn_005140D0(file);
	int result;
	if (!fn_00514630(file, "effects.arr", 1, 0x98967f)) {
		int lines = 0;
		if (!fn_00514A10(file)) {
			for (;;) {
				int count = 0;
				int kind = 0;
				char name[0x2c];
				char list[0x29 * 0x20];
				if (fn_0051CA60(file, &lines, name, &kind, &count, list)) {
					fn_005140F0(file);
					return -1;
				}
				int total = *(int*)0x2c9f998;
				int found = -1;
				int scan = 0;
				if (total > 0) {
					EmRow* row = (EmRow*)0x2c8f060;
					do {
						if (_stricmp(name, row->name) == 0) {
							int have = (row->flags >> 0x1b) & 7;
							found = scan;
							if (kind != have)
								kind = have;
						}
						total = *(int*)0x2c9f998;
						scan++;
						row++;
					} while (scan < total);
				}
				if (found < 0) {
					if (total >= 0xc0) {
						fn_005140F0(file);
						return -1;
					}
					found = total;
					EmRow* row = em_row(found);
					row->name = fn_00544CD0(0x2c7dd98, name);
					row->flags = ((kind & 7) << 0x1b) | (row->flags & 0x47ffffff);
					row->buffer = 0;
					row->flags &= 0xfe7fffff;
					*(int*)0x2c9f998 += 1;
				}
				if (count > 0) {
					char* item = list;
					int made = 0;
					while (made < count) {
						int have = *(int*)0x2c9f99c;
						if (have >= 0x1b8) {
							for (;;)
								fn_00531CB0(em_file, 0x6f7, "Too many effects! MAX_EFFECT_BASES needs to be increased!");
						}
						_strlwr(item);
						*(char**)(0x27281d8 + have * 4) = fn_00544CD0(0x2c7dd98, item);
						*(int*)(0x2c8e7c8 + have * 4) = found;
						*(unsigned char*)(have + 0x2722820) = 0xff;
						*(int*)0x2c9f99c = have + 1;
						item += 0x29;
						made++;
					}
				}
				if (fn_00514A10(file))
					break;
			}
		}
		fn_00514740(file);
		fn_005140F0(file);
		result = 0;
	} else {
		fn_005140F0(file);
		result = -1;
	}
	return result;
}

}

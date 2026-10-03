// Original: D:\projects\Summoner\pccode\Engine\ai\pathfinding.cpp
// Loads a level's .pfg graph. Version 0x6d is current. An older graph
// asserts and does not return.

#include <string.h>

struct VfsFile {
	void set_user(int value);
};

extern "C" void vfs_file_ctor(void* file);
extern "C" void vfs_file_dtor(void* file);
extern "C" int vfs_file_open_mode(void* file, const char* name, int mode, int path);
extern "C" int vfs_file_close(void* file);
extern "C" int vfs_read_dword(void* file, int version, int fallback);
extern "C" int vfs_read_dword_b(void* file, int version, int fallback);
extern "C" float vfs_read_float(void* file, float version, float fallback);
extern "C" int vfs_read_bool(void* file, int version, int fallback);
extern "C" int vfs_read_word(void* file, int version, int fallback);
extern "C" int vfs_read_word_b(void* file, int version, int fallback);
extern "C" void* vfs_heap_alloc(int size);
extern "C" int sprintf(char* dst, const char* fmt, ...);
extern "C" void fn_00531CB0(const char* file, int line, const char* message);
extern "C" void fn_005127F0(void* path);
extern "C" void fn_00503E80(int value);
extern "C" void fn_004A9D60(void);
extern "C" void fn_0049D930(void);
extern "C" void fn_005328D0(int scale);
extern "C" void fn_005130A0(char* dst, const char* name, const char* ext);
extern "C" int fn_0049C970(void* dst, void* file, int count);
extern "C" void fn_0049CA20(void* file, int count, void* dst);
extern "C" void fn_0049CC50(void* file, void* dst);
extern "C" void fn_0049CD90(void* file);
extern "C" void fn_0049CAF0(void);
extern "C" void fn_004A4F30(int layer, int index);
extern "C" char fn_004A4E60(int a, int b);

#define cell_ptr() (*(int**)0x2460ae4)

extern "C" int fn_0049CDD0(char* name)
{
	char file[0x120];
	char path[0x100];
	char message[0x100];
	volatile int version;
	int count;
	int block_count;
	int open_result;
	int saved;
	int used;
	int i;
	int j;
	int layer;
	int index;
	int nseg;
	int nnode;
	int left;
	int* edges;
	int* segs;
	int* seg;
	int* nodes;
	char* node;
	int* list;
	int* slot;
	int* cell;
	int connected;
	int* walk;
	int* link;
	int point;
	int other;

	vfs_file_ctor(file);
	fn_004A9D60();
	fn_0049D930();
	fn_005328D0(0x3e8);
	fn_005130A0(path, name, (const char*)0x59cc6c);
	open_result = vfs_file_open_mode(file, path, 1, *(int*)0x60ac30);
	if (open_result != 0) {
		if (open_result == -4) {
			vfs_file_dtor(file);
			return 0;
		}
		sprintf(message, (const char*)0x59cc3c, path);
		for (;;)
			fn_00531CB0((const char*)0x59cc04, 0x284, message);
	}

	fn_005127F0((void*)0x22e43c0);
	saved = *(int*)0x22e43c8;
	version = vfs_read_dword(file, 0, 0);
	if (version != 0x6d) {
		vfs_file_close(file);
		for (;;)
			fn_00531CB0((const char*)0x59cc04, 0x292, (const char*)0x59cb70);
	}
	((VfsFile*)file)->set_user(version);
	vfs_read_dword_b(file, 0, 0);

	count = vfs_read_dword(file, 0, 0);
	*(int*)0x2460af8 = count;
	if (count > 0)
		*(int*)0x245c9b0 = (int)vfs_heap_alloc(count * 2);

	count = vfs_read_dword(file, 0, 0);
	*(int*)0x245f3a0 = count;
	if (count > 0)
		*(int*)0x245c9b4 = (int)vfs_heap_alloc(count << 5);

	count = vfs_read_dword(file, 0, 0);
	*(int*)0x245f3b4 = count;
	if (count > 0) {
		void* block = vfs_heap_alloc(count * 0x28);
		*(int*)0x245c9b8 = (int)block;
		memset(block, 0, count * 0x28);
	}

	count = vfs_read_dword(file, 0, 0);
	*(int*)0x2460aec = count;
	if (count > 0)
		*(int*)0x245c9bc = (int)vfs_heap_alloc(count << 2);

	*(int*)0x2460c90 = vfs_read_dword(file, 0, 0);
	count = vfs_read_dword(file, 0, 0);
	*(int*)0x2460af0 = count;
	if (count > 0)
		*(int*)0x245c9c4 = (int)vfs_heap_alloc(count << 4);

	count = vfs_read_dword(file, 0, 0);
	*(int*)0x2460ae8 = count;
	if (count > 0)
		*(int*)0x245c9c8 = (int)vfs_heap_alloc(count << 3);

	count = vfs_read_dword(file, 0, 0);
	*(int*)0x2460af4 = count;
	if (count > 0)
		*(int*)0x245c9cc = (int)vfs_heap_alloc(count * 0xc);

	count = vfs_read_dword(file, 0, 0);
	*(int*)0x2460c8c = count;
	if (count > 0)
		*(int*)0x245c9d0 = (int)vfs_heap_alloc(count << 2);

	count = vfs_read_dword(file, 0, 0);
	*(int*)0x2460c98 = count;
	if (count > 0) {
		float* points = (float*)vfs_heap_alloc(count << 3);
		*(int*)0x2460ca8 = (int)points;
		for (i = 0; i < count; i++) {
			points[i * 2] = vfs_read_float(file, 0.0f, 0.0f);
			points[i * 2 + 1] = vfs_read_float(file, 0.0f, 0.0f);
		}
	}

	block_count = vfs_read_dword(file, 0, 0);
	if (block_count > 0)
		*(int*)0x2460cb0 = (int)vfs_heap_alloc(block_count << 4);
	*(int*)0x2460c9c = 0;
	*(int*)0x2460cb4 = fn_0049C970((void*)0x2460ca0, file, block_count);

	count = vfs_read_dword(file, 0, 0);
	*(int*)0x2460ca4 = count;
	if (count > 0) {
		edges = (int*)vfs_heap_alloc(count * 0x14);
		*(int*)0x2460cb8 = (int)edges;
		for (i = 0; i < count; i++) {
			int* edge = edges + i * 5;
			int a;
			int b;
			int c;
			int d;
			edge[4] = (i + 1 >= count) ? 0 : (int)(edge + 5);
			a = vfs_read_dword(file, 0, 0);
			b = vfs_read_dword(file, 0, 0);
			c = vfs_read_dword(file, 0, 0);
			d = vfs_read_dword(file, 0, 0);
			edge[0] = *(int*)0x2460ca8 + (a << 3);
			edge[1] = *(int*)0x2460ca8 + (b << 3);
			edge[2] = c;
			edge[3] = d;
			*(int*)(0x2452cb8 + c * 4) |= 1 << (d & 0x1f);
			*(int*)(0x2452cb8 + d * 4) |= 1 << (c & 0x1f);
			count = *(int*)0x2460ca4;
			edges = (int*)*(int*)0x2460cb8;
		}
	}

	count = vfs_read_dword(file, 0, 0);
	*(int*)0x2460ae0 = count;
	if (count > 0)
		*(int*)0x245c9d4 = (int)vfs_heap_alloc(count << 2);

	for (slot = (int*)0x245f3bc; (int)slot < 0x245f45c; slot += 2) {
		int n = vfs_read_dword(file, 0, 0);
		if (n == 0) {
			slot[-1] = 0;
			slot[0] = 0;
		} else {
			left = *(int*)0x2460ae0 - n;
			*(int*)0x2460ae0 = left;
			slot[-1] = n;
			slot[0] = *(int*)0x245c9d4 + (left << 2);
			for (i = 0; i < n; i++)
				((int*)slot[0])[i] = vfs_read_dword(file, 0, 0);
		}
	}

	for (layer = 0; layer < 3; layer++) {
		for (index = 0; index < 0x14; index++) {
			fn_004A4F30(layer, index);
			nseg = vfs_read_dword(file, 0, 0);
			if (nseg != 0) {
				left = *(int*)0x245f3a0 - nseg;
				*(int*)0x245f3a0 = left;
				segs = (int*)(*(int*)0x245c9b4 + (left << 5));
				*(int*)((char*)cell_ptr() + 0x20) = (int)segs;
				for (i = 0; i < nseg; i++) {
					seg = segs + i * 8;
					seg[7] = (i + 1 >= nseg) ? 0 : (int)(seg + 8);
					seg[1] = fn_0049C970(seg, file, block_count);
					*(float*)(seg + 5) = vfs_read_float(file, 0.0f, 0.0f);
					*(float*)(seg + 6) = vfs_read_float(file, 0.0f, 0.0f);
					*(float*)(seg + 3) = vfs_read_float(file, 0.0f, 0.0f);
					*(float*)(seg + 4) = vfs_read_float(file, 0.0f, 0.0f);
				}
			}

			fn_00503E80(0);
			cell = cell_ptr();
			*cell = vfs_read_dword(file, 0, 0);
			if (*cell > 0) {
				left = *(int*)0x245f3b4 - *cell;
				*(int*)0x245f3b4 = left;
				cell[4] = *(int*)0x245c9b8 + left * 0x28;
			}
			cell = cell_ptr();
			if (*cell > 0) {
				for (i = 0; i < *cell; i++) {
					node = (char*)cell[4] + i * 0x28;
					*(unsigned short*)(node + 6) = (unsigned short)index;
					point = vfs_read_dword(file, 0, 0);
					*(int*)node = *(int*)0x2460ca8 + (point << 3);
					*(unsigned short*)(node + 8) = (unsigned short)vfs_read_dword(file, 0, 0);
					if (*(unsigned short*)(node + 8) != 0) {
						left = *(int*)0x2460aec - *(short*)(node + 8);
						*(int*)0x2460aec = left;
						*(int*)(node + 0xc) = *(int*)0x245c9bc + (left << 2);
					}
					if (version >= 0x6c) {
						if (vfs_read_bool(file, 0, 1))
							*(unsigned short*)(node + 4) |= 0x80;
						else {
							point = vfs_read_dword(file, 0, 0);
							*(int*)(node + 0x10) = *(int*)0x2460cb0 + (point << 4);
							point = vfs_read_dword(file, 0, 0);
							*(int*)(node + 0x14) = *(int*)0x2460cb0 + (point << 4);
						}
					}
					cell = cell_ptr();
				}
			}

			cell = cell_ptr();
			*(int*)((char*)cell + 4) = vfs_read_dword(file, 0, 0);
			nnode = *(int*)((char*)cell + 4);
			if (nnode > 0) {
				left = *(int*)0x245f3b4 - nnode;
				*(int*)0x245f3b4 = left;
				*(int*)((char*)cell + 0x14) = *(int*)0x245c9b8 + left * 0x28;
			}
			cell = cell_ptr();
			nnode = *(int*)((char*)cell + 4);
			if (nnode > 0) {
				for (i = 0; i < nnode; i++) {
					node = (char*)*(int*)((char*)cell + 0x14) + i * 0x28;
					point = vfs_read_dword(file, 0, 0);
					*(int*)node = *(int*)0x2460ca8 + (point << 3);
					*(unsigned short*)(node + 8) = (unsigned short)vfs_read_dword(file, 0, 0);
					if (*(unsigned short*)(node + 8) != 0) {
						left = *(int*)0x2460aec - *(short*)(node + 8);
						*(int*)0x2460aec = left;
						*(int*)(node + 0xc) = *(int*)0x245c9bc + (left << 2);
					}
					list = *(int**)(node + 0xc);
					for (j = 0; j < *(short*)(node + 8); j++) {
						point = vfs_read_dword(file, 0, 0);
						list[j] = *(int*)((char*)cell_ptr() + 0x14) + point * 0x28;
					}
					cell = cell_ptr();
					nnode = *(int*)((char*)cell + 4);
				}
			}

			fn_00503E80(0);
			count = vfs_read_dword(file, 0, 0);
			for (i = 0; i < count; i++) {
				vfs_read_float(file, 0.0f, 0.0f);
				vfs_read_float(file, 0.0f, 0.0f);
				nseg = vfs_read_dword(file, 0, 0);
				for (j = 0; j < nseg; j++)
					vfs_read_dword(file, 0, 0);
				nseg = vfs_read_dword(file, 0, 0);
				if (nseg != 0) {
					*(int*)0x2460af8 -= nseg;
					for (j = 0; j < nseg; j++)
						vfs_read_word(file, 0, 0);
				}
			}

			fn_00503E80(0);
			cell = cell_ptr();
			*(int*)((char*)cell + 0xc) = vfs_read_dword(file, 0, 0);
			left = *(int*)0x2460af0 - *(int*)((char*)cell + 0xc);
			*(int*)0x2460af0 = left;
			*(int*)((char*)cell + 0x1c) = (*(int*)0x245c9c4) + (left << 4);
			cell = cell_ptr();
			fn_0049CA20(file, *(int*)((char*)cell + 0xc), (void*)*(int*)((char*)cell + 0x1c));
		}
	}

	fn_0049C970(message, file, block_count);
	for (layer = 0; layer < 3; layer++) {
		for (index = 0; index < 0x14; index++) {
			fn_004A4F30(layer, index);
			cell = cell_ptr();
			fn_0049CC50(file, (char*)cell + 0x24);
		}
	}

	for (layer = 0; layer < 3; layer++) {
		for (index = 0; index < 0x14; index++) {
			fn_004A4F30(layer, index);
			cell = cell_ptr();
			nnode = *cell;
			for (i = 0; i < nnode; i++) {
				node = (char*)cell[4] + i * 0x28;
				list = *(int**)(node + 0xc);
				for (j = 0; j < *(short*)(node + 8); j++) {
					other = vfs_read_word_b(file, 0, 0);
					point = vfs_read_dword(file, 0, 0);
					list[j] = *(int*)((layer * 0x14 + other) * 0x60 + 0x245f470) + point * 0x28;
				}
				if (version < 0x6c && (*(unsigned short*)(node + 4) & 0x80) == 0) {
					connected = 0;
					for (walk = *(int**)((char*)cell_ptr() + 0x20); walk != 0; walk = (int*)walk[7]) {
						if (walk[0] <= 0)
							continue;
						link = (int*)walk[1];
						for (j = 0; j < walk[0]; j++) {
							int hit = fn_004A4E60(link[j * 4], *(int*)node);
							if (hit == 0)
								hit = fn_004A4E60(link[j * 4 + 1], *(int*)node);
							if (hit != 0) {
								if (connected >= 2) {
									for (;;)
										fn_00531CB0((const char*)0x59cc04, 0x411, (const char*)0x59cbbc);
								}
								*(int*)(node + 0x10 + connected * 4) = (int)&link[j * 4];
								connected++;
							}
						}
					}
				}
				cell = cell_ptr();
				nnode = *cell;
			}
		}
	}

	fn_005127F0(0);
	used = *(int*)0x22e43c8 - saved;
	*(int*)0x2452d50 = used;
	fn_005328D0(0x3e8);
	fn_0049CD90(file);
	fn_0049CAF0();
	sprintf((char*)0x257ed80, (const char*)0x59cbec, used);
	open_result = vfs_file_close(file);
	vfs_file_dtor(file);
	return open_result;
}

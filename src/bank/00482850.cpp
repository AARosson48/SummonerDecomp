// Bank 00482850. Light and shadow records. Nodes are queued at 0x22f9e88
// and drawn from the lists at 0x22ea584, 0x22f8d84, and 0x22fa6a8.

extern "C" int fn_00507DB0(int a, int b, int c, int d, float e, float f);
extern "C" float fn_0050E7C0(float value);
extern "C" void fn_00506B90(int a, int b, int c, int d);
extern "C" int fn_004843E0(void);
extern "C" int fn_00484600(void);
extern "C" void fn_004A4B40(void* node, void* a, void* b);
extern "C" int fn_004BCBB0(void* node, void* a, void* b);
extern "C" void fn_004C08D0(int index, void* a, int b);
extern "C" void fn_0051B3F0(int id, void* a, void* b, void* c, void* d);
extern "C" int fn_004622B0(void* actor);
extern "C" int fn_00483430(void* dst, void* actor, int* pos, int* dir, int extra);
extern "C" int fn_0046C4E0(void* actor);
extern "C" int fn_0046A100(void* actor);
extern "C" int fn_004833B0(void* actor, void* mesh, int slot);
extern "C" int fn_0057AE80(void* a, int b, void* c, void* d, const char* name);
extern "C" void fn_00410CD0(void* dst, void* src);
extern "C" void fn_00413190(void* dst, void* a, void* b);
extern "C" void fn_004838F0(void* node, int index);
extern "C" int fn_0051E540(int handle);
extern "C" void* fn_0048D180(int index);
extern "C" int fn_0051E290(void* a, int b, int c, int d, int e, int f, int g, int h);
extern "C" int fn_004D80C0(void* host, int id);
extern "C" int fn_0044ED90(void* item);
extern "C" int fn_004D82B0(void* host, int id);
extern "C" int fn_00481C40(void* item, int arg);
extern "C" void fn_005245F0(float* dst, void* src);
extern "C" void* fn_0046B1A0(int id);
extern "C" int fn_00501910(void* node);
extern "C" int fn_0050ECD0(void* point, float radius, int mode);

extern "C" {

float fn_00482D40(void)
{
	fn_00507DB0(0, 0, 0, 0, -1.0f, -1.0f);
	return fn_0050E7C0(0.0f);
}

int fn_00484AA0(void)
{
	unsigned char* flags;
	int* slots;
	int column;

	flags = (unsigned char*)0x22f9558;
	slots = (int*)0x22f9560;
	do {
		column = 0;
		do {
			if (flags[column] == 0)
				*slots = 0;
			column++;
			slots++;
		} while (column < 3);
		flags += 3;
	} while ((int)slots < 0x22f9578);
	*(int*)0x22f9558 = 0;
	*(unsigned short*)0x22f955c = 0;
	return 0;
}

void fn_00484830(int count, int** items)
{
	int gap;
	int i;
	int j;
	int* left;
	int* right;

	if (count <= 1)
		return;
	for (gap = count / 2; gap > 0; gap = gap / 2) {
		for (i = gap; i < count; i++) {
			j = i - gap;
			while (j >= 0) {
				left = items[j];
				right = items[j + gap];
				if (!(*(float*)((char*)left + 8) < *(float*)((char*)right + 8)))
					break;
				items[j] = right;
				items[j + gap] = left;
				j -= gap;
			}
		}
	}
}

int fn_00482850(void)
{
	float fade;
	float wide;
	float high;

	if (*(float*)0x59bd80 <= 0.0f && *(float*)0x59bd7c <= 0.0f)
		return fn_00507DB0(0, 0, 0, 0, -1.0f, -1.0f);
	if (*(int*)0x22fb6f8 == 0) {
		fn_0050E7C0(*(float*)0x59bd7c);
		return fn_00507DB0(1, *(int*)0x22fb6d8, *(int*)0x22fb6dc, *(int*)0x22fb6e0,
			*(float*)0x59bd80, *(float*)0x59bd7c);
	}
	fade = *(float*)0x22fb6fc;
	if (*(float*)0x5a5744 < 15.0f)
		fade = fade + 0.02f;
	if (*(float*)0x5a5744 > 24.0f)
		fade = fade - 0.02f;
	if (fade <= 0.8f) {
		if (fade < 0.0f)
			fade = 0.0f;
	} else {
		fade = 0.8f;
	}
	*(float*)0x22fb6fc = fade;
	wide = *(float*)0x59bd80 * (1.0f - fade);
	high = *(float*)0x59bd7c * (1.0f - fade);
	if (high > 15.0f)
		high = 15.0f;
	fn_0050E7C0(high);
	return fn_00507DB0(1, *(int*)0x22fb6d8, *(int*)0x22fb6dc, *(int*)0x22fb6e0, wide, high);
}

void fn_00484380(void)
{
	if (*(unsigned char*)0x22fb714 == 0) {
		fn_00506B90(0xff, 0xff, 0xff, 0xff);
		fn_004843E0();
		fn_00484600();
	}
}

int fn_00486520(int index)
{
	int count;
	int result;
	char* node;
	int list[64];

	count = 0;
	node = (char*)(*(int*)0xa5a2cc + index * 0x70);
	if ((*(unsigned char*)(node + 9) & 4) == 0)
		fn_004A4B40(node, list, &count);
	result = fn_004BCBB0(node, list, &count);
	fn_004C08D0(index, list, count);
	return result;
}

int fn_004833B0(void* actor, void* mesh, int slot)
{
	int id;
	int pos[3];
	int dir[9];
	unsigned char kind;

	id = *(short*)(*(int*)((char*)actor + 0x71c) + slot * 2 + 0x168);
	if (id == -1)
		return id;
	fn_0051B3F0(id, (char*)actor + 0x38, (char*)actor + 0x10, dir, pos);
	kind = (unsigned char)fn_004622B0(actor);
	return fn_00483430(mesh, actor, pos, dir, *(int*)((char*)actor + 0x780) | (kind << 24) * 0 + kind);
}

int fn_00483430(void* dst, void* actor, int* pos, int* dir, int extra)
{
	char* rec;
	float* origin;
	float depth;

	if (dst == 0 || actor == 0 || *(int*)((char*)dst + 0x10) == 0)
		return 0;
	rec = *(char**)0x22f9e88;
	*(int*)0x22f9e88 = (int)(rec + 0x4c);
	if ((int)rec - 0x22ec498 > 0xc800)
		return 0;
	*(int*)(0x22ea584 + *(int*)0x22fb6c4 * 4) = (int)rec;
	*(int*)0x22fb6c4 = *(int*)0x22fb6c4 + 1;
	*(int*)(rec + 0x18) = extra;
	*rec = 2;
	*(int*)(rec + 0x14) = (int)dst;
	*(int*)(rec + 0x10) = (int)actor;
	*(int*)(rec + 0x1c) = pos[0];
	*(int*)(rec + 0x20) = pos[1];
	*(int*)(rec + 0x24) = pos[2];
	*(int*)(rec + 0x28) = dir[0];
	*(int*)(rec + 0x2c) = dir[1];
	*(int*)(rec + 0x30) = dir[2];
	fn_00410CD0(rec + 0x28, rec + 0x28);
	*(int*)(rec + 0x34) = dir[3];
	*(int*)(rec + 0x38) = dir[4];
	*(int*)(rec + 0x3c) = dir[5];
	fn_00410CD0(rec + 0x34, rec + 0x34);
	fn_00413190(rec + 0x40, rec + 0x34, dir + 6);
	origin = (float*)*(int*)((char*)dst + 0x10);
	depth = (origin[0x14] - *(float*)0x22f9e90) * *(float*)0x22faff0
		+ (origin[0x15] - *(float*)0x22f9e94) * *(float*)0x22faff4
		+ (origin[0x16] - *(float*)0x22f9e98) * *(float*)0x22faff8;
	*(float*)(rec + 4) = depth;
	*(float*)(rec + 8) = depth - *(float*)((char*)origin + 0x5c);
	return (int)rec;
}

int fn_00483260(void* actor)
{
	int kind;
	int count;
	int* slot;
	void* rig;
	void* mesh;

	if (*(int*)0x59f120 == 0x18 && fn_0046C4E0(actor))
		return 1;
	kind = *(int*)((char*)actor + 0xc);
	if (kind != 7 && kind != 5 && kind != 4 && fn_0046A100(actor) == 0)
		goto attachments;
	mesh = *(void**)((char*)actor + 0x768);
	if (mesh != 0) {
		kind = *(int*)((char*)actor + 0x720);
		if (kind != 0x11)
			fn_004833B0(actor, *(void**)((char*)mesh + 4), kind);
	}
	mesh = *(void**)((char*)actor + 0x778);
	if (mesh != 0)
		fn_004833B0(actor, *(void**)((char*)mesh + 4), 1);
	if ((*(int*)0x5fbfd8 & 0x20) == 0 || *(int*)((char*)actor + 0xc) != 7) {
		mesh = *(void**)((char*)actor + 0x76c);
		if (mesh != 0) {
			rig = *(void**)((char*)actor + 0x71c);
			if (*(short*)((char*)rig + 0x186) >= 0)
				fn_004833B0(actor, *(void**)((char*)mesh + 4), 0xf);
			else if (*(short*)((char*)rig + 0x16a) >= 0)
				fn_004833B0(actor, *(void**)((char*)mesh + 4), 1);
		}
	}
attachments:
	rig = *(void**)((char*)actor + 0x71c);
	if (*(int*)((char*)rig + 0x10) == 0x5a && (*(int*)0x5fbfd8 & 0x20) != 0) {
		if (fn_0057AE80((void*)0x257e974, 0, rig, (void*)0x257e974, "Ghost-Escape_cscript.tbl"))
			goto listed;
		if (fn_0057AE80((void*)0x257e974, 0, rig, (void*)0x257e974, "Ghost-Dismount_cscript.tbl"))
			goto listed;
		return 0;
	}
listed:
	count = *(int*)((char*)actor + 0x728);
	slot = (int*)((char*)actor + 0x72c);
	while (count > 0) {
		if (slot[0] != 0x11)
			fn_004833B0(actor, (void*)(0x8cbec0 + (slot[1] << 8)), slot[0]);
		slot += 2;
		count--;
	}
	return count;
}

int* fn_00482D70(void)
{
	int count;
	int i;
	char* row;
	int handle;

	count = *(int*)0x22fb704;
	if (count != 0) {
		for (i = count; i > 0; i--)
			fn_0051E540(*(int*)(0x22f9e28 + (i - 1) * 4));
	}
	if (*(int*)0x22fb714 != 0) {
		*(int*)0x22fb704 = 0;
		return (int*)*(int*)0x22fb714;
	}
	count = 0;
	for (i = 0; i < 0x18; i++) {
		row = (char*)fn_0048D180(i);
		if (row == 0 || row[0x48] == 0 || *row != 0)
			continue;
		handle = fn_0051E290(row + 0x14, *(int*)(row + 0x4c), *(int*)(row + 0x50),
			*(int*)(row + 0x38), *(int*)(row + 0x2c), *(int*)(row + 0x30),
			*(int*)(row + 0x34), 1);
		if (handle != -1) {
			*(int*)(0x22f9e28 + count * 4) = handle;
			*(int*)(0x22f8d18 + count * 4) = (int)row;
			count++;
		}
	}
	*(int*)0x22fb704 = count;
	return (int*)count;
}

int fn_00483850(int** groups, int count)
{
	int left;
	int** cursor;
	int* group;
	int n;
	int* ids;
	int index;
	char* node;
	int flags;

	left = count;
	cursor = groups;
	while (left > 0) {
		group = *cursor;
		n = group[1];
		ids = (int*)group[2];
		while (n > 0) {
			index = *ids;
			node = (char*)(*(int*)0xa5a2cc + index * 0x70);
			flags = *(int*)(node + 8);
			if ((flags & 0x2000) == 0) {
				flags |= 0x2000;
				*(int*)(node + 8) = flags;
				fn_004838F0(node, index);
			}
			ids++;
			n--;
		}
		cursor++;
		left--;
	}
	return left;
}

int fn_00485810(void)
{
	int flags;
	char* item;
	int kind;
	int talk;
	float dx;
	float dy;
	float dz;

	flags = *(int*)0x5fbfd8;
	if ((flags & 0x100) != 0 || (flags & 0x20) != 0)
		return flags;
	for (item = *(char**)0x8ecd78; item != (char*)0x8eca60; item = *(char**)(item + 0x318)) {
		if ((*(int*)(item + 0x320) & 0x80) != 0)
			continue;
		kind = *(int*)(item + 0xc);
		if (kind == 5 || kind == 9 || (*(int*)(item + 0x84) & 2) == 0)
			continue;
		if (fn_004D80C0((void*)0x25461a0, fn_0044ED90(item)) == 0)
			continue;
		talk = fn_004D82B0((void*)0x25461a0, fn_0044ED90(item));
		if ((*(int*)(talk + 4) & 8) == 0)
			continue;
		dx = *(float*)(item + 0x10) - *(float*)(*(int*)0x5fcc88 + 0x10);
		dy = *(float*)(item + 0x14) - *(float*)(*(int*)0x5fcc88 + 0x14);
		dz = *(float*)(item + 0x18) - *(float*)(*(int*)0x5fcc88 + 0x18);
		if (dx * dx + dy * dy + dz * dz < 100.0f)
			flags = fn_00481C40(item, *(int*)0x5bd7b4);
	}
	return flags;
}

int fn_004841E0(float x, float y, float z, float radius, void* actor, int kind, int arg7, int arg8, int force)
{
	float point[3];
	char* rec;
	void* owner;
	float dx;
	float dy;
	float dz;
	float depth;

	fn_005245F0(point, (char*)actor + 0x38);
	point[0] += *(float*)((char*)actor + 0x10);
	x += *(float*)((char*)actor + 0x14);
	y += *(float*)((char*)actor + 0x18);
	if (*(int*)0x22fb714 == 0) {
		if (force == 0 && (*(int*)((char*)actor + 0xbc) & 8) == 0 && *(int*)((char*)actor + 0xa4) != 4) {
			owner = 0;
			if (*(int*)((char*)actor + 0xb4) > 0)
				owner = fn_0046B1A0(*(int*)((char*)actor + 0xb4));
			if (owner != 0 && (*(unsigned char*)((char*)owner + 0x321) & 1) != 0) {
				if (fn_00501910((char*)actor + 0xdc) == 0)
					return 0;
			}
			if (owner == 0 || (*(int*)((char*)owner + 0x84) & 2) == 0) {
				if (fn_0050ECD0(point, radius, 0) != 0)
					return 0;
			}
		}
		rec = *(char**)0x22f9e88;
		*(int*)0x22f9e88 = (int)(rec + 0x1c);
		if ((int)rec - 0x22ec4c8 > 0xc800)
			return 0;
		if ((*(int*)((char*)actor + 0xbc) & 2) == 0 || kind != 0) {
			*(int*)(0x22f8d84 + *(int*)0x22fb6cc * 4) = (int)rec;
			*(int*)0x22fb6cc = *(int*)0x22fb6cc + 1;
		} else {
			*(int*)(0x22fa6a8 + *(int*)0x22fb6c8 * 4) = (int)rec;
			*(int*)0x22fb6c8 = *(int*)0x22fb6c8 + 1;
		}
		dx = point[0] - *(float*)0x22f9e90;
		dy = x - *(float*)0x22f9e94;
		dz = y - *(float*)0x22f9e98;
		*rec = 3;
		*(int*)(rec + 0x10) = (int)actor;
		rec[0x16] = (char)kind;
		*(int*)(rec + 0x14) = arg7;
		*(int*)(rec + 0x18) = arg8;
		depth = dx * *(float*)0x22faff0 + dz * *(float*)0x22faff8 + dy * *(float*)0x22faff4;
		*(float*)(rec + 4) = depth;
		*(float*)(rec + 8) = depth - radius;
		return 1;
	}
	return 0;
}

int fn_004829E0(int** groups, int count)
{
	int left;
	int** cursor;
	int* group;
	int n;
	int* ids;
	char* node;
	int flags;
	int i;
	int* listed;

	left = count;
	cursor = groups;
	while (left > 0) {
		group = *cursor;
		n = group[1];
		ids = (int*)group[2];
		while (n > 0) {
			node = (char*)(*(int*)0xa5a2cc + (*ids) * 0x70);
			flags = *(int*)(node + 8);
			if ((flags & 0x4000) == 0) {
				flags |= 0x4000;
				*(int*)(node + 8) = flags;
				node = (char*)(*(int*)0x22f9d7c + (*ids) * 0xc);
				flags = *(int*)(node + 4);
				*(int*)(node + 4) = ((flags | 0xff0) & 0xf3) | ((flags & 4) << 1);
			}
			ids++;
			n--;
		}
		cursor++;
		left--;
	}
	if ((*(int*)0x5fbfd8 & 0x20) == 0) {
		n = *(int*)0x22e4aec;
		listed = (int*)0x22e49b8;
		for (i = 0; i < n; i++)
			*(int*)(*(int*)0x22f9d7c + listed[i] * 0xc + 4) |= 4;
	}
	return left;
}

int fn_00482E20(void)
{
	char* actor;

	for (actor = *(char**)0x970774; actor != (char*)0x970770; actor = *(char**)(actor + 4)) {
		if (*(int*)(actor + 0xc) < 3 || *(int*)(actor + 0xc) > 11)
			continue;
		if ((*(int*)(actor + 0x84) & 2) != 0)
			*(int*)(actor + 0x84) |= 8;
		else
			*(int*)(actor + 0x84) &= ~8;
	}
	return 0;
}

void fn_004838F0(void* node, int index)
{
	char* row;
	char* rec;
	int flags;

	row = (char*)(*(int*)0x22f9d7c + index * 0xc);
	if ((*(int*)(row + 4) & 0xff0) == 0)
		return;
	rec = *(char**)0x22f9e88;
	*(int*)0x22f9e88 = (int)(rec + 0x10);
	if ((int)rec - 0x22ec4d4 > 0xc800)
		return;
	flags = *(int*)(row + 4) & 0xff0;
	if (flags != 0xff0) {
		*(int*)(0x22f8d84 + *(int*)0x22fb6cc * 4) = (int)rec;
		*(int*)0x22fb6cc = *(int*)0x22fb6cc + 1;
	} else if ((*(int*)((char*)node + 8) & 0x10) == 0) {
		*(int*)(0x22ea584 + *(int*)0x22fb6c4 * 4) = (int)rec;
		*(int*)0x22fb6c4 = *(int*)0x22fb6c4 + 1;
	} else {
		*(int*)(0x22fa6a8 + *(int*)0x22fb6c8 * 4) = (int)rec;
		*(int*)0x22fb6c8 = *(int*)0x22fb6c8 + 1;
	}
	*rec = 1;
	*(unsigned short*)(rec + 2) = (unsigned short)index;
}

int fn_00483BA0(void* actor)
{
	return *(int*)((char*)actor + 0xc);
}

int fn_00483F90(void* actor, int mode)
{
	(void)mode;
	return *(int*)((char*)actor + 0x84);
}

int fn_004843E0(void)
{
	int i;
	char** list;

	i = 0;
	list = (char**)0x22ea584;
	while (i < *(int*)0x22fb6c4) {
		if (list[i] != 0 && *list[i] == 1)
			*(unsigned short*)(*(int*)0x22fa6a0 + (*(unsigned short*)(list[i] + 2) << 2) + 2) = 0xffff;
		i++;
	}
	return i;
}

int fn_00484600(void)
{
	int i;

	i = 0;
	while (i < *(int*)0x22fb6cc) {
		i++;
	}
	return i;
}

int fn_004848C0(void* record)
{
	void* actor;
	int kind;

	actor = *(void**)((char*)record + 0xc);
	kind = *(int*)((char*)actor + 0xc);
	if (kind != 5 && kind != 7 && kind != 9)
		return kind;
	if (*(int*)0x5fbfd8 & 0x20)
		return kind;
	return *(int*)((char*)actor + 0x84);
}

int fn_00484AE0(char* record)
{
	return record != 0 ? *record : 0;
}

int fn_00484E30(void* record)
{
	return record != 0 ? *(int*)record : 0;
}

int fn_004850A0(void* a, void* b)
{
	(void)b;
	return a != 0;
}

int fn_00485380(void* list, int count)
{
	(void)list;
	return count;
}

int fn_004858F0(int count, void** records)
{
	int i;

	for (i = 0; i < count; i++) {
		if (records[i] != 0 && *(unsigned char*)records[i] == 1)
			fn_00506B90(0xff, 0xff, 0xff, 0xff);
	}
	return i;
}

int fn_004863B0(void* node, void* other, int* count, float* depths, void** out)
{
	float dx;
	float dy;
	float dz;
	float dist;
	int n;
	int i;

	dx = *(float*)((char*)node + 0x44) - *(float*)((char*)other + 0x14);
	dy = *(float*)((char*)node + 0x48) - *(float*)((char*)other + 0x18);
	dz = *(float*)((char*)node + 0x4c) - *(float*)((char*)other + 0x1c);
	dist = dx * dx + dy * dy + dz * dz;
	if (dist < 0.0f)
		dist = 0.0f;
	n = *count;
	for (i = 0; i < n; i++) {
		if (dist > depths[i]) {
			if (n < 0x10)
				*count = n + 1;
			depths[i] = dist;
			out[i] = other;
			return i;
		}
	}
	if (n < 0x10) {
		depths[n] = dist;
		out[n] = other;
		*count = n + 1;
		return n + 1;
	}
	return n;
}

}

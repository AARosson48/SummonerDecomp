// Bank 0049D930. AI path nodes. The assert string in this block says
// the node table overflow should be reported to DaveA or Allender.

extern "C" {

int fn_0049D970(void)
{
	int count = *(int*)0x245c9dc + 1;
	*(int*)0x245c9dc = count;
	return count;
}

// Push a node onto the free list at 0x2452d30. The link is at +0x24.
void fn_004A0440(int* node)
{
	*(int*)((char*)node + 0x24) = *(int*)0x2452d30;
	*(int*)0x2452d30 = (int)node;
	(*(int*)0x2452d4c)--;
}

struct Vec3 {
	float x;
	float y;
	float z;
	Vec3* fn_00410CD0(Vec3* src);
};

void fn_004A4F30(int extra, int id);
int* fn_004A5D40(Vec3* at, int id);
int fn_0049ECB0(Vec3* a, Vec3* b, Vec3* c);
int fn_004A5DC0(Vec3* goal, int extra, int bit);
int* fn_004A6600(int bit, int category, Vec3* goal);
int* fn_004A0330(Vec3* at, int tag, int zero);
int fn_004A55F0(Vec3* a, int id, Vec3* b, int* tag, int mode);
void fn_0049F340(Vec3* a, int id, Vec3* b, int* tag);
void fn_004A0480(int* node);

// Finds a path from start to goal. Category selects a 20-bit mask at
// 0x2452cb8. A direct probe can return the node immediately. Otherwise the
// cells from 0x2460dc0 to 0x246a050 are cleared and the open list is searched.
// The success walk calls fn_004A03C0 along the link at node+0x1c.
int* fn_0049FA70(Vec3* start, int id, Vec3* goal, int category, int extra, float cost)
{
	Vec3 start_copy;
	Vec3 snapped;
	int* from;
	int* to;
	int* node;
	int bit;
	int found;
	int tag;
	unsigned char* cell;

	*(int*)0x245c9a8 = 0;
	*(int*)0x2452d38 = 0;
	fn_004A4F30(extra, id);
	from = fn_004A5D40(start, id);
	to = fn_004A5D40(goal, category);
	if (to == 0) {
		if (from == 0)
			return 0;
		if (fn_0049ECB0(&snapped, goal, start)) {
			goal->x = snapped.x;
			goal->z = snapped.z;
			to = fn_004A5D40(goal, category);
		}
		if (to == 0)
			return 0;
	}

	found = -1;
	for (bit = 0; bit < 0x14; bit++) {
		if ((*(int*)(0x2452cb8 + category * 4) & (1 << bit)) == 0)
			continue;
		if (fn_004A5DC0(goal, extra, bit) == 1) {
			found = bit;
			break;
		}
	}
	fn_004A4F30(extra, id);
	tag = category;
	if (found != -1)
		tag = (int)fn_004A6600(found, category, goal);

	start_copy.fn_00410CD0(start);
	node = fn_004A0330(&start_copy, tag, 0);
	if (node == 0)
		return 0;
	*(float*)((char*)node + 0x1c) = -cost;

	if (from == to) {
		if (fn_004A55F0(start, id, goal, (int*)tag, 2)
			&& fn_004A55F0(start, id, goal, (int*)tag, 4))
			return node;
	} else if (fn_004A55F0(start, id, goal, (int*)tag, 3)
		&& fn_004A55F0(start, id, goal, (int*)tag, 4)) {
		return node;
	}

	fn_0049F340(start, id, goal, (int*)tag);
	cell = (unsigned char*)0x2460dc0;
	while (cell < (unsigned char*)0x246a050) {
		int count;
		int* slot;
		if ((cell[-0xc] & 1) && *(int*)cell == extra) {
			count = *(short*)(cell - 2);
			slot = (int*)(cell + 0x294);
			while (count > 0) {
				*slot = 0;
				slot = (int*)((char*)slot + 0x44);
				count--;
			}
		}
		cell += 0xa78;
	}
	fn_004A0480(node);
	return 0;
}

}

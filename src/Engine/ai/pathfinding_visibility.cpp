// Original: D:\projects\Summoner\pccode\Engine\ai\pathfinding_visibility.cpp
// Edge tree for point visibility. Nodes are 0x28 bytes.

struct VisNode {
	int a;
	int b;
	int pad_8;
	int pad_c;
	float at_10;
	float at_14;
	float at_18;
	int pad_1c;
	VisNode* prev;
	VisNode* next;
};

extern "C" int sprintf(char* dst, const char* fmt, ...);
extern "C" void fn_00531CB0(const char* file, int line, const char* message);
extern "C" int fn_004A4F60(int a, void* b);
extern "C" int fn_004A5750(void* a, void* b, void* c);
extern "C" int fn_004B86E0(float* point, int layer);
extern "C" double fn_0056E520(double value);
extern "C" unsigned int fn_005328D0(int scale);
extern "C" int fn_005329B1();
extern "C" int* fn_004B77F0(float* point);
extern "C" void fn_004B7AF0(float* point);

extern "C" int fn_004B7DA0();
extern "C" void* fn_004B7D30();
extern "C" void* fn_004B7DF0(void* node, int layer);
extern "C" void* fn_004B8150(void* node);
extern "C" void* fn_004B8190(void* node, int layer);
extern "C" void* fn_004B7E80(int* edge, int layer, int point);
extern "C" int fn_004B8230(int* from, int* edge, char wide);
extern "C" int fn_004B8350(float* point, short plane);

extern "C" {

int fn_004B7DA0()
{
	int ready = *(int*)0x247fcec;
	if (ready)
		return ready;
	int sentinel = 0x247fcc0;
	int node = 0x247ed00;
	*(int*)0x247fce4 = sentinel;
	int prev = sentinel;
	do {
		*(int*)(node + 0x20) = prev;
		*(int*)(node + 0x24) = sentinel;
		*(int*)(prev + 0x24) = node;
		prev = node;
		node += 0x28;
	} while (node < 0x247fca0);
	*(int*)0x247fce0 = prev;
	*(int*)0x247fcec = 1;
	return node;
}

void* fn_004B7D30()
{
	if (!*(int*)0x247fcec)
		fn_004B7DA0();
	int node = *(int*)0x247fce4;
	if (node == 0x247fcc0) {
		sprintf((char*)0x257ed80, (const char*)0x59d81c, 0x64);
		for (;;)
			fn_00531CB0((const char*)0x59d7d8, 0x128, (const char*)0x257ed80);
	}
	int* prev = (int*)(node + 0x20);
	int* next = (int*)(node + 0x24);
	*(int*)(*prev + 0x24) = *next;
	*(int*)(*next + 0x20) = *prev;
	*next = 0;
	*prev = 0;
	return (void*)node;
}

void* fn_004B8150(void* node)
{
	if (!*(int*)0x247fcec)
		fn_004B7DA0();
	*(int*)((char*)node + 0x20) = *(int*)0x247fce0;
	*(int*)((char*)node + 0x24) = 0x247fcc0;
	*(int*)(*(int*)0x247fce0 + 0x24) = (int)node;
	*(int*)0x247fce0 = (int)node;
	return node;
}

static VisNode* layer_head(int layer)
{
	return *(VisNode**)(0x247e98c + layer * 0x28);
}

static VisNode* layer_sentinel(int layer)
{
	return (VisNode*)(0x247e968 + layer * 0x28);
}

void* fn_004B7DF0(void* node, int layer)
{
	VisNode* sent = layer_sentinel(layer);
	VisNode* cursor = layer_head(layer);
	float key = *(float*)((char*)node + 0x10);
	if (cursor == sent) {
		((VisNode*)node)->prev = *(VisNode**)(0x247e988 + layer * 0x28);
		((VisNode*)node)->next = sent;
		(*(VisNode**)(0x247e988 + layer * 0x28))->next = (VisNode*)node;
		*(int*)(0x247e988 + layer * 0x28) = (int)node;
		return node;
	}
	while (cursor != sent) {
		if (key <= cursor->at_10) {
			cursor->prev->next = (VisNode*)node;
			((VisNode*)node)->prev = cursor->prev;
			cursor->prev = (VisNode*)node;
			((VisNode*)node)->next = cursor;
			return cursor->prev;
		}
		cursor = cursor->next;
	}
	((VisNode*)node)->prev = *(VisNode**)(0x247e988 + layer * 0x28);
	((VisNode*)node)->next = sent;
	(*(VisNode**)(0x247e988 + layer * 0x28))->next = (VisNode*)node;
	*(int*)(0x247e988 + layer * 0x28) = (int)node;
	return *(void**)(0x247e988 + layer * 0x28);
}

void* fn_004B8190(void* node, int layer)
{
	VisNode* sent = layer_sentinel(layer);
	VisNode* cursor = layer_head(layer);
	VisNode* item = (VisNode*)node;
	if (cursor == sent) {
		item->prev = *(VisNode**)(0x247e988 + layer * 0x28);
		item->next = sent;
		(*(VisNode**)(0x247e988 + layer * 0x28))->next = item;
		*(int*)(0x247e988 + layer * 0x28) = (int)item;
		return item;
	}
	while (cursor != sent) {
		if (fn_004A4F60(item->a, cursor)) {
			cursor->prev->next = item;
			item->prev = cursor->prev;
			cursor->prev = item;
			item->next = cursor;
			return item;
		}
		cursor = cursor->next;
	}
	item->prev = *(VisNode**)(0x247e988 + layer * 0x28);
	item->next = sent;
	(*(VisNode**)(0x247e988 + layer * 0x28))->next = item;
	*(int*)(0x247e988 + layer * 0x28) = (int)item;
	return *(void**)(0x247e988 + layer * 0x28);
}

void* fn_004B7E80(int* edge, int layer, int point)
{
	VisNode* sent = layer_sentinel(layer);
	VisNode* cursor = layer_head(layer);
	while (cursor != sent) {
		int same = (cursor->a == edge[0] && cursor->b == edge[1]) || (cursor->a == edge[1] && cursor->b == edge[0]);
		if (same) {
			cursor->prev->next = cursor->next;
			cursor->next->prev = cursor->prev;
			cursor->next = 0;
			cursor->prev = 0;
			return fn_004B8150(cursor);
		}
		cursor = cursor->next;
	}
	float* end_a = (float*)edge[0];
	float* end_b = (float*)edge[1];
	float dx = end_a[0] - end_b[0];
	float dy = end_a[1] - end_b[1];
	float* sweep = *(float**)0x247fca8;
	float* here = *(float**)0x247fcac;
	float sx = sweep[0] - here[0];
	float sy = sweep[1] - here[1];
	if (dx * sy - dy * sx < 0.0f)
		return 0;
	VisNode* made = (VisNode*)fn_004B7D30();
	if (edge[0] != point) {
		made->a = edge[1];
		made->b = edge[0];
		dx = -dx;
		dy = -dy;
	} else {
		made->a = edge[0];
		made->b = edge[1];
	}
	float length = (float)fn_0056E520((double)(dx * dx + dy * dy));
	*(float*)((char*)made + 0x10) = length;
	*(float*)((char*)made + 0x14) = dx / length;
	*(float*)((char*)made + 0x18) = dy / length;
	if (!*(int*)0x247ecf0)
		return fn_004B8190(made, layer);
	return made;
}

int fn_004B8230(int* from, int* edge, char wide)
{
	if (*(int*)0x247ecdc) {
		float* a = (float*)from[0];
		float* b = (float*)edge[0];
		float mid[2];
		mid[0] = (a[0] + b[0]) * 0.5f;
		mid[1] = (a[1] + b[1]) * 0.5f;
		int layer = from[6] + *(int*)0x245f458 * 0x14;
		if (!fn_004B86E0(mid, *(int*)(0x245f47c + layer * 0x60))) {
			int base = from[6] * 0x28;
			VisNode* cursor = *(VisNode**)(0x247e98c + base);
			VisNode* sent = (VisNode*)(0x247e968 + base);
			while (cursor != sent) {
				if (fn_004A5750(from, cursor, 0) == 1)
					return 1;
				if (!wide)
					break;
				cursor = cursor->next;
			}
			return 0;
		}
	}
	return 1;
}

int fn_004B8350(float* point, short plane)
{
	unsigned int started = fn_005328D0(0x3e8);
	int clock = fn_005329B1();
	int held[0x32];
	int held_count = 0;
	int i;
	for (i = 0; i < 0x32; i++)
		held[i] = 0;
	*(int*)0x247fca8 = (int)point;
	*(int*)0x247ec88 = 0;
	*(int*)0x247fcb8 = 0;
	int* edge = fn_004B77F0(point);
	fn_004B7AF0(point);
	int* previous = 0;
	while (edge) {
		int layer = edge[6];
		char crossed = (edge[1] & 2) || (edge[6] && (*(int*)(edge[6] + 4) & 2)) ? 1 : 0;
		if ((edge[1] & 2) == 0)
			*(int*)0x247ecdc = 1;
		*(int*)0x247fcac = edge[0];
		int before = fn_005329B1();
		int blocked = fn_004B8230(previous ? previous : edge, edge, crossed);
		*(int*)0x247fcb8 += fn_005329B1() - before;
		if (blocked)
			edge[1] &= ~1;
		else
			edge[1] |= 1;
		int mark = fn_005329B1();
		if ((edge[1] & 0x80) == 0) {
			if (!crossed || held_count == 0x32) {
				*(int*)0x247ecf0 = 0;
				fn_004B7E80((int*)edge[4], layer, edge[0]);
				fn_004B7E80((int*)edge[5], layer, edge[0]);
			}
			if (crossed && held_count < 0x32)
				held[held_count++] = (int)edge;
			if (crossed && held_count == 0x32 && *(int*)0x247fce8) {
				for (;;)
					fn_00531CB0((const char*)0x59d7d8, 0x3ca, "Too many points on same sweep line.  Get DaveA");
			}
		}
		*(int*)0x247ec88 += fn_005329B1() - mark;
		if (blocked)
			*(int*)0x247ecdc = 0;
		else {
			*(int*)0x247ecdc = 1;
			previous = edge;
		}
		edge = (int*)edge[6];
	}
	int* head = (int*)0x247e98c;
	while (head < (int*)0x247ecac) {
		void* node = (void*)*head;
		while (node != (void*)(head - 9)) {
			VisNode* item = (VisNode*)node;
			item->prev->next = item->next;
			item->next->prev = item->prev;
			item->next = 0;
			item->prev = 0;
			fn_004B8150(item);
			node = (void*)*head;
		}
		head += 10;
	}
	int elapsed = fn_005329B1() - clock;
	fn_005328D0(0x3e8);
	sprintf((char*)0x257ed80, "determine_point_visibility() took %d ms.\n", elapsed);
	(void)started;
	(void)plane;
	return elapsed;
}

}

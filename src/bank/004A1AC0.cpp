// One pass of bank/004A1AC0. Strings in this block: 333@333@, Set the height level to draw obstacles relative to main player pos, obstacle_height_offset.

struct ConsoleCmd {
	void fn_0050A180(const char* name, const char* help, void (*handler)(void));
};

extern "C" {

void fn_004A3DE0(void)
{
	((ConsoleCmd*)0x245cd90)->fn_0050A180((char*)0x59cdac, (char*)0x59cdc4, (void (*)(void))0x4a3e10);
}

void fn_00506B90(int r, int g, int b, int a);
void fn_004A4EC0(float* src, float* dst);
void fn_0053E610(float* from, float* to, int mode);

struct ObstacleEdge {
	float* a;
	float* b;
	float* extra0;
	float* extra1;
};

struct ObstacleGroup {
	int count;
	ObstacleEdge* edges;
	int pad[5];
	ObstacleGroup* next;
};

static float obstacle_height(void)
{
	int player = *(int*)0x5fcc88;
	return *(float*)0x245ce04 + *(float*)(player + 0x14) + 0.01f;
}

static void draw_edge(float* a, float* b)
{
	float from[3];
	float to[3];
	float lifted[3];
	float lifted_to[3];
	float height = obstacle_height();
	int i;

	fn_004A4EC0(a, from);
	fn_004A4EC0(b, to);
	for (i = 0; i < 3; i++) {
		lifted[i] = from[i];
		lifted_to[i] = to[i];
	}
	lifted[1] = height;
	lifted_to[1] = height;
	fn_0053E610(lifted, lifted_to, *(int*)0x25d88c0);
}

// Draws the avoidance obstacles. The current layer is bright. The other
// nineteen layers draw only while 0x245cdf8 is set. Cells are the same
// 0xa78-byte rows the path query clears.
void fn_004A3890(void)
{
	int layer;
	int page;
	int count;
	ObstacleGroup* group;
	ObstacleEdge* edge;
	float** extra;
	int* cell;
	int flags;
	short* link;
	float* vert_a;
	float* vert_b;

	if (*(int*)0x59cd94)
		*(int*)0x245cde8 = *(int*)(*(int*)0x5fcc88 + 0x68c);

	page = *(int*)0x59cd90;
	for (layer = 0; layer < 0x14; layer++) {
		if (layer == *(int*)0x245cde8)
			fn_00506B90(0, 0xff, 0x40, 0xff);
		else if (*(int*)0x245cdf8)
			fn_00506B90(0, 0x80, 0x20, 0xff);
		else
			continue;
		group = *(ObstacleGroup**)(0x245f480 + (layer + page * 0x14) * 0x60);
		while (group) {
			for (count = 0; count < group->count; count++) {
				edge = &group->edges[count];
				draw_edge(edge->a, edge->b);
			}
			group = group->next;
		}
	}

	fn_00506B90(0x80, 0x20, 0, 0xff);
	extra = *(float***)0x2460cb8;
	for (count = 0; count < *(int*)0x2460ca4; count++) {
		draw_edge(extra[0], extra[1]);
		extra += 5;
	}

	cell = (int*)0x2460db4;
	while (cell < (int*)0x246a044) {
		flags = cell[0];
		if (flags & 1) {
			if (flags & 2)
				fn_00506B90(0x20, 0x20, 0x80, 0xff);
			else
				fn_00506B90(0x80, 0x20, 0x20, 0xff);
			if (cell[4] == *(int*)0x245cde8 && cell[3] == page && cell[2] > 0) {
				float from[3];
				float to[3];
				float height = obstacle_height();
				link = (short*)(cell + 5);
				for (count = 0; count < cell[2]; count++) {
					vert_a = (float*)((char*)cell - 4 + link[0] * 0x44 + 0x270);
					vert_b = (float*)((char*)cell - 4 + link[1] * 0x44 + 0x270);
					from[0] = vert_a[0];
					from[2] = vert_a[1];
					to[0] = vert_b[0];
					to[2] = vert_b[1];
					from[1] = height;
					to[1] = height;
					fn_0053E610(from, to, *(int*)0x25d88c0);
					link += 10;
				}
			}
		}
		cell += 0x29e;
	}
}

}

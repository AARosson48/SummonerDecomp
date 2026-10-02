// One pass of bank/0048A230. Strings in this block: Sets Time_mult, time_mult, Sets Space_mult, space_mult, Sets Small_mesh_threshold, small_mesh_threshold.

struct ConsoleCmd {
	void fn_0050A180(const char* name, const char* help, void (*handler)(void));
};

extern "C" {

void fn_0048DC20(void)
{
	((ConsoleCmd*)0x23fc838)->fn_0050A180((char*)0x59c488, (char*)0x59c494, (void (*)(void))0x48dc50);
}

void fn_0048DCE0(void)
{
	((ConsoleCmd*)0x2432478)->fn_0050A180((char*)0x59c4b0, (char*)0x59c4bc, (void (*)(void))0x48dd10);
}

void fn_0048DDA0(void)
{
	((ConsoleCmd*)0x23f81d8)->fn_0050A180((char*)0x59c4d8, (char*)0x59c4f0, (void (*)(void))0x48ddd0);
}

void* fn_0046B130(int id);

// Attach a value at +0xc0 when the object kind at +0xc is 2 and the slot is empty.
void fn_0048CF50(int id, int value)
{
	int* obj = (int*)fn_0046B130(id);
	if (obj == 0)
		return;
	if (obj[3] != 2)
		return;
	if (obj[0x30] != 0)
		return;
	obj[0x30] = value;
}

// Rows are 152 bytes at 0x2cb45d0. A first byte of 0xFF means the row is unused.
unsigned char* fn_0048D180(int index)
{
	unsigned char* row = (unsigned char*)(0x2cb45d0 + index * 152);
	if (row[0] == 0xff)
		return 0;
	return row;
}

void fn_005468C0(int effect, void* at, int mode, float scale);
int fn_0048B750(int* entity, int arg);
void fn_0048CF80(int id);
int fn_0048C080(const char* name, int parent, int a, int b, int c, int d);
void fn_0048D280(int spawned, void* parent, int mode);
void* fn_0046B1A0(int id);
void fn_00494BA0(int arg);
void fn_004553A0(void* node, int kind, int value);

struct Obj4902A8 {
	void fn_00501880(int ticks);
	char fn_00501910(void);
};

// Walks the entity list at 0x23059b8. The link is +0x98 and the sentinel is
// 0x2305920. Kind +0xa4 selects one of seventeen cases. Kind 9 attaches
// spell-status.vfx. Kinds 1, 2, and 10 share the case that can attach
// stone-fade.vfx. Subtypes 0x1a, 0x1d, 0x1f, 0x23, and 0x4e are queued, then
// the list at 0x8ecd78 is walked through +0x318 up to the sentinel 0x8eca60.
void fn_0048A230(int arg, char skip)
{
	int* entity;
	int* queued[16];
	int count = 0;
	int kind;
	int subtype;
	int spawned;
	void* obj;
	Obj4902A8* timer;
	int flags;
	int i;

	if (skip)
		return;
	*(int*)0x2c9f968 = 0;
	entity = *(int**)0x23059b8;
	while (entity != (int*)0x2305920) {
		if (entity[0x38] != -1)
			fn_005468C0(entity[0x38], entity + 4, 0, 1.75f);
		if (*(unsigned char*)((char*)entity + 0x84) & 0x44) {
			entity = (int*)entity[0x26];
			continue;
		}
		kind = entity[0x29];
		if (kind == 0) {
			if (fn_0048B750(entity, arg) == 1)
				fn_0048CF80(entity[0x20]);
			entity = (int*)entity[0x26];
			continue;
		}
		if (kind == 9) {
			timer = (Obj4902A8*)((char*)entity + 0xdc);
			if (timer->fn_00501910()) {
				spawned = fn_0048C080((const char*)0x59c2d0, entity[0x2d], -1, -1, 0, 0);
				if (spawned != -1) {
					obj = fn_0046B130(spawned);
					flags = *(int*)((char*)obj + 0xbc);
					flags = flags ^ (((flags ^ ((flags << 15) >> 15)) & 0x1fdff));
					*(int*)((char*)obj + 0xbc) = flags | 0x200;
					obj = fn_0046B130(entity[0x2d]);
					fn_0048D280(spawned, obj, 0);
					timer->fn_00501880(0x2710);
				}
			}
		}
		subtype = entity[0x2a];
		if (subtype == 0x1a || subtype == 0x1d || subtype == 0x1f || subtype == 0x23
			|| subtype == 0x4e) {
			if (count < 16)
				queued[count++] = entity;
		}
		entity = (int*)entity[0x26];
	}

	fn_00494BA0(arg);
	entity = *(int**)0x8ecd78;
	while (entity != (int*)0x8eca60) {
		if (entity[3] != 4) {
			for (i = 0; i < count; i++)
				fn_0046B1A0(queued[i][0x2c]);
			fn_004553A0(entity, 0xf, 1);
		}
		entity = (int*)entity[0xc6];
	}
}

}

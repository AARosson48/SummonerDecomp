// Original: D:\projects\Summoner\pccode\Engine\rendering\levelrender.cpp
// Level mesh draw. The vertex buffer is created when the render mode is 0x66.

struct LrVec {
	float x;
	float y;
	float z;
};

struct LrLight {
	int kind;
	char object[0x10];
	LrVec position;
	float at_20;
	float at_24;
	float at_28;
	float scale;
	float at_34;
	float radius_a;
	float radius_b;
	unsigned char flags;
	unsigned char pad_41[7];
	int model;
};

struct LrMesh {
	int pad0;
	void* section;
	int flags;
	void* faces;
	void* list;
};

struct VbDesc {
	int size;
	int caps;
	int fvf;
	int count;
};

typedef int (__stdcall *CreateVB)(void* device, VbDesc* desc, void** out_buf, int unused);
typedef int (__stdcall *ReleaseFn)(void* self);
typedef int (__stdcall *LockFn)(void* self, int a, int b, int c);
typedef int (__stdcall *UnlockFn)(void* self);
typedef int (__stdcall *DrawFn)(void* self, int prim, void* buffer, int count, void* mesh, void* indices, int zero, int zero2);

extern "C" void fn_0048DE60(void* mesh, void* dest);
extern "C" void fn_0051E0C0();
extern "C" void fn_0051E230(float x, float y, float z);
extern "C" LrVec* fn_00522CC0(LrVec* out);
extern "C" LrVec* fn_005243D0(LrVec* out, LrVec* dir);
extern "C" void fn_0051E3F0(LrVec* pos, LrVec* dir, float a, float b, float c, float d, float e, float f, float g, int h, int i, int model, void* object);
extern "C" void fn_0051E290(LrVec* pos, float a, float b, float c, float d, float e, float scale, int zero);
extern "C" int fn_00549560(int a, int b, int c, int d, int* e, int f);
extern "C" void fn_0054A100(int value);
extern "C" void fn_005245A0(void* matrix);
extern "C" float fn_0050ECB0(float value);
extern "C" int fn_0050EB30(void* face, float* pos);
extern "C" int fn_0056C7E4();
extern "C" void fn_00534A40(int value);
extern "C" int fn_0054D050(int a, int b, int c, int d, int* e, int f);
extern "C" char* fn_00538C70(int err);
extern "C" void fn_00531CB0(const char* file, int line, const char* message);
extern "C" int fn_0053E9D0(int kind, void* verts);
extern "C" void fn_0056D1CD(void (*fn)());

static const char* lr_file = "D:\\projects\\Summoner\\pccode\\Engine\\rendering\\levelrender.cpp";

extern "C" {

void fn_0048FD90()
{
}

void fn_0048FDA0()
{
}

// Create the level vertex buffer. Asserts "Unable to create level vertex buffer for D3d!"
void fn_0048E470()
{
	if (*(int*)0x25d8d3c != 0x66)
		return;
	if (*(void**)0x243e03c != 0)
		return;
	void* device = *(void**)0x2dc6c3c;
	VbDesc desc;
	desc.size = 0x10;
	desc.caps = 0x10800;
	if (*(unsigned char*)0x2dc6c31 != 0)
		desc.caps += 1;
	desc.fvf = 0x2c4;
	desc.count = 0x2140;
	CreateVB create_vb = (CreateVB)(*(int*)(*(int*)device + 0x14));
	int err = create_vb(device, &desc, (void**)0x243e03c, 0);
	if (err < 0) {
		for (;;) {
			fn_00531CB0(lr_file, 0x450, "Unable to create level vertex buffer for D3d!");
		}
	}
}

void fn_0048E4F0()
{
	if (*(int*)0x25d8d3c != 0x66)
		return;
	void* buffer = *(void**)0x243e03c;
	if (buffer == 0)
		return;
	ReleaseFn release = (ReleaseFn)(*(int*)(*(int*)buffer + 8));
	release(buffer);
	*(void**)0x243e03c = 0;
}

void fn_0048E520()
{
	fn_0051E0C0();
	float scale = *(float*)0x59c358;
	fn_0051E230(*(float*)0xa5a374 * scale, *(float*)0xa5a378 * scale, *(float*)0xa5a37c * scale);
	int count = *(int*)0xa5a2e8;
	if (count <= 0)
		return;
	char* base = *(char**)0xa5a2ec;
	for (int i = 0; i < count; i++) {
		LrLight* light = (LrLight*)(base + i * 0x4c);
		float ra;
		float rb;
		if (light->flags & 2) {
			ra = light->radius_b;
			rb = light->radius_a;
		} else {
			ra = 20000.0f;
			rb = 20000.0f;
		}
		int kind = light->kind;
		if (kind == 0) {
			fn_0051E290(&light->position, rb, ra, light->at_20, light->at_24, light->at_28, light->scale * scale, 0);
			continue;
		}
		if (kind != 1 && kind != 4)
			continue;
		LrVec spun;
		LrVec dir;
		dir.x = 0;
		dir.y = -1.0f;
		dir.z = 0;
		fn_00522CC0(&spun);
		LrVec* aimed = fn_005243D0(&dir, &spun);
		int model = light->model;
		if (model != -1) {
			if (model < 0 || model >= *(int*)0xa5a318)
				continue;
			model = ((int*)*(int*)0xa5a320)[model];
		}
		fn_0051E3F0(&light->position, aimed, light->at_34, ra, rb, light->at_20, light->at_24, light->at_28,
			light->scale * scale, 0, 0, model, (void*)((char*)light + 4));
	}
}

int fn_0048FBB0(float* out, float* origin, float* dir, void** lights, int count)
{
	int hits = 0;
	out[0] = 0;
	out[1] = 0;
	out[2] = 0;
	if (count <= 0)
		return 0;
	for (int i = 0; i < count; i++) {
		char* light = (char*)lights[i];
		float dx = *(float*)(light + 0x20) - origin[0];
		float dy = *(float*)(light + 0x24) - origin[1];
		float dz = *(float*)(light + 0x28) - origin[2];
		float dist = dx * dx + dy * dy + dz * dz;
		dist = dist > 0 ? dist : 0;
		unsigned char flagged = *(unsigned char*)(light + 0x48);
		if (flagged != 0 && dist > *(float*)(light + 0x50))
			continue;
		float weight;
		if (dist < 1.0f)
			weight = 1.0f;
		else {
			weight = (dx * dir[0] + dy * dir[1] + dz * dir[2]) / dist;
			if (weight <= 0)
				continue;
		}
		if (flagged != 0 && dist > *(float*)(light + 0x4c)) {
			float span = *(float*)(light + 0x50) - *(float*)(light + 0x4c);
			weight *= (*(float*)(light + 0x50) - dist) / span;
		}
		weight *= *(float*)(light + 0x38) * 0.75f;
		float r = weight * *(float*)(light + 0x2c);
		if (r < 1.0f)
			r = 1.0f;
		out[0] += r;
		float g = weight * *(float*)(light + 0x30);
		if (g < 1.0f)
			g = 1.0f;
		out[1] += g;
		float b = weight * *(float*)(light + 0x34);
		if (b < 1.0f)
			b = 1.0f;
		out[2] += b;
		hits++;
	}
	if (out[0] < 1.0f)
		out[0] = 1.0f;
	if (out[1] < 1.0f)
		out[1] = 1.0f;
	if (out[2] < 1.0f)
		out[2] = 1.0f;
	return hits;
}

void* fn_0048F8C0(void* mesh, void* face, int* indices, void* positions, int use_extra, unsigned char* flags, float* verts, unsigned char* colors, int* out_count)
{
	unsigned short* words = (unsigned short*)((char*)face + 2);
	char* dst = (char*)0x23c5ab0;
	int written = 0;
	for (int i = 0; i < 3; i++) {
		int index = words[i];
		float* src = (float*)((char*)positions + indices[i] * 12);
		float* corner = (float*)(dst - 8);
		*(int*)((char*)0x23ec630 + i * 4) = (int)corner;
		corner[0] = src[0];
		corner[1] = src[1];
		corner[2] = src[2];
		int uv = ((int*)(*(int*)(*(int*)((char*)mesh + 4) + 0x14)))[index * 2];
		*(int*)(dst + 0x14) = uv;
		*(int*)(dst + 0x18) = ((int*)(*(int*)(*(int*)((char*)mesh + 4) + 0x14)))[index * 2 + 1];
		if (use_extra) {
			*(int*)(dst + 0x1c) = indices[0];
			*(int*)(dst + 0x20) = indices[3];
		}
		unsigned char* rgb = (unsigned char*)(*(int*)((char*)mesh + 0x68) + index * 3);
		if (*(unsigned char*)0x25d8dc0 != 0) {
			float sum = (float)rgb[0] + (float)rgb[1] + (float)rgb[2];
			if (sum < 0)
				sum = 0;
			if (sum > 255.0f)
				sum = 255.0f;
			dst[0x24] = (unsigned char)(int)(sum * *(float*)0x25d8dc4);
			dst[0x25] = (unsigned char)(int)(sum * *(float*)0x25d8dc8);
			dst[0x26] = (unsigned char)(int)(sum * *(float*)0x25d8dcc);
		} else if (colors != 0 && use_extra) {
			float* tint = (float*)(colors + index * 12);
			dst[0x24] = (unsigned char)(int)(tint[0] * 255.0f);
			dst[0x25] = (unsigned char)(int)(tint[1] * 255.0f);
			dst[0x26] = (unsigned char)(int)(tint[2] * 255.0f);
		} else if (colors != 0) {
			float* tint = (float*)(colors + index * 12);
			int r = rgb[0] - (int)(tint[0] * -255.0f);
			int g = rgb[1] - (int)(tint[1] * -255.0f);
			int b = rgb[2] - (int)(tint[2] * -255.0f);
			if (r > 255)
				r = 255;
			if (g > 255)
				g = 255;
			if (b > 255)
				b = 255;
			dst[0x24] = (unsigned char)r;
			dst[0x25] = (unsigned char)g;
			dst[0x26] = (unsigned char)b;
		} else {
			dst[0x24] = rgb[0];
			dst[0x25] = rgb[1];
			dst[0x26] = rgb[2];
		}
		dst[0x27] = (unsigned char)0xff;
		dst[0x12] = (unsigned char)0xf;
		dst[0x11] = 0;
		dst[0x13] = (unsigned char)written;
		dst[0x10] = flags[indices[i]];
		dst += 0x30;
		written++;
	}
	int built = fn_0053E9D0(3, (void*)0x23ec630);
	if (out_count != 0)
		*out_count = built;
	return (void*)0x23ec630;
}

static void lr_assert(int err, int line)
{
	for (;;) {
		fn_00531CB0(lr_file, line, fn_00538C70(err));
	}
}

static int lr_clip_flags(float* p, int wide)
{
	int flags = 0;
	if (p[0] > p[2])
		flags |= 4;
	if (p[1] > p[2])
		flags |= 0x10;
	if (-p[2] > p[0])
		flags |= 2;
	if (-p[2] > p[1])
		flags |= 8;
	if (p[2] <= 0)
		flags |= 0x20;
	if (*(unsigned char*)0x2692e84 != 0 && p[2] > *(float*)0x2692e8c)
		flags |= 1;
	if (flags == 0) {
		float inv = 1.0f / p[2];
		p[0] = (inv * p[0] + 1.0f) * *(float*)0x2692824 + *(float*)0x25d8d5c;
		p[1] = (1.0f - inv * p[1]) * *(float*)0x26927e8 + *(float*)0x25d8d60;
		if (wide)
			p[2] = 1.0f / (p[2] - fn_0050ECB0(0.1f));
		else
			p[2] = inv;
	}
	return flags;
}

int fn_0048E6C0(LrMesh* mesh, int arg2, int arg3, void** lights)
{
	int done = -1;
	if (*(unsigned char*)0x2f132b8 == 1 && *(int*)0x2f132b0 == -1)
		return done;
	if ((*(unsigned char*)0x23eb68c & 1) == 0) {
		*(unsigned char*)0x23eb68c |= 1;
		fn_0056D1CD(fn_0048FDA0);
	}
	if ((*(unsigned char*)0x23eb68c & 2) == 0) {
		*(unsigned char*)0x23eb68c |= 2;
		fn_0056D1CD(fn_0048FD90);
	}
	int wide = (mesh->flags & 0x200400) != 0;
	float* positions;
	if (mesh->flags & 0x20) {
		fn_0048DE60(mesh, (void*)0x23ec658);
		positions = (float*)0x23ec658;
	} else {
		positions = *(float**)(*(int*)mesh->section + 0xc);
	}
	char* section = (char*)mesh->section;
	int use_far = *(float*)(section + 0x30) > *(float*)0x59c47c;
	float near_z = use_far ? *(float*)0x25d8db8 : *(float*)0x25d8dac;
	float far_z = use_far ? *(float*)0x25d8dbc : *(float*)0x25d8db0;
	int fog = *(unsigned char*)0x25d8dc0 != 0;
	int special = fog || ((*(int*)(*(int*)((char*)mesh + 0xc) + 4) << 8) < 0) || (mesh->flags & 0x80000);
	int surfaces = *(int*)(section + 0x50);
	int cursor = 0;
	float scale_a = 0;
	float scale_b = 0;
	if (special) {
		int last_mat = -1;
		for (int s = 0; s < surfaces; s++) {
			short* group = (short*)(*(int*)(section + 0x4c) + s * 4);
			short face_id = *(short*)(*(int*)((char*)mesh + 0xc) + cursor * 8);
			if (last_mat != 1) {
				int which = *(int*)0x25d8d28;
				if (face_id > 0)
					which = *(int*)0x25d8df8;
				fn_0054A100(which);
				last_mat = 1;
			}
			if (face_id > 0) {
				int tex = ((int*)*(int*)0xa5a320)[face_id - 1];
				int one = 1;
				fn_00549560(tex, 0, (int)&scale_a, (int)&scale_b, &one, 1);
				if ((mesh->flags >> 0x17) & 1)
					scale_b = scale_a;
			}
			int vert_count = group[1];
			for (int v = 0; v < vert_count; v++) {
				int index = cursor + v;
				short* src_index = (short*)(*(int*)(section + 0x18) + index * 2);
				float* pos = positions + index * 3;
				float local[3];
				unsigned char clip;
				if (*src_index < 0) {
					if (arg3)
						fn_0048FBB0((float*)(0x23d3608 + index * 12), pos, (float*)(*(int*)(section + 0x10) + index * 12), lights, arg2);
					local[0] = pos[0] - *(float*)0x26924a8;
					local[1] = pos[1] - *(float*)0x26924ac;
					local[2] = pos[2] - *(float*)0x26924b0;
					fn_005245A0(local);
					clip = (unsigned char)lr_clip_flags(local, wide);
					((unsigned char*)0x2437a74)[index] = clip;
				} else {
					int from = index - *src_index;
					clip = ((unsigned char*)0x2437a74)[from];
					((unsigned char*)0x2437a74)[index] = clip;
				}
				(void)near_z;
				(void)far_z;
				(void)clip;
			}
			cursor += vert_count;
		}
	} else {
		fn_0054A100(*(int*)0x25d8d1c);
		for (int s = 0; s < surfaces; s++) {
			short* group = (short*)(*(int*)(section + 0x4c) + s * 4);
			int vert_count = group[1];
			for (int v = 0; v < vert_count; v++) {
				int index = cursor + v;
				short src = *(short*)(*(int*)(section + 0x18) + index * 2);
				float* pos = positions + index * 3;
				if (src < 0) {
					if (arg3)
						fn_0048FBB0((float*)(0x23d3608 + index * 12), pos, (float*)(*(int*)(section + 0x10) + index * 12), lights, arg2);
					float local[3];
					local[0] = pos[0] - *(float*)0x26924a8;
					local[1] = pos[1] - *(float*)0x26924ac;
					local[2] = pos[2] - *(float*)0x26924b0;
					fn_005245A0((void*)0x26924d8);
					((unsigned char*)0x2437a74)[index] = (unsigned char)lr_clip_flags(local, wide);
				} else {
					((unsigned char*)0x2437a74)[index] = ((unsigned char*)0x2437a74)[index - src];
				}
			}
			int face_count = group[0];
			for (int f = 0; f < face_count; f++) {
				int* face = (int*)(*(int*)(section + 0x20) + (cursor + f) * 0x14);
				int visible = 1;
				if ((face[1] & 2) == 0) {
					int vi = *(short*)((char*)face + 8);
					visible = fn_0050EB30(face + 2, positions + vi * 3);
				}
				if (!visible)
					continue;
				int tri_count = 3;
				fn_0048F8C0(mesh, face, 0, (void*)0x2437a74, (int)(void*)0x2400ec8, (unsigned char*)&tri_count, (float*)0x23d3608, (unsigned char*)arg3, &tri_count);
			}
			cursor += vert_count;
		}
	}
	return cursor;
}

void fn_0048FDB0(int esi_arg, LrMesh* mesh, int arg3, char* arg4, void** lights)
{
	if ((*(unsigned char*)0x23d3600 & 1) == 0) {
		*(unsigned char*)0x23d3600 |= 1;
		fn_0056D1CD((void (*)())0x4914f0);
	}
	if ((*(unsigned char*)0x23d3600 & 2) == 0) {
		*(unsigned char*)0x23d3600 |= 2;
		fn_0056D1CD((void (*)())0x4914e0);
	}
	(void)esi_arg;
	char* section = (char*)mesh->section;
	int wide = (mesh->flags & 0x200400) != 0;
	int use_far = *(float*)(section + 0x30) > *(float*)0x59c47c;
	float near_z = use_far ? *(float*)0x25d8db8 : *(float*)0x25d8dac;
	float far_z = use_far ? *(float*)0x25d8dbc : *(float*)0x25d8db0;
	(void)near_z;
	(void)far_z;
	(void)wide;
	float* positions;
	if (mesh->flags & 0x20) {
		fn_0048DE60(mesh, (void*)0x23ec658);
		positions = (float*)0x23ec658;
	} else {
		positions = *(float**)(section + 0xc);
	}
	int fog = *(unsigned char*)0x25d8dc0 != 0;
	int special = fog || ((*(int*)(*(int*)mesh->faces + 4) << 8) < 0) || (mesh->flags & 0x80000);
	if (!special) {
		int queued = *(int*)0x240d9e8;
		if (queued >= 0x1f40)
			*(int*)0x240d9e8 = 0;
		fn_00534A40(queued ? *(int*)0x5a9d28 : *(int*)0x5a9d24);
		void* device = *(void**)0x243e03c;
		LockFn lock = (LockFn)(*(int*)(*(int*)device + 0xc));
		int err = lock(device, *(int*)0x5a9d28, 0, 0);
		if (err < 0)
			lr_assert(err, 0x727);
		int surfaces = *(int*)(section + 0x50);
		int cursor = 0;
		for (int s = 0; s < surfaces; s++) {
			short* group = (short*)(*(int*)(section + 0x4c) + s * 4);
			int vert_count = group[1];
			for (int v = 0; v < vert_count; v++) {
				int index = cursor + v;
				short src = *(short*)(*(int*)(section + 0x18) + index * 2);
				float* pos = positions + index * 3;
				if (src < 0) {
					if (arg4)
						fn_0048FBB0((float*)(0x23d3608 + index * 12), pos, (float*)(*(int*)(section + 0x10) + index * 12), lights, arg3);
					float local[3];
					local[0] = pos[0] - *(float*)0x26924a8;
					local[1] = pos[1] - *(float*)0x26924ac;
					local[2] = pos[2] - *(float*)0x26924b0;
					fn_005245A0((void*)0x26924d8);
					((unsigned char*)0x2437a74)[index] = (unsigned char)lr_clip_flags(local, wide);
				}
			}
			int face_count = group[0];
			for (int f = 0; f < face_count; f++) {
				int* face = (int*)(*(int*)(section + 0x20) + (cursor + f) * 0x14);
				int visible = 1;
				if ((face[1] & 2) == 0)
					visible = fn_0050EB30(face + 2, positions + (*(short*)((char*)face + 8)) * 3);
				if (visible) {
					int tri_count = 3;
					fn_0048F8C0(mesh, face, 0, (void*)0x2437a74, (int)(void*)0x2400ec8, (unsigned char*)&tri_count, (float*)0x23d3608, (unsigned char*)arg4, &tri_count);
					fn_0054D050(1, face[1], 0, (int)(void*)0, (int*)0, 1);
				}
			}
			cursor += vert_count;
		}
		UnlockFn unlock = (UnlockFn)(*(int*)(*(int*)device + 0x10));
		unlock(device);
		void* draw_dev = *(void**)0x2dc6c40;
		DrawFn draw = (DrawFn)(*(int*)(*(int*)draw_dev + 0x80));
		draw(draw_dev, 4, device, *(int*)0x240d9e8, mesh, (void*)0x2433424, 0, 0);
		*(int*)0x240d9e8 += cursor;
	} else {
		fn_00534A40(*(int*)0x25d8d1c);
		fn_0048E6C0(mesh, arg3, arg4 != 0, lights);
	}
	(void)positions;
}

}

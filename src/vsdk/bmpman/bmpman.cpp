// Original: D:\projects\Summoner\pccode\vsdk\bmpman\bmpman.cpp
// Bitmap table. Asserts in this range name bm_set_mark, bm_release_from_mark,
// bm_read_header, and bm_lock. Each slot is 0x48 bytes at 0x2591c78.

struct BmSlot {
	char* name;
	int key;
	unsigned short width;
	unsigned short height;
	int pixels_size;
	int kind;
	int user;
	int format;
	unsigned char levels;
	unsigned char frames;
	unsigned char pad_1e[2];
	void* pixels;
	float rate;
	void* palette;
	BmSlot* next;
	BmSlot* prev;
	unsigned char refs;
	unsigned char pad_35[3];
	int stride;
	int path_id;
	unsigned char halved;
	unsigned char pad_41[3];
	int mark_count;
};

struct StringPool {
	int fn_00503DB0();
	void fn_00544E30(int saved);
	char* fn_00544C40(const char* text);
	void fn_00544BD0();
};

struct VfsFile {
	void fn_005140D0();
	int fn_00514630(const char* name, int mode, int path);
	void fn_00525FE0(void* dst, int size, int a, int b);
	int fn_00525990(int a, int b);
	void fn_00514740();
	void fn_005140F0();
	void fn_00514910(int size, int mode);
	char fn_005141C0(const char* name, int path);
};

static BmSlot* bm_slots()
{
	return (BmSlot*)0x2591c78;
}

static BmSlot** bm_hash()
{
	return (BmSlot**)0x257f0b0;
}

static BmSlot* bm_list()
{
	return (BmSlot*)0x25c3000;
}

static BmSlot* bm_free()
{
	return (BmSlot*)0x25c5c18;
}

static int bm_index(BmSlot* slot)
{
	return (int)(slot - bm_slots());
}

extern "C" int fn_00501CE0(int width, int height, int levels);
extern "C" int stricmp(const char* a, const char* b);
extern "C" char* strrchr(const char* text, int ch);
extern "C" int sprintf(char* dst, const char* fmt, ...);
extern "C" int tolower(int ch);
extern "C" void fn_00531CB0(const char* file, int line, const char* message);
extern "C" void* vfs_heap_alloc(int size);
extern "C" void fn_00512570(void* block);
extern "C" void fn_00503C80(int index);
extern "C" void fn_00503CF0(int a, int b, int c, void* pixels, int d, int e);
extern "C" int fn_00503DC0(int width, int height);
extern "C" int fn_00506C70(int index);
extern "C" void fn_005069E0(int handle, int zero);
extern "C" int fn_00554B60(const char* name, int* w, int* h, int zero);
extern "C" int fn_00554D00(const char* name, void* info, int zero);
extern "C" int fn_00555110(const char* name, int* w, int* h, int* fmt, int* info, int zero, int zero2, int path);
extern "C" int fn_00555450(const char* name, void* pixels, void* palette, int flag, int path);
extern "C" int fn_00555CF0(const char* name, int* w, int* h, int* info, int zero, int path);
extern "C" int fn_00555F60(const char* name, void* pixels, void* palette, int path);
extern "C" char* fn_005565A0(int err);
extern "C" double fn_0056E5DA(double value);
extern "C" int fn_0056C7E4();

extern "C" {

int fn_005021D0(int handle);
int fn_00502560(int handle);
void fn_00502240(int handle, int enable);
void fn_005023E0(int index);
void fn_00502040(BmSlot* slot);
int fn_00502650(const char* name);
int fn_00502E30(const char* name);
int fn_005025D0(const char* name);
int fn_00502A00(const char* name, int* out_w, int* out_h, int* out_fmt, int* out_levels, int* out_frames, int* out_bytes, int* out_info, int path);
int fn_00502720(const char* name, int reuse, int path);
int fn_00502F70(char* name, int* out_handle, int path);
int fn_005030A0(const char* name);
int fn_00501D20(const char* name, int* out_a, int* out_b, int* out_c, int* out_d, int* out_e, int* out_f, int path);
BmSlot* fn_00502EB0();
BmSlot* fn_00502EE0(int count);
int fn_00502450(int format);
int fn_00502500(int handle, float* out_rate);
void fn_005020F0(int handle, int drop_all);
bool fn_005033C0(int width, int height);
void fn_00503550(int handle, int* out_w, int* out_h, int* out_size, int* out_levels);
int fn_005035C0(int handle);
int fn_00503AA0(int index);
int fn_00503710(int index);
int fn_005038F0(int index);
int fn_00503B20(int index);

int fn_005021D0(int handle)
{
	return handle;
}

int fn_00502450(int format)
{
	switch (format) {
	case 3:
	case 4:
	case 5:
	case 8:
		return 2;
	case 6:
		return 3;
	case 7:
		return 4;
	default:
		return 1;
	}
}

int fn_005024A0(int format)
{
	return fn_00502450(format) << 3;
}

int fn_00502560(int handle)
{
	int index = fn_005021D0(handle);
	BmSlot* slot = &bm_slots()[index];
	if (slot->kind != 1)
		return index;
	int clock = *(int*)0x257f08c;
	float scaled = (float)clock * slot->rate;
	int frame = fn_0056C7E4();
	(void)scaled;
	int frames = slot->frames;
	return (frame % frames) + index + 1;
}

int fn_00502300(int handle)
{
	int index = fn_00502560(handle);
	return bm_slots()[index].halved;
}

int fn_005024C0(int handle)
{
	int index = fn_00502560(handle);
	if (index == -1)
		return index;
	return bm_slots()[index].stride;
}

int fn_005024E0(int handle)
{
	int index = fn_00502560(handle);
	if (index == -1)
		return 0;
	return bm_slots()[index].kind;
}

int fn_00502500(int handle, float* out_rate)
{
	BmSlot* slot = &bm_slots()[fn_005021D0(handle)];
	if (slot->kind == 1) {
		if (out_rate != 0)
			*out_rate = ((float)slot->frames / slot->rate) * 0.001f;
		return slot->frames;
	}
	if (out_rate != 0)
		*out_rate = 1.0f;
	return 1;
}

int fn_00502650(const char* name)
{
	unsigned int hash = 0;
	if (name == 0)
		return -1;
	while (*name != 0) {
		int ch = tolower((unsigned char)*name);
		hash = (hash << 6) | (hash >> 26);
		hash ^= (unsigned int)ch;
		name++;
	}
	if ((int)hash < 0)
		hash = 0 - hash;
	return (int)hash;
}

void fn_00502040(BmSlot* slot)
{
	int key = slot->key;
	int slot_n = key % 0xaf1;
	while (bm_hash()[slot_n] != 0) {
		key++;
		slot_n = key % 0xaf1;
	}
	bm_hash()[key % 0xaf1] = slot;
}

int fn_005025D0(const char* name)
{
	int key = fn_00502650(name);
	if (key < 0)
		return -1;
	int slot_n = key;
	int probe = key % 0xaf1;
	BmSlot* found = bm_hash()[probe];
	while (found != 0) {
		if (found->key == key && stricmp(found->name, name) == 0)
			return bm_index(found);
		slot_n++;
		found = bm_hash()[slot_n % 0xaf1];
	}
	return -1;
}

void fn_005020F0(int handle, int drop_all)
{
	int index = fn_005021D0(handle);
	BmSlot* slot = &bm_slots()[index];
	if (slot->kind == 1) {
		if (slot->kind == 2) {
		}
	} else if (slot->kind == 2) {
		int frames = slot->frames;
		slot = slot - frames * 7;
	}
	if (slot->kind == 1 || slot->kind == 2) {
		if (!drop_all) {
			slot->refs--;
			if (slot->refs != 0)
				return;
		}
		if (slot->pixels != 0) {
			fn_00512570(slot->pixels);
			slot->pixels = 0;
		}
		if (slot->palette != 0) {
			fn_00512570(slot->palette);
			slot->palette = 0;
		}
		int frames = slot->frames;
		for (int i = 0; i < frames; i++) {
			BmSlot* frame = slot + 1 + i;
			frame->pixels = 0;
			frame->palette = 0;
		}
		return;
	}
	if (slot->format == 3)
		return;
	if (slot->pixels != 0) {
		fn_00512570(slot->pixels);
		slot->pixels = 0;
	}
	if (slot->palette != 0) {
		fn_00512570(slot->palette);
		slot->palette = 0;
	}
}

void fn_005021E0(int enable)
{
	if (*(unsigned char*)0x25c688d == (unsigned char)enable)
		return;
	*(unsigned char*)0x25c688d = (unsigned char)enable;
	BmSlot* node = *(BmSlot**)0x25c302c;
	while (node != bm_list()) {
		fn_00502240(bm_index(node), enable);
		node = node->next;
	}
}

void fn_00502240(int handle, int enable)
{
	BmSlot* slot = &bm_slots()[fn_005021D0(handle)];
	if (enable) {
		if (slot->user != 0 || slot->halved != 0)
			return;
		if (slot->levels <= 1 || slot->frames != 1 || slot->refs != 0)
			return;
		slot->halved = 1;
		slot->width = (unsigned short)(slot->width >> 1);
		slot->height = (unsigned short)(slot->height >> 1);
		slot->levels--;
	} else {
		if (slot->halved == 0 || slot->refs != 0)
			return;
		slot->halved = 0;
		slot->width = (unsigned short)(slot->width << 1);
		slot->height = (unsigned short)(slot->height << 1);
		slot->levels++;
	}
	slot->pixels_size = fn_00501CE0(slot->width, slot->height, slot->levels);
	fn_00506C70(bm_index(slot));
}

void fn_005023E0(int index)
{
	BmSlot* slot = &bm_slots()[index];
	BmSlot* prev = slot->prev;
	BmSlot* next = slot->next;
	prev->next = next;
	next->prev = prev;
	BmSlot* free_tail = *(BmSlot**)0x25c5c48;
	slot->prev = free_tail;
	slot->next = bm_free();
	free_tail->next = slot;
	slot->name = 0;
	slot->key = -1;
	slot->format = 0;
	slot->width = 0;
	slot->height = 0;
	slot->levels = 0;
	slot->pixels_size = 0;
	slot->format = 0;
	slot->pixels = 0;
	slot->palette = 0;
	slot->refs = 0;
	slot->stride = 0;
	*(BmSlot**)0x25c5c48 = slot;
	slot->mark_count = 0;
}

void fn_00502320()
{
	if (*(unsigned char*)0x25c688e == 1)
		return;
	BmSlot* free_sent = bm_free();
	*(BmSlot**)0x25c5c44 = free_sent;
	*(BmSlot**)0x25c5c48 = free_sent;
	BmSlot* sent = bm_list();
	*(BmSlot**)0x25c302c = sent;
	BmSlot* slot = bm_slots();
	BmSlot* last = slot;
	while ((char*)slot < (char*)0x25c2ff8) {
		slot->prev = sent;
		slot->next = sent;
		sent->next = slot;
		last = slot;
		slot++;
	}
	*(BmSlot**)0x25c3030 = last;
	int i;
	for (i = 0; i < 0xaf1; i++)
		bm_hash()[i] = 0;
	for (i = 0; i < 0xaf0; i++)
		fn_005023E0(i);
	unsigned char* pixel = (unsigned char*)0x25c5c8c;
	for (int y = 0; y < 0x20; y++) {
		for (int x = 0; x < 0x20; x++) {
			if ((x & 7) != 0 && (y & 7) != 0) {
				pixel[0] = 0x8e;
				pixel[1] = 0x79;
				pixel[2] = 0x79;
			} else {
				pixel[0] = 0x40;
				pixel[1] = 0x40;
				pixel[2] = 0x40;
			}
			pixel += 3;
		}
	}
	*(unsigned char*)0x25c688e = 1;
}

// bm_set_mark. Assert: "bm_set_mark within bm_set_mark not allowed!"
void fn_00501EB0()
{
	if (*(int*)0x25c6890 != 0) {
		for (;;) {
			fn_00531CB0("D:\\projects\\Summoner\\pccode\\vsdk\\bmpman\\bmpman.cpp", 0x51d,
				(const char*)0x5a5924);
		}
	}
	*(int*)0x25c6890 = 1;
	*(int*)0x25c5c60 = ((StringPool*)0x25c5c68)->fn_00503DB0();
	BmSlot* node = *(BmSlot**)0x25c302c;
	while (node != bm_list()) {
		node->mark_count++;
		node = node->next;
	}
	for (int i = 0; i < 0xaf1; i++)
		((BmSlot**)0x25c3054)[i] = bm_hash()[i];
}

void fn_00502090()
{
	BmSlot* node = *(BmSlot**)0x25c302c;
	while (node != bm_list()) {
		int index = bm_index(node);
		fn_005020F0(index, 1);
		fn_00503C80(index);
		node = *(BmSlot**)0x25c302c;
	}
	*(unsigned char*)0x25c688e = 0;
	((StringPool*)0x25c5c68)->fn_00544BD0();
}

// bm_release_from_mark.
void fn_00501F20()
{
	if (*(int*)0x25c6890 == 0) {
		for (;;) {
			fn_00531CB0("D:\\projects\\Summoner\\pccode\\vsdk\\bmpman\\bmpman.cpp", 0x52f,
				(const char*)0x5a5984);
		}
	}
	char copied[0x100];
	int saved = *(int*)0x25c5c60;
	*(int*)0x25c6890 = 0;
	((StringPool*)0x25c5c68)->fn_00544E30(saved);
	BmSlot* node = *(BmSlot**)0x25c302c;
	while (node != bm_list()) {
		BmSlot* next = node->next;
		if (node->mark_count > 0)
			node->mark_count--;
		else
			fn_00503C80(bm_index(node));
		node = next;
	}
	for (int i = 0; i < 0xaf1; i++)
		bm_hash()[i] = ((BmSlot**)0x25c3054)[i];
	node = *(BmSlot**)0x25c302c;
	while (node != bm_list()) {
		if ((int)node->name >= saved) {
			const char* src = node->name;
			char* dst = copied;
			while ((*dst++ = *src++) != 0) {
			}
			node->name = ((StringPool*)0x25c5c68)->fn_00544C40(copied);
			fn_00502040(node);
		}
		node = node->next;
	}
}

BmSlot* fn_00502EB0()
{
	BmSlot* slot = *(BmSlot**)0x25c5c44;
	BmSlot* prev = slot->prev;
	BmSlot* next = slot->next;
	prev->next = next;
	next->prev = prev;
	BmSlot* head = *(BmSlot**)0x25c302c;
	*(BmSlot**)0x25c302c = slot;
	slot->next = head;
	head->prev = slot;
	slot->prev = bm_list();
	return slot;
}

BmSlot* fn_00502EE0(int count)
{
	int run = 0;
	int start = 0;
	int index = 0;
	BmSlot* cursor = &bm_slots()[2];
	while ((char*)cursor < (char*)0x25c3008) {
		if (cursor->kind == 0) {
			if (run == 0)
				start = index;
			run++;
		} else {
			run = 0;
		}
		if (run == count)
			break;
		cursor = (BmSlot*)((char*)cursor + 0x48);
		index++;
	}
	if (run != count)
		return 0;
	for (int i = 0; i < count; i++) {
		BmSlot* slot = &bm_slots()[start + i];
		BmSlot* prev = slot->prev;
		BmSlot* next = slot->next;
		prev->next = next;
		next->prev = prev;
		BmSlot* tail = *(BmSlot**)0x25c3030;
		slot->prev = tail;
		slot->next = bm_list();
		tail->next = slot;
		*(BmSlot**)0x25c3030 = slot;
	}
	return &bm_slots()[start];
}

int fn_00502E30(const char* name)
{
	const char* ext = strrchr(name, '.');
	if (ext == 0)
		return 0;
	ext++;
	if (stricmp(ext, "pcx") == 0)
		return 1;
	if (stricmp(ext, "tga") == 0)
		return 2;
	if (stricmp(ext, "vbm") == 0)
		return 4;
	if (stricmp(ext, "m2v") == 0)
		return 5;
	return 0;
}

int fn_00502690(int handle)
{
	if (handle == -1)
		return *(unsigned char*)0x25c688c;
	BmSlot* slot = &bm_slots()[fn_00502560(handle)];
	if (slot->kind == 3 && stricmp(slot->name, "USERBMAP") == 0)
		return 1;
	return 0;
}

int fn_00502F70(char* name, int* out_handle, int path)
{
	*out_handle = -1;
	if (fn_00502E30(name) != 2)
		return 0;
	const char* dot = strrchr(name, '.');
	char stem[0x100];
	int stem_len = (int)(dot - name);
	int i;
	for (i = 0; i < stem_len; i++)
		stem[i] = name[i];
	stem[stem_len] = 0;
	*(int*)(stem + stem_len) = *(int*)0x5a5b58;
	stem[stem_len + 4] = *(char*)0x5a5b5c;
	int handle = fn_005025D0(stem);
	VfsFile file;
	file.fn_005140D0();
	if (handle < 0) {
		if (file.fn_005141C0(stem, path) == 0) {
			file.fn_005140F0();
			return 0;
		}
	}
	for (i = 0; stem[i] != 0; i++)
		name[i] = stem[i];
	name[stem_len] = 0;
	*out_handle = handle;
	file.fn_005140F0();
	return handle;
}

int fn_005030A0(const char* name)
{
	if (*(unsigned char*)0x25c688c == 0)
		*(unsigned char*)0x25c688c = 1;
	int handle = 0;
	fn_00503CF0(6, 0x20, 0x20, (void*)0x25c5c8c, 0, -1);
	(void)handle;
	int index = fn_00502560(0);
	BmSlot* slot = &bm_slots()[index];
	slot->name = ((StringPool*)0x25c5c68)->fn_00544C40(name);
	slot->key = fn_00502650(name);
	fn_00502040(slot);
	return 0;
}

int fn_00501D20(const char* name, int* out_a, int* out_b, int* out_c, int* out_d, int* out_e, int* out_f, int path)
{
	VfsFile file;
	file.fn_005140D0();
	if (file.fn_00514630(name, 1, path) < 0) {
		char message[0x100];
		for (;;) {
			sprintf(message, "Unable to open %s.  Maybe file doesn't exist.", name);
			fn_00531CB0("D:\\projects\\Summoner\\pccode\\vsdk\\bmpman\\bmpman.cpp", 0x3f9, message);
		}
	}
	int header = 0;
	file.fn_00525FE0(&header, 4, 0, 0);
	*out_a = file.fn_00525990(0, 0);
	*out_b = file.fn_00525990(0, 0);
	int kind = file.fn_00525990(0, 0);
	if (kind == 0)
		*out_c = 5;
	else if (kind == 1)
		*out_c = 4;
	else
		*out_c = 3;
	*out_d = file.fn_00525990(0, 0);
	*out_e = file.fn_00525990(0, 0);
	int levels = file.fn_00525990(0, 0) + 1;
	*out_f = levels;
	if (*(unsigned char*)0x5a5760 == 0)
		*out_f = 1;
	file.fn_00514740();
	file.fn_005140F0();
	return kind;
}

// bm_read_header.
int fn_00502A00(const char* name, int* out_w, int* out_h, int* out_fmt, int* out_levels, int* out_frames, int* out_bytes, int* out_info, int path)
{
	int kind = fn_00502E30(name);
	*out_frames = 1;
	*out_bytes = 0;
	*out_levels = 1;
	*out_info = -1;
	if (kind < 1 || kind > 5) {
		char message[0x100];
		for (;;) {
			sprintf(message, "Cannot determine bitmap type in bm_read_header for file %s", name);
			fn_00531CB0("D:\\projects\\Summoner\\pccode\\vsdk\\bmpman\\bmpman.cpp", 0x340, message);
		}
	}
	if (kind == 1) {
		int err = fn_00555CF0(name, out_w, out_h, out_info, 0, path);
		if (err != 0) {
			char message[0x100];
			for (;;) {
				sprintf(message, "Could not open file: %s", name);
				fn_00531CB0("D:\\projects\\Summoner\\pccode\\vsdk\\bmpman\\bmpman.cpp", 0x2a8, message);
			}
		}
		*out_fmt = 1;
		return kind;
	}
	if (kind == 2) {
		int info = 0;
		int err = fn_00555110(name, out_w, out_h, &info, out_info, 0, 0, path);
		if (err != 0)
			return 0;
		int bpp = info;
		if (bpp < 8 || bpp > 0x20)
			*out_fmt = 0;
		else if (bpp == 8)
			*out_fmt = 1;
		else if (bpp == 16)
			*out_fmt = 5;
		else if (bpp == 24)
			*out_fmt = 6;
		else if (bpp == 32)
			*out_fmt = 7;
		else
			*out_fmt = 0;
		if (*(unsigned char*)0x5a5760 != 0) {
			char stem[0x100];
			const char* src = name;
			char* dst = stem;
			while ((*dst++ = *src++) != 0) {
			}
			char* dot = strrchr(stem, '.');
			if (dot != 0)
				*dot = 0;
			int mip_w = *out_w / 2;
			int mip_h = *out_h / 2;
			int mips = 1;
			while (fn_00503DC0(mip_w, mip_h) >= 1) {
				char mip_name[0x100];
				sprintf(mip_name, "%s-mip%d.tga", stem, mips);
				if ((int)(strrchr(mip_name, 0) - mip_name) > 0x28) {
					sprintf(mip_name, "Mipmap filename exceeds maximum length: '%s'\n", stem);
					return kind;
				}
				int mip_fmt = 0;
				int mw = 0;
				int mh = 0;
				if (fn_00555110(mip_name, &mw, &mh, &mip_fmt, 0, 0, 1, path) != 0)
					break;
				if (mw != mip_w || mh != mip_h || mip_fmt != info)
					break;
				mips++;
				mip_w /= 2;
				mip_h /= 2;
			}
			*out_frames = mips;
		}
		return kind;
	}
	if (kind == 4)
		return fn_00501D20(name, out_w, out_h, out_fmt, out_levels, out_frames, out_bytes, path);
	int bytes = fn_00554B60(name, out_w, out_h, 0);
	*out_info = bytes;
	if (bytes <= 0)
		return 0;
	*out_levels = 1;
	*out_frames = 1;
	*out_bytes = 0;
	*out_fmt = 5;
	return kind;
}

int fn_00502720(const char* name, int reuse, int path)
{
	int existing = -1;
	if (reuse == 0) {
		existing = fn_005025D0(name);
		if (existing >= 0)
			return existing;
	}
	if (path == -1)
		path = 0x98967f;
	char stored[0x100];
	const char* src = name;
	char* dst = stored;
	while ((*dst++ = *src++) != 0) {
	}
	int width = 0;
	int height = 0;
	int format = 0;
	int levels = 1;
	int frames = 1;
	int info = 0;
	int bytes = 0;
	int kind = 0;
	if (reuse == 0) {
		kind = fn_00502F70(stored, &existing, path);
		if (existing >= 0)
			return existing;
	}
	kind = fn_00502A00(stored, &width, &height, &format, &levels, &frames, &bytes, &info, path);
	if (kind == 0)
		return fn_005030A0(name);
	int half = 0;
	if (*(unsigned char*)0x25c688d != 0 && levels > 1 && frames == 1) {
		half = 1;
		levels--;
		width >>= 1;
		height >>= 1;
	}
	int pitch = fn_00501CE0(width, height, levels);
	BmSlot* slot;
	if (frames == 1) {
		slot = fn_00502EB0();
		slot->user = 0;
	} else {
		slot = fn_00502EE0(frames + 1);
		slot->user = 1;
	}
	int index = bm_index(slot);
	slot->format = kind;
	slot->kind = info;
	slot->name = ((StringPool*)0x25c5c68)->fn_00544C40(stored);
	if (reuse == 0) {
		slot->key = fn_00502650(stored);
		fn_00502040(slot);
	}
	slot->frames = (unsigned char)frames;
	slot->rate = (float)frames * 0.001f;
	slot->width = (unsigned short)width;
	slot->height = (unsigned short)height;
	slot->pixels = 0;
	slot->palette = 0;
	slot->levels = (unsigned char)levels;
	slot->pixels_size = pitch;
	slot->path_id = path;
	slot->halved = (unsigned char)half;
	if (kind == 5)
		slot->stride = bytes;
	else
		slot->stride = fn_00502450(info) * pitch;
	if (frames > 1) {
		for (int i = 0; i < frames; i++) {
			BmSlot* frame = slot + 1 + i;
			frame->name = 0;
			frame->key = -1;
			frame->format = kind;
			frame->user = 2;
			frame->kind = info;
			frame->width = (unsigned short)width;
			frame->height = (unsigned short)height;
			frame->levels = (unsigned char)levels;
			frame->frames = (unsigned char)(i + 1);
			frame->pixels = 0;
			frame->palette = 0;
			frame->pixels_size = pitch;
			frame->path_id = path;
			frame->halved = 0;
			if (kind == 5)
				frame->stride = bytes;
			else
				frame->stride = fn_00502450(info) * pitch;
		}
	}
	return index;
}

int fn_005026E0(const char* name, int path, int mode)
{
	int handle = fn_00502720(name, path, 0);
	if (*(int*)0x25c6894 != 0 && handle >= 0)
		fn_005069E0(handle, 0);
	return handle;
}

int fn_00503190(int handle)
{
	int index = fn_00502560(handle);
	BmSlot* slot = &bm_slots()[index];
	return fn_00502720(slot->name, 1, slot->path_id);
}

int fn_00503110(const char* name, int* out_frame, int* out_count, int path)
{
	int handle = fn_00502720(name, 0, path);
	int index = fn_005021D0(handle);
	BmSlot* slot = &bm_slots()[index];
	if (slot->kind == 1) {
		*out_frame = slot->frames;
		double scaled = (double)(slot->rate * 1000.0f);
		*out_count = (int)fn_0056E5DA(scaled);
		return handle;
	}
	*out_frame = 1;
	*out_count = 0;
	return handle;
}

int fn_00503480(const char* name, int* out_frame, int* out_count)
{
	int handle = fn_00502720(name, -1, 0);
	int index = fn_005021D0(handle);
	BmSlot* slot = &bm_slots()[index];
	if (slot->kind != 1) {
		*out_frame = 1;
		*out_count = 0;
		return handle;
	}
	*out_frame = slot->frames;
	double scaled = (double)(slot->rate * 1000.0f);
	*out_count = (int)fn_0056E5DA(scaled);
	return handle + 1;
}

bool fn_005033C0(int width, int height)
{
	int tw = width;
	if (width <= 4)
		tw = 4;
	else if (width <= 8)
		tw = 8;
	else if (width <= 0x10)
		tw = 0x10;
	else if (width <= 0x20)
		tw = 0x20;
	else if (width <= 0x40)
		tw = 0x40;
	else if (width <= 0x80)
		tw = 0x80;
	else
		tw = 0x100;
	int th = height;
	if (height <= 4)
		th = 4;
	else if (height <= 8)
		th = 8;
	else if (height <= 0x10)
		th = 0x10;
	else if (height <= 0x20)
		th = 0x20;
	else if (height <= 0x40)
		th = 0x40;
	else if (height <= 0x80)
		th = 0x80;
	else
		th = 0x100;
	return tw == width && th == height;
}

int fn_005031C0(int handle, const char* name, int path)
{
	BmSlot* slot = &bm_slots()[fn_005021D0(fn_00502560(handle))];
	if (slot->levels != 0)
		return -1;
	if (slot->frames > 1)
		return -1;
	if (path < 0)
		path = 0x98967f;
	char stored[0x100];
	const char* src = name;
	char* dst = stored;
	while ((*dst++ = *src++) != 0) {
	}
	int existing = 0;
	fn_00502F70(stored, &existing, path);
	int width = 0;
	int height = 0;
	int format = 0;
	int levels = 1;
	int frames = 1;
	int info = 0;
	int bytes = 0;
	int kind = fn_00502A00(stored, &width, &height, &format, &levels, &frames, &bytes, &info, path);
	if (kind == 0 || kind == 5)
		return -1;
	if (fn_005033C0(width, height) == 0)
		return -1;
	int half = 0;
	if (slot->halved != 0 && levels > 1 && frames == 1) {
		half = 1;
		levels--;
		width >>= 1;
		height >>= 1;
	}
	if (levels < slot->levels)
		levels = slot->levels;
	int pitch = fn_00501CE0(width, height, levels);
	if (frames != slot->frames)
		return -1;
	if (width != slot->width || height != slot->height)
		return -1;
	if (levels != slot->levels || pitch != slot->pixels_size)
		return -1;
	slot->format = kind;
	slot->name = ((StringPool*)0x25c5c68)->fn_00544C40(stored);
	slot->key = fn_00502650(stored);
	fn_00502040(slot);
	slot->rate = (float)frames * 0.001f;
	slot->path_id = path;
	slot->kind = info;
	slot->halved = (unsigned char)half;
	fn_00506C70(path);
	return 0;
}

void fn_00503500(int handle, int* out_w, int* out_h)
{
	int index = fn_00502560(handle);
	if (index == -1) {
		*out_w = 0;
		*out_h = 0;
		return;
	}
	BmSlot* slot = &bm_slots()[index];
	*out_w = slot->width;
	*out_h = slot->height;
}

void fn_00503550(int handle, int* out_w, int* out_h, int* out_size, int* out_levels)
{
	int index = fn_00502560(handle);
	BmSlot* slot = &bm_slots()[index];
	if (slot->kind == 0) {
		*out_w = 0;
		*out_h = 0;
		return;
	}
	*out_w = slot->width;
	*out_h = slot->height;
	*out_size = slot->pixels_size;
	*out_levels = slot->levels;
}

int fn_005035C0(int handle)
{
	int index = fn_00502560(handle);
	return bm_slots()[index].format;
}

int fn_005035E0(int handle)
{
	if (handle < 0)
		return 0;
	int format = fn_005035C0(handle);
	if (format == 4 || format == 7 || format == 5)
		return 1;
	return 0;
}

void fn_00503610(int handle, int* out_pixels, int* out_palette)
{
	int index = fn_00502560(handle);
	BmSlot* slot = &bm_slots()[index];
	*out_pixels = (int)slot->pixels;
	*out_palette = (int)slot->palette;
	(void)slot->format;
}

int fn_00503AA0(int index)
{
	BmSlot* slot = &bm_slots()[index];
	int bpp = fn_00502450(slot->format);
	slot->pixels = vfs_heap_alloc(slot->pixels_size * bpp);
	if (slot->format == 1)
		slot->palette = vfs_heap_alloc(0x300);
	int ok = fn_00555F60(slot->name, slot->pixels, slot->palette, slot->path_id);
	return ok == 0;
}

int fn_00503710(int index)
{
	BmSlot* slot = &bm_slots()[index];
	int bpp = fn_00502450(slot->format);
	slot->pixels = vfs_heap_alloc(slot->pixels_size * bpp);
	if (slot->format == 1)
		slot->palette = vfs_heap_alloc(0x300);
	if (slot->levels <= 1 && slot->halved == 0) {
		int ok = fn_00555450(slot->name, slot->pixels, slot->palette, 0, slot->path_id);
		return ok == 0;
	}
	char stem[0x100];
	const char* src = slot->name;
	char* dst = stem;
	while ((*dst++ = *src++) != 0) {
	}
	char* dot = strrchr(stem, '.');
	if (dot != 0)
		*dot = 0;
	int levels = slot->levels;
	for (int i = 0; i < levels; i++) {
		char mip_name[0x100];
		if (slot->halved != 0)
			sprintf(mip_name, "%s-mip%d.tga", stem, i + 1);
		else if (i == 0) {
			src = slot->name;
			dst = mip_name;
			while ((*dst++ = *src++) != 0) {
			}
		} else {
			sprintf(mip_name, "%s-mip%d.tga", stem, i);
		}
		fn_00555450(mip_name, slot->pixels, slot->palette, 1, slot->path_id);
	}
	return 1;
}

int fn_005038F0(int index)
{
	BmSlot* slot = &bm_slots()[index];
	VfsFile file;
	file.fn_005140D0();
	int bpp = fn_00502450(slot->format);
	if (slot->user == 0) {
		file.fn_00514630(slot->name, 1, slot->path_id);
		file.fn_00514910(0x20, 0);
		slot->pixels = vfs_heap_alloc(slot->pixels_size * bpp);
		if (slot->halved != 0) {
			int bytes = (int)slot->width * (int)slot->height * bpp * 4;
			file.fn_00514910(bytes, 1);
		}
		file.fn_00525FE0(slot->pixels, slot->pixels_size * bpp, 0, 0);
		file.fn_00514740();
	} else {
		int frames = slot->frames;
		BmSlot* base = slot - frames * 7;
		if (base->pixels == 0) {
			file.fn_00514630(base->name, 1, slot->path_id);
			file.fn_00514910(0x20, 0);
			int bytes = base->pixels_size * frames * bpp;
			base->pixels = vfs_heap_alloc(bytes);
			file.fn_00525FE0(base->pixels, bytes, 0, 0);
			file.fn_00514740();
			for (int i = 0; i < base->frames; i++) {
				BmSlot* frame = base + 1 + i;
				int offset = (frame->frames - 1) * frame->pixels_size * bpp;
				frame->pixels = (char*)base->pixels + offset;
			}
			base->refs = base->frames;
		}
	}
	file.fn_005140F0();
	return 1;
}

int fn_00503B20(int index)
{
	BmSlot* slot = &bm_slots()[index];
	int ok = fn_00554D00(slot->name, &slot->rate, 0);
	return ok != 0;
}

// bm_lock. Assert: "Unsupported type in bm_lock".
int fn_00503650(int handle, int* out_pixels, int* out_palette)
{
	int index = fn_00502560(handle);
	BmSlot* slot = &bm_slots()[index];
	int kind = slot->kind;
	int loaded = 0;
	if (kind == 3) {
		for (;;) {
			fn_00531CB0("D:\\projects\\Summoner\\pccode\\vsdk\\bmpman\\bmpman.cpp", 0x8e8,
				"Unsupported type in bm_lock");
		}
	}
	if (kind == 1)
		loaded = fn_00503AA0(index);
	else if (kind == 2)
		loaded = fn_00503710(index);
	else if (kind == 4)
		loaded = fn_005038F0(index);
	else if (kind == 5)
		loaded = fn_00503B20(index);
	else {
		for (;;) {
			fn_00531CB0("D:\\projects\\Summoner\\pccode\\vsdk\\bmpman\\bmpman.cpp", 0x8e8,
				"Unsupported type in bm_lock");
		}
	}
	if (loaded == 0)
		return 0;
	*out_pixels = (int)slot->pixels;
	*out_palette = (int)slot->palette;
	return slot->format;
}

void fn_00503B50(int handle)
{
	fn_005020F0(handle, 0);
}

int fn_00503B60(int handle)
{
	BmSlot* slot = &bm_slots()[fn_005021D0(handle)];
	if (slot->kind == 2) {
		int frames = slot->frames;
		slot = slot - frames * 7;
	}
	return (int)slot->name;
}

int fn_00503BA0(int handle)
{
	int index = fn_00502560(handle);
	if (index < 0)
		return -1;
	float rate = 0;
	int frames = fn_00502500(handle, &rate);
	int width = 0;
	int height = 0;
	int size = 0;
	int levels = 0;
	fn_00503550(handle, &width, &height, &size, &levels);
	return size * frames * width;
}

void fn_00503C00(int handle, const char* mode)
{
	int index = fn_00502560(handle);
	BmSlot* slot = &bm_slots()[index];
	int info = 0;
	char ignored[0x100];
	int err = fn_00555CF0(slot->name, &info, &info, &info, (int)mode, 0x98967f);
	if (err != 0) {
		char message[0x100];
		for (;;) {
			sprintf(message, "Couldn't open %s.\n", slot->name);
			fn_00531CB0("D:\\projects\\Summoner\\pccode\\vsdk\\bmpman\\bmpman.cpp", 0x936, message);
		}
	}
}

}

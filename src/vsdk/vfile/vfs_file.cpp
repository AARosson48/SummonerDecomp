// File object recovered from Sum.exe VA 0x513EC0-0x514D60 and the
// buffered read at VA 0x525FE0. Not byte-matched.
// Not byte-matched.

#if defined(_WIN64) || defined(__x86_64__) || defined(__amd64__)
#error Sum.exe is a 32-bit image; VfsFile is 0x11C bytes
#endif

#include "vfs.h"

#include <cstdio>
#include <cstring>

extern int vfs_stat(const char* path, void* scratch);
extern int vfs_loose_length(void* file_field);
extern void vfs_prepare_directory(int path_id);
extern void vfs_note_open(int flag);
extern char g_default_ext[];
extern char g_hd_root[];
extern "C" unsigned int __stdcall GetFileAttributesA(const char* path);
extern "C" int __stdcall SetFileAttributesA(const char* path, unsigned int attributes);
extern "C" int __stdcall DeleteFileA(const char* path);
extern "C" int __stdcall MoveFileA(const char* from, const char* to);

unsigned char g_vfs_cache_area[VFS_OPEN_SLOTS * 0x8040];

static int g_cached_path = -1;
static char g_cached_ext[32] = "none";

static int text_icmp(const char* a, const char* b) {
    while (*a || *b) {
        unsigned char ca = (unsigned char)*a++;
        unsigned char cb = (unsigned char)*b++;
        if (ca >= 'A' && ca <= 'Z') {
            ca = (unsigned char)(ca + 0x20);
        }
        if (cb >= 'A' && cb <= 'Z') {
            cb = (unsigned char)(cb + 0x20);
        }
        if (ca != cb) {
            return (int)ca - (int)cb;
        }
        if (ca == 0) {
            return 0;
        }
    }
    return 0;
}

static int ext_listed(const char* list, const char* ext) {
    if (!list || !ext || !ext[0]) {
        return 0;
    }
    char needle[64];
    int n = 0;
    for (; ext[n] && n < 62; n++) {
        unsigned char ch = (unsigned char)ext[n];
        if (ch >= 'A' && ch <= 'Z') {
            ch = (unsigned char)(ch + 0x20);
        }
        needle[n] = (char)ch;
    }
    needle[n] = 0;
    return strstr(list, needle) != 0;
}

static unsigned char* cache_slot(int index) {
    return g_vfs_cache_area + index * 0x8040;
}

static int alloc_slot(void) {
    for (int i = 0; i < VFS_OPEN_SLOTS; i++) {
        if (*(int*)cache_slot(i) == 0) {
            return i;
        }
    }
    return -1;
}

static int is_absolute(const char* name) {
    if (name[1] == ':') {
        return 1;
    }
    if (name[0] == '/' || name[0] == '\\') {
        return 1;
    }
    return 0;
}

static const char* extension_of(const char* name) {
    const char* dot = strrchr(name, '.');
    if (!dot) {
        return g_default_ext;
    }
    return dot;
}

void vfs_file_ctor(VfsFile* file) {
    file->path[0] = 0;
    file->entry = 0;
    file->slot = -1;
    file->found = 0;
}

int vfs_file_close(VfsFile* file) {
    if (file->slot < 0) {
        return 0;
    }
    unsigned char* slot = cache_slot(file->slot);
    int failed = 0;
    if (file->entry) {
        packfile_close_file(file->entry);
    } else {
        FILE* loose = *(FILE**)(slot + 0x802C);
        if (loose) {
            failed = fclose(loose);
        }
    }
    *(FILE**)(slot + 0x802C) = 0;
    file->entry = 0;
    if (failed && *(int*)(slot + 0x0C) == 0) {
        *(int*)(slot + 0x0C) = (int)VFS_OPEN_FAILED;
    }
    *(int*)slot = 0;
    file->slot = -1;
    return *(int*)(slot + 0x0C);
}

void vfs_file_dtor(VfsFile* file) {
    if (file->slot >= 0) {
        vfs_file_close(file);
    }
}

extern "C" int vfs_file_probe(VfsFile* file, const char* path, PackfileEntry** out_entry, int search_packs) {
    char scratch[0x24];
    *out_entry = 0;
    if (vfs_stat(path, scratch) == 0) {
        file->found = 1;
        return 1;
    }
    if (search_packs) {
        PackfileEntry* entry = packfile_find_file(path);
        *out_entry = entry;
        if (entry) {
            file->found = 1;
            return 1;
        }
    }
    return 0;
}

extern "C" int vfs_file_try_hd(VfsFile* file, const char* name, int path_id, PackfileEntry** out_entry, int search_packs) {
    vfs_make_hd_path(path_id, name, file->path);
    file->path_id = path_id;
    return vfs_file_probe(file, file->path, out_entry, search_packs);
}

int vfs_file_open(VfsFile* file, const char* name, int path_id) {
    int i;
    if (!name || !name[0]) {
        return 0;
    }
    file->found = 0;
    file->opened = 0;
    if (is_absolute(name)) {
strcpy(file->path, name);
        file->path_id = -1;
        return vfs_file_probe(file, file->path, &file->entry, 0);
    }
    if (path_id == VFS_ANY_PATH) {
        char ext[64];
        const char* dot = strrchr(name, '.');
        ext[0] = 0;
        if (dot) {
strncpy(ext, dot, 62);
            ext[62] = 0;
            int len = (int)strlen(ext);
            ext[len] = ' ';
            ext[len + 1] = 0;
        }
        for (i = 0; i < VFS_SEARCH_SLOTS; i++) {
            if (!vfs_path_id_ok(i)) {
                continue;
            }
            if (!ext_listed(vfs_search_extensions(i), ext)) {
                continue;
            }
            if (vfs_file_try_hd(file, name, i, &file->entry, 0)) {
                return 1;
            }
        }
        if (vfs_file_try_hd(file, name, 0, &file->entry, 1)) {
            return 1;
        }
    } else {
        file->path_id = path_id;
        int search_packs = path_id == vfs_path_root();
        if (vfs_file_try_hd(file, name, path_id, &file->entry, search_packs)) {
            return 1;
        }
        if (vfs_file_try_hd(file, name, 0, &file->entry, 1)) {
            return 1;
        }
    }

    char ext[64];
    const char* dot = strrchr(name, '.');
    ext[0] = 0;
    if (dot) {
strncpy(ext, dot, 62);
        ext[62] = 0;
        int len = (int)strlen(ext);
        ext[len] = ' ';
        ext[len + 1] = 0;
    }
    int found = -1;
    for (i = 0; i < VFS_SEARCH_SLOTS; i++) {
        if (!vfs_path_id_ok(i)) {
            continue;
        }
        if (ext_listed(vfs_search_extensions(i), ext)) {
            found = i;
            break;
        }
    }
    file->path_id = found >= 0 && found < VFS_SEARCH_SLOTS ? found : 0;
    vfs_make_hd_path(file->path_id, name, file->path);
    file->opened = 0;
    return 0;
}

int vfs_file_acquire(VfsFile* file, unsigned int mode) {
    if ((mode & 2) == 0 && !file->found) {
        return (int)VFS_OPEN_NOTFOUND;
    }
    int slot = alloc_slot();
    if (slot < 0) {
        return (int)VFS_OPEN_NOSLOT;
    }
    unsigned char* cache = cache_slot(slot);
    if (mode & 2) {
        if (file->path_id > 0) {
            vfs_prepare_directory(file->path_id);
        }
        const char* how = (mode & 0x80000000u) ? "wt" : "wb";
        FILE* loose = fopen(file->path, how);
        *(FILE**)(cache + 0x802C) = loose;
        if (!loose) {
            return (int)VFS_OPEN_FAILED;
        }
        *(unsigned int*)cache = mode;
        *(int*)(cache + 4) = 0;
        *(int*)(cache + 8) = 0;
        *(int*)(cache + 0x0C) = 0;
        *(int*)(cache + 0x10) = 0;
        *(int*)(cache + 0x14) = 0;
        *(int*)(cache + 0x18) = 0;
        *(int*)(cache + 0x1C) = 0;
        *(VfsFile**)(cache + 0x8030) = file;
        file->found = 1;
        file->slot = slot;
        return 0;
    }
    if (!file->found) {
        return (int)VFS_OPEN_NOTFOUND;
    }
    *(FILE**)(cache + 0x802C) = 0;
    if (file->entry) {
        if (!packfile_open(file->entry)) {
            return (int)VFS_OPEN_FAILED;
        }
        *(unsigned int*)(cache + 0x1C) = file->entry->size;
    } else {
        FILE* loose = fopen(file->path, "rb");
        *(FILE**)(cache + 0x802C) = loose;
        if (!loose) {
            return (int)VFS_OPEN_FAILED;
        }
        *(int*)(cache + 0x1C) = vfs_loose_length(*(void**)((unsigned char*)loose + 0x10));
    }
    FILE* loose = *(FILE**)(cache + 0x802C);
    if (loose) {
setvbuf(loose, 0, _IONBF, 0);
    }
    *(unsigned int*)cache = mode;
    *(int*)(cache + 4) = 0;
    *(int*)(cache + 8) = 0;
    *(int*)(cache + 0x0C) = 0;
    *(int*)(cache + 0x10) = 0;
    *(int*)(cache + 0x14) = 0;
    *(int*)(cache + 0x18) = 0;
    *(int*)(cache + 0x24) = 0;
    *(int*)(cache + 0x28) = 0;
    *(int*)(cache + 0x20) = -1;
    *(VfsFile**)(cache + 0x8030) = file;
    file->slot = slot;
    vfs_note_open(1);
    return 0;
}

int vfs_file_open_mode(VfsFile* file, const char* name, int path_id, unsigned int mode) {
    const char* ext = extension_of(name);
    if (path_id != VFS_ANY_PATH && vfs_file_open(file, name, path_id)) {
strncpy(g_cached_ext, ext, 31);
        g_cached_ext[31] = 0;
        return vfs_file_acquire(file, mode);
    }
    if (g_cached_path != -1 && vfs_path_id_ok(g_cached_path) && text_icmp(g_cached_ext, ext) == 0 && g_cached_path != vfs_path_root()) {
        if (vfs_file_open(file, name, g_cached_path)) {
            return vfs_file_acquire(file, mode);
        }
    }
    vfs_file_open(file, name, VFS_ANY_PATH);
    int code = vfs_file_acquire(file, mode);
    if (code != 0) {
        return code;
    }
    g_cached_path = file->path_id;
strncpy(g_cached_ext, ext, 31);
    g_cached_ext[31] = 0;
    return 0;
}

int vfs_file_try_cd(VfsFile* file, const char* name, int path_id, int search_packs) {
    PackfileEntry* discarded = 0;
    vfs_make_cd_path(path_id, name, file->path);
    file->path_id = path_id;
    return vfs_file_probe(file, file->path, &discarded, search_packs);
}

int vfs_file_archive_path(VfsFile* file, char* dst, int* out_offset, int max_len) {
    if (!file->entry) {
        return 0;
    }
    char built[0x200];
strcpy(built, g_hd_root);
strcat(built, file->entry->pack->name);
    if ((unsigned)strlen(built) > (unsigned)(max_len - 1)) {
        return 0;
    }
strcpy(dst, built);
    *out_offset = file->entry->sector << 11;
    return 1;
}

unsigned int vfs_file_mode_mask(VfsFile* file, unsigned int mask) {
    return *(unsigned int*)cache_slot(file->slot) & mask;
}

int vfs_file_user_ge(VfsFile* file, int value) {
    return *(int*)(cache_slot(file->slot) + 8) >= value;
}

int vfs_file_user(VfsFile* file) {
    return *(int*)(cache_slot(file->slot) + 8);
}

void vfs_file_set_user(VfsFile* file, int value) {
    *(int*)(cache_slot(file->slot) + 8) = value;
}

char* vfs_file_path(VfsFile* file) {
    return file->path;
}

char* vfs_file_leaf(VfsFile* file) {
    char* slash = strrchr(file->path, '\\');
    if (!slash) {
        return file->path;
    }
    return slash + 1;
}

int vfs_file_read_ready(VfsFile* file) {
    if (file->slot >= 0) {
        VfsFile* owner = *(VfsFile**)(cache_slot(file->slot) + 0x8030);
        PackfileEntry* entry = owner->entry;
        if (entry && (entry->pack->is_cd & 1)) {
            return packfile_read_ok();
        }
    }
    return 1;
}

int vfs_file_flush(VfsFile* file) {
    unsigned char* slot = cache_slot(file->slot);
    FILE* loose = *(FILE**)(slot + 0x802C);
    if (loose && fflush(loose) != 0 && *(int*)(slot + 0x0C) == 0) {
        *(int*)(slot + 0x0C) = (int)VFS_OPEN_FAILED;
    }
    return *(int*)(slot + 0x0C);
}

int vfs_file_length(VfsFile* file, const char* name, int path_id) {
    if (name) {
        if (!vfs_file_open(file, name, path_id)) {
            return (int)VFS_OPEN_NOTFOUND;
        }
    }
    if (file->entry) {
        return (int)file->entry->size;
    }
    if (file->slot < 0) {
        FILE* loose = fopen(file->path, "rb");
        if (!loose) {
            return (int)VFS_OPEN_NOTFOUND;
        }
        int length = vfs_loose_length(*(void**)((unsigned char*)loose + 0x10));
fclose(loose);
        return length;
    }
    return *(int*)(cache_slot(file->slot) + 0x1C);
}

int vfs_file_from_cd(VfsFile* file) {
    if (!file->entry) {
        return 0;
    }
    return (file->entry->pack->is_cd & 1) ? 1 : 0;
}

int vfs_file_seek(VfsFile* file, int offset, int origin) {
    unsigned char* slot = cache_slot(file->slot);
    int pos = offset;
    if (origin == 1) {
        pos = offset + *(int*)(slot + 0x10);
    } else if (origin == 2) {
        pos = offset + vfs_file_length(file, 0, VFS_ANY_PATH);
    }
    int base = *(int*)(slot + 0x20);
    if (vfs_file_mode_mask(file, 1) && base != -1) {
        int end = base + *(int*)(slot + 0x28);
        if (pos >= base && pos < end) {
            *(int*)(slot + 0x10) = pos;
            *(int*)(slot + 0x24) = pos - base;
            return 0;
        }
    }
    *(int*)(slot + 0x10) = pos;
    *(int*)(slot + 0x24) = 0;
    *(int*)(slot + 0x28) = 0;
    *(int*)(slot + 0x20) = -1;
    int failed;
    if (file->entry) {
        failed = !packfile_seek(file->entry, pos, 0);
    } else {
        failed = fseek(*(FILE**)(slot + 0x802C), pos, 0) != 0;
    }
    if (failed) {
        return (int)VFS_OPEN_FAILED;
    }
    return 0;
}

int vfs_file_position(VfsFile* file) {
    return *(int*)(cache_slot(file->slot) + 0x10);
}

int vfs_file_at_end(VfsFile* file) {
    unsigned char* slot = cache_slot(file->slot);
    return *(int*)(slot + 0x10) >= *(int*)(slot + 0x1C);
}

int vfs_file_error(VfsFile* file) {
    if (file->slot < 0) {
        return 0;
    }
    return *(int*)(cache_slot(file->slot) + 0x0C);
}

int vfs_file_get_path_id(VfsFile* file) {
    return file->path_id;
}

int vfs_file_is_open(VfsFile* file) {
    return file->slot >= 0;
}

int vfs_file_remove(VfsFile* file, const char* name, int path_id) {
    if (name && !vfs_file_open(file, name, path_id)) {
        return (int)VFS_OPEN_NOTFOUND;
    }
    if (file->entry) {
        return (int)VFS_OPEN_PACKED;
    }
    file->found = 0;
    if (DeleteFileA(file->path)) {
        return 0;
    }
    return (int)VFS_OPEN_FAILED;
}

int vfs_file_rename(VfsFile* file, const char* new_name, const char* name, int path_id) {
    if (name && !vfs_file_open(file, name, path_id)) {
        return (int)VFS_OPEN_NOTFOUND;
    }
    if (file->entry) {
        return (int)VFS_OPEN_PACKED;
    }
    char dir[0x200];
strcpy(dir, file->path);
    *strrchr(dir, '\\') = 0;
    char built[0x100];
sprintf(built, "%s\\%s", dir, new_name);
    if (!MoveFileA(file->path, built)) {
        return (int)VFS_OPEN_FAILED;
    }
strcpy(file->path, built);
    return 0;
}

static int attributes_of(VfsFile* file, const char* name, int path_id, unsigned int* out_attr) {
    if (name && !vfs_file_open(file, name, path_id)) {
        return (int)VFS_OPEN_NOTFOUND;
    }
    if (file->entry) {
        return (int)VFS_OPEN_PACKED;
    }
    unsigned int attr = GetFileAttributesA(file->path);
    if (attr == 0xFFFFFFFFu) {
        return (int)VFS_OPEN_FAILED;
    }
    *out_attr = attr;
    return 0;
}

int vfs_file_attributes(VfsFile* file, const char* name, int path_id) {
    if (name && !vfs_file_open(file, name, path_id)) {
        return (int)VFS_OPEN_NOTFOUND;
    }
    if (file->entry) {
        return 3;
    }
    unsigned int attr = GetFileAttributesA(file->path);
    if (attr == 0xFFFFFFFFu) {
        return (int)VFS_OPEN_FAILED;
    }
    int result = 0;
    if (attr & 1) {
        result |= VFS_ATTR_READONLY;
    }
    if (attr & 0x20) {
        result |= VFS_ATTR_ARCHIVE;
    }
    return result;
}

static int change_attributes(VfsFile* file, unsigned int flags, const char* name, int path_id, int enable) {
    unsigned int attr = 0;
    int code = attributes_of(file, name, path_id, &attr);
    if (code != 0) {
        return code;
    }
    if (flags & VFS_ATTR_READONLY) {
        if (enable) {
            attr |= 1;
        } else {
            attr &= ~1u;
        }
    }
    if (flags & VFS_ATTR_ARCHIVE) {
        if (enable) {
            attr |= 0x20;
        } else {
            attr &= ~0x20u;
        }
    }
    if (!SetFileAttributesA(file->path, attr)) {
        return (int)VFS_OPEN_FAILED;
    }
    return 0;
}

int vfs_file_set_attributes(VfsFile* file, unsigned int flags, const char* name, int path_id) {
    return change_attributes(file, flags, name, path_id, 1);
}

int vfs_file_clear_attributes(VfsFile* file, unsigned int flags, const char* name, int path_id) {
    return change_attributes(file, flags, name, path_id, 0);
}

struct VfsSlot {
    unsigned int mode;
    unsigned int flags;
    int user;
    int error;
    int position;
    int reads;
    int total;
    int size;
    int window_base;
    int window_pos;
    int window_size;
    unsigned char window[0x8000];
    FILE* loose;
    VfsFile* owner;
    unsigned int scramble[3];
};

static VfsSlot* open_slot(VfsFile* file) {
    return (VfsSlot*)cache_slot(file->slot);
}

static int lesser(int a, int b) {
    return a < b ? a : b;
}

static unsigned int g_crc_table[256];
static int g_crc_ready;

extern "C" void crc32_build(unsigned int* scratch) {
    for (int i = 0; i < 256; i++) {
        *scratch = (unsigned int)i;
        for (int bit = 0; bit < 8; bit++) {
            unsigned int value = *scratch;
            if (value & 1) {
                value = (value >> 1) ^ 0xEDB88320u;
            } else {
                value >>= 1;
            }
            *scratch = value;
        }
        g_crc_table[i] = *scratch;
    }
    g_crc_ready = 1;
    *scratch = 0;
}

extern "C" unsigned int crc32_update(unsigned int* crc, const unsigned char* data, int length) {
    if (!g_crc_ready) {
        crc32_build(crc);
    }
    if (length == 0) {
        return *crc;
    }
    unsigned int value = *crc;
    for (int i = 0; i < length; i++) {
        unsigned int index = (value ^ data[i]) & 0xFF;
        value = (value >> 8) ^ g_crc_table[index];
        *crc = value;
    }
    return value;
}

extern "C" void scramble_save(const unsigned int* state, unsigned int* dst) {
    dst[0] = state[0];
    dst[1] = state[1];
    dst[2] = state[2];
}

extern "C" void scramble_init(unsigned int* state, const unsigned int* seed) {
    state[0] = seed[0] ? seed[0] : 0x891E7682u;
    state[1] = seed[1] ? seed[1] : 0x6C485F72u;
    state[2] = seed[2] ? seed[2] : 0x5BD48E3Au;
}

extern "C" void scramble_update(unsigned int* state, unsigned char* data, int length) {
    if (length == 0) {
        return;
    }
    for (int left = length; left > 0; left--) {
        unsigned char mixed = 0;
        for (int bit = 0; bit < 8; bit++) {
            unsigned int carry = 0;
            int value = (int)state[0];
            if (value & 1) {
                state[0] = ((unsigned int)value ^ 0xC0000031u) | 0x80000000u;
                value = (int)state[1];
                if (value & 1) {
                    carry = 1;
                    state[1] = ((unsigned int)value ^ 0x20000010u) | 0x40000000u;
                } else {
                    state[1] = (unsigned int)(value >> 1);
                }
            } else {
                state[0] = (unsigned int)(value >> 1);
                value = (int)state[2];
                if (value & 1) {
                    carry = 1;
                    state[2] = ((unsigned int)value ^ 0x08000001u) | 0x10000000u;
                } else {
                    state[2] = (unsigned int)(value >> 1);
                }
            }
            mixed = (unsigned char)((mixed << 1) | carry);
        }
        *data = (unsigned char)(*data ^ mixed);
        data++;
    }
}

void vfs_file_set_scramble(VfsFile* file, int key) {
    VfsSlot* slot = open_slot(file);
    if (!key) {
        slot->flags &= ~1u;
        return;
    }
    slot->flags |= 1;
    unsigned int seed[3];
    seed[0] = (unsigned int)key;
    seed[1] = 0;
    seed[2] = 0;
    scramble_init(slot->scramble, seed);
}

int vfs_file_read(VfsFile* file, void* dst, int count, int min_user, int unused) {
    (void)unused;
    VfsSlot* slot = open_slot(file);
    slot->reads += 1;
    if (slot->user < min_user || slot->error != 0) {
        return 0;
    }
    int from_window = lesser(slot->window_size - slot->window_pos, count);
    int copy_len = from_window < 0 ? 0 : from_window;
    unsigned char* out = (unsigned char*)dst;
memcpy(out, slot->window + slot->window_pos, (size_t)copy_len);
    slot->window_pos += from_window;
    int remain = count - from_window;
    if (remain > 0) {
        unsigned char* cursor = out + from_window;
        if (remain >= 0x8000) {
            slot->window_pos = 0;
            slot->window_size = 0;
            slot->window_base = -1;
            int got;
            if (slot->loose) {
                got = (int)fread(cursor, 1, (size_t)remain, slot->loose);
            } else {
                got = packfile_read(cursor, 1, remain, file->entry);
            }
            from_window += got;
        } else {
            int left = slot->size - from_window - slot->position;
            int fill = lesser(left, 0x8000);
            if (slot->loose) {
                int got = (int)fread(slot->window, 1, (size_t)fill, slot->loose);
                slot->window_size = got;
                if (got < fill && !vfs_file_at_end(file)) {
                    slot->error = (int)VFS_OPEN_FAILED;
                }
                slot->window_size = fill;
            } else {
                slot->window_size = packfile_read(slot->window, 1, fill, file->entry);
            }
            slot->window_base = slot->position + from_window;
            int take = lesser(left, remain);
            if (take < 0) {
                take = 0;
            }
memcpy(cursor, slot->window, (size_t)take);
            slot->window_pos = take;
            from_window += take;
        }
    }
    if (slot->flags & 1) {
        scramble_update(slot->scramble, out, from_window);
    }
    slot->position += from_window;
    slot->total += from_window;
    if (count != from_window) {
        slot->error = -1;
    }
    return from_window;
}

int vfs_file_checksum(VfsFile* file, unsigned int* out_crc, const char* name, int path_id) {
    unsigned int crc = 0;
    int opened_here = 0;
    int saved = 0;
    if (name) {
        if (!vfs_file_open(file, name, path_id)) {
            return (int)VFS_OPEN_NOTFOUND;
        }
    }
    if (file->slot < 0) {
        opened_here = 1;
        int code = vfs_file_acquire(file, 1);
        if (code < 0) {
            return code;
        }
    } else {
        if ((vfs_file_mode_mask(file, 1) & 1) == 0) {
            return -1;
        }
        saved = vfs_file_position(file);
        if (saved < 0) {
            return saved;
        }
        int code = vfs_file_seek(file, 0, 0);
        if (code != 0) {
            return code;
        }
    }
    int done = 0;
    int size = vfs_file_length(file, 0, VFS_ANY_PATH);
    for (;;) {
        int chunk = done + 0x400 > size ? size - done : 0x400;
        unsigned char buf[0x400];
        vfs_file_read(file, buf, chunk, 0, 0);
        if (vfs_file_error(file) < 0) {
            break;
        }
        crc32_update(&crc, buf, chunk);
        *out_crc = crc;
        done += chunk;
        if (done >= size) {
            break;
        }
    }
    if (opened_here) {
        return vfs_file_close(file);
    }
    vfs_file_seek(file, saved, 0);
    return vfs_file_error(file);
}

static char g_blank_string[] = "";
static char g_blank_cstr[] = "";
static char g_blank_line[] = "";

void vfs_read_text(VfsFile* file, VfsText* text) {
    vfs_text_clear(text);
    unsigned short length = vfs_read_word_b(file, 0, 0);
    if (!length) {
        return;
    }
    vfs_text_resize(text, length);
    vfs_file_read(file, text->data, length, 0, 0);
    text->data[length] = 0;
}

int vfs_read_bool(VfsFile* file, int version, int fallback) {
    if (!vfs_file_user_ge(file, version)) {
        return (unsigned char)fallback;
    }
    unsigned char value = 0;
    vfs_file_read(file, &value, 1, 0, 0);
    if (vfs_file_error(file)) {
        return (unsigned char)fallback;
    }
    return value != 0;
}

static unsigned char read_byte(VfsFile* file, int version, int fallback) {
    if (!vfs_file_user_ge(file, version)) {
        return (unsigned char)fallback;
    }
    unsigned char value = 0;
    vfs_file_read(file, &value, 1, 0, 0);
    if (vfs_file_error(file)) {
        return (unsigned char)fallback;
    }
    return value;
}

unsigned char vfs_read_byte(VfsFile* file, int version, int fallback) {
    return read_byte(file, version, fallback);
}

unsigned char vfs_read_byte_b(VfsFile* file, int version, int fallback) {
    return read_byte(file, version, fallback);
}

static unsigned short read_word(VfsFile* file, int version, int fallback) {
    if (!vfs_file_user_ge(file, version)) {
        return (unsigned short)fallback;
    }
    unsigned short value = 0;
    vfs_file_read(file, &value, 2, 0, 0);
    if (vfs_file_error(file)) {
        return (unsigned short)fallback;
    }
    return value;
}

unsigned short vfs_read_word(VfsFile* file, int version, int fallback) {
    return read_word(file, version, fallback);
}

unsigned short vfs_read_word_b(VfsFile* file, int version, int fallback) {
    return read_word(file, version, fallback);
}

static unsigned int read_dword(VfsFile* file, int version, int fallback) {
    if (!vfs_file_user_ge(file, version)) {
        return (unsigned int)fallback;
    }
    unsigned int value = 0;
    vfs_file_read(file, &value, 4, 0, 0);
    if (vfs_file_error(file)) {
        return (unsigned int)fallback;
    }
    return value;
}

unsigned int vfs_read_dword(VfsFile* file, int version, int fallback) {
    return read_dword(file, version, fallback);
}

unsigned int vfs_read_dword_b(VfsFile* file, int version, int fallback) {
    return read_dword(file, version, fallback);
}

float vfs_read_float(VfsFile* file, int version, float fallback) {
    if (!vfs_file_user_ge(file, version)) {
        return fallback;
    }
    float value = 0;
    vfs_file_read(file, &value, 4, 0, 0);
    if (vfs_file_error(file)) {
        return fallback;
    }
    return value;
}

void vfs_write_raw(VfsFile* file, void* data, int count) {
    VfsSlot* slot = open_slot(file);
    slot->reads += 1;
    if (slot->error != 0 || count == 0) {
        return;
    }
    unsigned char* bytes = (unsigned char*)data;
    if (slot->flags & 1) {
        unsigned int saved[3];
        scramble_save(slot->scramble, saved);
        scramble_update(slot->scramble, bytes, count);
        int wrote = (int)fwrite(bytes, (size_t)count, 1, slot->loose);
        scramble_init(slot->scramble, saved);
        scramble_update(slot->scramble, bytes, count);
        if (!wrote) {
            slot->error = (int)VFS_IO_FAILED;
            return;
        }
    } else {
        int wrote = (int)fwrite(bytes, (size_t)count, 1, slot->loose);
        if (!wrote) {
            slot->error = (int)VFS_IO_FAILED;
            return;
        }
    }
    slot->position += count;
    slot->total += count;
}

void vfs_write_bool(VfsFile* file, unsigned char value) {
    unsigned char byte = value == 1;
    vfs_write_raw(file, &byte, 1);
}

void vfs_write_byte(VfsFile* file, unsigned char value) {
    vfs_write_raw(file, &value, 1);
}

void vfs_write_byte_b(VfsFile* file, unsigned char value) {
    vfs_write_byte(file, value);
}

void vfs_write_word(VfsFile* file, unsigned short value) {
    vfs_write_raw(file, &value, 2);
}

void vfs_write_word_b(VfsFile* file, unsigned short value) {
    vfs_write_word(file, value);
}

void vfs_write_dword(VfsFile* file, unsigned int value) {
    vfs_write_raw(file, &value, 4);
}

void vfs_write_dword_b(VfsFile* file, unsigned int value) {
    vfs_write_dword(file, value);
}

void vfs_write_dword_c(VfsFile* file, unsigned int value) {
    vfs_write_dword(file, value);
}

void vfs_write_vec3(VfsFile* file, const void* value) {
    vfs_write_raw(file, (void*)value, 12);
}

void vfs_write_vec4(VfsFile* file, const void* value) {
    const unsigned int* v = (const unsigned int*)value;
    vfs_write_dword_c(file, v[0]);
    vfs_write_dword_c(file, v[1]);
    vfs_write_dword_c(file, v[2]);
    vfs_write_dword_c(file, v[3]);
}

void vfs_write_basis(VfsFile* file, const void* value) {
    const unsigned char* v = (const unsigned char*)value;
    vfs_write_vec3(file, v + 0x18);
    vfs_write_vec3(file, v);
    vfs_write_vec3(file, v + 0x0C);
}

void vfs_write_mat34(VfsFile* file, const void* value) {
    static const int offsets[12] = {
        0x00, 0x0C, 0x18, 0x04, 0x10, 0x1C, 0x08, 0x14, 0x20, 0x24, 0x28, 0x2C
    };
    const unsigned char* v = (const unsigned char*)value;
    for (int i = 0; i < 12; i++) {
        unsigned int bits;
memcpy(&bits, v + offsets[i], 4);
        vfs_write_dword_c(file, bits);
    }
}

void vfs_write_string(VfsFile* file, const char* text, int length) {
    if (!text) {
        text = g_blank_string;
    }
    if (length < 0) {
        length = (int)strlen(text);
    }
    vfs_write_word_b(file, (unsigned short)length);
    if (length) {
        vfs_write_raw(file, (void*)text, length);
    }
}

void vfs_write_cstr(VfsFile* file, const char* text) {
    if (!text) {
        text = g_blank_cstr;
    }
    vfs_write_raw(file, (void*)text, (int)strlen(text) + 1);
}

void vfs_write_line(VfsFile* file, const char* text, int length) {
    VfsSlot* slot = open_slot(file);
    if (slot->error != 0) {
        return;
    }
    if (!text) {
        text = g_blank_line;
    }
    if (length < 0) {
        length = (int)strlen(text);
    }
    if (length) {
        if (!fwrite(text, (size_t)length, 1, slot->loose)) {
            slot->error = (int)VFS_IO_FAILED;
            return;
        }
    }
    char newline = '\n';
    if (!fwrite(&newline, 1, 1, slot->loose)) {
        slot->error = (int)VFS_IO_FAILED;
    }
}

void vfs_io_bool(VfsFile* file, unsigned char* value, int version, int fallback) {
    if (vfs_file_mode_mask(file, 1)) {
        *value = (unsigned char)vfs_read_bool(file, version, fallback);
    } else {
        vfs_write_bool(file, *value);
    }
}

void vfs_io_byte(VfsFile* file, unsigned char* value, int version, int fallback) {
    if (vfs_file_mode_mask(file, 1)) {
        *value = vfs_read_byte(file, version, fallback);
    } else {
        vfs_write_byte(file, *value);
    }
}

void vfs_io_byte_b(VfsFile* file, unsigned char* value, int version, int fallback) {
    if (vfs_file_mode_mask(file, 1)) {
        *value = vfs_read_byte_b(file, version, fallback);
    } else {
        vfs_write_byte_b(file, *value);
    }
}

void vfs_io_word(VfsFile* file, unsigned short* value, int version, int fallback) {
    if (vfs_file_mode_mask(file, 1)) {
        *value = vfs_read_word(file, version, fallback);
    } else {
        vfs_write_word(file, *value);
    }
}

void vfs_io_word_b(VfsFile* file, unsigned short* value, int version, int fallback) {
    if (vfs_file_mode_mask(file, 1)) {
        *value = vfs_read_word_b(file, version, fallback);
    } else {
        vfs_write_word_b(file, *value);
    }
}

void vfs_io_dword(VfsFile* file, unsigned int* value, int version, int fallback) {
    if (vfs_file_mode_mask(file, 1)) {
        *value = vfs_read_dword(file, version, fallback);
    } else {
        vfs_write_dword(file, *value);
    }
}

void vfs_io_dword_b(VfsFile* file, unsigned int* value, int version, int fallback) {
    if (vfs_file_mode_mask(file, 1)) {
        *value = vfs_read_dword_b(file, version, fallback);
    } else {
        vfs_write_dword_b(file, *value);
    }
}

void vfs_io_float(VfsFile* file, float* value, int version, float fallback) {
    if (vfs_file_mode_mask(file, 1)) {
        *value = vfs_read_float(file, version, fallback);
    } else {
        unsigned int bits;
memcpy(&bits, value, 4);
        vfs_write_dword_c(file, bits);
    }
}

float g_vfs_zero_vec[3];

static void copy_n(char* dst, const char* src, int count) {
    int i = 0;
    for (; i < count && src[i]; i++) {
        dst[i] = src[i];
    }
    for (; i < count; i++) {
        dst[i] = 0;
    }
}

void vfs_read_vec3(VfsFile* file, void* dst, int version, const void* fallback) {
    if (!vfs_file_user_ge(file, version)) {
memcpy(dst, fallback, 12);
        return;
    }
    vfs_file_read(file, dst, 12, version, 0);
}

void vfs_read_vec4(VfsFile* file, void* dst, int version, const void* fallback) {
    if (!vfs_file_user_ge(file, version)) {
memcpy(dst, fallback, 16);
        return;
    }
    float* out = (float*)dst;
    out[0] = vfs_read_float(file, 0, 0);
    out[1] = vfs_read_float(file, 0, 0);
    out[2] = vfs_read_float(file, 0, 0);
    out[3] = vfs_read_float(file, 0, 0);
}

void vfs_read_basis(VfsFile* file, void* dst, int version, const void* fallback) {
    if (!vfs_file_user_ge(file, version)) {
memcpy(dst, fallback, 0x24);
        return;
    }
    unsigned char* out = (unsigned char*)dst;
    vfs_read_vec3(file, out + 0x18, 0, g_vfs_zero_vec);
    vfs_read_vec3(file, out, 0, g_vfs_zero_vec);
    vfs_read_vec3(file, out + 0x0C, 0, g_vfs_zero_vec);
}

void vfs_read_mat34(VfsFile* file, void* dst, int version, const void* fallback) {
    if (!vfs_file_user_ge(file, version)) {
memcpy(dst, fallback, 0x30);
        return;
    }
    float* out = (float*)dst;
    static const int slots[12] = {0, 3, 6, 1, 4, 7, 2, 5, 8, 9, 10, 11};
    for (int i = 0; i < 12; i++) {
        out[slots[i]] = vfs_read_float(file, 0, 0);
    }
    out[3] = 0;
    out[6] = 0;
    out[9] = 0;
    out[12] = 1.0f;
}

void vfs_read_text_ver(VfsFile* file, VfsText* text, int version, const char* fallback) {
    if (!vfs_file_user_ge(file, version)) {
        vfs_text_assign(text, fallback);
        return;
    }
    vfs_read_text(file, text);
}

void vfs_read_cstr_text(VfsFile* file, VfsText* text, int version, const char* fallback) {
    if (!vfs_file_user_ge(file, version)) {
        vfs_text_assign(text, fallback);
        return;
    }
    char buf[0x100];
    buf[0] = (char)vfs_read_byte(file, 0, 0);
    if (buf[0]) {
        int i = 1;
        unsigned char ch;
        do {
            ch = vfs_read_byte(file, 0, 0);
            buf[i++] = (char)ch;
        } while (ch);
    }
    vfs_text_assign(text, buf);
}

int vfs_read_line_text(VfsFile* file, VfsText* text, int version) {
    vfs_text_assign(text, "");
    if (!vfs_file_user_ge(file, version)) {
        return 0;
    }
    int count = 0;
    unsigned char ch = vfs_read_byte(file, 0, 0);
    while (ch && ch != '\r' && ch != '\n') {
        vfs_text_append_char(text, (char)ch);
        ch = vfs_read_byte(file, 0, 0);
        count++;
    }
    while (ch == '\r' || ch == '\n') {
        ch = vfs_read_byte(file, 0, 0);
    }
    if (ch) {
        vfs_file_seek(file, -1, 1);
    }
    return count;
}

void vfs_read_chars(VfsFile* file, char* dst, int max_len, int version, const char* fallback) {
    if (max_len < 0) {
        max_len = 0xFFFF;
    }
    if (!vfs_file_user_ge(file, version)) {
        if (fallback) {
            copy_n(dst, fallback, max_len);
        }
        return;
    }
    unsigned short length = vfs_read_word_b(file, 0, 0);
    if (!length) {
        dst[0] = 0;
        return;
    }
    if (length < max_len) {
        vfs_file_read(file, dst, length, 0, 0);
        dst[length] = 0;
        return;
    }
    int keep = max_len - 1;
    vfs_file_read(file, dst, keep, 0, 0);
    dst[keep] = 0;
    int extra = length - max_len + 1;
    while (extra > 0) {
        vfs_read_byte(file, 0, 0);
        extra--;
    }
}

void vfs_read_cstr(VfsFile* file, char* dst, int max_len, int version, const char* fallback) {
    if (max_len < 0) {
        max_len = 0xFFFF;
    }
    if (!vfs_file_user_ge(file, version)) {
        if (fallback) {
strcpy(dst, fallback);
        }
        return;
    }
    int index = 0;
    for (;;) {
        unsigned char ch = vfs_read_byte(file, 0, 0);
        if (!ch) {
            dst[index] = 0;
            return;
        }
        if (index < max_len) {
            dst[index++] = (char)ch;
        }
    }
}

int vfs_read_line(VfsFile* file, char* dst, int unused, int version) {
    (void)unused;
    dst[0] = 0;
    if (!vfs_file_user_ge(file, version)) {
        return 0;
    }
    int count = 0;
    unsigned char ch = vfs_read_byte(file, 0, 0);
    while (ch && ch != '\r' && ch != '\n') {
        dst[count++] = (char)ch;
        ch = vfs_read_byte(file, 0, 0);
    }
    dst[count] = 0;
    while (ch == '\r' || ch == '\n') {
        ch = vfs_read_byte(file, 0, 0);
    }
    if (ch) {
        vfs_file_seek(file, -1, 1);
    }
    return count;
}

void vfs_io_vec3(VfsFile* file, void* value, int version, const void* fallback) {
    if (vfs_file_mode_mask(file, 1)) {
        vfs_read_vec3(file, value, version, fallback);
    } else {
        vfs_write_vec3(file, value);
    }
}

void vfs_io_vec4(VfsFile* file, void* value, int version, const void* fallback) {
    if (vfs_file_mode_mask(file, 1)) {
        vfs_read_vec4(file, value, version, fallback);
    } else {
        vfs_write_vec4(file, value);
    }
}

void vfs_io_basis(VfsFile* file, void* value, int version, const void* fallback) {
    if (vfs_file_mode_mask(file, 1)) {
        vfs_read_basis(file, value, version, fallback);
    } else {
        vfs_write_basis(file, value);
    }
}

void vfs_io_mat34(VfsFile* file, void* value, int version, const void* fallback) {
    if (vfs_file_mode_mask(file, 1)) {
        vfs_read_mat34(file, value, version, fallback);
    } else {
        vfs_write_mat34(file, value);
    }
}

void vfs_io_text(VfsFile* file, VfsText* text, int version, const char* fallback) {
    if (vfs_file_mode_mask(file, 1)) {
        vfs_read_text_ver(file, text, version, fallback);
    } else {
        vfs_write_string(file, vfs_text_cstr(text), -1);
    }
}

void vfs_io_chars(VfsFile* file, char* dst, int max_len, int version, const char* fallback) {
    if (vfs_file_mode_mask(file, 1)) {
        vfs_read_chars(file, dst, max_len, version, fallback);
    } else {
        vfs_write_string(file, dst, -1);
    }
}

void vfs_io_bytes(VfsFile* file, void* data, int count, int version) {
    if (vfs_file_mode_mask(file, 1)) {
        vfs_file_read(file, data, count, version, 0);
    } else {
        vfs_write_raw(file, data, count);
    }
}

extern "C" int __fastcall fn_005273C0(void* self)
{
    return *(int*)((char*)self + 0x274);
}

extern "C" int __fastcall fn_005273D0(void* self)
{
    return (int)((char*)self + 0x27c);
}

extern "C" void __fastcall fn_00552120(void* self)
{
    *(int*)self = 0;
}

extern "C" int __fastcall fn_005530A0(void* self)
{
    return (int)((char*)self + 0x278);
}

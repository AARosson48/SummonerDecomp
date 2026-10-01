#ifndef SUMMONER_VFS_H
#define SUMMONER_VFS_H

/* On-disk and in-memory packfile layout recovered from Sum.exe
 * vsdk/vfile/packfile/file_packfile.cpp (VA 0x533DF0..0x534A20).
 * Checked against tables.vpp: magic, 638 files, first file
 * AoqiIntro_cscript.tbl at sector 21.
 */

#define PACKFILE_MAGIC 0x51890ACEu
#define PACKFILE_SECTOR 0x800
#define PACKFILE_DIR_NAME 60
#define PACKFILE_ENTRIES_PER_SECTOR 32
#define PACKFILE_MAX_FILES 0x34BC
#define PACKFILE_MAX_ARCHIVES 16
#define PACKFILE_HASH_SLOTS 0x50E9
#define PACKFILE_ENTRY_SIZE 0x1C

#ifdef __cplusplus
extern "C" {
#endif

typedef struct PackfileHeader {
    unsigned int magic;
    unsigned int version;
    unsigned int file_count;
    unsigned int archive_size;
} PackfileHeader;

typedef struct PackfileDiskEntry {
    char name[PACKFILE_DIR_NAME];
    unsigned int size;
} PackfileDiskEntry;

typedef struct Packfile Packfile;
typedef struct PackfileEntry PackfileEntry;

struct PackfileEntry {
    int hash;
    char* name;
    int sector;
    unsigned int size;
    Packfile* pack;
    void* hd_file;
    void* cd_file;
};

struct Packfile {
    char name[0x20];
    int is_cd;
    int unused_24;
    int file_count;
    PackfileEntry* entries;
    unsigned int archive_size;
};

int packfile_init(void);
void packfile_shutdown(void);
int packfile_add(const char* name);
Packfile* packfile_find(const char* name);
PackfileEntry* packfile_find_file(const char* name);
int packfile_open(PackfileEntry* entry);
void packfile_close_file(PackfileEntry* entry);
int packfile_seek(PackfileEntry* entry, int offset, int origin);
int packfile_read(void* dst, int size, int count, PackfileEntry* entry);
int packfile_read_ok(void);
int packfile_read_archive(void* dst, int offset, const char* name);
int packfile_lookup(const char* pack_name, const char* file_name, int* out_size, int* out_offset);

#define VFS_SEARCH_SLOTS 512

typedef struct VfsSearch {
    char* path;
    char* extensions;
} VfsSearch;

int vfs_add_search_path(const char* path, const char* extensions);
void vfs_clear_search_path(int index);
void vfs_init_search_paths(void);
int vfs_path_models_levels(void);
int vfs_path_root(void);
const char* vfs_search_extensions(int index);
char* vfs_resolve_path(int path_id, char* dst);
char* vfs_copy_search_dir(int path_id, char* dst);
int vfs_path_id_ok(int path_id);
char* vfs_make_hd_path(int path_id, const char* filename, char* dst);
char* vfs_make_cd_path(int path_id, const char* filename, char* dst);
void vfs_shutdown(void);
void vfs_set_root(const char* path, int mount_packs);

int level_archives_ready(void);
void level_mount_archives(void);

#define VFS_OPEN_SLOTS 4
#define VFS_ANY_PATH 0x98967F
#define VFS_OPEN_NOSLOT 0xFFFFFFFA
#define VFS_OPEN_NOTFOUND 0xFFFFFFFC
#define VFS_OPEN_PACKED 0xFFFFFFFD
#define VFS_OPEN_FAILED 0xFFFE7961
#define VFS_ATTR_READONLY 2
#define VFS_ATTR_ARCHIVE 4
#define VFS_IO_FAILED 0xFFFFFFFE

typedef struct VfsText {
    int length;
    char* data;
} VfsText;

void* vfs_heap_alloc3(int size, int unused_a, int unused_b);
void* vfs_heap_forward(int size, int unused_a, int unused_b);
void* vfs_heap_alloc(int size);
void vfs_heap_free(void* block);
void vfs_text_set(VfsText* text, const char* src, int max_len);
void vfs_text_set_char(VfsText* text, char value);
void vfs_text_resize(VfsText* text, int length);
void __fastcall vfs_text_clear(VfsText* text);
VfsText* vfs_text_ctor(VfsText* text);
VfsText* vfs_text_ctor_str(VfsText* text, const char* src);
VfsText* vfs_text_ctor_char(VfsText* text, char value);
VfsText* vfs_text_ctor_copy(VfsText* text, const VfsText* other);
VfsText* vfs_text_ctor_len(VfsText* text, int length);
void vfs_text_dtor(VfsText* text);
const char* vfs_text_cstr(VfsText* text);
int vfs_text_size(VfsText* text);
bool __fastcall vfs_text_empty(VfsText* text);
bool __fastcall vfs_text_blank(VfsText* text);
void vfs_text_lower(VfsText* text);
void vfs_text_upper(VfsText* text);
VfsText* vfs_text_assign(VfsText* text, const char* src);
VfsText* vfs_text_assign_char(VfsText* text, char value);
VfsText* vfs_text_assign_text(VfsText* text, const VfsText* other);
VfsText* vfs_text_slice(VfsText* text, VfsText* out, int start, int end);
VfsText* vfs_text_left(VfsText* text, VfsText* out, int count);
VfsText* vfs_text_right(VfsText* text, VfsText* out, int count);
VfsText* vfs_text_mid(VfsText* text, VfsText* out, int start, int count);
int vfs_text_find(VfsText* text, const char* needle, int from_end);
void vfs_text_trim_left(VfsText* text);
void vfs_text_trim_right(VfsText* text);
void vfs_text_trim(VfsText* text);
float vfs_text_float(VfsText* text);
int vfs_text_is_empty(VfsText* text);
char* vfs_text_at(VfsText* text, int index);
VfsText* vfs_text_concat(VfsText* out, const VfsText* left, const VfsText* right);
VfsText* vfs_text_concat_cstr(VfsText* out, const VfsText* text, const char* src);
VfsText* vfs_text_concat_cstr_left(VfsText* out, const char* src, const VfsText* text);
VfsText* vfs_text_concat_char(VfsText* out, const VfsText* text, char value);
VfsText* vfs_text_concat_char_left(VfsText* out, char value, const VfsText* text);
VfsText* vfs_text_append(VfsText* text, const VfsText* other);
VfsText* vfs_text_append_cstr(VfsText* text, const char* src);
VfsText* vfs_text_append_char(VfsText* text, char value);
bool vfs_text_eq(const VfsText* left, const VfsText* right);
int vfs_text_eq_cstr(const char* src, const VfsText* text);
int vfs_text_eq_rcstr(const VfsText* text, const char* src);
bool vfs_text_ne(const VfsText* left, const VfsText* right);
int vfs_text_ne_cstr(const char* src, const VfsText* text);
int vfs_text_ne_rcstr(const VfsText* text, const char* src);
bool vfs_text_lt(const VfsText* left, const VfsText* right);
int vfs_text_lt_cstr(const char* src, const VfsText* text);
int vfs_text_lt_rcstr(const VfsText* text, const char* src);
bool vfs_text_gt(const VfsText* left, const VfsText* right);
int vfs_text_gt_cstr(const char* src, const VfsText* text);
int vfs_text_gt_rcstr(const VfsText* text, const char* src);
bool vfs_text_le(const VfsText* left, const VfsText* right);
int vfs_text_le_cstr(const char* src, const VfsText* text);
int vfs_text_le_rcstr(const VfsText* text, const char* src);
bool vfs_text_ge(const VfsText* left, const VfsText* right);
int vfs_text_ge_cstr(const char* src, const VfsText* text);
int vfs_text_ge_rcstr(const VfsText* text, const char* src);
VfsText* vfs_text_from_int(VfsText* out, int value);
VfsText* vfs_text_from_hex(VfsText* out, unsigned int value);
VfsText* vfs_text_from_float(VfsText* out, float value, int precision);

typedef struct VfsFile {
    int path_id;
    char found;
    char path[0x103];
    PackfileEntry* entry;
    char opened;
    char pad_10d[3];
    int slot;
    char tail[8];
} VfsFile;

void vfs_file_ctor(VfsFile* file);
void vfs_file_dtor(VfsFile* file);
int vfs_file_try_cd(VfsFile* file, const char* name, int path_id, int search_packs);
int vfs_file_archive_path(VfsFile* file, char* dst, int* out_offset, int max_len);
unsigned int vfs_file_mode_mask(VfsFile* file, unsigned int mask);
int vfs_file_user_ge(VfsFile* file, int value);
int vfs_file_user(VfsFile* file);
void vfs_file_set_user(VfsFile* file, int value);
char* vfs_file_path(VfsFile* file);
char* vfs_file_leaf(VfsFile* file);
int vfs_file_open(VfsFile* file, const char* name, int path_id);
int vfs_file_acquire(VfsFile* file, unsigned int mode);
int vfs_file_open_mode(VfsFile* file, const char* name, int path_id, unsigned int mode);
int vfs_file_close(VfsFile* file);
int vfs_file_read_ready(VfsFile* file);
int vfs_file_flush(VfsFile* file);
int vfs_file_length(VfsFile* file, const char* name, int path_id);
int vfs_file_from_cd(VfsFile* file);
int vfs_file_seek(VfsFile* file, int offset, int origin);
int vfs_file_position(VfsFile* file);
int vfs_file_at_end(VfsFile* file);
int vfs_file_error(VfsFile* file);
int vfs_file_get_path_id(VfsFile* file);
int vfs_file_is_open(VfsFile* file);
int vfs_file_remove(VfsFile* file, const char* name, int path_id);
int vfs_file_rename(VfsFile* file, const char* new_name, const char* name, int path_id);
int vfs_file_attributes(VfsFile* file, const char* name, int path_id);
int vfs_file_set_attributes(VfsFile* file, unsigned int flags, const char* name, int path_id);
int vfs_file_clear_attributes(VfsFile* file, unsigned int flags, const char* name, int path_id);
void vfs_file_set_scramble(VfsFile* file, int key);
int vfs_file_read(VfsFile* file, void* dst, int count, int min_user, int unused);
int vfs_file_checksum(VfsFile* file, unsigned int* out_crc, const char* name, int path_id);

void vfs_read_text(VfsFile* file, VfsText* text);
int vfs_read_bool(VfsFile* file, int version, int fallback);
unsigned char vfs_read_byte(VfsFile* file, int version, int fallback);
unsigned char vfs_read_byte_b(VfsFile* file, int version, int fallback);
unsigned short vfs_read_word(VfsFile* file, int version, int fallback);
unsigned short vfs_read_word_b(VfsFile* file, int version, int fallback);
unsigned int vfs_read_dword(VfsFile* file, int version, int fallback);
unsigned int vfs_read_dword_b(VfsFile* file, int version, int fallback);
float vfs_read_float(VfsFile* file, int version, float fallback);

void vfs_write_raw(VfsFile* file, void* data, int count);
void vfs_write_bool(VfsFile* file, unsigned char value);
void vfs_write_byte(VfsFile* file, unsigned char value);
void vfs_write_byte_b(VfsFile* file, unsigned char value);
void vfs_write_word(VfsFile* file, unsigned short value);
void vfs_write_word_b(VfsFile* file, unsigned short value);
void vfs_write_dword(VfsFile* file, unsigned int value);
void vfs_write_dword_b(VfsFile* file, unsigned int value);
void vfs_write_dword_c(VfsFile* file, unsigned int value);
void vfs_write_vec3(VfsFile* file, const void* value);
void vfs_write_vec4(VfsFile* file, const void* value);
void vfs_write_basis(VfsFile* file, const void* value);
void vfs_write_mat34(VfsFile* file, const void* value);
void vfs_write_string(VfsFile* file, const char* text, int length);
void vfs_write_cstr(VfsFile* file, const char* text);
void vfs_write_line(VfsFile* file, const char* text, int length);

void vfs_io_bool(VfsFile* file, unsigned char* value, int version, int fallback);
void vfs_io_byte(VfsFile* file, unsigned char* value, int version, int fallback);
void vfs_io_byte_b(VfsFile* file, unsigned char* value, int version, int fallback);
void vfs_io_word(VfsFile* file, unsigned short* value, int version, int fallback);
void vfs_io_word_b(VfsFile* file, unsigned short* value, int version, int fallback);
void vfs_io_dword(VfsFile* file, unsigned int* value, int version, int fallback);
void vfs_io_dword_b(VfsFile* file, unsigned int* value, int version, int fallback);
void vfs_io_float(VfsFile* file, float* value, int version, float fallback);
void vfs_read_vec3(VfsFile* file, void* dst, int version, const void* fallback);
void vfs_read_vec4(VfsFile* file, void* dst, int version, const void* fallback);
void vfs_read_basis(VfsFile* file, void* dst, int version, const void* fallback);
void vfs_read_mat34(VfsFile* file, void* dst, int version, const void* fallback);
void vfs_read_text_ver(VfsFile* file, VfsText* text, int version, const char* fallback);
void vfs_read_cstr_text(VfsFile* file, VfsText* text, int version, const char* fallback);
int vfs_read_line_text(VfsFile* file, VfsText* text, int version);
void vfs_read_chars(VfsFile* file, char* dst, int max_len, int version, const char* fallback);
void vfs_read_cstr(VfsFile* file, char* dst, int max_len, int version, const char* fallback);
int vfs_read_line(VfsFile* file, char* dst, int unused, int version);
void vfs_io_vec3(VfsFile* file, void* value, int version, const void* fallback);
void vfs_io_vec4(VfsFile* file, void* value, int version, const void* fallback);
void vfs_io_basis(VfsFile* file, void* value, int version, const void* fallback);
void vfs_io_mat34(VfsFile* file, void* value, int version, const void* fallback);
void vfs_io_text(VfsFile* file, VfsText* text, int version, const char* fallback);
void vfs_io_chars(VfsFile* file, char* dst, int max_len, int version, const char* fallback);
void vfs_io_bytes(VfsFile* file, void* data, int count, int version);

#ifdef __cplusplus
}
#endif

#endif

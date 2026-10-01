// Original: D:\projects\Summoner\pccode\vsdk\vfile\packfile\file_packfile.cpp
// Recovered from Sum.exe VA 0x533DF0-0x534A20. Not byte-matched.

#if defined(_WIN64) || defined(__x86_64__) || defined(__amd64__)
#error Sum.exe is a 32-bit image; packfile pointers are 4 bytes
#endif

#include "vfs.h"

#include <cctype>
#include <cstdio>
#include <cstring>

extern char g_hd_root[];
extern char g_cd_root[];

extern int packfile_timer(int scale);
extern void packfile_error(const char* file, int line, const char* msg);
extern void packfile_basename(char* dst, const char* src);
extern void packfile_pool_init(void* pool, int bytes);
extern void packfile_pool_shutdown(void* pool);
extern char* packfile_pool_copy(void* pool, const char* text);

static const float kMsToSeconds = 0.001f;

struct HashSlot {
    int key;
    PackfileEntry* entry;
};

/* Retail BSS order, starting at VA 0x2D0BF98:
 * pool[0x18], entries[13500], packs[16], hash[20713], file_count, pack_count, loaded.
 */
static unsigned char g_pool[0x18];
static PackfileEntry g_entries[PACKFILE_MAX_FILES];
static Packfile g_packs[PACKFILE_MAX_ARCHIVES];
static HashSlot g_hash[PACKFILE_HASH_SLOTS];
static int g_file_count;
static int g_pack_count;
static int g_loaded;

static int pack_stricmp(const char* a, const char* b) {
    for (;;) {
        unsigned char ca = (unsigned char)*a++;
        unsigned char cb = (unsigned char)*b++;
        if (ca >= 'A' && ca <= 'Z') {
            ca = (unsigned char)(ca + 0x20);
        }
        if (cb >= 'A' && cb <= 'Z') {
            cb = (unsigned char)(cb + 0x20);
        }
        if (ca != cb || ca == 0) {
            return (int)ca - (int)cb;
        }
    }
}

extern "C" int filename_hash(const char* name) {
    if (!name) {
        return -1;
    }
    unsigned int hash = 0;
    unsigned char ch = (unsigned char)*name;
    while (ch != 0) {
        int folded = (int)(char)tolower((int)(char)ch);
        hash = (hash << 5) | (hash >> 27);
        hash ^= (unsigned int)folded;
        name++;
        ch = (unsigned char)*name;
    }
    return (int)hash < 0 ? -(int)hash : (int)hash;
}

extern "C" void hash_insert(PackfileEntry* entry) {
    int slot = entry->hash % PACKFILE_HASH_SLOTS;
    if (slot < 0) {
        slot += PACKFILE_HASH_SLOTS;
    }
    for (int n = 0; n < PACKFILE_HASH_SLOTS; n++) {
        if (g_hash[slot].key == -1) {
            g_hash[slot].key = entry->hash;
            g_hash[slot].entry = entry;
            return;
        }
        slot++;
        if (slot >= PACKFILE_HASH_SLOTS) {
            slot = 0;
        }
    }
}

extern "C" int check_header(Packfile* pack, const PackfileHeader* header) {
    pack->file_count = (int)header->file_count;
    pack->archive_size = header->archive_size;
    if (header->magic != PACKFILE_MAGIC) {
        return 0;
    }
    if (header->version < 1) {
        return 0;
    }
    return (int)header->file_count;
}

extern "C" int read_entries(Packfile* pack, const unsigned char* sector, int count, int* index) {
    if (count <= 0) {
        return 1;
    }
    for (int n = 0; n < count; n++) {
        char name[PACKFILE_DIR_NAME + 1];
        memcpy(name, sector, PACKFILE_DIR_NAME);
        name[PACKFILE_DIR_NAME] = 0;
        sector += PACKFILE_DIR_NAME;
        unsigned int size = *(const unsigned int*)sector;
        sector += 4;

        int hash = filename_hash(name);
        PackfileEntry* entry = &pack->entries[*index];
        entry->hash = hash;
        entry->name = packfile_pool_copy(g_pool, name);
        entry->size = size;
        entry->pack = pack;
        entry->hd_file = 0;
        hash_insert(entry);
        (*index)++;
    }
    return 1;
}

extern "C" void assign_sectors(Packfile* pack, int sector) {
    PackfileEntry* entry = pack->entries;
    for (int n = pack->file_count; n > 0; n--) {
        entry->sector = sector;
        entry->cd_file = 0;
        sector += (int)((entry->size + 0x7ff) >> 11);
        entry++;
    }
}

extern "C" int reserve_entries(Packfile* pack) {
    int total = g_file_count + pack->file_count;
    if (total > PACKFILE_MAX_FILES) {
printf("SIZEOF FILE ENTRY : %d\n", PACKFILE_ENTRY_SIZE);
printf("MAX_PACKFILE_FILES : %d\n", PACKFILE_MAX_FILES);
printf("TOTAL SIZE FOR ALL PACKFILES : %d\n", PACKFILE_MAX_FILES * PACKFILE_ENTRY_SIZE);
        for (;;) {
printf("Out of packfile space!\n");
            packfile_error(
                "D:\\projects\\Summoner\\pccode\\vsdk\\vfile\\packfile\\file_packfile.cpp",
                0x1A7,
                "Out of packfile space!\n");
        }
    }
    int base = g_file_count;
    g_file_count = total;
    pack->entries = &g_entries[base];
    return pack->entries != 0;
}

static int load_root(Packfile* pack, const char* root) {
    char path[0x100];
sprintf(path, "%s%s", root, pack->name);
    FILE* file = fopen(path, "rb");
    if (!file) {
        return 0;
    }

    unsigned char sector[PACKFILE_SECTOR];
fread(sector, PACKFILE_SECTOR, 1, file);
    if (!check_header(pack, (const PackfileHeader*)sector)) {
fclose(file);
        return 0;
    }
    if (!reserve_entries(pack)) {
fclose(file);
        return 0;
    }

    int left = pack->file_count;
    int index = 0;
    int page = 1;
    while (left > 0) {
        int batch = left > PACKFILE_ENTRIES_PER_SECTOR ? PACKFILE_ENTRIES_PER_SECTOR : left;
fread(sector, PACKFILE_SECTOR, 1, file);
        left -= batch;
        if (!read_entries(pack, sector, batch, &index)) {
fclose(file);
            return 0;
        }
        page++;
    }
    assign_sectors(pack, page);
fclose(file);
    return 1;
}

int packfile_init(void) {
    for (int i = 0; i < PACKFILE_HASH_SLOTS; i++) {
        g_hash[i].key = -1;
        g_hash[i].entry = 0;
    }
    g_file_count = 0;
    g_pack_count = 0;
    packfile_pool_init(g_pool, 0x40000);

    int start = packfile_timer(1000);
    packfile_add("tables.vpp");
    packfile_add("summoner.vpp");
    packfile_add("chars.vpp");
    packfile_add("items.vpp");
    packfile_add("effects.vpp");
    packfile_add("music.vpp");
    packfile_add("sounds.vpp");
    packfile_add("cutscene.vpp");
    int elapsed = packfile_timer(1000) - start;
    double seconds = (double)elapsed * (double)kMsToSeconds;
printf("TOTAL PACKFILE LOAD TIME : %.2f\n", seconds);
printf("Packfile static alloc : %d\n", PACKFILE_MAX_FILES * PACKFILE_ENTRY_SIZE);
    int printed = printf("TOTAL PACKFILE FILES: %d\n", g_file_count);
    g_loaded = 1;
    return printed;
}

void packfile_shutdown(void) {
    if (g_loaded) {
        packfile_pool_shutdown(g_pool);
    }
}

int packfile_add(const char* name) {
    for (int i = 0; i < g_pack_count; i++) {
        if (pack_stricmp(g_packs[i].name, name) == 0) {
printf("Ignoring packfile (%s) which was already loaded!\n", name);
            return 1;
        }
    }

    char path[0x80];
sprintf(path, "%s%s", g_hd_root, name);
    FILE* probe = fopen(path, "rb");
    int kind;
    if (probe) {
        kind = 1;
    } else {
sprintf(path, "%s%s", g_cd_root, name);
        probe = fopen(path, "rb");
        if (!probe) {
            return 0;
        }
        kind = 2;
    }
fclose(probe);

    if (g_pack_count >= PACKFILE_MAX_ARCHIVES || !name || strlen(name) > 0x1f) {
        return 0;
    }

    Packfile* pack = &g_packs[g_pack_count];
    g_pack_count++;
strncpy(pack->name, name, 0x1f);
    pack->name[0x1f] = 0;
    pack->is_cd = kind == 2;
    pack->unused_24 = 0;
    pack->file_count = 0;
    pack->entries = 0;
    if (kind == 2) {
printf("Adding CD PACKFILE : %s\n", name);
        return load_root(pack, g_cd_root);
    }
printf("Adding HARD DRIVE PACKFILE : %s\n", name);
    return load_root(pack, g_hd_root);
}

Packfile* packfile_find(const char* name) {
    for (int i = 0; i < g_pack_count; i++) {
        if (pack_stricmp(g_packs[i].name, name) == 0) {
            return &g_packs[i];
        }
    }
    return 0;
}

PackfileEntry* packfile_find_file(const char* name) {
    char normalized[0x100];
    packfile_basename(normalized, name);
    int hash = filename_hash(normalized);
    int slot = hash % PACKFILE_HASH_SLOTS;
    if (slot < 0) {
        slot += PACKFILE_HASH_SLOTS;
    }
    for (int n = 0; n < PACKFILE_HASH_SLOTS; n++) {
        if (g_hash[slot].key == -1) {
            return 0;
        }
        PackfileEntry* entry = g_hash[slot].entry;
        if (g_hash[slot].key == hash && entry && entry->name && pack_stricmp(normalized, entry->name) == 0) {
            return entry;
        }
        slot++;
        if (slot >= PACKFILE_HASH_SLOTS) {
            slot = 0;
        }
    }
    return 0;
}

int packfile_open(PackfileEntry* entry) {
    if (!entry || !entry->pack || entry->hd_file || entry->cd_file) {
        return 0;
    }
    char path[0x80];
    const char* root = entry->pack->is_cd ? g_cd_root : g_hd_root;
sprintf(path, "%s%s", root, entry->pack->name);
    FILE* file = fopen(path, "rb");
    if (!file) {
        return 0;
    }
    if (entry->pack->is_cd) {
        entry->cd_file = file;
    } else {
        entry->hd_file = file;
    }
fseek(file, entry->sector << 11, SEEK_SET);
    return 1;
}

void packfile_close_file(PackfileEntry* entry) {
    if (!entry || !entry->pack) {
        return;
    }
    if (entry->pack->is_cd) {
        if (entry->cd_file) {
fclose((FILE*)entry->cd_file);
            entry->cd_file = 0;
        }
        return;
    }
    if (entry->hd_file) {
fclose((FILE*)entry->hd_file);
        entry->hd_file = 0;
    }
}

int packfile_seek(PackfileEntry* entry, int offset, int origin) {
    if (!entry || !entry->pack) {
        return 0;
    }
    FILE* file = entry->pack->is_cd ? (FILE*)entry->cd_file : (FILE*)entry->hd_file;
    if (!file) {
        return 0;
    }
    return fseek(file, (entry->sector << 11) + offset, origin) == 0;
}

int packfile_read(void* dst, int size, int count, PackfileEntry* entry) {
    if (!entry || !entry->pack) {
printf("FREAD BAIL\n");
        return 0;
    }
    FILE* file = entry->pack->is_cd ? (FILE*)entry->cd_file : (FILE*)entry->hd_file;
    if (!file) {
        if (entry->pack->is_cd) {
printf("FREAD BAIL\n");
        }
        return 0;
    }
    return (int)fread(dst, (size_t)size, (size_t)count, file);
}

int packfile_lookup(const char* pack_name, const char* file_name, int* out_size, int* out_offset) {
    Packfile* pack = packfile_find(pack_name);
    if (!pack) {
        return 0;
    }
    for (int i = 0; i < pack->file_count; i++) {
        PackfileEntry* entry = &pack->entries[i];
        if (entry->name && pack_stricmp(file_name, entry->name) == 0) {
            *out_size = (int)entry->size;
            *out_offset = entry->sector << 11;
            return 1;
        }
    }
    return 0;
}

int packfile_read_ok(void) {
    return 1;
}

int packfile_read_archive(void* dst, int offset, const char* name) {
    Packfile* pack = packfile_find(name);
    if (!pack) {
printf("A\n");
        return 0;
    }
    if (offset >= (int)pack->archive_size) {
printf("B : (%d %d)\n", offset, pack->archive_size);
        return 0;
    }
    if (pack->is_cd) {
        return 1;
    }
    FILE* file = fopen(pack->name, "rb");
    if (!file) {
printf("C\n");
        return 0;
    }
fread(dst, pack->archive_size, 1, file);
fclose(file);
    return 1;
}

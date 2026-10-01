// Search paths recovered from Sum.exe VA 0x512E60 and VA 0x43DCA0.
// Not byte-matched.

#if defined(_WIN64) || defined(__x86_64__) || defined(__amd64__)
#error Sum.exe is a 32-bit image; search-path pointers are 4 bytes
#endif

#include "vfs.h"

#include <cstring>
#include <cstdlib>

extern char* string_pool_copy(void* pool, const char* text);
extern char* string_pool_keep(void* pool, const char* text);
extern unsigned char g_vfs_string_pool[];
extern unsigned char g_vfs_cache_area[];
extern void vfs_after_root_0(void);
extern void vfs_after_root_1(void);
extern void vfs_after_root_2(void);
extern "C" unsigned int __stdcall GetCurrentDirectoryA(unsigned int length, char* buffer);

char g_hd_root[0x104];
char g_cd_root[0x104];
static char g_vfs_started;
static VfsSearch g_search[VFS_SEARCH_SLOTS];
static char g_search_ready;

static void lower_copy(char* dst, const char* src) {
    while (*src) {
        unsigned char ch = (unsigned char)*src++;
        if (ch >= 'A' && ch <= 'Z') {
            ch = (unsigned char)(ch + 0x20);
        }
        *dst++ = (char)ch;
    }
    *dst = 0;
}

static int same_text(const char* a, const char* b) {
    while (*a && *a == *b) {
        a++;
        b++;
    }
    return *a == *b;
}

int vfs_add_search_path(const char* path, const char* extensions) {
    char path_buf[0x100];
    char ext_buf[0x100];
    lower_copy(path_buf, path ? path : "");
    lower_copy(ext_buf, extensions ? extensions : "");

    int ext_len = (int)strlen(ext_buf);
    if (ext_len != 0) {
        ext_buf[ext_len] = ' ';
        ext_buf[ext_len + 1] = 0;
    }

    for (char* p = path_buf; *p; p++) {
        if (*p == '/') {
            *p = '\\';
        }
    }

    if (path_buf[0] == 0) {
        g_search[0].extensions = string_pool_copy(g_vfs_string_pool, ext_buf);
        g_search_ready = 1;
        return 0;
    }

    int free_slot = -1;
    for (int index = 1; index < VFS_SEARCH_SLOTS; index++) {
        if (!g_search[index].path) {
            if (free_slot < 0) {
                free_slot = index;
            }
            continue;
        }
        if (same_text(g_search[index].path, path_buf)) {
            return index;
        }
    }
    if (free_slot < 0) {
        return -1;
    }

    g_search[free_slot].path = string_pool_keep(g_vfs_string_pool, path_buf);
    g_search[free_slot].extensions = string_pool_keep(g_vfs_string_pool, ext_buf);
    return free_slot;
}

void vfs_clear_search_path(int index) {
    g_search[index].path = 0;
    g_search[index].extensions = 0;
}

static int g_path_root;
static int g_path_data;
static int g_path_multicache;
static int g_path_interface;
static int g_path_maps_characters;
static int g_path_maps_effects;
static int g_path_maps_items;
static int g_path_maps_levels;
static int g_path_models_levels;
static int g_path_models_levels_alias;
static int g_path_scripts;
static int g_path_characterinfo;
static int g_path_internal;
static int g_path_internal_alias;
static int g_path_players;
static int g_path_savegame;
static int g_path_tables;
static int g_path_tables_levels;
static int g_path_movies;

int vfs_path_models_levels(void) {
    return g_path_models_levels;
}

int vfs_path_root(void) {
    return g_path_root;
}

const char* vfs_search_extensions(int index) {
    if (index < 0 || index >= VFS_SEARCH_SLOTS) {
        return 0;
    }
    return g_search[index].extensions;
}

void vfs_init_search_paths(void) {
    g_path_root = vfs_add_search_path("", ".cfg .vpp .vcs .brw .arr .ico .sys .pre");
    g_path_data = vfs_add_search_path("data", ".cfg");
    vfs_add_search_path("data\\ca", ".mvf");
    vfs_add_search_path("data\\fonts", ".vf .tga .vbm");
    vfs_add_search_path("data\\pack\\svfs", ".svf");
    g_path_multicache = vfs_add_search_path("multicache", ".cfg .pcx");
    g_path_interface = vfs_add_search_path("data\\interface\\pc", ".vbm .tga .m2v .vaf ");
    vfs_add_search_path("data\\sounds\\pc", ".wav");
    vfs_add_search_path("data\\sounds\\pc\\cutscenes", ".wav");
    vfs_add_search_path("data\\sounds\\pc\\music", ".wav");
    g_path_maps_characters = vfs_add_search_path("data\\maps\\characters", ".vbm .tga .m2v");
    g_path_maps_effects = vfs_add_search_path("data\\maps\\effects", ".vbm .tga .m2v");
    g_path_maps_items = vfs_add_search_path("data\\maps\\items", ".vbm .tga .m2v");
    g_path_maps_levels = vfs_add_search_path("data\\maps\\levels", ".vbm .tga .m2v");
    vfs_add_search_path("data\\maps\\cutscenes", ".vbm .tga .m2v");
    vfs_add_search_path("data\\tables\\ui\\worldmap", ".ui");
    vfs_add_search_path("data\\effects\\characters", ".tga .m2v .vfx .vbm");
    vfs_add_search_path("data\\effects\\cutscenes", ".tga .m2v .vfx .vbm");
    vfs_add_search_path("data\\effects\\items", ".tga .m2v .vfx .vbm");
    vfs_add_search_path("data\\effects\\levels", ".tga .m2v .vfx .vbm");
    vfs_add_search_path("data\\effects\\spells", ".tga .m2v .vfx .vbm");
    vfs_add_search_path("data\\effects", ".vfx .rfx");
    vfs_add_search_path("data\\tables\\boss", ".tbl");
    g_path_tables = vfs_add_search_path("data\\tables", ".tbl");
    vfs_add_search_path("data\\v3d", ".v3d .s3d .vfx");
    g_path_models_levels = vfs_add_search_path("data\\models\\levels", ".v3d .s3d .vfx .vlm");
    g_path_models_levels_alias = g_path_models_levels;
    vfs_add_search_path("data\\models\\characters", ".v3d .vfx .vim");
    vfs_add_search_path("data\\models\\items", ".v3d .vfx .vim");
    g_path_scripts = vfs_add_search_path("data\\scripts", ".scr .tbl");
    g_path_characterinfo = vfs_add_search_path("data\\tables\\characterinfo", ".tbl");
    g_path_internal = vfs_add_search_path("data\\internal", ".pfg .mlo .vis .vex .bsp .lkf .eax");
    g_path_internal_alias = g_path_internal;
    g_path_players = vfs_add_search_path("players", ".plr");
    g_path_savegame = vfs_add_search_path("savegame", ".sav");
    g_path_tables_levels = vfs_add_search_path("data\\tables\\levels", ".tbl");
    g_path_movies = vfs_add_search_path("data\\movies", "*.bik");
    (void)g_path_data;
    (void)g_path_multicache;
    (void)g_path_interface;
    (void)g_path_maps_characters;
    (void)g_path_maps_effects;
    (void)g_path_maps_items;
    (void)g_path_maps_levels;
    (void)g_path_models_levels_alias;
    (void)g_path_scripts;
    (void)g_path_characterinfo;
    (void)g_path_internal_alias;
    (void)g_path_players;
    (void)g_path_savegame;
    (void)g_path_tables;
    (void)g_path_tables_levels;
    (void)g_path_movies;
}

static void append_text(char* dst, const char* src) {
strcat(dst, src);
}

char* vfs_resolve_path(int path_id, char* dst) {
strcpy(dst, g_hd_root);
    if (path_id) {
        append_text(dst, g_search[path_id].path);
        return dst;
    }
    int length = (int)strlen(dst);
    if (length > 0) {
        dst[length - 1] = 0;
    }
    return dst;
}

char* vfs_copy_search_dir(int path_id, char* dst) {
    dst[0] = 0;
    if (path_id) {
strcpy(dst, g_search[path_id].path);
    }
    return dst;
}

int vfs_path_id_ok(int path_id) {
    if (path_id == 0x98967F) {
        return 1;
    }
    if (path_id < 0 || path_id >= VFS_SEARCH_SLOTS) {
        return 0;
    }
    if (path_id > 0 && !g_search[path_id].path) {
        return 0;
    }
    return 1;
}

static char* vfs_make_path(const char* root, int path_id, const char* filename, char* dst) {
    if (!vfs_path_id_ok(path_id)) {
        return 0;
    }
    if (strlen(root) >= 0x100) {
        return 0;
    }
strcpy(dst, root);
    if (path_id) {
        append_text(dst, g_search[path_id].path);
        append_text(dst, "\\");
    }
    if (filename) {
        append_text(dst, filename);
    }
    return dst;
}

char* vfs_make_hd_path(int path_id, const char* filename, char* dst) {
    return vfs_make_path(g_hd_root, path_id, filename, dst);
}

char* vfs_make_cd_path(int path_id, const char* filename, char* dst) {
    return vfs_make_path(g_cd_root, path_id, filename, dst);
}

void vfs_shutdown(void) {
    int i;
    for (i = 0; i < 4; i++) {
        unsigned char* flag = g_vfs_cache_area + i * 0x8040;
        if (*(int*)flag) {
            vfs_file_close(*(VfsFile**)(flag + 0x8030));
        }
    }
    for (i = 0; i < VFS_SEARCH_SLOTS; i++) {
        g_search[i].path = 0;
        g_search[i].extensions = 0;
    }
    packfile_shutdown();
    g_hd_root[0] = 0;
    g_cd_root[0] = 0;
    g_vfs_started = 0;
}

void vfs_set_root(const char* path, int mount_packs) {
    g_hd_root[0] = 0;
    if (path) {
strcpy(g_hd_root, path);
        int length = (int)strlen(g_hd_root);
        if (length == 0 || g_hd_root[length - 1] != '\\') {
            g_hd_root[length] = '\\';
            g_hd_root[length + 1] = 0;
        }
    } else {
        char cwd[0x100];
        GetCurrentDirectoryA(0xFF, cwd);
strcpy(g_hd_root, cwd);
        char* slash = strrchr(g_hd_root, '\\');
        if (slash) {
            slash[1] = 0;
        }
    }
    vfs_after_root_0();
    vfs_after_root_1();
    vfs_after_root_2();
    g_vfs_started = 1;
    if (mount_packs) {
        packfile_init();
    }
atexit(vfs_shutdown);
}

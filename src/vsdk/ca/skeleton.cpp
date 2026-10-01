// Original: D:\projects\Summoner\pccode\vsdk\ca\skeleton.cpp
// Same SDK file as Red Faction vsdk/ca/skeleton.cpp.
// Builds a skeleton, frees one, and samples one bone.

#include <cstring>

struct Vec3 {
    float x;
    float y;
    float z;

    Vec3* fn_00505530();
    Vec3* fn_0050F790(Vec3* out, float scale);
    Vec3* fn_00410CD0(Vec3* src);
};

struct StringPool {
    char* fn_00544CD0(const char* text);
};

struct BoneRec {
    short count;
    short unused;
    void* data;
    int unused_8;
};

struct Anim {
    void* blocks;
    void* blocks_b;
    char name[0x40];
    int count_a;
    int count_b;
    Anim* prev;
    Anim* next;
    char* pooled;
    char active;
    char flag_5d;
    char pad_5e[2];
    int field_60;
    int field_64;
    Vec3 direction;
    Vec3 other;
    int unused_80;
    int count_c;
    void* ptr_c;
    void* ptr_d;
    int field_90;
    int count_e;
    void* ptr_e;
    BoneRec* records;

    Anim* fn_00543D50(const char* name, const char* key, char active, int unused, char flag);
    void fn_00543E10();
    Vec3* fn_00543FC0(Vec3* out);
};

extern "C" int g_skeleton_count;
extern "C" int g_skeleton_max;
extern "C" int g_skeleton_bytes;
extern "C" StringPool g_string_pool;
extern "C" void vfs_heap_free(void* block);
extern "C" void parse_fail(const char* file, int line, const char* msg);

Anim* Anim::fn_00543D50(const char* name, const char* key, char active, int unused, char flag) {
    (void)unused;
    direction.fn_00505530();
    other.fn_00505530();
    int count = ++g_skeleton_count;
    if (g_skeleton_max >= 0 && count > g_skeleton_max) {
        for (;;) {
            parse_fail(
                "D:\\projects\\Summoner\\pccode\\vsdk\\ca\\skeleton.cpp",
                0x96,
                "Too many skeletons");
        }
    }
    flag_5d = flag;
    count_a = 0;
    count_b = 0;
    blocks = 0;
    blocks_b = 0;
    this->active = active;
    pooled = g_string_pool.fn_00544CD0(key);
    int len = (int)strlen(name);
    field_90 = 0;
    count_e = 0;
    ptr_e = 0;
    records = 0;
    memcpy(this->name, name, len + 1);
    return this;
}

void Anim::fn_00543E10() {
    Anim* link_next = next;
    Anim* link_prev = prev;
    g_skeleton_count--;
    g_skeleton_bytes -= 0xD0;
    link_next->prev = link_prev;
    link_prev->next = link_next;
    prev = 0;
    next = 0;
    if (ptr_c != 0) {
        vfs_heap_free(ptr_c);
        g_skeleton_bytes -= count_c;
    }
    if (ptr_d != 0) {
        vfs_heap_free(ptr_d);
        g_skeleton_bytes -= count_c * 44;
    }
    if (blocks != 0) {
        vfs_heap_free(blocks);
        g_skeleton_bytes -= count_a * 20;
    }
    if (blocks_b != 0) {
        vfs_heap_free(blocks_b);
        g_skeleton_bytes -= count_b * 40;
    }
    if (ptr_e != 0) {
        vfs_heap_free(ptr_e);
        ptr_e = 0;
        g_skeleton_bytes -= count_e * 4;
    }
    if (records != 0) {
        int count = count_e;
        for (int i = 0; i < count; i++) {
            vfs_heap_free(records[i].data);
            g_skeleton_bytes -= records[i].count * 16;
        }
        vfs_heap_free(records);
        records = 0;
        name[0] = 0;
        g_skeleton_bytes -= count_e * 12;
        return;
    }
    name[0] = 0;
}

Vec3* Anim::fn_00543FC0(Vec3* out) {
    float span = (float)(field_64 - field_60);
    float scale = 1.0f / (span * 0.00625f * 0.033333335f);
    Vec3 local;
    direction.fn_0050F790(&local, scale);
    return out->fn_00410CD0(&local);
}

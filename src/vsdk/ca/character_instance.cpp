// Original: D:\projects\Summoner\pccode\vsdk\ca\character_instance.cpp
// Same SDK file as Red Faction vsdk/ca/character_instance.cpp.
// Sums the active animation samples on a character instance.

#include <cstdio>

struct Vec3 {
    float x;
    float y;
    float z;

    Vec3* fn_0043B580(float x, float y, float z);
    void fn_00413190(Vec3* dst, Vec3* src);
    Vec3* fn_00410CD0(Vec3* src);
    void fn_0048E440(Vec3* src);
    Vec3* fn_0050F790(Vec3* out, float scale);
};

struct Anim {
    char pad[0x5c];
    char active;
    char pad_5d[7];
    int field_64;

    Vec3* fn_00543FC0(Vec3* out);
};

struct CharacterDef {
    char pad[0x48];
    int anim_count;
    char pad_4c[8];
    Anim* anims[1];
};

struct CharacterInstance {
    char pad[0x3c];
    CharacterDef* def;
    int* samples;
    float* weights;
    char pad_48[0xc];
    int index;
    char pad_58[0xc];
    Vec3 sum;

    Vec3* fn_0051ABD0(Vec3* out);
};

extern "C" char g_anim_msg[];
extern "C" void parse_fail(const char* file, int line, const char* msg);

Vec3* CharacterInstance::fn_0051ABD0(Vec3* out) {
    Vec3 tmp;
    Vec3 scaled;
    tmp.fn_0043B580(0.0f, 0.0f, 0.0f);
    sum.fn_00413190(&scaled, &tmp);
    int count = def->anim_count;
    for (int i = 0; i < count; i++) {
        Anim* anim = def->anims[i];
        if (anim == 0) {
            for (;;) {
                sprintf(g_anim_msg, "Character has NULL anim: %s (%d)", def, i);
                parse_fail(
                    "D:\\projects\\Summoner\\pccode\\vsdk\\ca\\character_instance.cpp",
                    0x59B,
                    g_anim_msg);
            }
        }
        if (anim->active == 0) {
            continue;
        }
        anim->fn_00543FC0(&tmp);
        if (i == index && samples[i] >= anim->field_64) {
            continue;
        }
        tmp.fn_0050F790(&scaled, weights[i]);
        sum.fn_0048E440(&scaled);
    }
    out->fn_00410CD0(&sum);
    return out;
}

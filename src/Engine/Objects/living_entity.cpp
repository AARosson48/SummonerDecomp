// Original: D:\projects\Summoner\pccode\Engine\Objects\living_entity.cpp
// This file keeps a frame pointer. The large bodies are the same shape.

struct Entity {
    char pad_0[0x34];
    int field_34;
    char pad_38[0x84 - 0x38];
    int field_84;
    char pad_88[0x314 - 0x88];
    void* anim;
    char pad_318[0x320 - 0x318];
    int flags_320;
    char pad_324[0x678 - 0x324];
    short foot_l;
    short foot_r;

    void fn_0045D4C0();
    bool fn_00455950(int which);
    int fn_00461B60();
    float fn_00462440(int which);
    float fn_004626B0(int which);
    int fn_00462840(int which);
    void fn_00454070();
    void fn_004553A0(int a, int b, int c);
    void fn_0044DDB0();
    void fn_00450850(float value, int source, int a, int b);
    void fn_00454700(int slot, int value, int kind, float scale, int field_80, int flag);
    void fn_0045DEA0(int kind, int source, int arg3, int arg4, int damage_type, int item, int arg7, int arg8);
    float fn_00456F80();
    Entity* fn_004570D0(int value);
    Entity* fn_004570F0(int* value);
    Entity* fn_00457120(void* value);
    Entity* fn_0045DE20(int mode);
    void* fn_00461370(int kind, int value);
    void* fn_004613B0(int kind, int value);
    float fn_004613F0(int id);
    int fn_00461450(int id, float* out);
    int fn_004614D0(int id);
    void* fn_00461500();
    void* fn_00461540();
    int fn_00461620(int id);
    float fn_00461670(int id);
    int fn_00461740(int id);
    int fn_004617F0();
    int fn_00461AD0(Entity* other);
    void* fn_00460F20(void* clip, float scale, int a, int b, int c);
    void* fn_004569C0(float* point, char relative);
    int fn_00457150(void* out);
    void* fn_00458800(void* in);
    void* fn_0045A120();
    void fn_0045A6E0(float value);
    float fn_0045A840(float* out);
    void* fn_0045AB00(float* delta);
    int fn_0045B6E0();
    float* fn_0045BF70();
    int fn_0045C340(int arg, float* arg3);
    int* fn_0045CD00();
    float fn_0045D540(int* pos, char side);
    float fn_0045D690();
    int fn_0045D830(int* out);
    int fn_0045D980(int kind);
    int fn_0045DA60(int kind, char flag);
    int* fn_00460190(int* args);
    float fn_004542B0();
    float fn_004542F0();
    int fn_004568E0();
    float fn_00456ED0(float value);
    int fn_00456FB0(float* out);
    float fn_0045A930(float value);
    void fn_004616F0(int id);
    int fn_00461840();
};

struct SubHit {
    void fn_00468160(int value);
    void fn_004B5790(int source);
};

struct BoneSet {
    int fn_00540F70(const char* name);
};

#pragma optimize("", off)

extern "C" void fn_0045CCB0(Entity* ent) {
    if ((ent->flags_320 & 0x40000) || ent->field_34 != 0) {
        ent->field_84 &= ~1;
    } else {
        ent->field_84 |= 4;
    }
}

extern "C" Entity* fn_0046B1A0(int id);
extern "C" unsigned char fn_004A28B0(Entity* ent);
extern "C" void fn_004A13B0(Entity* ent, int a, int b, int c);
extern "C" int fn_0048C080(int a, int b, int c, int d, int e, int f);
extern "C" float fn_00524810(void* a, void* b);
extern "C" int fn_0047A8C0(int kind);
extern "C" int fn_0047A890(int kind);
extern "C" int fn_0047AA80(int kind, int amount);
extern "C" float fn_0047F4D0(Entity* ent, int kind, int hit, int delta, float scale);
extern "C" float fn_005426E0();
extern "C" int fn_0048BD80(int a, int b, int c, int d);
extern "C" int fn_0046C240(Entity* ent, int index, int flag);
extern "C" void fn_0044B2B0(int value);
extern "C" void fn_00439580(int value);
extern "C" int printf(const char* text, ...);
extern "C" double ceil(double value);
extern "C" void fn_00524430(float* vec);
extern "C" void fn_00511080(int* out, float* vec);
extern "C" void* fn_00489F00(int id);
extern "C" float fn_0047A9C0(int slot, int amount);
extern "C" float fn_0047F2A0(Entity* ent, int kind);
extern "C" void fn_00450C70(Entity* ent, float value);
extern "C" void fn_0047E4D0(int a, int b, int c, int d, void* pos);
extern "C" void fn_00474350(void* node, int value);
extern "C" int fn_004B9E50();
extern "C" void fn_004743E0(void* node);
extern "C" int fn_004A3410(int a, int b);

static const unsigned char fn_0045DEA0_group[0x6c] = {
    0, 1, 2, 3, 0, 3, 2, 2, 2, 30, 30, 4, 5, 6, 7, 8, 2, 9, 30, 2, 30, 2, 2, 2, 10, 11, 11, 30,
    12, 30, 12, 30, 11, 13, 2, 30, 2, 2, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
    30, 30, 30, 14, 15, 16, 17, 2, 2, 18, 19, 20, 20, 20, 2, 21, 30, 30, 30, 30, 22, 30, 30, 23, 11,
    24, 12, 2, 30, 30, 30, 5, 2, 2, 5, 2, 2, 2, 11, 30, 30, 30, 30, 30, 30, 30, 30, 12, 5, 25, 26, 27,
    3, 28, 29
};

extern char g_rand_gate;

extern int g_player_list;
extern char g_spell_rows[];

struct SpellItem {
    char pad[8];
    int value;
};

void Entity::fn_0045DEA0(int kind, int source, int arg3, int arg4, int damage_type, int item, int arg7, int arg8) {
    char* self = (char*)this;
    int amount;
    float scale;
    float roll;
    float factor;
    Entity* other;
    int* node;
    Entity* found;
    int mode = 0;
    (void)arg8;
    (void)mode;

    if (((*(int*)(self + 0x320) & 0x80) || *(float*)(self + 0x5f8) <= 0.0f) &&
        kind != 5 && kind != 3 && kind != 0x69) {
        return;
    }

    for (node = *(int**)&g_player_list; node != (int*)&g_player_list; node = *(int**)node) {
        found = fn_0046B1A0(node[3]);
        if (found != 0 && (found->flags_320 & 0x80) == 0) {
            break;
        }
    }
    if (node == &g_player_list) {
        return;
    }
    if (fn_004A28B0(this) != 0) {
        return;
    }
    if (*(int*)(self + 0xc) == 9) {
        fn_004A13B0(this, 0, source, 0);
    }

    if (kind == 0x48) {
        fn_0045DEA0(7, source, arg3, arg4, damage_type, item, arg7, 0);
        fn_0045DEA0(8, source, arg3, arg4, damage_type, item, arg7, 0);
        fn_0045DEA0(0x43, source, arg3, arg4, damage_type, item, arg7, 0);
        if (fn_00455950(0) != 0) {
            fn_0048C080(*(int*)0x22E52C4, *(int*)(self + 0x80), -1, -1, 0, 1);
            return;
        }
        if (fn_00455950(1) != 0) {
            fn_0048C080(*(int*)0x22E52C4, *(int*)(self + 0x80), -1, -1, 0, 1);
            return;
        }
        if (fn_00455950(9) != 0) {
            fn_0048C080(*(int*)0x22E52C4, *(int*)(self + 0x80), -1, -1, 0, 1);
            return;
        }
        return;
    }

    char apply = 1;
    char mark = 0;
    char use_roll = 1;
    int hit = 0;
    int delta = 0;
    int slot;
    float result = 0.0f;

    other = fn_0046B1A0(source);
    if (other == 0) {
        return;
    }
    if ((*(int*)(g_spell_rows + (kind << 7) + 0x9c) & 1) != 0 &&
        *(int*)(self + 0xc) == 7 &&
        other->fn_00461B60() != 0) {
        fn_00439580(1);
    }
    if (*(float*)(g_spell_rows + (kind << 7) + 0x98) > 0.0f) {
        float dist = fn_00524810(self + 0x10, (char*)other + 0x10);
        if (dist > *(float*)(g_spell_rows + (kind << 7) + 0x98)) {
            return;
        }
    }

    amount = 0;
    if (damage_type == 0 || damage_type == 8) {
        amount = (int)*(short*)((char*)other + 0x600);
    } else if (damage_type == 6) {
        float divisor = 2.0f;
        if (kind == 0) {
            divisor = 3.0f;
        } else if (kind == 4) {
            divisor = 4.0f;
        }
        amount = (int)((float)(int)*(short*)((char*)other + 0x600) / divisor);
        amount = (int)((float)amount * (1.0f + (float)*(int*)((char*)other + 0x578) * 0.15f));
    } else if (damage_type == 7) {
        amount = 100;
    } else if (damage_type == 1) {
        amount = (int)*(short*)(*(char**)((char*)other + 0x71c) + 0x3e);
        if (amount <= 0) {
            amount = (int)*(short*)((char*)other + 0x600);
        }
    } else if (damage_type == 2) {
        amount = (int)*(short*)(*(char**)((char*)other + 0x71c) + 0x46);
        if (amount == 0) {
            amount = (int)*(short*)((char*)other + 0x600);
        }
    } else if (damage_type == 3) {
        SpellItem* special;
        printf("Item special\n");
        if (item == 0 || *(int*)(item + 0x94) == 0) {
            printf("Item special failure 2!\n");
            return;
        }
        printf("Item special item found!\n");
        special = *(SpellItem**)(item + 0x94);
        amount = special->value;
        if (amount == 0) {
            printf("Item special failure 1!\n");
            amount = (int)*(short*)((char*)other + 0x600);
        }
    } else if (damage_type == 4) {
        SpellItem* reflex;
        if (item == 0 || *(int*)(item + 0xf8) == 0) {
            return;
        }
        reflex = *(SpellItem**)(item + 0xf8);
        amount = reflex->value;
        if (amount == 0) {
            amount = (int)*(short*)((char*)other + 0x600);
        }
        printf("Reflex abil\n");
    } else if (damage_type == 5) {
        if (item == 0 || *(int*)(item + 0x58) == 0) {
            return;
        }
        amount = *(int*)(*(char**)(item + 0x58) + 0xc);
        if (amount <= 0) {
            amount = (int)*(short*)((char*)other + 0x600);
        }
    } else {
        return;
    }

    slot = fn_0047A8C0(kind);
    if (*(int*)(*(char**)((char*)other + 0x71c) + 0x10) == 0xb6) {
        scale = (float)((int)*(short*)((char*)other + 0x600) * 2) / 100.0f;
    } else if (kind == 0x50 && *(int*)0x2480310 == 3) {
        scale = 0.5f;
    } else if (damage_type == 8) {
        scale = 0.30000001192092896f;
    } else {
        scale = *(float*)(g_spell_rows + (kind << 7) + 0x84);
    }
    roll = fn_004626B0(3);
    factor = (0.5f + roll) - (float)amount * 0.03999999910593033f;
    factor = 1.0f - (1.0f - factor) * scale;
    hit = 0;
    delta = 0;

    switch (kind <= 0x6b ? fn_0045DEA0_group[kind] : 30) {
    case 0:
        hit = amount;
        delta = *(int*)(g_spell_rows + (kind << 7) + 0x88);
        break;
    case 8:
        mark = 1;
        hit = amount;
        if (damage_type == 0) {
            float scaled = (float)hit;
            hit = (int)(scaled * (other->fn_00462440(0x1e) + 1.0f));
        }
        delta = -*(int*)(g_spell_rows + (kind << 7) + 0x88);
        mode = 0x82;
        break;
    case 1:
        if (amount >= 100) {
            fn_004553A0(0, 1, 0);
            fn_004553A0(1, 1, 0);
            fn_004553A0(3, 1, 0);
            fn_004553A0(9, 1, 0);
            fn_004553A0(0xA, 1, 0);
            fn_004553A0(0xB, 1, 0);
            fn_004553A0(0xC, 1, 0);
            fn_004553A0(0xD, 1, 0);
            fn_004553A0(0x10, 1, 0);
        } else {
            fn_004553A0(0, 0, 0);
            fn_004553A0(1, 0, 0);
            fn_004553A0(3, 0, 0);
            fn_004553A0(9, 0, 0);
            fn_004553A0(0xA, 0, 0);
            fn_004553A0(0xB, 0, 0);
            fn_004553A0(0xC, 0, 0);
            fn_004553A0(0xD, 0, 0);
            fn_004553A0(0x10, 0, 0);
        }
        break;
    case 3:
        if (kind == 0x69 && *(int*)(self + 0xc) != 7) {
            result = (float)(int)*(short*)((char*)other + 0x600) * -7.0f;
            hit = (int)*(short*)((char*)other + 0x600) * 7;
            delta = 0;
            mode = 0x82;
            break;
        }
        apply = 0;
        fn_00454070();
        if (kind == 5) {
            *(float*)(self + 0x5f8) = (float)fn_00462840(0);
        } else if (kind == 0x69) {
            float cap = (float)((int)*(short*)((char*)other + 0x600) * 2);
            float current = *(float*)(self + 0x5f8);
            if (cap > current) {
                current = cap;
            }
            cap = (float)fn_00462840(0);
            if (current < cap) {
                current = cap;
            }
            *(float*)(self + 0x5f8) = current;
        } else {
            *(int*)(self + 0x5f8) = 0x3f800000;
        }
        break;
    case 0x1b:
        mark = 1;
        apply = 0;
        result = (float)(int)*(short*)((char*)other + 0x600) * -20.0f;
        hit = (int)*(short*)((char*)other + 0x600) * 0x14;
        delta = 0;
        mode = 0x82;
        fn_0048BD80(*(int*)(g_spell_rows + (kind << 7) + 0x44), arg3, arg4, 1);
        break;
    case 0x1c:
        fn_0045DEA0(0x17, source, arg3, arg4, damage_type, item, arg7, 0);
        fn_0045DEA0(7, source, arg3, arg4, damage_type, item, arg7, 0);
        fn_0045DEA0(8, source, arg3, arg4, damage_type, item, arg7, 0);
        fn_0045DEA0(0x3C, source, arg3, arg4, damage_type, item, arg7, 0);
        result = (float)(int)*(short*)((char*)other + 0x600) * -10.0f;
        hit = (int)*(short*)((char*)other + 0x600) * 0xA;
        delta = 0;
        mode = 0x82;
        apply = 0;
        if (fn_00455950(3) || fn_00455950(0) || fn_00455950(1) || fn_00455950(0xB)) {
            apply = 1;
        }
        break;
    case 0x1d:
        if (*(int*)(self + 0xc) == 7) {
            fn_0045DEA0(0x16, source, arg3, arg4, damage_type, item, arg7, 0);
            fn_0045DEA0(0x13, source, arg3, arg4, damage_type, item, arg7, 0);
            fn_0045DEA0(2, source, arg3, arg4, damage_type, item, arg7, 0);
            fn_0045DEA0(0x10, source, arg3, arg4, damage_type, item, arg7, 0);
            apply = 0;
            if (fn_00455950(2) || fn_00455950(4) || fn_00455950(5) || fn_00455950(6)) {
                apply = 1;
            }
        } else {
            result = (float)(int)*(short*)((char*)other + 0x600) * -8.0f;
            hit = (int)*(short*)((char*)other + 0x600) << 3;
            delta = 0;
            apply = 0;
            mode = 0x82;
        }
        break;
    case 0x19:
        mark = 1;
        apply = 0;
        if (g_rand_gate == 0 && fn_005426E0() < factor) {
            break;
        }
        if (other != 0) {
            float dist = fn_00524810((char*)other + 0x10, self + 0x10);
            if (dist < 10.0f) {
                result = (float)(-amount);
            }
        }
        hit = amount;
        delta = -*(int*)(g_spell_rows + (kind << 7) + 0x88);
        break;
    case 5:
        mark = 1;
        apply = 0;
        if (kind == 0x65) {
            apply = 1;
        }
        if (*(int*)(self + 0xc) == 9 || (*(int*)(self + 0x320) & 0x40) != 0) {
            break;
        }
        if (g_rand_gate == 0 && fn_005426E0() < factor) {
            if (kind == 0x65) {
                fn_0045DEA0(0x67, source, 0, 0, 0, 0, 0, 0);
            }
            break;
        }
        if (kind == 0x54) {
            apply = 1;
        }
        result = -*(float*)(self + 0x5f8);
        hit = (int)ceil((double)*(float*)(self + 0x5f8));
        delta = 0;
        use_roll = 0;
        if (*(int*)(self + 0xc) != 7) {
            int index;
            for (index = 0; index < 9; index++) {
                if (*(int*)(self + index * 4 + 0x754) != 0) {
                    fn_0044B2B0(fn_0046C240(this, index, 1));
                }
            }
        }
        mode = 0x82;
        break;
    case 6:
        if (g_rand_gate == 0 && fn_005426E0() < factor) {
            break;
        }
        if (*(short*)(self + 0x600) > 1) {
            *(short*)(self + 0x600) = (short)(*(short*)(self + 0x600) - 1);
            fn_0044DDB0();
        }
        break;
    case 2: {
        int take_hit;
        apply = 0;
        if (kind == 0x17 && (*(int*)((char*)other + 0x320) & 0x800) == 0) {
            float vec[3];
            int orient[3];
            int effect;
            void* spawned;
            vec[0] = *(float*)((char*)other + 0x10) - *(float*)(self + 0x10);
            vec[1] = *(float*)((char*)other + 0x14) - *(float*)(self + 0x14);
            vec[2] = *(float*)((char*)other + 0x18) - *(float*)(self + 0x18);
            fn_00524430(vec);
            fn_00511080(orient, vec);
            if (arg8 == 0) {
                printf("Preloaded curse\n");
                effect = fn_0048C080(fn_0047A890(kind), *(int*)(self + 0x80), -1, -1, 0, 1);
            } else {
                effect = fn_0048C080(*(int*)0x22E5AC4, *(int*)(self + 0x80), -1, -1, 0, 1);
            }
            spawned = fn_00489F00(effect);
            if (spawned != 0) {
                ((int*)spawned)[14] = orient[0];
                ((int*)spawned)[15] = orient[1];
                ((int*)spawned)[16] = orient[2];
            }
        }
        take_hit = g_rand_gate != 0 || (*(int*)(g_spell_rows + (kind << 7) + 0x9c) & 1) == 0;
        if (take_hit == 0) {
            take_hit = fn_005426E0() > factor;
        }
        if (take_hit != 0) {
            int duration;
            float power;
            int type = *(int*)(*(char**)((char*)other + 0x71c) + 0x10);
            if (type == 0xc3) {
                duration = (int)((float)(int)*(short*)((char*)other + 0x600) * 3.0f * 1000.0f);
            } else if (type != 0xb6) {
                duration = fn_0047AA80(kind, amount);
            } else {
                duration = (int)((float)(int)*(short*)((char*)other + 0x600) * 4.0f * 1000.0f);
            }
            power = fn_0047A9C0(slot, amount);
            if (!(fn_0047F2A0(this, kind) <= 0.00999999978f)) {
                if (damage_type == 0 && *(int*)((char*)other + 0x3a8) != kind) {
                    fn_00454700(slot, duration, -1, power, *(int*)((char*)other + 0x80), damage_type == 7);
                } else if (arg8 == 0) {
                    fn_00454700(slot, duration, -1, power, *(int*)((char*)other + 0x80), damage_type == 7);
                } else {
                    fn_00454700(slot, duration, kind, power, *(int*)((char*)other + 0x80), damage_type == 7);
                }
                if ((*(int*)(g_spell_rows + (kind << 7) + 0x9c) & 1) != 0 && kind != 0x24) {
                    mode = 0x81;
                }
            }
        } else if (*(int*)(g_spell_rows + (kind << 7) + 0x44) != 0 && kind != 0x17 && kind != 0x24 &&
                   (*(int*)((char*)other + 0x320) & 0x800) == 0) {
            if (arg8 == 0) {
                fn_0048C080(fn_0047A890(kind), *(int*)(self + 0x80), -1, -1, 0, 1);
            } else {
                fn_0048C080(*(int*)(g_spell_rows + (kind << 7) + 0x44), *(int*)(self + 0x80), -1, -1, 0, 1);
            }
        }
        break;
    }
    case 4: {
        int duration = fn_0047AA80(0xb, amount);
        float power = 0.0f;
        apply = 0;
        if ((*(unsigned char*)0x5FBFD8 & 1) != 0 && amount >= 0x32) {
            power = 1.0f;
        }
        if (arg8 == 0) {
            fn_00454700(slot, duration, -1, power, *(int*)((char*)other + 0x80), 0);
        } else {
            fn_00454700(slot, duration, kind, power, *(int*)((char*)other + 0x80), 0);
        }
        break;
    }
    case 7: {
        int duration = fn_0047AA80(0xe, amount);
        apply = 0;
        if (arg8 == 0) {
            fn_00454700(slot, duration, -1, 0.0f, *(int*)((char*)other + 0x80), 0);
        } else {
            fn_00454700(slot, duration, kind, 0.0f, *(int*)((char*)other + 0x80), 0);
        }
        break;
    }
    case 9: {
        int passed = g_rand_gate != 0;
        apply = 0;
        mark = 1;
        if (passed == 0) {
            passed = !(fn_005426E0() < factor);
        }
        if (passed != 0) {
            int drained;
            float give;
            if (!(fn_005426E0() < 0.0500000007f)) {
                drained = (int)(*(float*)(self + 0x5fe) * (float)amount * 0.00999999978f + 0.5f);
            } else {
                drained = *(int*)(self + 0x5fe);
            }
            if (drained < 0) {
                drained = 0;
            }
            fn_00450C70(this, (float)(-drained));
            give = (float)drained;
            if (!(give < 20.0f)) {
                give = 20.0f;
            }
            fn_00450C70(other, give);
            fn_0045DE20(0x82);
        }
        break;
    }
    case 0xa:
        if (*(int*)(self + 0xc) == 9 || (*(int*)(self + 0x320) & 0x40) != 0) {
            mark = 1;
            apply = 0;
            hit = amount;
            delta = -*(int*)(g_spell_rows + (kind << 7) + 0x88);
            ((SubHit*)(self + 0x558))->fn_00468160(2);
            if (arg8 == 0) {
                printf("Trying to get_hit_by_spell by an invalid method!!\n");
                fn_0048BD80(fn_0047A890(kind), arg3, arg4, 1);
            } else {
                int spell = fn_0048BD80(*(int*)(g_spell_rows + (kind << 7) + 0x44), arg3, arg4, 1);
                if (kind == 0x1a && spell != -1) {
                    fn_0047E4D0(*(int*)((char*)other + 0x80), spell, arg3, *(int*)(self + 0x68c), self + 0x38);
                }
            }
        } else {
            float half;
            mark = 1;
            apply = 0;
            use_roll = 0;
            ((SubHit*)(self + 0x558))->fn_00468160(2);
            if (arg8 == 0) {
                fn_0048BD80(fn_0047A890(kind), arg3, arg4, 1);
            } else {
                fn_0048BD80(*(int*)(g_spell_rows + (kind << 7) + 0x44), arg3, arg4, 1);
            }
            half = *(float*)(self + 0x5f8) * 0.5f;
            printf("jade dmg: %d\n", (int)half);
            fn_00450850(-(half), source, 1, 0);
        }
        break;
    case 0xb:
        mark = 1;
        apply = 0;
        hit = amount;
        if (damage_type == 0) {
            if (kind == 0x19) {
                hit = (int)((float)hit * (other->fn_00462440(0x20) + 1.0f));
            } else if (kind == 0x20) {
                hit = (int)((float)hit * (other->fn_00462440(0x21) + 1.0f));
            }
        }
        delta = -*(int*)(g_spell_rows + (kind << 7) + 0x88);
        ((SubHit*)(self + 0x558))->fn_00468160(2);
        if (arg8 == 0) {
            printf("Trying to get_hit_by_spell by an invalid method!!\n");
            fn_0048BD80(fn_0047A890(kind), arg3, arg4, 1);
        } else {
            int spell = fn_0048BD80(*(int*)(g_spell_rows + (kind << 7) + 0x44), arg3, arg4, 1);
            if (kind == 0x1a && spell != -1) {
                fn_0047E4D0(*(int*)((char*)other + 0x80), spell, arg3, *(int*)(self + 0x68c), self + 0x38);
            }
        }
        break;
    case 0xc:
        hit = amount;
        if (*(int*)(*(char**)((char*)other + 0x71c) + 0x10) != 0x58) {
            delta = -*(int*)(g_spell_rows + (kind << 7) + 0x88);
        } else {
            delta = -10;
        }
        mode = 0x82;
        apply = 0;
        mark = 1;
        break;
    case 0xd:
        delta = -*(int*)(g_spell_rows + (kind << 7) + 0x88);
        hit = amount;
        break;
    case 0xe:
        mark = 1;
        apply = 0;
        hit = 3;
        delta = -*(int*)(g_spell_rows + (kind << 7) + 0x88);
        ((SubHit*)(self + 0x558))->fn_00468160(2);
        if (arg8 == 0) {
            fn_0048BD80(fn_0047A890(kind), arg3, arg4, 1);
        } else {
            int spell = fn_0048BD80(*(int*)(g_spell_rows + (kind << 7) + 0x44), arg3, arg4, 1);
            if (spell != -1) {
                fn_0047E4D0(*(int*)((char*)other + 0x80), spell, arg3, *(int*)(self + 0x68c), self + 0x38);
            }
        }
        break;
    case 0xf:
        mark = 1;
        apply = 0;
        hit = 3;
        delta = -*(int*)(g_spell_rows + (kind << 7) + 0x88);
        ((SubHit*)(self + 0x558))->fn_00468160(2);
        break;
    case 0x10: {
        int passed = g_rand_gate != 0;
        apply = 0;
        if (passed == 0) {
            passed = !(fn_005426E0() < factor);
        }
        if (passed != 0) {
            fn_004743E0(self + 0x324);
        }
        break;
    }
    case 0x11: {
        int passed = g_rand_gate != 0;
        apply = 0;
        if (passed == 0) {
            passed = !(fn_005426E0() < factor);
        }
        if (passed != 0) {
            fn_00474350(self + 0x324, fn_004A3410(0xbb8, 0x1b58));
        }
        break;
    }
    case 0x12:
        apply = 0;
        if (g_rand_gate != 0 || !(fn_005426E0() < factor)) {
            result = *(float*)&arg7 * 0.0500000007f * (float)amount;
        }
        break;
    case 0x13:
        apply = 0;
        if (g_rand_gate != 0 || !(fn_005426E0() < factor)) {
            fn_00450C70(this, -(*(float*)&arg7) * 0.0500000007f * (float)amount);
        }
        break;
    case 0x14:
        result = (float)(-amount);
        hit = amount;
        delta = 0;
        apply = 0;
        break;
    case 0x15:
        apply = 0;
        if (g_rand_gate != 0 || !(fn_005426E0() < factor)) {
            fn_00450C70(this, (float)(-amount));
        }
        break;
    case 0x16:
        if (other != 0) {
            int type = *(int*)(*(char**)((char*)other + 0x71c) + 0x10);
            int duration = 0x1388;
            if (type == 0x59) {
                duration = 0xbb8;
            } else if (type == 0x57) {
                duration = 0x1f40;
            }
            fn_00454700(0xa, duration, kind, 0.0f, *(int*)((char*)other + 0x80), 0);
            return;
        }
        break;
    case 0x17:
        apply = 0;
        if (g_rand_gate != 0 || !(fn_005426E0() < factor)) {
            if (arg8 == 0) {
                fn_00454700(0xa, 0x2bf20, -1, 0.0f, -1, 0);
            } else {
                fn_00454700(0xa, 0x2bf20, 0x4c, 0.0f, -1, 0);
            }
        }
        break;
    case 0x18:
        mark = 1;
        apply = 0;
        hit = 3;
        delta = -*(int*)(g_spell_rows + (kind << 7) + 0x88);
        ((SubHit*)(self + 0x558))->fn_00468160(2);
        if (arg8 == 0) {
            fn_0048BD80(fn_0047A890(kind), arg3, arg4, 1);
            fn_00474350(self + 0x324, fn_004B9E50());
        } else {
            int spell = fn_0048BD80(*(int*)(g_spell_rows + (kind << 7) + 0x44), arg3, arg4, 1);
            if (spell != -1) {
                fn_0047E4D0(*(int*)((char*)other + 0x80), spell, arg3, *(int*)(self + 0x68c), self + 0x38);
                fn_00474350(self + 0x324, fn_004B9E50());
            }
        }
        break;
    case 0x1a: {
        int spell_kinds[5];
        int slots[5];
        int index;
        char landed = 0;
        apply = 0;
        spell_kinds[0] = 0x3d;
        spell_kinds[1] = 7;
        spell_kinds[2] = 0x3c;
        spell_kinds[3] = 8;
        spell_kinds[4] = 0x17;
        slots[0] = 0xa;
        slots[1] = 0;
        slots[2] = 0xb;
        slots[3] = 1;
        slots[4] = 3;
        for (index = 0; index < 5; index++) {
            int take_hit = g_rand_gate != 0 || (*(int*)(g_spell_rows + (kind << 7) + 0x9c) & 1) == 0;
            if (take_hit == 0) {
                take_hit = !(fn_005426E0() < factor);
            }
            if (take_hit != 0) {
                int duration = fn_0047AA80(spell_kinds[index], amount);
                float power = fn_0047A9C0(slots[index], amount);
                if (!(fn_0047F2A0(this, spell_kinds[index]) <= 0.00999999978f)) {
                    landed = 1;
                    if (arg8 == 0) {
                        fn_00454700(slots[index], duration, -1, power, *(int*)((char*)other + 0x80), 0);
                    } else {
                        fn_00454700(slots[index], duration, spell_kinds[index], power,
                            *(int*)((char*)other + 0x80), 0);
                    }
                }
            }
        }
        if (landed != 0) {
            mode = 0x81;
        }
        break;
    }
    default:
        break;
    }

    result = fn_0047F4D0(this, kind, hit, delta, use_roll ? roll : 0.0f);
    fn_00450850(result, source, 0, 0);
    if (result < 0.0f && mode != 0) {
        fn_0045DE20(mode);
    }
    if (apply == 1) {
        int effect = *(int*)(g_spell_rows + (kind << 7) + 0x44);
        if (arg8 != 0) {
            if (effect != 0) {
                fn_0048C080(effect, *(int*)(self + 0x80), -1, -1, 0, 1);
            }
        } else if (effect != 0) {
            fn_0048C080(fn_0047A890(kind), *(int*)(self + 0x80), -1, -1, 0, 1);
        }
    }
    if (kind == 0x21) {
        float sample = fn_005426E0();
        if (g_rand_gate == 0 && sample <= roll) {
            ((SubHit*)(self + 0x558))->fn_00468160(2);
        } else {
            int picked = fn_0047AA80(0x21, amount);
            apply = 0;
            if (arg8 != 0) {
                fn_00454700(slot, picked, kind, 0, *(int*)((char*)other + 0x80), 0);
            } else {
                fn_00454700(slot, picked, -1, 0, *(int*)((char*)other + 0x80), 0);
            }
            fn_0045DE20(0x81);
        }
    }
    if (mark != 0) {
        ((SubHit*)(self + 0x324))->fn_004B5790(source);
    }
}

void Entity::fn_0045D4C0() {
    int left = 0;
    int right = 0;
    BoneSet* bones = *(BoneSet**)((char*)anim + 0x3c);
    left = bones->fn_00540F70("BDBN-Foot-L");
    bones = *(BoneSet**)((char*)anim + 0x3c);
    right = bones->fn_00540F70("BDBN-Foot-R");
    foot_l = (short)left;
    foot_r = (short)right;
}

#pragma optimize("", on)

struct Vec3 {
    float x;
    float y;
    float z;

    float* fn_0045B680(float* out, float* other);
    float* fn_0045B6B0(float* out, float* other);
};

struct ConsoleCmd {
    void fn_0050A180(const char* name, const char* help, void (*handler)());
};

extern "C" int fn_00462DE0(Entity* ent);
extern "C" void* fn_004A1B70(Entity* ent, int* value);
extern "C" void* fn_004A1BE0(Entity* ent, void* value);
extern "C" float fn_0051B000(void* anim, int id);
extern "C" float fn_0051B0C0(void* anim, int id);
extern "C" void* fn_00446CB0(void* table, int kind, int value, int flag);
extern "C" void* fn_00446D10(void* table, int kind, int value, int flag);
extern "C" int fn_00570140(void* text, const char* name, int length);
extern "C" int fn_0056D790();
extern "C" int fn_0046A100(Entity* ent);
extern "C" void fn_00454670(Entity* ent, int value);
extern "C" void fn_0044E1C0(void* slot, int a, int b, float* c, int* d, int e);
extern "C" void fn_00472450(void* node);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int fn_0057AE80(void* left, void* right);
extern "C" void fn_0044EDB0(void* slot, char* name);
extern "C" int fn_0049E8C0(void* pos, int a, int b);
extern "C" void fn_00455FB0(void* slot);
extern "C" void fn_00515800(void* out, float* in, int flag);
extern "C" void fn_00531CB0(const char* file, int line, const char* msg);

extern "C" int fn_004587F0() {
    return 0x17c;
}

float Entity::fn_00456F80() {
    if (fn_00462DE0(this) == 0) {
        return *(float*)((char*)this + 0x698);
    }
    return 0.0f;
}

Entity* Entity::fn_004570D0(int value) {
    *(int*)((char*)this + 0x6ac) = value;
    return this;
}

Entity* Entity::fn_004570F0(int* value) {
    if (*(int*)((char*)this + 0xc) != 9) {
        return this;
    }
    return (Entity*)fn_004A1B70(this, value);
}

Entity* Entity::fn_00457120(void* value) {
    if (*(int*)((char*)this + 0xc) != 9) {
        return this;
    }
    return (Entity*)fn_004A1BE0(this, value);
}

float* Vec3::fn_0045B680(float* out, float* other) {
    out[0] = other[0] + x;
    out[1] = other[1] + y;
    out[2] = other[2] + z;
    return out;
}

float* Vec3::fn_0045B6B0(float* out, float* other) {
    out[0] = x - other[0];
    out[1] = y - other[1];
    out[2] = z - other[2];
    return out;
}

extern "C" void fn_0045DD80(Entity* ent, int value) {
    int index;
    void* row;
    for (index = 0; index < 5; index++) {
        if (*(int*)(index * 0xc + 0x9416C8) == -1) {
            *(int*)(index * 0xc + 0x9416C8) = *(int*)((char*)ent + 0x80);
            *(int*)(index * 0xc + 0x9416CC) = value;
            row = ent->fn_00461500();
            *(float*)(index * 0xc + 0x9416D0) =
                0.5f / fn_0051B0C0(*(void**)((char*)ent + 0x314), *(int*)((char*)row + 8));
            break;
        }
    }
}

Entity* Entity::fn_0045DE20(int mode) {
    char* self = (char*)this;
    void* clip;
    int played;
    if (*(int*)(self + 0xc) == 9) {
        return this;
    }
    clip = fn_00461370(4, mode);
    if (clip == 0) {
        return 0;
    }
    played = (int)fn_00460F20(clip, 1.0f, 1, 1, 0);
    if (played == -1) {
        return (Entity*)-1;
    }
    fn_00454670(this, -1);
    *(int*)(self + 0x320) |= 4;
    *(int*)(self + 0x56c) = 0;
    return this;
}

void* Entity::fn_00461370(int kind, int value) {
    if (kind == 4 && *(int*)((char*)this + 0xc) == 9) {
        return 0;
    }
    return fn_00446CB0(*(void**)((char*)this + 0x71c), kind, value, 1);
}

void* Entity::fn_004613B0(int kind, int value) {
    if (kind == 4 && *(int*)((char*)this + 0xc) == 9) {
        return 0;
    }
    return fn_00446D10(*(void**)((char*)this + 0x71c), kind, value, 1);
}

float Entity::fn_004613F0(int id) {
    float sample;
    float length;
    char* self = (char*)this;
    if (fn_00461450(id, &sample) == 0) {
        return 1.0f;
    }
    length = fn_0051B0C0(*(void**)(self + 0x314), *(int*)(*(char**)(self + 0x6d0) + 8));
    return (length - sample) / length;
}

int Entity::fn_00461450(int id, float* out) {
    float local;
    float value;
    char* self = (char*)this;
    if (id == -1 || id != *(int*)(self + 0x6cc)) {
        return 0;
    }
    if (out == 0) {
        out = &local;
    }
    value = fn_0051B000(*(void**)(self + 0x314), *(int*)(*(char**)(self + 0x6d0) + 8));
    *out = value;
    if (!(value < 0.00009999999747378752f)) {
        return 1;
    }
    *(int*)(self + 0x6cc) = -1;
    return 0;
}

int Entity::fn_004614D0(int id) {
    return fn_00461450(id, 0) != 0;
}

void* Entity::fn_00461500() {
    if (fn_00461450(*(int*)((char*)this + 0x6cc), 0) == 0) {
        return 0;
    }
    return *(void**)((char*)this + 0x6d0);
}

void* Entity::fn_00461540() {
    char* node;
    char* first = 0;
    int count = 0;
    int pick;
    char* head = *(char**)(*(char**)((char*)this + 0x71c) + 0x6c);
    for (node = head; node != 0; node = *(char**)(node + 0x40)) {
        if (fn_00570140(*(void**)node, "talk", 4) == 0) {
            if (first == 0) {
                first = node;
            }
            count += 1;
        }
    }
    if (count == 0) {
        return 0;
    }
    if (count == 1) {
        return first;
    }
    pick = fn_0056D790() % count;
    for (node = first; node != 0; node = *(char**)(node + 0x40)) {
        if (fn_00570140(*(void**)node, "talk", 4) == 0) {
            if (pick == 0) {
                return node;
            }
            pick -= 1;
        }
    }
    return 0;
}

int Entity::fn_00461620(int id) {
    int index;
    char* row;
    for (index = 0; index < 3; index++) {
        row = *(char**)((char*)this + 0x6d0) + (index << 3);
        if (*(int*)(row + 0x1c) == id) {
            return 1;
        }
    }
    return 0;
}

float Entity::fn_00461670(int id) {
    int index;
    char* row;
    char* self = (char*)this;
    if (*(int*)(self + 0x6d0) != 0) {
        for (index = 0; index < 3; index++) {
            row = *(char**)(self + 0x6d0) + (index << 3);
            if (*(int*)(row + 0x1c) == id) {
                return *(float*)(row + 0x20) - (*(float*)0x60A328 - *(float*)(self + 0x9c));
            }
        }
    }
    return 0.0f;
}

int Entity::fn_00461740(int id) {
    int index;
    char* row;
    char* self = (char*)this;
    for (index = 0; index < 3; index++) {
        if (*(int*)(self + 0x6d0) == 0) {
            return -1;
        }
        row = *(char**)(self + 0x6d0) + (index << 3);
        if (*(int*)(row + 0x1c) == id) {
            if (*(char*)(self + index + 0xa0) != 0) {
                return 0;
            }
            if (!(fn_00461670(id) <= 0.00009999999747378752f)) {
                return 0;
            }
            *(char*)(self + index + 0xa0) = 1;
            return 1;
        }
    }
    return -1;
}

int Entity::fn_004617F0() {
    float threshold = (float)fn_00462840(0) * 0.30000001192092896f;
    if (threshold <= *(float*)((char*)this + 0x5f8)) {
        return fn_00455950(0xd) != 0;
    }
    return 1;
}

extern "C" void fn_00461870() {
    ((ConsoleCmd*)0x8ED970)->fn_0050A180("immortal", "Toggles Players_immortal", (void (*)())0x461890);
}

extern "C" void fn_00461860() {
    fn_00461870();
}

extern "C" void fn_00461990() {
    ((ConsoleCmd*)0x941508)->fn_0050A180(
        "health", "Sets the percentage health for the debug object", (void (*)())0x4619B0);
}

extern "C" void fn_00461980() {
    fn_00461990();
}

int Entity::fn_00461AD0(Entity* other) {
    char* self = (char*)this;
    int bit;
    if (*(int*)((char*)other + 0xc) == 4 || *(int*)((char*)other + 0xc) == 1) {
        return 0;
    }
    if (fn_0046A100(other) != 0) {
        return 0;
    }
    bit = 1 << (*(int*)((char*)other + 0x98) & 0x1f);
    if ((*(int*)(self + 0x2f0) & bit) != 0) {
        return 1;
    }
    if (*(int*)(self + 0x98) != 0 || *(int*)((char*)other + 0x98) != 0) {
        return 0;
    }
    return other->fn_00455950(0x10) != 0;
}

int Entity::fn_00461B60() {
    return *(int*)((char*)this + 0x98);
}

extern "C" void* fn_00461B80(int arg1, int arg2, float* arg3, int* arg4, char arg5, int arg6, int arg7, char arg8) {
    int index;
    char* slot;
    int number;
    int unique;
    char name[0x40];
    int cursor;
    int taken;
    (void)arg5;
    for (index = 0; index < 0xaf; index++) {
        slot = (char*)(index * 0x788 + 0x8ED980);
        if (*(int*)(slot + 0xc) == 0) {
            break;
        }
    }
    if (index == 0xaf) {
        while (1) {
            fn_00531CB0(
                "D:\\projects\\Summoner\\pccode\\Engine\\Objects\\living_entity.cpp",
                0x20e8,
                "No more space for living entities.  Please increase MAX_LIVING_ENTIITES in living_entity.cpp");
        }
    }
    fn_0044E1C0(slot, arg1, arg2, arg3, arg4, arg7);
    *(int*)(slot + 0x68c) = arg6;
    fn_00472450(slot + 0x324);
    if (*(int*)(slot + 0xc) == 5 || *(int*)(slot + 0xc) == 4) {
        *(int*)(slot + 0x374) |= 0x40;
    }
    number = 1;
    unique = 0;
    while (unique == 0) {
        taken = 0;
        sprintf(name, "%s#%d", **(char***)(slot + 0x71c), number);
        for (cursor = *(int*)0x8ECD78; cursor != 0x8ECA60; cursor = *(int*)(cursor + 0x318)) {
            if (fn_0057AE80((void*)(cursor + 0x2f4), name) == 0) {
                taken = 1;
                break;
            }
        }
        if (taken == 0) {
            unique = 1;
            fn_0044EDB0(slot, name);
        }
        number += 1;
    }
    if (*(int*)(slot + 0xc) == 7 || arg8 != 0) {
        return slot;
    }
    fn_0049E8C0(slot + 0x10, *(int*)(*(char**)(slot + 0x71c) + 0x90), *(int*)(slot + 0x68c));
    *(float*)(slot + 0x14) += 1.0f;
    fn_00455FB0(slot);
    if (*(int*)(slot + 0x68a) == -1) {
        fn_00515800(name, arg3, 1);
    }
    return slot;
}

extern "C" void fn_00510240(void* orient, void* a, void* b, int* yaw);
extern "C" int fn_00501940(void* text);
extern "C" int fn_004746D0(void* node, void* out);
extern "C" void* fn_00433EB0(int id);
extern "C" void fn_005100C0(float a, float b, float yaw);
extern "C" void fn_00501880(void* timer, int value);
extern "C" void fn_00501900(void* timer);
extern "C" int fn_00489A60(int value);
extern "C" int fn_004747E0(void* node, void* in);
extern "C" void fn_00455A70(Entity* ent, int slot, int effect);
extern "C" int fn_00473200(void* node, int a, int b);
extern "C" void fn_0051B100(void* anim);
extern "C" float fn_0051B250(void* anim, int id);
extern "C" int fn_00473120(void* node);
extern "C" void fn_0051AE90(void* anim, int id, float weight);
extern "C" void fn_0051B590(void* anim, int r, int g, int b);
extern "C" int fn_0048D000(int id, void* unused);
extern "C" int fn_00501910(void* timer);
extern "C" int fn_00501990(void* timer);
extern "C" void fn_0046ADF0(Entity* ent, float value);
extern "C" void fn_00473880(void* node);
extern "C" void fn_004546B0(Entity* ent);
extern "C" float fn_00462810(Entity* ent);
extern "C" void fn_004AF320(int* a, int* b, int* c);
extern "C" float* fn_0051A950(void* anim);
extern "C" float* fn_0051ABD0(void* anim, void* out);
extern "C" void fn_004530D0(Entity* ent);
extern "C" void fn_00510110(void* mat, void* orient, float angle);
extern "C" int* fn_0050FBC0(void* mat, void* out, void* orient);
extern "C" int fn_00469010(int layer, int page);
extern "C" int fn_00468DE0(float id, int* pos);
extern "C" void fn_00521800(int id, int* out);
extern "C" float fn_00542770(float a, float b);
extern "C" float fn_005217C0(int id);
extern "C" void fn_0051AD00(void* anim, float* pos, void* extra, int bone);
extern "C" int fn_0051B290(void* anim, const char* name);
extern "C" int fn_004A2870(Entity* ent, int arg);
extern "C" int fn_00473620(void* node);
extern "C" int fn_0044B4F0(void* item);
extern "C" int fn_0047A830(int kind);
extern "C" int fn_0044DD30(Entity* ent, int kind);
extern "C" int fn_0047A870(int kind);
extern "C" void fn_0047DFE0(int kind, Entity* ent);
extern "C" void fn_0047E6C0(Entity* ent, int effect);
extern "C" void* fn_0051C720(int id);
extern "C" void fn_0044DC70(Entity* ent, int kind, int a, int b);
extern "C" void fn_004750B0(int kind, int effect);
extern "C" void* fn_0046B130(int id);
extern "C" void fn_0048D0A0(int id);
extern "C" void fn_0051B140(void* anim);
extern "C" void fn_0051AEE0(void* anim, int clip, float scale, int flag, float rate);
extern "C" void fn_0051A890(void* anim, float a, int b, int c);
extern "C" int fn_00521650(int id);
extern "C" void fn_00521620(int id, int value);
extern "C" int fn_0048CE50(int id);
extern "C" void fn_00455B20(Entity* ent);
extern "C" void fn_0047C5D0(void* other, int kind, int field, void* a, int b, void* c, int d);
extern "C" float sqrtf(float value);
extern "C" int fn_005241E0(float* vec);

static const int fn_00457150_order[18] = {
    0x10, 0x0a, 0x09, 0x08, 0x01, 0x07, 0x05, 0x0b, 0x02,
    0x03, 0x04, 0x06, 0x0c, 0x0d, 0x0e, 0x0f, 0x11, 0
};

static void slot_write(char** cursor, Entity* ent, int index) {
    char* self = (char*)ent;
    char* row = self + (index << 5);
    int state = *(int*)(row + 0xac);
    *(*cursor)++ = (char)state;
    *(*cursor)++ = (char)index;
    *(*cursor)++ = *(char*)(row + 0xbc);
    *(int*)*cursor = fn_00501940(row + 0xb0);
    *cursor += 4;
    *(int*)*cursor = fn_00501940(row + 0xc0);
    *cursor += 4;
    *(int*)*cursor = *(int*)(row + 0xc4);
    *cursor += 4;
    *(int*)*cursor = *(int*)(row + 0xc8);
    *cursor += 4;
}

static void slot_pad(char** cursor) {
    *(*cursor)++ = (char)0xff;
    *(*cursor)++ = 0;
    *(*cursor)++ = 0;
    *(int*)*cursor = 0;
    *cursor += 4;
    *(int*)*cursor = 0;
    *cursor += 4;
    *(int*)*cursor = 0;
    *cursor += 4;
    *(int*)*cursor = 0;
    *cursor += 4;
}

static void slot_read(char** cursor, Entity* ent) {
    char* self = (char*)ent;
    int state = (unsigned char)*(*cursor)++;
    if (state == 0 || state == 2) {
        int index = (unsigned char)*(*cursor)++;
        char* row = self + (index << 5);
        *(int*)(row + 0xac) = state;
        *(int*)(row + 0xbc) = (unsigned char)*(*cursor)++;
        int first = *(int*)*cursor;
        *cursor += 4;
        if (first < 0) {
            first = 0;
        }
        if (first != 0x3ff1a100) {
            fn_00501880(row + 0xb0, first);
        } else {
            fn_00501900(row + 0xb0);
        }
        int second = *(int*)*cursor;
        *cursor += 4;
        if (second < 0) {
            second = 0;
        }
        if (second != 0x3ff1a100) {
            fn_00501880(row + 0xc0, second);
        } else {
            fn_00501900(row + 0xc0);
        }
        *(int*)(row + 0xc4) = *(int*)*cursor;
        *cursor += 4;
        *(int*)(row + 0xc8) = *(int*)*cursor;
        *cursor += 4;
        if (index == 0x10) {
            fn_00455A70(ent, 0x10, fn_0048C080(*(int*)0x22E6158, *(int*)(self + 0x80), -1, -1, 1, 1));
        }
    } else {
        *cursor += 2 + 16;
    }
}

void* Entity::fn_004569C0(float* point, char relative) {
    char* self = (char*)this;
    int flags = *(int*)(self + 0x320);
    float x;
    float y;
    float z;
    float radius;
    float top;
    char* other;
    if ((flags & 0x20) || (flags & 2) || (flags & 0x400)) {
        return 0;
    }
    if (fn_00456F80() < 0.00100000005f) {
        return 0;
    }
    x = point[0];
    y = point[1];
    z = point[2];
    if (relative) {
        x += *(float*)(self + 0x10);
        y += *(float*)(self + 0x14);
        z += *(float*)(self + 0x18);
    }
    radius = fn_004542B0();
    top = fn_004542F0() + y;
    for (other = *(char**)0x8ECD78; other != (char*)0x8ECA60; other = *(char**)(other + 0x318)) {
        float other_r;
        float reach;
        float dx;
        float dz;
        if ((*(int*)(other + 0x320) & 0x20) != 0 || other == self) {
            continue;
        }
        other_r = ((Entity*)other)->fn_004542B0();
        if (*(int*)(self + 0x98) == *(int*)(other + 0x98)) {
            radius *= 0.75f;
            other_r *= 0.75f;
        }
        if (!(*(float*)(other + 0x10) + other_r > x - radius)) {
            continue;
        }
        if (!(*(float*)(other + 0x10) - other_r < x + radius)) {
            continue;
        }
        if (!(*(float*)(other + 0x18) + other_r > z - radius)) {
            continue;
        }
        if (!(*(float*)(other + 0x18) - other_r < z + radius)) {
            continue;
        }
        if (!(((Entity*)other)->fn_004542F0() + *(float*)(other + 0x14) > y)) {
            continue;
        }
        if (!(top > *(float*)(other + 0x14))) {
            continue;
        }
        dx = x - *(float*)(other + 0x10);
        dz = z - *(float*)(other + 0x18);
        reach = radius + other_r;
        if (!(reach * reach <= dx * dx + dz * dz)) {
            if (relative == 0) {
                return (void*)1;
            }
            {
                float push[3];
                push[0] = x - *(float*)(other + 0x10);
                push[1] = y - *(float*)(other + 0x14);
                push[2] = z - *(float*)(other + 0x18);
                if (fn_005241E0(push) == 0) {
                    fn_00524430(push);
                    point[0] = push[0] * reach + *(float*)(other + 0x10) - *(float*)(self + 0x10);
                    point[1] = *(float*)(other + 0x14) - *(float*)(self + 0x14);
                    point[2] = push[2] * reach + *(float*)(other + 0x18) - *(float*)(self + 0x18);
                }
            }
            return 0;
        }
    }
    return 0;
}

int Entity::fn_00457150(void* out) {
    char* self = (char*)this;
    char* cursor = (char*)out;
    int yaw = 0;
    int kind = *(int*)(self + 0xc);
    int written = 0;
    int count = 0;
    int index;
    *(int*)cursor = kind;
    cursor += 4;
    *(int*)cursor = *(int*)(self + 0x10);
    cursor += 4;
    *(int*)cursor = *(int*)(self + 0x14);
    cursor += 4;
    *(int*)cursor = *(int*)(self + 0x18);
    cursor += 4;
    fn_00510240(self + 0x38, &written, &written, &yaw);
    *(int*)cursor = yaw;
    cursor += 4;
    *(int*)cursor = *(int*)(self + 0x68c);
    cursor += 4;
    *(int*)cursor = *(int*)(self + 0x84);
    cursor += 4;
    *(int*)cursor = *(int*)(self + 0x320);
    cursor += 4;
    if (kind == 4 || kind == 5 || kind == 9 || kind == 7) {
        for (index = 0; index < 18 && count < 4; index++) {
            int slot = fn_00457150_order[index];
            int state = *(int*)(self + (slot << 5) + 0xac);
            if (state == 0 || state == 2) {
                slot_write(&cursor, this, slot);
                count += 1;
            }
        }
        for (index = count; index < 4; index++) {
            slot_pad(&cursor);
        }
        *(short*)cursor = *(short*)(self + 0x600);
        cursor += 2;
        if (kind == 7) {
            *(int*)cursor = *(int*)(self + 0x604);
            cursor += 4;
        }
        *(float*)cursor = *(float*)(self + 0x5f8);
        cursor += 4;
        *(float*)cursor = *(float*)(self + 0x5f4);
        cursor += 4;
        *(short*)cursor = *(short*)(self + 0x5fe);
        cursor += 2;
        *(short*)cursor = *(short*)(self + 0x5fc);
        cursor += 2;
        *(int*)cursor = *(int*)(self + 0x720);
        cursor += 4;
        if (kind == 7) {
            int item;
            void* block = fn_00433EB0(*(int*)(self + 0x80));
            for (item = 0; item < 9; item++) {
                if (*(int*)(self + item * 4 + 0x754) == 0) {
                    *(int*)cursor = -1;
                } else {
                    *(int*)cursor = (*(int*)(*(char**)(self + item * 4 + 0x754) + 4) - 0x8CBEC0) >> 8;
                }
                cursor += 4;
            }
            for (item = 0; item < 0x24; item++) {
                *(int*)cursor = fn_00501940((char*)block + (item + 0xa) * 4);
                cursor += 4;
            }
            for (item = 0; item < 0x24; item++) {
                *cursor = *(char*)(self + 0x614 + item);
                cursor += 1;
            }
        }
        cursor += fn_004746D0(self + 0x324, cursor);
        if (kind != 7) {
            int who = *(int*)(self + 0x6dc);
            if (who < 0) {
                *(int*)cursor = 0;
                cursor += 4;
                *(int*)cursor = -1;
                cursor += 4;
                *cursor = 0;
                cursor += 1;
            } else {
                *(int*)cursor = fn_00501940((char*)(who * 0x550 + 0x24F6748));
                cursor += 4;
                *(int*)cursor = *(int*)(who * 0x550 + 0x24F6750);
                cursor += 4;
                *cursor = (char)(*(int*)(who * 0x550 + 0x24F6758) == 1);
                cursor += 1;
            }
        }
    }
    if ((*(int*)(self + 0x320) & 0x80) != 0) {
        *(int*)cursor = *(int*)(self + 0xa4);
        cursor += 4;
    } else {
        *(int*)cursor = 0;
        cursor += 4;
    }
    return (int)(cursor - (char*)out);
}

void* Entity::fn_00458800(void* in) {
    char* self = (char*)this;
    char* cursor = (char*)in;
    int kind;
    int index;
    float yaw;
    kind = *(int*)cursor;
    cursor += 4;
    *(int*)(self + 0xc) = kind;
    *(int*)(self + 0x10) = *(int*)cursor;
    cursor += 4;
    *(int*)(self + 0x14) = *(int*)cursor;
    cursor += 4;
    *(int*)(self + 0x18) = *(int*)cursor;
    cursor += 4;
    yaw = *(float*)cursor;
    cursor += 4;
    fn_005100C0(0.0f, 0.0f, yaw);
    *(int*)(self + 0x1c) = *(int*)(self + 0x10);
    *(int*)(self + 0x20) = *(int*)(self + 0x14);
    *(int*)(self + 0x24) = *(int*)(self + 0x18);
    *(int*)(self + 0x68c) = *(int*)cursor;
    cursor += 4;
    *(int*)(self + 0x84) = *(int*)cursor;
    cursor += 4;
    *(int*)(self + 0x320) = *(int*)cursor;
    cursor += 4;
    if (kind == 4 || kind == 5 || kind == 9 || kind == 7) {
        for (index = 0; index < 4; index++) {
            slot_read(&cursor, this);
        }
        *(short*)(self + 0x600) = *(short*)cursor;
        cursor += 2;
        *(int*)(self + 0x604) = fn_00489A60(*(short*)(self + 0x600));
        fn_0044DDB0();
        *(float*)(self + 0x5f8) = *(float*)cursor;
        cursor += 4;
        *(float*)(self + 0x5f4) = *(float*)cursor;
        cursor += 4;
        *(short*)(self + 0x5fe) = *(short*)cursor;
        cursor += 2;
        *(short*)(self + 0x5fc) = *(short*)cursor;
        cursor += 2;
        *(int*)(self + 0x720) = *(int*)cursor;
        cursor += 4;
        cursor += fn_004747E0(self + 0x324, cursor);
        if (*(int*)(self + 0x6dc) < 0) {
            cursor += 9;
        } else {
            int who = *(int*)(self + 0x6dc);
            int value = *(int*)cursor;
            cursor += 4;
            if (value != 0x3ff1a100) {
                fn_00501880((void*)(who * 0x550 + 0x24F6748), value);
            } else {
                fn_00501900((void*)(who * 0x550 + 0x24F6748));
            }
            *(int*)(who * 0x550 + 0x24F6750) = *(int*)cursor;
            cursor += 4;
            *(char*)(who * 0x550 + 0x24F6758) = *cursor;
            cursor += 1;
        }
    }
    if ((*(int*)(self + 0x320) & 0x80) != 0) {
        *(int*)(self + 0xa4) = (unsigned char)*cursor;
        cursor += 1;
        if (*(int*)(self + 0xc) != 4) {
            *(void**)(self + 0xa8) = fn_00461370(6, *(int*)(self + 0xa4));
        } else {
            *(void**)(self + 0xa8) = fn_00461370(8, *(int*)(self + 0xa4));
            if (*(int*)(self + 0xa8) == 0) {
                *(void**)(self + 0xa8) = fn_00461370(6, *(int*)(self + 0xa4));
            }
        }
    }
    return cursor;
}

void* Entity::fn_0045A120() {
    char* self = (char*)this;
    int idle;
    int move;
    int blend_from;
    int blend_to;
    int held;
    if ((*(int*)(self + 0x320) & 8) != 0) {
        return this;
    }
    fn_0051B100(*(void**)(self + 0x314));
    if (fn_00473200(self + 0x324, 8, 0x2d) != 1 && fn_00473200(self + 0x324, 8, 0x2e) == 0) {
        if ((*(int*)(self + 0x320) & 0x400) == 0) {
            if (fn_004617F0() == 0) {
                if ((*(int*)(self + 0x320) & 1) == 0) {
                    held = *(int*)(self + 0x5ac);
                    move = *(int*)(self + 0x5b8);
                    idle = *(int*)(self + 0x5b0);
                    blend_to = *(int*)(self + 0x5b4);
                    blend_from = held;
                } else {
                    held = *(int*)(self + 0x5ac);
                    move = *(int*)(self + 0x5b8);
                    idle = *(int*)(self + 0x5bc);
                    blend_to = *(int*)(self + 0x5c0);
                    blend_from = move;
                }
            } else if ((*(int*)(self + 0x320) & 1) == 0) {
                held = *(int*)(self + 0x5c4);
                move = *(int*)(self + 0x5d0);
                idle = *(int*)(self + 0x5c8);
                blend_to = *(int*)(self + 0x5cc);
                blend_from = held;
            } else {
                held = *(int*)(self + 0x5c4);
                move = *(int*)(self + 0x5d0);
                idle = *(int*)(self + 0x5d4);
                blend_to = *(int*)(self + 0x5d8);
                blend_from = move;
            }
        } else {
            move = *(int*)(self + 0x5e8);
            idle = *(int*)(self + 0x5ec);
            held = *(int*)(self + 0x5b8);
            blend_from = move;
            blend_to = -1;
        }
        if (fn_00473200(self + 0x324, 9, -1) != 1) {
            *(float*)(self + 0x6b8) = fn_0051B250(*(void**)(self + 0x314), idle);
            if (blend_to >= 0) {
                *(float*)(self + 0x6b4) = fn_0051B250(*(void**)(self + 0x314), blend_to);
            } else {
                *(float*)(self + 0x6b4) = *(float*)(self + 0x6b8);
            }
            if (*(float*)(self + 0x6a8) < 0.000500000024f) {
                fn_0051AE90(*(void**)(self + 0x314), blend_from, 1.0f);
            } else if (*(int*)(self + 0x5e0) != -1) {
                fn_0051AE90(*(void**)(self + 0x314), *(int*)(self + 0x5e0), 1.0f - *(float*)(self + 0x694));
                fn_0051AE90(*(void**)(self + 0x314), move, *(float*)(self + 0x694));
            }
        } else if (*(int*)(self + 0x5e4) != -1) {
            fn_0051AE90(*(void**)(self + 0x314), *(int*)(self + 0x5e4), 1.0f);
        }
    }
    return this;
}

void Entity::fn_0045A6E0(float value) {
    char* self = (char*)this;
    float low = *(float*)(self + 0x6b4);
    float high = *(float*)(self + 0x6b8);
    float span;
    if (low - 0.00999999978f <= value) {
        value = low;
    }
    if (!(value <= high)) {
        span = low - high;
        if (!(span <= 0.0f)) {
            *(float*)(self + 0x6a8) = (value - high) / span / 2.0f + 0.5f;
        } else {
            *(float*)(self + 0x6a8) = 0.0f;
        }
        return;
    }
    span = high - low;
    if (span < 0.00999999978f || span <= -0.00999999978f) {
        if (!(high <= 0.0f)) {
            *(float*)(self + 0x6a8) = value / high / 2.0f;
        } else {
            *(float*)(self + 0x6a8) = 0.0f;
        }
        return;
    }
    if (!(high <= 0.0f)) {
        *(float*)(self + 0x6a8) = value / high;
    } else {
        *(float*)(self + 0x6a8) = 0.0f;
    }
}

float Entity::fn_0045A840(float* out) {
    char* self = (char*)this;
    float scale;
    float* velocity;
    float length;
    if (*(float*)(self + 0x6bc) == 0.0f) {
        return 0.0f;
    }
    scale = *(float*)(self + 0x6b0) * *(float*)(self + 0x6bc);
    velocity = fn_0051A950(*(void**)(self + 0x314));
    length = sqrtf(velocity[0] * velocity[0] + velocity[1] * velocity[1] + velocity[2] * velocity[2]);
    if (out != 0) {
        out[0] = velocity[0];
        out[1] = velocity[1];
        out[2] = velocity[2];
    }
    return length * length / (scale + scale);
}

void* Entity::fn_0045AB00(float* delta) {
    char* self = (char*)this;
    float moved[3];
    if ((*(int*)(self + 0x320) & 0x1000) != 0) {
        return this;
    }
    moved[0] = *(float*)(self + 0x10) + delta[0];
    moved[1] = *(float*)(self + 0x14) + delta[1];
    moved[2] = *(float*)(self + 0x18) + delta[2];
    if (fn_0049E8C0(moved, *(int*)(*(char**)(self + 0x71c) + 0x90), *(int*)(self + 0x68c)) == 1) {
        float back[3];
        back[0] = moved[0] - *(float*)(self + 0x10);
        back[1] = moved[1] - *(float*)(self + 0x14);
        back[2] = moved[2] - *(float*)(self + 0x18);
        delta[0] = back[0];
        delta[1] = back[1];
        delta[2] = back[2];
    }
    return this;
}

int Entity::fn_0045B6E0() {
    char* self = (char*)this;
    int index;
    int active = 0;
    unsigned char* gate = (unsigned char*)0x5FBFD8;
    int debug = *(int*)0x651598;
    if ((*gate & 2) == 0 || debug == 0 || (*(int*)(debug + 0x10) & 1) != 0) {
        fn_00455B20(this);
    }
    if ((*gate & 0x20) != 0) {
        return *gate & 0x20;
    }
    for (index = 0; index < 0x12; index++) {
        char* row = self + (index << 5);
        if (*(int*)(row + 0xac) == -1) {
            continue;
        }
        if (index == 0) {
            fn_0051B590(*(void**)(self + 0x314), 0, 0, 0);
        } else if (index == 1) {
            fn_0051B590(*(void**)(self + 0x314), 0x80, 0, 0x80);
        } else if (index == 2 || index == 4) {
            fn_0051B590(*(void**)(self + 0x314), 0xff, 0xff, 0xff);
        } else if (index == 3) {
            fn_0051B590(*(void**)(self + 0x314), 0, 0, 0xff);
        } else if (index == 5) {
            fn_0051B590(*(void**)(self + 0x314), 0xff, 0xff, 0);
        } else if (index == 6) {
            fn_0051B590(*(void**)(self + 0x314), 0xff, 0, 0);
        } else if (index == 9) {
            fn_0051B590(*(void**)(self + 0x314), 0, 0xc8, 0xc8);
        } else if (index == 0xb) {
            fn_0051B590(*(void**)(self + 0x314), 0, 0xff, 0);
        } else if (index == 0xc || index == 0xd) {
            fn_0051B590(*(void**)(self + 0x314), 0x80, 0x37, 0);
        }
        if (*(int*)(row + 0xb4) != -1 && fn_0048D000(*(int*)(row + 0xb4), 0) != 0) {
            *(int*)(row + 0xb4) = -1;
        }
        if (*(int*)(row + 0xac) == 1 || fn_00501990(row + 0xb0) != 0) {
            if (((*gate & 2) == 0 || debug == 0 || (*(int*)(debug + 0x10) & 1) != 0) &&
                fn_00501910(row + 0xc0) != 0 && *(int*)(row + 0xc4) > 0) {
                if (index == 0) {
                    fn_00450850(*(float*)(row + 0xc8), *(int*)(row + 0xb8), 0, 0);
                } else if (index >= 5 && index <= 0x11) {
                    float amount = *(float*)(row + 0xc8);
                    if (g_rand_gate == 0 && amount < 0.0f) {
                        amount = (1.0f - fn_004626B0(3)) * amount;
                        if (amount < 0.0f) {
                            amount = 0.0f;
                        }
                    }
                    fn_00450850(amount, *(int*)(row + 0xb8), 0, 0);
                }
                fn_00501880(row + 0xc0, *(int*)(row + 0xc4));
            }
        }
    }
    if (*(int*)(self + 0x1ec) == -1 && *(int*)(self + 0x1ac) != -1) {
        int node;
        for (node = *(int*)0x23059B8; node != 0x2305920; node = *(int*)(node + 0x98)) {
            if ((*(int*)(node + 0xa4) == 6 || *(int*)(node + 0xa4) == 5) &&
                *(int*)(node + 0xb4) == *(int*)(self + 0x80)) {
                break;
            }
        }
        if (node == 0x2305920) {
            *(int*)(*(char**)(self + 0x314) + 0xb4) = 0x14;
        }
    } else if ((*(int*)(self + 0x320) & 0x400) == 0 && fn_00473200(self + 0x324, 9, -1) == 0) {
        *(int*)(*(char**)(self + 0x314) + 0xb4) = 0xff;
    }
    if (*(int*)(self + 0x1ec) == -1) {
        return fn_0048CE50(*(int*)(self + 0x80));
    }
    return 0;
}

float* Entity::fn_0045BF70() {
    char* self = (char*)this;
    float step;
    if (fn_00455950(0xa) != 0 || fn_00455950(9) != 0 || fn_00455950(0x10) != 0) {
        fn_0046ADF0(this, 0.0f);
        *(int*)(self + 0x320) |= 2;
        fn_00473880(self + 0x324);
    } else {
        fn_0046ADF0(this, *(float*)(self + 0x610));
    }
    step = *(float*)(self + 0x610);
    if (*(float*)(self + 0x610) <= 0.0f) {
        step = *(float*)0x5A5740;
    }
    *(float*)(self + 0x6f0) -= step;
    if (*(float*)(self + 0x6f0) < 0.0f) {
        *(float*)(self + 0x6f0) = 0.0f;
    }
    *(float*)(self + 0x6f4) -= step;
    if (*(float*)(self + 0x6f4) < 0.0f) {
        *(float*)(self + 0x6f4) = 0.0f;
    }
    *(float*)(self + 0x70c) -= step;
    if (*(float*)(self + 0x70c) < 0.0f) {
        *(float*)(self + 0x70c) = 0.0f;
    }
    if (*(int*)(self + 0x6cc) == -1 || fn_00461450(*(int*)(self + 0x6cc), 0) == 0) {
        if ((*(int*)(self + 0x320) & 4) != 0) {
            fn_004546B0(this);
        }
        *(int*)(self + 0x6cc) = -1;
    }
    if (fn_00501990(self + 0x2ec) != 0 && fn_00501910(self + 0x2ec) != 0) {
        int gained = (int)((float)fn_00462840(1) * (fn_00462810(this) * 0.0500000007f));
        int cap;
        if (gained < 1) {
            gained = 1;
        }
        if ((*(unsigned char*)0x5FBFD8 & 2) != 0) {
            gained <<= 1;
        }
        *(short*)(self + 0x5fe) = (short)(*(short*)(self + 0x5fe) + gained);
        cap = fn_00462840(1);
        if (*(short*)(self + 0x5fe) > cap) {
            *(short*)(self + 0x5fe) = (short)cap;
        }
        fn_00501880(self + 0x2ec, 0x1d4c);
    }
    fn_0045B6E0();
    if ((*(int*)(self + 0x320) & 0x80) == 0 && *(int*)(self + 0x688) >= 0 &&
        *(int*)(self + 0x688) < *(int*)0xA5A2C8 && *(int*)(self + 0x68a) >= 0) {
        int a = 0;
        int b = 0;
        int c = 0;
        fn_004AF320(&b, &a, &c);
        *(float*)(self + 0x780) = (float)(b + a + c) / 3.0f / 255.0f * 0.550000011920929f;
    }
    return (float*)this;
}

int Entity::fn_0045C340(int arg, float* arg3) {
    char* self = (char*)this;
    float local[3];
    float* velocity = fn_0051ABD0(*(void**)(self + 0x314), local);
    float length = sqrtf(velocity[0] * velocity[0] + velocity[1] * velocity[1] + velocity[2] * velocity[2]);
    float limit = *(float*)(self + 0x6ac);
    (void)arg;
    (void)arg3;
    if (length < limit) {
        length = length + *(float*)(*(char**)(self + 0x71c) + 0x228) * *(float*)(self + 0x6bc) * *(float*)(self + 0x610);
        if (length > limit) {
            length = limit;
        }
    }
    return (int)length;
}

int* Entity::fn_0045CD00() {
    char* self = (char*)this;
    float sample = 0.0f;
    if (*(int*)(self + 0xa8) != 0) {
        sample = fn_0051B000(*(void**)(self + 0x314), *(int*)(*(char**)(self + 0xa8) + 8));
    }
    fn_004530D0(this);
    fn_0046ADF0(this, *(float*)0x5A5740);
    if (*(int*)(self + 0x314) != 0 && *(int*)(self + 0xa8) != 0 && sample <= 0.00009999999747378752f) {
        if (*(float*)(self + 0x718) != *(float*)(self + 0x714)) {
            *(float*)(self + 0x718) = *(float*)(self + 0x714);
        }
    }
    return (int*)this;
}

float Entity::fn_0045D540(int* pos, char side) {
    char* self = (char*)this;
    int which = fn_00469010(*(int*)(self + 0x688), *(short*)(self + 0x68a));
    char* table = *(char**)(self + 0x71c);
    char* row = table + (side == 0 ? 0x1dc : 0x1a0);
    int count = *(int*)(table + 0x19a);
    float id = -1.0f;
    int sound;
    if (count == 1) {
        id = *(float*)(row + (which << 2));
    } else if (count > 1) {
        id = *(float*)(row + (fn_0056D790() % count) * 0x14 + (which << 2));
    }
    if (id < 0.0f) {
        return id;
    }
    sound = fn_00468DE0(id, pos);
    if (sound < 0) {
        return (float)sound;
    }
    {
        int span = 0;
        fn_00521800(sound, &span);
        fn_00542770(span * 0.8999999761581421f, (float)span);
    }
    return fn_005217C0(sound);
}

float Entity::fn_0045D690() {
    char* self = (char*)this;
    float pos[3];
    float extra[8];
    if (*(short*)(self + 0x678) != -1) {
        fn_0051AD00(*(void**)(self + 0x314), pos, extra, *(short*)(self + 0x678));
        pos[0] += *(float*)(self + 0x10);
        pos[1] += *(float*)(self + 0x14);
        pos[2] += *(float*)(self + 0x18);
        if (fn_0051B290(*(void**)(self + 0x314), "left-foot") != 0) {
            return fn_0045D540((int*)pos, 1);
        }
    }
    if (*(short*)(self + 0x67a) != -1) {
        fn_0051AD00(*(void**)(self + 0x314), pos, extra, *(short*)(self + 0x67a));
        pos[0] += *(float*)(self + 0x10);
        pos[1] += *(float*)(self + 0x14);
        pos[2] += *(float*)(self + 0x18);
        if (fn_0051B290(*(void**)(self + 0x314), "right-foot") != 0) {
            return fn_0045D540((int*)pos, 0);
        }
    }
    return 0.0f;
}

int Entity::fn_0045D830(int* out) {
    char* self = (char*)this;
    int state;
    float ratio;
    if (*(int*)(self + 0xc) == 9) {
        return fn_004A2870(this, *out);
    }
    state = fn_00473620(self + 0x324);
    if (*(int*)(self + 0xc) == 7 && (*(int*)(self + 0x374) & 1) != 0) {
        return 0;
    }
    if ((*(int*)(self + 0x374) & 0x80) != 0 || fn_00455950(1) != 0 || (state != 4 && state != 5)) {
        if (*(int*)(self + 0x768) == 0) {
            return 0;
        }
        if (*(int*)(*(char**)(*(char**)(self + 0x768) + 4) + 4) != 0) {
            return 0;
        }
        if (fn_0044B4F0(*(char**)(*(char**)(self + 0x768) + 4) + 0x68) != 1) {
            return 0;
        }
        *out = *(int*)(*(char**)(*(char**)(self + 0x768) + 4) + 0x74);
        return 1;
    }
    ratio = *(float*)(self + 0x5fe) / (float)fn_00462840(1);
    if (*(int*)(self + 0xc) == 7) {
        if (ratio <= 0.25f) {
            return 0;
        }
    } else if (ratio <= 0.05000000074505806f) {
        return 0;
    }
    *out = 0x41000000;
    return 1;
}

int Entity::fn_0045D980(int kind) {
    char* self = (char*)this;
    int group = fn_0047A830(kind);
    void* clip;
    if (group == -1 || fn_00455950(1) == 1) {
        return 0;
    }
    clip = fn_00461370(7, *(int*)(g_spell_rows + (kind << 7) + 0x78));
    if (clip == 0) {
        return 0;
    }
    if (*(float*)(self + 0x5fe) < *(float*)(g_spell_rows + (kind << 7) + 0x7c)) {
        return 0;
    }
    if (fn_0044DD30(this, kind) != 0) {
        return 0;
    }
    if (fn_00462440(*(int*)((char*)0x59B988 + group * 4)) < *(float*)(g_spell_rows + (kind << 7) + 0x80)) {
        return 0;
    }
    return 1;
}

int Entity::fn_0045DA60(int kind, char flag) {
    char* self = (char*)this;
    int result;
    int effect;
    void* spawned;
    if (*(int*)(self + 0xc) == 9) {
        result = *(int*)(self + 0x6cc);
    } else {
        void* clip = fn_00461370(7, *(int*)(g_spell_rows + (kind << 7) + 0x78));
        if (clip == 0) {
            return -1;
        }
        result = (int)fn_00460F20(clip, 1.0f, 0, 0, flag);
        if (result == -1) {
            return -1;
        }
    }
    if (*(int*)(self + 0xc) == 7 && (*(int*)(g_spell_rows + (kind << 7) + 0x9c) & 0x10) != 0) {
        fn_0045DD80(this, result);
    }
    effect = fn_0047A870(kind);
    if (effect != 0) {
        int played;
        if (*(int*)(*(char**)(self + 0x71c) + 0x10) != 0xc4) {
            played = fn_0048C080(effect, *(int*)(self + 0x80), -1, result, 0, 0);
        } else {
            played = fn_0048C080(effect, *(int*)(self + 0x80), *(int*)0x247FD34, result, 0, 0);
        }
        if (played == -1) {
            return -1;
        }
        fn_0047DFE0(kind, this);
        if ((unsigned)(kind - 0x18) <= 0x43) {
            fn_0047E6C0(this, played);
        }
        spawned = fn_0046B130(played);
        if (spawned == 0) {
            return -1;
        }
        *(int*)((char*)spawned + 0xa8) = kind;
        *(int*)((char*)spawned + 0xbc) = (*(int*)((char*)spawned + 0xbc) & 0xfffe0000) | 1;
        {
            void* info = fn_0051C720((*(int*)(*(char**)((char*)spawned + 0xa0)) >> 9) & 0x7fffff);
            if (info == 0) {
                return -1;
            }
            fn_0044DC70(this, kind, (int)(*(float*)((char*)info + 0x12) / 15.0f),
                *(int*)(g_spell_rows + (kind << 7) + 0x8e));
        }
        fn_004750B0(kind, played);
    }
    if (*(int*)0x599694 != 0) {
        int blocked = 0;
        if ((*(unsigned char*)0x5FBFD8 & 2) != 0 && *(int*)0x651598 != 0) {
            blocked = *(int*)(*(char**)0x651598 + 0x10) & 1;
        }
        if ((*(unsigned char*)0x5FBFD8 & 2) == 0 || *(int*)0x651598 == 0 || blocked != 0) {
            if (kind < 0x26 || kind >= 0x36) {
                fn_00450C70(this, -*(float*)(g_spell_rows + (kind << 7) + 0x7c));
            } else {
                fn_00450C70(this, -*(float*)(0x8B3F8C + (kind - 2) * 0xd * 4));
            }
        }
    }
    return result;
}

void* Entity::fn_00460F20(void* clip, float scale, int restart, int reset, int flag) {
    char* self = (char*)this;
    int current;
    int index;
    int played;
    if (clip == 0 || *(int*)((char*)clip + 8) == -1) {
        return (void*)-1;
    }
    if (fn_00473120(self + 0x324) != 0 && *(int*)(self + 0x4b8) != -1) {
        return (void*)-1;
    }
    current = (int)fn_00461500();
    if (restart == 0) {
        if (current != 0 && fn_00461620(9) != 0 && fn_00461670(9) <= 0.00009999999747378752f) {
            return (void*)-1;
        }
        fn_0048D0A0(*(int*)(self + 0x6cc));
    }
    if (reset != 0) {
        fn_0051B140(*(void**)(self + 0x314));
    }
    *(void**)(self + 0x6d0) = clip;
    *(int*)(self + 0x9c) = *(int*)0x60A328;
    *(int*)0x941708 += 1;
    if (*(int*)0x941708 < 0) {
        *(int*)0x941708 = 0;
    }
    *(int*)(self + 0x6cc) = *(int*)0x941708;
    for (index = 0; index < 3; index++) {
        *(char*)(self + index + 0xa0) = 0;
    }
    if (*(int*)((char*)clip + 0x10) == 6) {
        int type = *(int*)(*(char**)(self + 0x71c) + 0x10);
        if (type == 4 || (type >= 0 && type <= 0xca)) {
            *(int*)(self + 0x320) |= 0x8000;
            flag = 1;
        } else {
            flag = 1;
        }
    }
    fn_0051AEE0(*(void**)(self + 0x314), *(int*)((char*)clip + 8), scale, flag, 1.0f);
    fn_0051A890(*(void**)(self + 0x314), 0.0f, 0, 0);
    if (*(int*)((char*)clip + 0x38) != -1 && (*(unsigned char*)0x5FBFD8 & 0x20) == 0) {
        if (fn_00521650(*(int*)(self + 0x674)) != 0) {
            fn_00521620(*(int*)(self + 0x674), 0x64);
        }
        *(int*)(self + 0x674) = fn_00468DE0(*(float*)((char*)clip + 0x38), (int*)(self + 0x10));
    }
    if (*(int*)((char*)clip + 0x48) != 0) {
        int type = *(int*)(*(char**)(self + 0x71c) + 0x10);
        if (*(int*)((char*)clip + 0x10) != 6 || (type != 0x5a && type != 0x57 && type != 0x59 && type != 0x58)) {
            played = fn_0048C080(*(int*)((char*)clip + 0x48), *(int*)(self + 0x80), -1, -1, 0, 0);
        } else {
            played = fn_0048BD80(*(int*)((char*)clip + 0x48), (int)(self + 0x10), (int)(self + 0x38), 1);
        }
        if (played != -1 && *(int*)((char*)clip + 0x10) == 6) {
            if (*(int*)(self + 0x60c) == -1 || fn_0048D000(*(int*)(self + 0x60c), 0) != 0) {
                *(int*)(self + 0x60c) = played;
            }
        }
    }
    return (void*)*(int*)(self + 0x6cc);
}

int* Entity::fn_00460190(int* args) {
    char* self = (char*)this;
    float health = *(float*)(self + 0x5f8);
    int kind;
    if ((*(int*)(self + 0x320) & 0x80) == 0 && !(health <= 0.0f)) {
        if (args == 0) {
            return 0;
        }
    }
    if (((*(int*)(self + 0x320) & 0x80) != 0 || health <= 0.0f) &&
        (args == 0 || (*args != 5 && *args != 3 && *args != 0x69))) {
        return args;
    }
    kind = *args;
    if (kind <= 0x67 && (args[7] & 0x80) != 0) {
        int slot = fn_0047A8C0(kind);
        if ((*(unsigned char*)0x5FBFD8 & 2) != 0 && *(int*)0x651598 != 0 &&
            (*(int*)(*(char**)0x651598 + 0x10) & 1) != 0) {
            fn_00454700(slot, args[8], kind, *(float*)&args[9], args[1], 0);
        }
    }
    return args;
}

extern "C" int fn_004775C0(int layer);
extern "C" int fn_00524640(float* out);
extern "C" void fn_0050A5B0(int kind);
extern "C" void fn_00450D90(Entity* ent, int kind);

int Entity::fn_004568E0() {
    char* self = (char*)this;
    char message[0x100];
    if (*(short*)(self + 0x68a) == -1) {
        return *(short*)(self + 0x68a);
    }
    if (*(int*)(self + 0x688) != -1) {
        *(int*)(self + 0x68c) = fn_004775C0(*(int*)(self + 0x688));
    } else {
        *(int*)(self + 0x68c) = -1;
    }
    if (*(int*)(self + 0x68c) == -1) {
        *(int*)(self + 0x68c) = 0;
        sprintf(message,
            "Invalid layer for mesh %s.  It is probably not in a $layer selection set.\nThis problem "
            "should be fixed immediately.  Problem could also be a hole in the world.",
            *(char**)(0xA5A2CC + *(int*)(self + 0x688) * 0x1c));
        while (1) {
            fn_00531CB0(
                "D:\\projects\\Summoner\\pccode\\Engine\\Objects\\living_entity.cpp",
                0xf0a,
                message);
        }
    }
    return *(int*)(self + 0x68c);
}

float Entity::fn_00456ED0(float value) {
    char* self = (char*)this;
    float previous = *(float*)(self + 0x6a8);
    float local[3];
    float* velocity;
    float length;
    if (value != previous) {
        *(float*)(self + 0x6a8) = value;
        fn_0045A120();
    }
    velocity = fn_0051ABD0(*(void**)(self + 0x314), local);
    length = sqrtf(velocity[0] * velocity[0] + velocity[1] * velocity[1] + velocity[2] * velocity[2]);
    if (value != previous) {
        *(float*)(self + 0x6a8) = previous;
        fn_0045A120();
    }
    return length;
}

int Entity::fn_00456FB0(float* out) {
    char* self = (char*)this;
    float scale;
    if (fn_00462DE0(this) != 0) {
        return fn_00524640(out);
    }
    scale = *(float*)(self + 0x698);
    out[0] = scale * *(float*)(self + 0x50);
    out[1] = scale * *(float*)(self + 0x54);
    out[2] = scale * *(float*)(self + 0x58);
    return (int)out[1];
}

float Entity::fn_0045A930(float value) {
    char* self = (char*)this;
    float* velocity = fn_0051A950(*(void**)(self + 0x314));
    float speed = sqrtf(velocity[0] * velocity[0] + velocity[1] * velocity[1] + velocity[2] * velocity[2]);
    float sampled = fn_00456ED0(value);
    float accel;
    float time;
    if (sampled <= speed) {
        accel = -(*(float*)(self + 0x6b0)) * *(float*)(self + 0x6bc);
    } else {
        accel = *(float*)(*(char**)(self + 0x71c) + 0x228) * *(float*)(self + 0x6bc);
    }
    time = (sampled - speed) / accel;
    return speed * time + accel * time * time / 2.0f;
}

void Entity::fn_004616F0(int id) {
    char* self = (char*)this;
    int index;
    for (index = 0; index < 3; index++) {
        if (*(int*)(*(char**)(self + 0x6d0) + (index << 3) + 0x1c) == id) {
            *(char*)(self + index + 0xa0) = 1;
            break;
        }
    }
}

int Entity::fn_00461840() {
    return *(int*)((char*)this + 0x60c);
}

extern "C" void fn_00461890() {
    if (*(int*)0x25DB218 != 0) {
        fn_0050A5B0(0xc1);
        if ((*(int*)0x25DAE04 & 0x40) != 0) {
            *(int*)0x941704 = 1;
        } else if ((*(int*)0x25DAE04 & 0x80) != 0) {
            *(int*)0x941704 = 0;
        } else if ((*(int*)0x25DAE04 & 1) != 0) {
            *(int*)0x941704 ^= 1;
        }
    }
    if (*(int*)0x25DB2E0 != 0) {
        sprintf((char*)0x257ED80,
            "Usage: %s [bool]\nSets %s to true or false. If nothing passed, then toggles it\n",
            "immortal", "Players_immortal");
    }
    if (*(int*)0x25DADF0 == 0) {
        return;
    }
    sprintf((char*)0x257ED80, "%s is %s\n", "immortal", *(int*)0x941704 != 0 ? "TRUE" : "FALSE");
}

extern "C" void fn_004619B0() {
    Entity* target = fn_0046B1A0(*(int*)0x593F50);
    float percent;
    if (*(int*)0x25DB218 != 0) {
        fn_0050A5B0(0x10);
        percent = *(float*)0x25DACE8;
        if (percent < 0.0f || !(percent <= 1.0f)) {
            *(int*)0x25DB2E0 = 1;
            *(int*)0x25DADF0 = 0;
        } else if (target != 0) {
            *(float*)((char*)target + 0x5f8) = *(float*)(*(char**)((char*)target + 0x71c) + 0x24) * percent;
            if (percent == 0.0f) {
                fn_00450D90(target, 0x22);
            }
        }
    }
    if (*(int*)0x25DADF0 != 0) {
        if (target == 0) {
            sprintf((char*)0x257ED80, "No study object selected.\n");
        } else {
            sprintf((char*)0x257ED80, "Health: %.3f\n", *(float*)((char*)target + 0x5f8));
        }
    }
    if (*(int*)0x25DB2E0 != 0) {
        sprintf((char*)0x257ED80,
            "Usage: health <percentage> -- where percentage is the percent to set the "
            "debug\ncharacters health to (i.e. between 0.0 and 1.0).");
    }
}

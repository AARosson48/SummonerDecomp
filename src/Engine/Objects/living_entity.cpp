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
    void fn_0045DE20(int mode);
    void fn_0044DDB0();
    void fn_00450850(float value, int source, int a, int b);
    void fn_00454700(int slot, int value, int kind, float scale, int field_80, int flag);
    void fn_0045DEA0(int kind, int source, int arg3, int arg4, int damage_type, int item, int arg7, int arg8);
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

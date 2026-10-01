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
    float fn_004626B0(int which);
    void fn_0045DEA0(int kind, int source, int arg3, int arg4, int damage_type, int item, int arg7, int arg8);
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
extern "C" int fn_004A28B0(Entity* ent);
extern "C" void fn_004A13B0(Entity* ent, int a, int b, int c);
extern "C" void fn_0048C080(int a, int b, int c, int d, int e, int f);
extern "C" float fn_00524810(void* a, void* b);
extern "C" int fn_0047A8C0(int kind);
extern "C" void fn_00439580(int value);
extern "C" int printf(const char* text, ...);

extern int g_player_list;
extern int g_spell_actor;
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
        if (fn_00455950(0) || fn_00455950(1) || fn_00455950(9)) {
            fn_0048C080(g_spell_actor, *(int*)(self + 0x80), -1, -1, 0, 1);
        }
        return;
    }

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

    (void)fn_0047A8C0(kind);
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
    (void)factor;
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

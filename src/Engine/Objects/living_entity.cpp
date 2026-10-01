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

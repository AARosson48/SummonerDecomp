// Original: D:\projects\Summoner\pccode\vsdk\ca\character.cpp
// Same SDK file as Red Faction vsdk/ca/character.cpp.

struct AnimSet {
    char pad[0x28];
    int* table;
};

struct Character {
    char pad[0x1e4];
    AnimSet* current;
    void* slot[2];

    int fn_00541270();
    void fn_00541240(int index, int arg);
};

struct CharacterPool {
    void fn_00544BA0(int count);
    void fn_00544BE0();
};

extern "C" int g_anim_cursor;
extern "C" int g_anim_sentinel;
extern "C" int g_anim_flag_a;
extern "C" int g_anim_flag_b;
extern "C" int g_anim_flag_c;
extern "C" char g_anim_block[];
extern "C" char g_anim_block_end[];
extern "C" char g_characters[];
extern "C" void fn_005031C0(int entry, int arg, int mode);
extern "C" void fn_00541300(char* record);
extern "C" void fn_00541360(char* record);

int Character::fn_00541270() {
    int index = 0;
    int current = *(int*)((char*)this + 0x1e4);
    int* slots = (int*)((char*)this + 0x1e8);
    for (; index < 2; index++) {
        if (current == slots[index]) {
            return index;
        }
    }
    return -1;
}

void Character::fn_00541240(int index, int arg) {
    fn_005031C0(current->table[index + 4], arg, -1);
}

extern "C" void fn_00541300(char* record) {
    record[0] = 0;
    *(int*)(record + 0x48) = 0;
    *(int*)(record + 0x50) = 0;
    *(int*)(record + 0x1e8) = 0;
    *(int*)(record + 0x1ec) = 0;
    *(int*)(record + 0x1f0) = 0;
    *(int*)(record + 0x44) = 0;
}

extern "C" void fn_00541290(int count) {
    g_anim_cursor = 0;
    g_anim_sentinel = -1;
    g_anim_flag_a = 0;
    g_anim_flag_b = 0;
    g_anim_flag_c = -1;
    if (count == -1) {
        count = 0x1400;
    }
    ((CharacterPool*)0x2dc7070)->fn_00544BA0(count);
    for (char* block = g_anim_block; (int)block < (int)g_anim_block_end; block += 0x30) {
        *(int*)block = 0;
    }
    for (char* record = g_characters; (int)record < (int)g_anim_block; record += 0x1f8) {
        fn_00541300(record);
    }
}

extern "C" void fn_00541330() {
    for (char* record = g_characters; (int)record < (int)g_anim_block; record += 0x1f8) {
        if (*(unsigned char*)(record + 0x44) & 1) {
            fn_00541360(record);
        }
    }
    ((CharacterPool*)0x2dc7070)->fn_00544BE0();
}

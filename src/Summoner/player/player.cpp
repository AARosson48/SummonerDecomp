// Original: D:\projects\Summoner\pccode\Summoner\player\player.cpp
// Same file as Red Faction gamesrc/player/player.cpp.
// Takes a slot off the free list and puts it on the used list.

struct PlayerSlot {
    PlayerSlot* next;
    PlayerSlot* prev;
    int unused_8;
    int field_c;
    int field_10;
    int field_14;
    int field_18;
    int field_1c;
    int field_20;
    int field_24;
    char gap[0x150];
    int field_178;
    int field_17c;
    int field_180;

    void fn_00433730();
};

extern "C" PlayerSlot g_free_slots;
extern "C" PlayerSlot g_used_slots;
extern "C" void parse_fail(const char* file, int line, const char* msg);

extern "C" PlayerSlot* fn_004334F0() {
    PlayerSlot* slot = g_free_slots.next;
    if (slot == &g_free_slots) {
        for (;;) {
            parse_fail(
                "D:\\projects\\Summoner\\pccode\\Summoner\\player\\player.cpp",
                0x1A5,
                "No more player slots availeble -- something went horribly wrong -- find allender");
        }
    }
    slot->prev->next = slot->next;
    slot->next->prev = slot->prev;
    slot->next = 0;
    slot->prev = 0;
    slot->field_14 = 0;
    slot->fn_00433730();
    slot->field_c = -1;
    slot->field_10 = 0;
    slot->field_20 = 0;
    slot->field_17c = 0;
    slot->field_180 = 0;
    slot->field_24 = 0;
    slot->field_178 = 0;
    slot->prev = g_used_slots.prev;
    slot->next = &g_used_slots;
    g_used_slots.prev->next = slot;
    g_used_slots.prev = slot;
    return slot;
}

// Original bank 0x004D39C0. Reads events.tbl.
// A level block is $Level plus +Flag, +Int, and +Float lines.
// A quest is $Quest, optional +Main quest, +Stage lines, and optional +Invalidate.

#include <cctype>
#include <cstring>

struct ParseText {
    char* start;
    char* cursor;
    char file_name[0x40];
    int line;

    void fn_00515960();
    void fn_005159D0();
    int fn_00515BA0(char* dst, int flag);
    int fn_00515CE0(void* dst);
    void fn_00515D00(void* dst);
    int fn_00515D20(const char* filename, int mode);
    void fn_00515E90();
    void fn_00515EC0();
    int fn_005160D0(const char* key);
    int fn_00516110(const char* key);
    void fn_005161E0(const char* key);
    int fn_005166B0(char* dst, int max_len, char open_ch, char close_ch);
    int fn_00516C70();
};

struct LevelEvent {
    int pad[9];
    int flag_count;
    int* flags;
    int pad_2c;
    int int_count;
    int* ints;
    int float_count;
    int* floats;
};

struct QuestStage {
    int text;
    int flag;
};

struct Quest {
    int name;
    char not_main;
    char pad[0x63];
    unsigned char stage_count;
    unsigned char invalidate_at;
    QuestStage stages[12];
};

extern "C" char* fn_00505510();
extern "C" int fn_004D6300(char* name);
extern "C" int fn_004D5F50(char* name);
extern "C" int fn_004D5ED0(int id);
extern "C" int fn_004D5F10(int id);
extern "C" int fn_005051A0(char* text, int flag);

struct NameTable {
    int fn_00544CD0(char* text);
};

extern "C" LevelEvent LEVEL_EVENTS[];
extern "C" int EVENT_CURSOR;
extern "C" int EVENT_COUNT;
extern "C" int QUEST_COUNT;
extern "C" Quest QUESTS[];
extern "C" NameTable QUEST_NAMES;

static int read_quoted(ParseText* parser, char* dst, int max_len) {
    return parser->fn_005166B0(dst, max_len, '"', '"');
}

extern "C" void fn_004D39C0() {
    ParseText parser;
    char section[0x40];
    char text[0x800];
    char* language;
    int level_index;
    int flag_count;
    int int_count;
    int float_count;
    LevelEvent* level;
    Quest* quest;
    int stage;
    int id;

    parser.fn_00515960();
    parser.fn_00515D20("events.tbl", 0x98967F);
    EVENT_COUNT = parser.fn_00516C70();
    section[0] = '#';
    section[1] = 0;
    language = fn_00505510();
    strcat(section, language);
    {
        char* cursor = section;
        while (*cursor) {
            *cursor = (char)tolower((unsigned char)*cursor);
            cursor++;
        }
    }
    parser.fn_00515EC0();
    parser.fn_00515BA0(section, 0);
    parser.fn_005161E0(section);
    EVENT_CURSOR = 0;
    while (!parser.fn_00516110("#End")) {
        parser.fn_005161E0("$Level:");
        read_quoted(&parser, text, 0xFF);
        level_index = fn_004D6300(text);
        if (level_index == -1) {
            parser.fn_00515E90();
            break;
        }
        level = &LEVEL_EVENTS[level_index];
        flag_count = 0;
        int_count = 0;
        float_count = 0;
        while (!parser.fn_005160D0("$Level:") && !parser.fn_005160D0("#End")) {
            if (parser.fn_00516110("+Flag:")) {
                read_quoted(&parser, text, 0xFF);
                flag_count++;
            } else if (parser.fn_00516110("+Int:")) {
                read_quoted(&parser, text, 0xFF);
                int_count++;
            } else if (parser.fn_00516110("+Float:")) {
                read_quoted(&parser, text, 0xFF);
                float_count++;
            }
        }
        level->flags = (int*)(EVENT_CURSOR);
        EVENT_CURSOR += flag_count;
        level->ints = (int*)(EVENT_CURSOR);
        EVENT_CURSOR += int_count;
        level->floats = (int*)(EVENT_CURSOR);
        EVENT_CURSOR += float_count;
        parser.fn_00515D00(section);
        while (!parser.fn_005160D0("$Level:") && !parser.fn_005160D0("#End")) {
            if (parser.fn_00516110("+Flag:")) {
                read_quoted(&parser, text, 0xFF);
                id = fn_004D5F50(text);
                if (fn_004D5ED0(id) == 0) {
                    level->flags[level->flag_count] = id;
                    level->flag_count++;
                }
            } else if (parser.fn_00516110("+Int:")) {
                read_quoted(&parser, text, 0xFF);
                id = fn_004D5F50(text);
                if (fn_004D5F10(id) == 0) {
                    level->ints[level->int_count] = id;
                    level->int_count++;
                }
            } else if (parser.fn_00516110("+Float:")) {
                read_quoted(&parser, text, 0xFF);
                id = fn_004D5F50(text);
                if (fn_004D5F10(id) == 0) {
                    level->floats[level->float_count] = id;
                    level->float_count++;
                }
            }
        }
    }
    while (!parser.fn_00516110("#End")) {
        parser.fn_005161E0("$Quest:");
        quest = 0;
        if (QUEST_COUNT < 0xC8) {
            quest = &QUESTS[QUEST_COUNT];
            QUEST_COUNT++;
        }
        read_quoted(&parser, text, 0x7FF);
        if (quest) {
            quest->name = QUEST_NAMES.fn_00544CD0(text);
            quest->not_main = parser.fn_00516110("+Main quest") ? 0 : 1;
            quest->stage_count = 0;
            quest->invalidate_at = 0xFF;
        } else {
            parser.fn_00516110("+Main quest");
        }
        while (!parser.fn_005160D0("$Quest:") && !parser.fn_005160D0("#End")) {
            if (parser.fn_00516110("+Invalidate:")) {
                read_quoted(&parser, text, 0x7FF);
                id = fn_004D5F50(text);
                if (quest) {
                    quest->invalidate_at = quest->stage_count;
                    quest->stages[quest->stage_count].flag = id;
                    quest->stage_count++;
                }
            } else {
                parser.fn_005161E0("+Stage:");
                read_quoted(&parser, text, 0x7FF);
                fn_005051A0(text, 1);
                if (quest) {
                    stage = quest->stage_count;
                    quest->stages[stage].text = QUEST_NAMES.fn_00544CD0(text);
                    quest->stages[stage].flag = 0;
                    if (parser.fn_00516110("+Flag:")) {
                        read_quoted(&parser, text, 0x7FF);
                        quest->stages[stage].flag = fn_004D5F50(text);
                    }
                    quest->stage_count++;
                }
            }
        }
    }
    parser.fn_005159D0();
}

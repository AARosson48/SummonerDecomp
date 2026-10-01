// Original: D:\projects\Summoner\pccode\Engine\gamesound\gamesound.cpp
// Reads music.tbl, then feedback.tbl for Rosalind, Flece, Joseph, and Jekhar.

#include <cstdio>
#include <cstring>

struct ParseText {
    char* start;
    char* cursor;
    char file_name[0x40];
    int line;

    void fn_00515960();
    void fn_005159D0();
    void fn_00515D20(const char* file, int path_id);
    void fn_00515E90();
    int fn_00516110(const char* key);
    void fn_005161E0(const char* key);
    int fn_005166B0(char* dst, int max_len, char open_ch, char close_ch);
    int fn_00516290();
    int fn_005160D0(const char* token);
};

struct StringPool {
    void fn_00544BD0();
    char* fn_00544CD0(const char* text);
};

struct MusicEntry {
    char* name;
    char* filename;
};

extern "C" int g_sound_path;
extern "C" int g_music_count;
extern "C" MusicEntry g_music[];
extern "C" StringPool g_music_pool;
extern "C" char g_feedback_count[4][16];
extern "C" char g_feedback_base[4][16];
extern "C" char g_feedback_value[4][16];
extern "C" int g_feedback_total;
extern "C" int g_feedback_ids[];
extern "C" int _stricmp(const char* a, const char* b);
extern "C" int fn_005207D0(const char* path, float a, float b, float c, int d, int e);
extern "C" void fn_00469580();
extern "C" void parse_fail(const char* file, int line, const char* msg);

static const char* FEEDBACK_KEYS[] = {
    "+Affirmative:",
    "+Attack:",
    "+NoEffect:",
    "+Locked:",
    "+Injured:",
    "+InvalidAction:",
    "+InvalidLocation:",
    "+InvalidTarget:",
    "+Success:",
    "+Failed:",
    "+Poison:",
    "+HideSuccess:",
    "+SneakSuccess:",
    "+Unlocked:",
    "+LevelUp:",
    "+Death:",
};

extern "C" void fn_004691E0() {
    ParseText parser;
    char text[0x100];
    char samples[4][0x100];
    char built[0x100];
    parser.fn_00515960();
    g_music_count = 0;
    g_music_pool.fn_00544BD0();
    parser.fn_00515D20("music.tbl", g_sound_path);
    while (parser.fn_00516110("#End") == 0) {
        parser.fn_005161E0("$Name:");
        parser.fn_005166B0(text, 0x100, '"', '"');
        g_music[g_music_count].name = g_music_pool.fn_00544CD0(text);
        parser.fn_005161E0("$Filename:");
        parser.fn_005166B0(text, 0x100, '"', '"');
        g_music[g_music_count].filename = g_music_pool.fn_00544CD0(text);
        g_music_count++;
    }
    parser.fn_00515E90();
    fn_00469580();
    memset(g_feedback_count, 0, sizeof(g_feedback_count));
    parser.fn_00515D20("feedback.tbl", g_sound_path);
    while (parser.fn_00516110("#End") == 0) {
        int who;
        parser.fn_005161E0("$Name:");
        parser.fn_005166B0(text, 0x100, '"', '"');
        if (_stricmp(text, "Rosalind") == 0) {
            who = 3;
        } else if (_stricmp(text, "Flece") == 0) {
            who = 2;
        } else if (_stricmp(text, "Joseph") == 0) {
            who = 0;
        } else if (_stricmp(text, "Jekhar") == 0) {
            who = 1;
        } else {
            for (;;) {
                parse_fail(
                    "D:\\projects\\Summoner\\pccode\\Engine\\gamesound\\gamesound.cpp",
                    0x295,
                    "Bad feedback party name.\n");
            }
        }
        for (int action = 0; action < 16; action++) {
            int count = 0;
            parser.fn_005161E0(FEEDBACK_KEYS[action]);
            g_feedback_value[who][action] = (char)parser.fn_00516290();
            while (parser.fn_005160D0("\"") != 0) {
                if (count >= 4) {
                    for (;;) {
                        parse_fail(
                            "D:\\projects\\Summoner\\pccode\\Engine\\gamesound\\gamesound.cpp",
                            0x2A8,
                            "Too many feedback sounds for this action!\n");
                    }
                }
                parser.fn_005166B0(samples[count], 0x100, '"', '"');
                count++;
            }
            g_feedback_count[who][action] = (char)count;
            g_feedback_base[who][action] = (char)g_feedback_total;
            for (int sample = 0; sample < count; sample++) {
                if (g_feedback_total >= 0xFF) {
                    for (;;) {
                        parse_fail(
                            "D:\\projects\\Summoner\\pccode\\Engine\\gamesound\\gamesound.cpp",
                            0x2B7,
                            "Too many feedback sounds!  c'mon guys ..\n");
                    }
                }
                sprintf(built, "feedback\\%s", samples[sample]);
                g_feedback_ids[g_feedback_total] = fn_005207D0(built, 5.0f, 1.0f, 1.0f, 0, 0);
                g_feedback_total++;
            }
        }
    }
    parser.fn_005159D0();
}

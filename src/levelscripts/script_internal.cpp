// Original: D:\projects\Summoner\pccode\levelscripts\script_internal.cpp
// Reads one navpoint record: $Name, $Type, +Plane, $Position, $Orientation.
// $Type is one of level, cutscene, camera, ambient sound. A name that starts
// with $npc, $player, $c_npc, or $camera is used when $Type is absent.
// level and cutscene are dropped onto the ground. camera and ambient sound are not.

#include <cstdio>
#include <cstring>

struct ParseText {
    char* start;
    char* cursor;
    char file_name[0x40];
    int line;

    void fn_005161E0(const char* key);
    int fn_00516110(const char* key);
    int fn_005166B0(char* dst, int max_len, char open_ch, char close_ch);
    int fn_00516290();
    void fn_00516510(float* dst);
    void fn_005165A0(float* dst);
    int fn_00516BD0(const char** table, int count);
};

struct Basis {
    void fn_00510240(float* a, float* b, int* heading);
};

struct Sector {
    int fn_00477600(float* point, int flag);
};

extern "C" int _strnicmp(const char* a, const char* b, unsigned int n);
extern "C" void parse_fail(const char* file, int line, const char* msg);
extern "C" void fn_004AF2C0(void* probe, void* hit);
extern "C" Sector* fn_00477780(int id);

static const char* NAV_TYPE[] = {
    "level",
    "cutscene",
    "camera",
    "ambient sound",
};

struct Navpoint {
    char name[16];
    int field_10;
    float position[3];
    int plane;
    float drop;
    int kind;
    int field_2c;
};

struct GroundProbe {
    float origin[3];
    float span[2];
    float zero_14;
    float down;
    float zero_1c;
    float zero_20;
    int zero_24;
    int flags;
    int zero_2c;
};

extern "C" void fn_004DB420(Navpoint* nav, ParseText* parser) {
    float matrix[12];
    float basis_a[4];
    float basis_b[4];
    int heading;
    int kind;

    parser->fn_005161E0("$Name:");
    parser->fn_005166B0(nav->name, 0x0F, '"', '"');
    kind = 0;
    if (parser->fn_00516110("$Type:")) {
        kind = parser->fn_00516BD0(NAV_TYPE, 4);
    } else if (_strnicmp(nav->name, "$npc", 4) == 0) {
        kind = 0;
    } else if (_strnicmp(nav->name, "$player", 7) == 0) {
        kind = 0;
    } else if (_strnicmp(nav->name, "$c_npc", 6) == 0) {
        kind = 1;
    } else if (_strnicmp(nav->name, "$camera", 7) == 0) {
        kind = 2;
    } else {
        kind = 0;
    }
    nav->kind = kind;
    nav->plane = -1;
    if (parser->fn_00516110("+Plane:")) {
        nav->plane = parser->fn_00516290();
    }
    parser->fn_005161E0("$Position:");
    parser->fn_00516510(nav->position);
    parser->fn_005161E0("$Orientation:");
    parser->fn_005165A0(matrix);
    heading = 0;
    ((Basis*)matrix)->fn_00510240(basis_a, basis_b, &heading);
    nav->field_10 = heading;
    nav->drop = 0.0f;
    if (nav->kind > 3) {
        nav->field_2c = -1;
        return;
    }
    if (nav->kind <= 1) {
        GroundProbe probe;
        int hit[11];
        float point[3];
        char message[0x100];
        int i;
        Sector* sector;

        probe.origin[0] = nav->position[0];
        probe.origin[1] = nav->position[1] + 2.0f;
        probe.origin[2] = nav->position[2];
        probe.span[0] = 0.0f;
        probe.span[1] = 0.0f;
        probe.zero_14 = 0.0f;
        probe.down = -1000.0f;
        probe.zero_1c = 0.0f;
        probe.zero_20 = 0.0f;
        probe.zero_24 = 0;
        probe.flags = 0x40;
        probe.zero_2c = 0;
        for (i = 0; i < 11; i++) {
            hit[i] = 0;
        }
        fn_004AF2C0(&probe, hit);
        if (hit[2] == 0) {
            for (;;) {
                sprintf(
                    message,
                    "Couldn't collide navpoint %s with the ground -- chances are that\n"
                    "it is below ground or above a mesh not in a $layer set.",
                    nav->name);
                parse_fail(
                    "D:\\projects\\Summoner\\pccode\\levelscripts\\script_internal.cpp",
                    0x383,
                    message);
            }
        }
        point[0] = probe.origin[0] + probe.span[0] * *(float*)&hit[1];
        point[2] = probe.origin[2] + probe.span[1] * *(float*)&hit[1];
        point[1] = probe.origin[1] + probe.down * *(float*)&hit[2];
        nav->drop = nav->position[1] - point[1];
        sector = fn_00477780(nav->plane);
        nav->plane = sector->fn_00477600(point, 1);
        if (nav->drop < 0.0f) {
            nav->position[0] = point[0];
            nav->position[1] = point[1];
            nav->position[2] = point[2];
            nav->drop = 0.0f;
        }
    } else {
        nav->plane = -1;
    }
    nav->field_2c = -1;
}

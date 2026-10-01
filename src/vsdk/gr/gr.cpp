// Original: D:\projects\Summoner\pccode\vsdk\gr\gr.cpp
// Same SDK file as Red Faction vsdk/gr/gr.cpp.
// The small entry points: dual-head Glide env check, and the mode accessors.

extern "C" int g_window_mode;
extern "C" char g_sst_dualhead;
extern "C" char g_gr_byte;
extern "C" int g_gr_value_a;
extern "C" int g_gr_value_b;
extern "C" int (__cdecl* g_get_env)(const char* name, char* buffer, int size);
extern "C" int fn_0056E311(const char* text);

extern "C" void fn_00506110() {
    if (g_window_mode == 0xC9) {
        char buffer[0x20];
        int length = g_get_env("SST_DUALHEAD", buffer, 0x1F);
        g_sst_dualhead = 0;
        if (length >= 1 && fn_0056E311(buffer) != 0) {
            g_sst_dualhead = 1;
        }
    }
}

extern "C" void fn_00506160(char value) {
    g_gr_byte = value;
}

extern "C" int fn_00506170() {
    return g_gr_value_a;
}

extern "C" int fn_00506180() {
    return g_gr_value_b;
}

// Original: D:\projects\Summoner\pccode\vsdk\gr\gr.cpp
// Same SDK file as Red Faction vsdk/gr/gr.cpp.
// The small entry points: dual-head Glide env check, and the mode accessors.

extern "C" int fn_0056E311(const char* text);
extern "C" __declspec(dllimport) unsigned int __stdcall GetEnvironmentVariableA(
    const char* name,
    char* buffer,
    unsigned int size);

extern "C" void fn_00506110() {
    if (*(int*)0x25d8d40 == 0xC9) {
        char buffer[0x20];
        unsigned int length = GetEnvironmentVariableA("SST_DUALHEAD", buffer, 0x1F);
        *(unsigned char*)0x25d8d44 = 0;
        if (length >= 1 && fn_0056E311(buffer) != 0) {
            *(unsigned char*)0x25d8d44 = 1;
        }
    }
}

extern "C" void fn_00506160(unsigned char value) {
    *(unsigned char*)0x25d8dd0 = value;
}

extern "C" int fn_00506170() {
    return *(int*)0x25d8d34;
}

extern "C" int fn_00506180() {
    return *(int*)0x25d8d38;
}

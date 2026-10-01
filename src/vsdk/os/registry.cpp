// Original: D:\projects\Summoner\pccode\vsdk\os\registry.cpp
// Turns an HKEY_ name into the Win32 root key. Anything else is a parse failure.

#include <cstdio>

extern "C" int _stricmp(const char* a, const char* b);
extern "C" void fn_00531CB0(const char* file, int line, const char* msg);

extern "C" unsigned fn_00511F90(const char* name) {
    char message[0x100];
    if (_stricmp(name, "HKEY_CLASSES_ROOT") == 0) {
        return 0x80000000u;
    }
    if (_stricmp(name, "HKEY_CURRENT_USER") == 0) {
        return 0x80000001u;
    }
    if (_stricmp(name, "HKEY_LOCAL_MACHINE") == 0) {
        return 0x80000002u;
    }
    if (_stricmp(name, "HKEY_USERS") == 0) {
        return 0x80000003u;
    }
    sprintf(message, "Bogus HKEY_ string: %s", name);
    for (;;) {
        fn_00531CB0("D:\\projects\\Summoner\\pccode\\vsdk\\os\\registry.cpp", 0xB9, message);
    }
}

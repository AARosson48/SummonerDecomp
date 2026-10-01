// Original: D:\projects\Summoner\pccode\Engine\characterinfo\characterinfo.cpp
// The 5200-byte reader in this file still has no C. These are the small ones beside it.

#include <stdlib.h>

struct InfoRow {
    char raw[0x3EC];
};

extern "C" InfoRow g_info_rows[4];
extern "C" int fn_0056CEBE(int ch);

extern "C" int fn_00447E80(const char* text) {
    unsigned hash;
    if (text == 0) {
        return -1;
    }
    hash = 0;
    while (*text != 0) {
        int ch = fn_0056CEBE((int)(signed char)*text);
        hash = _rotl(hash, 6);
        hash ^= (unsigned)ch;
        text++;
    }
    return (int)hash;
}

extern "C" InfoRow* fn_00446DE0(char* slot) {
    int index;
    for (index = 0; index < 4; index++) {
        if ((char*)&g_info_rows[index] + 4 == slot) {
            return &g_info_rows[index];
        }
    }
    return 0;
}

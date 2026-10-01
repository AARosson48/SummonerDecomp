// Original: D:\projects\Summoner\pccode\vsdk\os\stringpool.cpp
// Copies a string into the pool and returns the copy. A null argument returns null.

#include <cstring>

struct StringPool {
    char* base;
    int unused;
    int capacity;
    int used;
    int count;

    char* fn_00544C40(const char* text);
};

extern "C" void parse_fail(const char* file, int line, const char* msg);

char* StringPool::fn_00544C40(const char* text) {
    if (text == 0) {
        return 0;
    }
    int len = (int)strlen(text);
    char* dest = base + used;
    int next = used + len + 1;
    if (next > capacity) {
        for (;;) {
            parse_fail(
                "D:\\projects\\Summoner\\pccode\\vsdk\\os\\stringpool.cpp",
                0xE9,
                "Length of string pool exceeded.");
        }
    }
    used = next;
    memcpy(dest, text, len + 1);
    count++;
    return dest;
}

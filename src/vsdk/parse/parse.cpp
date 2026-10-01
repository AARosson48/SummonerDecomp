// Original: D:\projects\Summoner\pccode\vsdk\parse\parse.cpp
// The table scanner. Level scripts call it for $Name, $Character, +Monster.

#include <cstdio>
#include <cstring>

struct ParseText {
    char* start;
    char* cursor;
    char file_name[0x40];
    int line;

    void fn_005159E0();
    void fn_00515A80();
    int fn_00516110(const char* key);
    void fn_00515EE0(const char* expected);
    void fn_005161E0(const char* key);
    int fn_005166B0(char* dst, int max_len, char open_ch, char close_ch);
};

extern "C" int _strnicmp(const char* a, const char* b, unsigned int n);
extern "C" void parse_fail(const char* file, int line, const char* msg);

// Skip a /* block. A CR counts a line. Stop on */ or a NUL.
void ParseText::fn_005159E0() {
    char* p = cursor;
    if (*p != 0) {
        for (;;) {
            char ch = *p;
            if (ch == '*' && p[1] == '/') {
                break;
            }
            if (ch == '\r') {
                line++;
            }
            char next = p[1];
            p++;
            cursor = p;
            if (next == 0) {
                break;
            }
        }
    }
    p = cursor;
    if (*p != 0) {
        cursor = p + 2;
    }
}

// Skip spaces, tabs, newlines, // comments, and /* comments.
void ParseText::fn_00515A80() {
    for (;;) {
        char* p = cursor;
        unsigned char ch = (unsigned char)*p;
        if (ch == 0) {
            return;
        }
        if (ch == ' ' || (ch >= 9 && ch < 0x0D)) {
            cursor = p + 1;
            continue;
        }
        if (ch == 0x0D) {
            cursor = p + 1;
            line++;
            continue;
        }
        if (ch != '/') {
            return;
        }
        if (p[1] == '/') {
            for (;;) {
                p = cursor;
                if (*p == '\r') {
                    break;
                }
                char next = p[1];
                cursor = p + 1;
                if (next == 0) {
                    break;
                }
            }
            continue;
        }
        if (p[1] != '*') {
            return;
        }
        cursor = p + 2;
        fn_005159E0();
    }
}

// True when the next token is key. The compare is case-insensitive.
int ParseText::fn_00516110(const char* key) {
    const char* end = key;
    while (*end) {
        end++;
    }
    int length = (int)(end - key);
    fn_00515A80();
    int matched = _strnicmp(cursor, key, (unsigned int)length) == 0;
    if (matched) {
        cursor += length;
    }
    return matched;
}

void ParseText::fn_00515EE0(const char* expected) {
    char found[0x40];
    char prior[0x40];
    char message[0x100];
    char* p = cursor;
    int i = 0;
    while (i < 0x28) {
        unsigned char ch = (unsigned char)p[i];
        found[i] = ch < 0x20 ? (char)0x16 : (char)ch;
        i++;
    }
    found[i] = 0;
    char* from = p - 0x28;
    if (from < start) {
        from = start;
    }
    int n = 0;
    while (from < p && n < 0x28) {
        unsigned char ch = (unsigned char)*from;
        prior[n] = ch < 0x20 ? (char)0x16 : (char)ch;
        from++;
        n++;
    }
    prior[n] = 0;
    for (;;) {
        sprintf(
            message,
            "Parsing file \"%s\", at line #%d\nExpected %s but found this text:\n\n[%s]\n\nPrior text was this:\n\n[%s]\n",
            file_name,
            line,
            expected,
            found,
            prior);
        parse_fail("D:\\projects\\Summoner\\pccode\\vsdk\\parse\\parse.cpp", 0x246, message);
    }
}

void ParseText::fn_005161E0(const char* key) {
    char shown[0x100];
    if (fn_00516110(key)) {
        return;
    }
    sprintf(shown, "\"%s\"", key);
    fn_00515EE0(shown);
}

// Read a quoted table field. open_ch and close_ch are both '"' for $Name values.
int ParseText::fn_005166B0(char* dst, int max_len, char open_ch, char close_ch) {
    char message[0x100];
    fn_00515A80();
    if (*cursor != open_ch) {
        fn_00515EE0("a string");
    }
    cursor++;
    int length = 0;
    char ch = *cursor;
    while (ch != 0 && ch != '\r' && ch != close_ch) {
        length++;
        ch = cursor[length];
    }
    if (cursor[length] != close_ch) {
        fn_00515EE0("a string terminator");
    }
    if (length >= max_len) {
        for (;;) {
            sprintf(
                message,
                "Not enough storage provided (%d) for parsed C string (len=%d)",
                max_len,
                length);
            parse_fail("D:\\projects\\Summoner\\pccode\\vsdk\\parse\\parse.cpp", 0x493, message);
        }
    }
    strncpy(dst, cursor, (size_t)length);
    cursor += length + 1;
    dst[length] = 0;
    return length;
}

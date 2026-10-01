// Original: D:\projects\Summoner\pccode\vsdk\os\cmdline.cpp
// Same SDK file as Red Faction vsdk/os/cmdline.cpp.
// Walks the argument list. A registered option may take the next argument.
// An argument that starts with '-' and is not registered is a parse failure.

#include <cstdio>

struct CmdOption {
    CmdOption* next;
    int unused;
    const char* name;
    char pad[9];
    char takes_value;
};

struct ArgSlot {
    char* text;
    int unused;
};

extern "C" CmdOption g_cmd_options;
extern "C" int g_cmd_argc;
extern "C" ArgSlot g_cmd_argv[];
extern "C" int _stricmp(const char* a, const char* b);
extern "C" void parse_fail(const char* file, int line, const char* msg);

extern "C" void fn_0053E310() {
    char message[0x100];
    int index;

    index = 0;
    if (g_cmd_argc <= 0) {
        return;
    }
    while (index < g_cmd_argc) {
        CmdOption* option = g_cmd_options.next;
        int matched = 0;
        while (option != &g_cmd_options) {
            if (_stricmp(g_cmd_argv[index].text, option->name) == 0) {
                if (option->takes_value) {
                    index++;
                }
                matched = 1;
                break;
            }
            option = option->next;
        }
        if (!matched && g_cmd_argv[index].text[0] == '-') {
            for (;;) {
                sprintf(message, "Unrecognized command line parameter %s", g_cmd_argv[index].text);
                parse_fail(
                    "D:\\projects\\Summoner\\pccode\\vsdk\\os\\cmdline.cpp",
                    0x104,
                    message);
            }
        }
        index++;
    }
}

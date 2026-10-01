// Original: D:\projects\Summoner\pccode\vsdk\gr\opengl\gr_opengl.cpp
// Checks the window mode, then records whether packed pixels are available.

#include <cstring>

extern "C" int g_window_mode;
extern "C" const char* (__cdecl* g_gl_query)(unsigned name);
extern "C" char g_packed_pixel;
extern "C" void fn_0054F410(int mode_arg);
extern "C" void parse_fail(const char* file, int line, const char* msg);

extern "C" void fn_0054F6F0(int mode_arg) {
    switch (g_window_mode) {
    case 0xC8:
        fn_0054F410(mode_arg);
        break;
    case 0xC9:
        for (;;) {
            parse_fail(
                "D:\\projects\\Summoner\\pccode\\vsdk\\gr\\opengl\\gr_opengl.cpp",
                0x291,
                "OpenGl can only be used in windowed modes for now.\n");
        }
    case 0xCA:
        break;
    default:
        for (;;) {
            parse_fail(
                "D:\\projects\\Summoner\\pccode\\vsdk\\gr\\opengl\\gr_opengl.cpp",
                0x29C,
                "Invalid window_mode passed to gr_direct3d_init\n");
        }
    }

    const char* version = g_gl_query(0x1F02);
    const char* extensions = g_gl_query(0x1F03);
    if (strstr(version, "1.2") != 0 || strstr(extensions, "GL_APPLE_packed_pixel") != 0) {
        g_packed_pixel = 1;
    } else {
        g_packed_pixel = 0;
    }
}

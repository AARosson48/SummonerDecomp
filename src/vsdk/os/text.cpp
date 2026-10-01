// Text object recovered from Sum.exe VA 0x5566E0-0x557C90.
// Not byte-matched. Layout is { int length; char* data }.

#if defined(_WIN64) || defined(__x86_64__) || defined(__amd64__)
#error Sum.exe is a 32-bit image
#endif

#include "vfs.h"

#include <cstdio>
#include <cstring>

extern int vfs_tolower(int value);
extern int vfs_toupper(int value);
extern int vfs_stricmp(const char* a, const char* b);
extern int vfs_strnicmp(const char* a, const char* b, unsigned int count);
extern double vfs_atof(const char* text);

static char g_text_empty[] = "";
static char g_text_hole;

static int text_ws(unsigned char ch) {
    return ch == ' ' || ch == '\t' || ch == '\n';
}

static void text_copy_z(char* dst, const char* src) {
    std::memcpy(dst, src, std::strlen(src) + 1);
}

static void text_append_z(char* dst, const char* src) {
    text_copy_z(dst + std::strlen(dst), src);
}

void vfs_text_clear(VfsText* text) {
    if (text->data) {
        vfs_heap_free(text->data);
        text->data = 0;
    }
}

void vfs_text_set(VfsText* text, const char* src, int max_len) {
    vfs_text_clear(text);
    int length = 0;
    if (src) {
        length = (int)std::strlen(src);
    }
    if (max_len >= 0 && max_len < length) {
        length = max_len;
    }
    text->length = length;
    if (!length) {
        text->data = 0;
        return;
    }
    char* data = (char*)vfs_heap_alloc(length + 1);
    text->data = data;
    std::memcpy(data, src, (size_t)length);
    data[length] = 0;
}

void vfs_text_set_char(VfsText* text, char value) {
    vfs_text_clear(text);
    text->length = 1;
    char* data = (char*)vfs_heap_alloc(2);
    text->data = data;
    data[0] = value;
    data[1] = 0;
}

void vfs_text_resize(VfsText* text, int length) {
    vfs_text_clear(text);
    text->length = length;
    if (!length) {
        text->data = 0;
        return;
    }
    char* data = (char*)vfs_heap_alloc(length + 1);
    text->data = data;
    data[0] = 0;
}

VfsText* vfs_text_ctor(VfsText* text) {
    text->data = 0;
    vfs_text_set(text, g_text_empty, -1);
    return text;
}

VfsText* vfs_text_ctor_str(VfsText* text, const char* src) {
    text->data = 0;
    vfs_text_set(text, src, -1);
    return text;
}

VfsText* vfs_text_ctor_char(VfsText* text, char value) {
    text->data = 0;
    vfs_text_set_char(text, value);
    return text;
}

VfsText* vfs_text_ctor_copy(VfsText* text, const VfsText* other) {
    text->data = 0;
    vfs_text_set(text, other->data, -1);
    return text;
}

VfsText* vfs_text_ctor_len(VfsText* text, int length) {
    text->data = 0;
    if (length < 0) {
        vfs_text_resize(text, 0);
    } else {
        vfs_text_resize(text, length);
    }
    return text;
}

void vfs_text_dtor(VfsText* text) {
    vfs_text_clear(text);
}

const char* vfs_text_cstr(VfsText* text) {
    if (text->data) {
        return text->data;
    }
    return g_text_empty;
}

int vfs_text_size(VfsText* text) {
    if (!text->data) {
        return 0;
    }
    return (int)std::strlen(text->data);
}

int vfs_text_empty(VfsText* text) {
    if (text->length <= 0) {
        return 1;
    }
    return text->data[0] == 0;
}

int vfs_text_blank(VfsText* text) {
    if (vfs_text_empty(text)) {
        return 1;
    }
    for (int i = 0; i < text->length; i++) {
        unsigned char ch = (unsigned char)text->data[i];
        if (ch != ' ' && ch != '\t' && ch != '\n') {
            return 0;
        }
    }
    return 1;
}

void vfs_text_lower(VfsText* text) {
    for (int i = 0; i < text->length; i++) {
        text->data[i] = (char)vfs_tolower((signed char)text->data[i]);
    }
}

void vfs_text_upper(VfsText* text) {
    for (int i = 0; i < text->length; i++) {
        text->data[i] = (char)vfs_toupper((signed char)text->data[i]);
    }
}

VfsText* vfs_text_assign(VfsText* text, const char* src) {
    if (!src) {
        vfs_text_clear(text);
        vfs_text_set(text, g_text_empty, -1);
        return text;
    }
    if (src == text->data) {
        return text;
    }
    int length = (int)std::strlen(src);
    if (length != text->length) {
        vfs_text_clear(text);
        vfs_text_set(text, src, -1);
        return text;
    }
    if (!text->data) {
        return text;
    }
    std::memcpy(text->data, src, (size_t)length + 1);
    return text;
}

VfsText* vfs_text_assign_char(VfsText* text, char value) {
    if (text->length == 1) {
        text->data[0] = value;
        text->data[1] = 0;
        return text;
    }
    vfs_text_clear(text);
    vfs_text_set_char(text, value);
    return text;
}

VfsText* vfs_text_slice(VfsText* text, VfsText* out, int start, int end) {
    VfsText local;
    if (end == -1) {
        end = text->length - 1;
    }
    int count = end - start + 1;
    vfs_text_ctor_len(&local, count);
    std::strncpy(local.data, text->data + start, (size_t)count);
    local.data[count] = 0;
    vfs_text_ctor_copy(out, &local);
    vfs_text_dtor(&local);
    return out;
}

VfsText* vfs_text_left(VfsText* text, VfsText* out, int count) {
    VfsText local;
    vfs_text_ctor(&local);
    if (count >= text->length) {
        vfs_text_set(&local, text->data, -1);
    } else if (count > 0) {
        VfsText part;
        vfs_text_slice(text, &part, 0, count - 1);
        vfs_text_assign_text(&local, &part);
        vfs_text_dtor(&part);
    }
    vfs_text_ctor_copy(out, &local);
    vfs_text_dtor(&local);
    return out;
}

VfsText* vfs_text_right(VfsText* text, VfsText* out, int count) {
    VfsText local;
    vfs_text_ctor(&local);
    if (count >= text->length) {
        vfs_text_set(&local, text->data, -1);
    } else if (count > 0) {
        int n = count + 1;
        vfs_text_resize(&local, n);
        std::strncpy(local.data, text->data + (text->length - count), (size_t)n);
    }
    vfs_text_ctor_copy(out, &local);
    vfs_text_dtor(&local);
    return out;
}

VfsText* vfs_text_mid(VfsText* text, VfsText* out, int start, int count) {
    VfsText local;
    vfs_text_ctor_len(&local, count);
    if (start + count > text->length) {
        count = text->length - start;
    }
    if (count > 0) {
        std::strncpy(local.data, text->data + start, (size_t)count);
        local.data[count] = 0;
    }
    vfs_text_ctor_copy(out, &local);
    vfs_text_dtor(&local);
    return out;
}

int vfs_text_find(VfsText* text, const char* needle, int from_end) {
    if (!needle) {
        return 0;
    }
    int nlen = (int)std::strlen(needle);
    if (!nlen) {
        return 0;
    }
    if ((unsigned char)from_end) {
        if (text->length == 0) {
            return -1;
        }
        int index = text->length - 1;
        for (;;) {
            if (vfs_strnicmp(text->data + index, needle, (unsigned int)nlen) == 0) {
                return index;
            }
            int prev = index;
            index--;
            if (!prev) {
                break;
            }
        }
        return -1;
    }
    if (text->length <= 0) {
        return -1;
    }
    for (int index = 0; index < text->length; index++) {
        if (vfs_strnicmp(text->data + index, needle, (unsigned int)nlen) == 0) {
            return index;
        }
    }
    return -1;
}

void vfs_text_trim_left(VfsText* text) {
    if (!text->data) {
        return;
    }
    int index = 0;
    while (index < text->length && text_ws((unsigned char)text->data[index])) {
        index++;
    }
    if (index == text->length) {
        vfs_text_clear(text);
        return;
    }
    if (index <= 0) {
        return;
    }
    char* src = text->data + index;
    std::memmove(text->data, src, std::strlen(src) + 1);
}

void vfs_text_trim_right(VfsText* text) {
    if (!text->data) {
        return;
    }
    int index = text->length - 1;
    if (index < 0) {
        vfs_text_clear(text);
        return;
    }
    while (index >= 0 && text_ws((unsigned char)text->data[index])) {
        index--;
    }
    if (index < 0) {
        vfs_text_clear(text);
        return;
    }
    text->data[index + 1] = 0;
}

float vfs_text_float(VfsText* text) {
    if (!text->data) {
        return 0.0f;
    }
    return (float)vfs_atof(text->data);
}

void vfs_text_trim(VfsText* text) {
    vfs_text_trim_right(text);
    vfs_text_trim_left(text);
}

int vfs_text_is_empty(VfsText* text) {
    return vfs_text_empty(text);
}

VfsText* vfs_text_assign_text(VfsText* text, const VfsText* other) {
    if (other == text) {
        return text;
    }
    if (other->length == text->length) {
        if (!text->length) {
            return text;
        }
        text_copy_z(text->data, other->data);
        return text;
    }
    vfs_text_clear(text);
    vfs_text_set(text, other->data, -1);
    return text;
}

VfsText* vfs_text_concat(VfsText* out, const VfsText* left, const VfsText* right) {
    VfsText local;
    vfs_text_ctor_len(&local, left->length + right->length);
    if (local.length) {
        if (left->length > 0) {
            text_copy_z(local.data, left->data);
            if (right->length > 0) {
                text_append_z(local.data, right->data);
            }
        } else {
            text_copy_z(local.data, right->data);
        }
    }
    vfs_text_ctor_copy(out, &local);
    vfs_text_dtor(&local);
    return out;
}

VfsText* vfs_text_concat_cstr(VfsText* out, const VfsText* text, const char* src) {
    if (!src) {
        vfs_text_ctor_copy(out, text);
        return out;
    }
    VfsText local;
    vfs_text_ctor_len(&local, (int)std::strlen(src) + text->length);
    if (local.length) {
        if (text->data) {
            text_copy_z(local.data, text->data);
        }
        text_append_z(local.data, src);
    }
    vfs_text_ctor_copy(out, &local);
    vfs_text_dtor(&local);
    return out;
}

VfsText* vfs_text_concat_cstr_left(VfsText* out, const char* src, const VfsText* text) {
    if (!src) {
        vfs_text_ctor_copy(out, text);
        return out;
    }
    VfsText local;
    vfs_text_ctor_len(&local, (int)std::strlen(src) + text->length);
    if (local.length) {
        text_copy_z(local.data, src);
        if (text->data) {
            text_append_z(local.data, text->data);
        }
    }
    vfs_text_ctor_copy(out, &local);
    vfs_text_dtor(&local);
    return out;
}

VfsText* vfs_text_concat_char(VfsText* out, const VfsText* text, char value) {
    int n = 0;
    if (text->data) {
        n = (int)std::strlen(text->data);
    }
    VfsText local;
    vfs_text_ctor_len(&local, n + 1);
    if (n) {
        text_copy_z(local.data, text->data);
    }
    local.data[n] = value;
    local.data[n + 1] = 0;
    vfs_text_ctor_copy(out, &local);
    vfs_text_dtor(&local);
    return out;
}

VfsText* vfs_text_concat_char_left(VfsText* out, char value, const VfsText* text) {
    int n = 0;
    if (text->data) {
        n = (int)std::strlen(text->data);
    }
    VfsText local;
    vfs_text_ctor_len(&local, n + 1);
    *vfs_text_at(&local, 0) = value;
    text_copy_z(local.data + 1, text->data);
    vfs_text_ctor_copy(out, &local);
    vfs_text_dtor(&local);
    return out;
}

VfsText* vfs_text_append(VfsText* text, const VfsText* other) {
    VfsText local;
    vfs_text_concat(&local, text, other);
    vfs_text_assign_text(text, &local);
    vfs_text_dtor(&local);
    return text;
}

VfsText* vfs_text_append_cstr(VfsText* text, const char* src) {
    VfsText local;
    vfs_text_concat_cstr(&local, text, src);
    vfs_text_assign_text(text, &local);
    vfs_text_dtor(&local);
    return text;
}

VfsText* vfs_text_append_char(VfsText* text, char value) {
    VfsText local;
    vfs_text_concat_char(&local, text, value);
    vfs_text_assign_text(text, &local);
    vfs_text_dtor(&local);
    return text;
}

static int cstr_blank(const char* src) {
    return !src || src[0] == 0;
}

int vfs_text_eq(const VfsText* left, const VfsText* right) {
    if (!left->length && !right->length) {
        return 1;
    }
    if (!left->length || !right->length) {
        return 0;
    }
    return vfs_stricmp(left->data, right->data) == 0;
}

int vfs_text_eq_cstr(const char* src, const VfsText* text) {
    if (cstr_blank(src) && !text->length) {
        return 1;
    }
    if (!src || !text->length) {
        return 0;
    }
    return vfs_stricmp(src, text->data) == 0;
}

int vfs_text_eq_rcstr(const VfsText* text, const char* src) {
    if (!text->length && cstr_blank(src)) {
        return 1;
    }
    if (!text->length || !src) {
        return 0;
    }
    return vfs_stricmp(text->data, src) == 0;
}

int vfs_text_ne(const VfsText* left, const VfsText* right) {
    if (!left->length && !right->length) {
        return 0;
    }
    if (!left->length || !right->length) {
        return 1;
    }
    return vfs_stricmp(left->data, right->data) != 0;
}

int vfs_text_ne_cstr(const char* src, const VfsText* text) {
    if (cstr_blank(src) && !text->length) {
        return 0;
    }
    if (!src || !text->length) {
        return 1;
    }
    return vfs_stricmp(src, text->data) != 0;
}

int vfs_text_ne_rcstr(const VfsText* text, const char* src) {
    if (!text->length && cstr_blank(src)) {
        return 0;
    }
    if (!text->length || !src) {
        return 1;
    }
    return vfs_stricmp(text->data, src) != 0;
}

int vfs_text_lt(const VfsText* left, const VfsText* right) {
    if (!left->length && !right->length) {
        return 0;
    }
    if (!left->length || !right->length) {
        return left->length == 0;
    }
    return vfs_stricmp(left->data, right->data) < 0;
}

int vfs_text_lt_cstr(const char* src, const VfsText* text) {
    if (cstr_blank(src) && !text->length) {
        return 0;
    }
    if (!src || !text->length) {
        return text->length > 0;
    }
    return vfs_stricmp(src, text->data) < 0;
}

int vfs_text_lt_rcstr(const VfsText* text, const char* src) {
    if (!text->length && cstr_blank(src)) {
        return 0;
    }
    if (!text->length || !src) {
        return text->length == 0;
    }
    return vfs_stricmp(text->data, src) < 0;
}

int vfs_text_gt(const VfsText* left, const VfsText* right) {
    if (!left->length && !right->length) {
        return 0;
    }
    if (!left->length || !right->length) {
        return right->length == 0;
    }
    return vfs_stricmp(left->data, right->data) > 0;
}

int vfs_text_gt_cstr(const char* src, const VfsText* text) {
    if (cstr_blank(src) && !text->length) {
        return 0;
    }
    if (!src || !text->length) {
        return text->length == 0;
    }
    return vfs_stricmp(src, text->data) > 0;
}

int vfs_text_gt_rcstr(const VfsText* text, const char* src) {
    if (!text->length && cstr_blank(src)) {
        return 0;
    }
    if (!text->length || !src) {
        return text->length > 0;
    }
    return vfs_stricmp(text->data, src) > 0;
}

int vfs_text_le(const VfsText* left, const VfsText* right) {
    if (!left->length && !right->length) {
        return 1;
    }
    if (!left->length || !right->length) {
        return right->length > 0;
    }
    return vfs_stricmp(left->data, right->data) <= 0;
}

int vfs_text_le_cstr(const char* src, const VfsText* text) {
    if (cstr_blank(src) && !text->length) {
        return 1;
    }
    if (!src || !text->length) {
        return text->length > 0;
    }
    return vfs_stricmp(src, text->data) <= 0;
}

int vfs_text_le_rcstr(const VfsText* text, const char* src) {
    if (!text->length && cstr_blank(src)) {
        return 1;
    }
    if (!text->length || !src) {
        return text->length != 0;
    }
    return vfs_stricmp(text->data, src) <= 0;
}

int vfs_text_ge(const VfsText* left, const VfsText* right) {
    if (!left->length && !right->length) {
        return 1;
    }
    if (!left->length || !right->length) {
        return left->length != 0;
    }
    return vfs_stricmp(left->data, right->data) >= 0;
}

int vfs_text_ge_cstr(const char* src, const VfsText* text) {
    if (cstr_blank(src) && !text->length) {
        return 1;
    }
    if (!src || !text->length) {
        return text->length == 0;
    }
    return vfs_stricmp(src, text->data) >= 0;
}

int vfs_text_ge_rcstr(const VfsText* text, const char* src) {
    if (!text->length && cstr_blank(src)) {
        return 1;
    }
    if (!text->length || !src) {
        return text->length != 0;
    }
    return vfs_stricmp(text->data, src) >= 0;
}

char* vfs_text_at(VfsText* text, int index) {
    if (index < 0 || index >= text->length) {
        g_text_hole = 0;
        return &g_text_hole;
    }
    return text->data + index;
}

VfsText* vfs_text_from_int(VfsText* out, int value) {
    char buf[0x40];
    std::sprintf(buf, "%d", value);
    VfsText local;
    vfs_text_ctor_str(&local, buf);
    vfs_text_ctor_copy(out, &local);
    vfs_text_dtor(&local);
    return out;
}

VfsText* vfs_text_from_hex(VfsText* out, unsigned int value) {
    char buf[0x40];
    std::sprintf(buf, "0x%x", value);
    VfsText local;
    vfs_text_ctor_str(&local, buf);
    vfs_text_ctor_copy(out, &local);
    vfs_text_dtor(&local);
    return out;
}

VfsText* vfs_text_from_float(VfsText* out, float value, int precision) {
    char buf[0x40];
    std::sprintf(buf, "%.*f", precision, (double)value);
    VfsText local;
    vfs_text_ctor_str(&local, buf);
    vfs_text_ctor_copy(out, &local);
    vfs_text_dtor(&local);
    return out;
}

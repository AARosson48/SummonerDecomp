// Original: D:\projects\Summoner\pccode\vsdk\math\matrix.cpp
// Same SDK file as Red Faction vsdk/math/matrix.cpp.
// Rotation matrix to an axis and an angle. A near-zero angle uses +Z.
// A near-180 angle takes the axis from the largest diagonal.

#include <math.h>

struct Vec3 {
    float x;
    float y;
    float z;

    void fn_00511C10(float x, float y, float z);
    void fn_00524430();
};

struct Mat3 {
    float m00, m01, m02;
    float m10, m11, m12;
    float m20, m21, m22;

    void fn_00510390(Vec3* axis, float* angle);
};

extern "C" void parse_fail(const char* file, int line, const char* msg);

static const float kAlmostOne = 0.9999998807907104f;

void Mat3::fn_00510390(Vec3* axis, float* angle) {
    float trace = ((m11 + m00) + m22 - 1.0f) * 0.5f;
    if (trace > kAlmostOne) {
        *angle = 0.0f;
        axis->fn_00511C10(0.0f, 0.0f, 1.0f);
        return;
    }
    if (trace > -kAlmostOne) {
        *angle = (float)acos(trace);
        axis->x = m12 - m21;
        axis->y = m20 - m02;
        axis->z = m01 - m10;
        axis->fn_00524430();
        return;
    }

    *angle = 3.14159265358979323846f;
    int index = 0;
    if (m11 > m00) {
        index = 1;
        if (m22 > m11) {
            index = 2;
        }
    } else if (m22 > m00) {
        index = 2;
    }

    switch (index) {
    case 0: {
        float scale = (float)sqrt(m00 + 1.0f);
        axis->x = scale;
        axis->y = m01 / scale;
        axis->z = m02 / scale;
        axis->fn_00524430();
        return;
    }
    case 1: {
        float scale = (float)sqrt(m11 + 1.0f);
        axis->y = scale;
        axis->x = m10 / scale;
        axis->z = m12 / scale;
        axis->fn_00524430();
        return;
    }
    case 2: {
        float scale = (float)sqrt(m22 + 1.0f);
        axis->z = scale;
        axis->x = m20 / scale;
        axis->y = m21 / scale;
        axis->fn_00524430();
        return;
    }
    default:
        for (;;) {
            parse_fail(
                "D:\\projects\\Summoner\\pccode\\vsdk\\math\\matrix.cpp",
                0x1F9,
                "invalid index - this should never happen");
        }
    }
}

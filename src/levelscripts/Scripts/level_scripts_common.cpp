// Original: D:\projects\Summoner\pccode\levelscripts\Scripts\level_scripts_common.cpp
// The init calls the navpoint reader, the level-item reader, and the
// masad.tbl reader. The player-start lookup uses the marker names stored
// in the .s3d: $player1-01, then $player01 if that set is missing.

struct Obj4902A8 {
    void fn_00501880(int count);
};

struct Obj4F1A0 {
    void fn_0044F1A0(int index);
};

extern "C" {
char DAT_0257ED80[];
unsigned char DAT_02491A5C;

void fn_004CD5F0(void);
void fn_004CB430(void);
void fn_004CB7B0(void);
void fn_004CEDB0(void);
int fn_004DC570(char* name);
void* fn_0046B1A0(int id);
void fn_00439510(int flag);
int sprintf(char* dst, const char* fmt, ...);
void fn_00531CB0(const char* file, int line, const char* msg);

void fn_004CAF40(void)
{
    char name[0x40];
    int index;
    int* slot;
    int group;

    group = *(int*)0x60a33c;
    if (group == -1)
        *(int*)0x60a33c = 1;
    index = 0;
    slot = (int*)0x60a32c;
    do {
        index = index + 1;
        sprintf(name, (char*)0x59e2c0, *(int*)0x60a33c, index);
        *slot = fn_004DC570(name);
        slot = slot + 1;
    } while ((int)slot < 0x60a33c);
    if (*(int*)0x60a32c == 0) {
        index = 0;
        slot = (int*)0x60a32c;
        do {
            index = index + 1;
            sprintf(name, (char*)0x59e2b4, index);
            *slot = fn_004DC570(name);
            slot = slot + 1;
        } while ((int)slot < 0x60a33c);
        if (*(int*)(0x60a32c + index * 4) == 0) {
            sprintf(DAT_0257ED80, (char*)0x59e278, (char*)0x60a2c8);
            for (;;)
                fn_00531CB0((char*)0x59e22c, 0x5b6, DAT_0257ED80);
        }
    }
}

void fn_004CB010(void)
{
    int index;
    int* node;

    index = 0;
    fn_004CAF40();
    node = *(int**)0x5fc350;
    while (node != (int*)0x5fc350) {
        ((Obj4F1A0*)fn_0046B1A0(node[3]))->fn_0044F1A0(index);
        node = (int*)node[0];
        index = index + 1;
    }
    fn_00439510(0);
}

void fn_004CB3A0(void)
{
    unsigned char flag;

    *(unsigned char*)0x255b4d8 = 0;
    *(unsigned char*)0x2563d88 = 0;
    ((Obj4902A8*)0x24902a8)->fn_00501880(0x384);
    flag = *(unsigned char*)0x2493a14;
    *(int*)0x24902c4 = 0;
    *(int*)0x24902a4 = 0;
    DAT_02491A5C = 0;
    if (flag == 0) {
        *(int*)0x2491a58 = 0;
        *(int*)0x24902b4 = -1;
        *(int*)0x2490dcc = 0;
        *(int*)0x25443a0 = 0;
        *(int*)0x2490c74 = 0;
        *(int*)0x24902a0 = 0;
        *(int*)0x2493a00 = 0;
        *(int*)0x2490c78 = 0;
        *(int*)0x2491a68 = 0;
    }
    fn_004CD5F0();
    fn_004CB430();
    fn_004CB7B0();
    fn_004CAF40();
    fn_004CEDB0();
}
}

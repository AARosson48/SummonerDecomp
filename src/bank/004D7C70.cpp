// Level-object methods in bank/004D7C70. The object is in ecx.
// New Game stores the level name. fn_004DAEF0 is what mode 4 calls to load it.

extern "C" {

void fn_004F13E0(void);
int fn_004D6300(char* name);
int fn_004D6340(int id, char* variant);
int __stdcall fn_004FEE50(char* variant);

int __fastcall fn_004D8F30(void* self)
{
	return (int)((char*)self + 0x1a4);
}

int __fastcall fn_004D8F40(void* self)
{
	return (int)((char*)self + 0x1b0);
}

void __fastcall fn_004DAC10(void* self)
{
	*(int*)((char*)self + 8) = 0;
}

int __fastcall fn_004DAC20(void* self)
{
	return *(int*)((char*)self + 8) == 0;
}

}

struct Obj532480 {
	void fn_00532480(void);
};

struct QuestHost {
	unsigned char fn_004D0590(int id, int slot);
	void fn_004D02D0(int id, int slot, int node);
};

struct ObjD7C70 {
	void fn_004D7C70(void);
	void fn_004D84D0(void);
	int fn_004D82F0(char* name, char* variant);
};

struct ObjD7CC0 {
	void fn_004D7CC0(int* node);
};

extern "C" int __stdcall fn_004DAEF0(char* name, char* variant)
{
	int id;
	int slot;

	fn_004F13E0();
	((ObjD7C70*)0x25461a0)->fn_004D7C70();
	slot = fn_004D6340(*(int*)0x59f120, (char*)0x60a2e8);
	id = *(int*)0x59f120;
	if (*(int*)(0x25388c4 + (id * 7 + slot) * 0x10) > 0)
		return ((ObjD7C70*)0x25461a0)->fn_004D82F0(name, variant);
	return 0;
}

void ObjD7C70::fn_004D7C70(void)
{
	*(int*)((char*)this + 0x22c) = -1;
	*(int*)((char*)this + 0x200) = 0;
	*(int*)((char*)this + 0x204) = 0;
	*(int*)((char*)this + 0x208) = 0;
	*(int*)((char*)this + 0x20c) = 0;
	((Obj532480*)((char*)this + 0x210))->fn_00532480();
}

int ObjD7C70::fn_004D82F0(char* name, char* variant)
{
	int found;
	int id;
	int slot;

	found = fn_004FEE50(variant);
	if (found != 0)
	{
		*(int*)((char*)this + 0x22c) = -1;
		return found;
	}
	id = fn_004D6300(name);
	*(int*)((char*)this + 0x22c) = id;
	slot = fn_004D6340(id, variant);
	*(int*)((char*)this + 0x230) = slot;
	if (((QuestHost*)0x2493a38)->fn_004D0590(id, slot) != 0)
	{
		fn_004D84D0();
		return found;
	}
	((QuestHost*)0x2493a38)->fn_004D02D0(id, slot, *(int*)((char*)this + 0x20c));
	return found;
}

void ObjD7CC0::fn_004D7CC0(int* node)
{
	*node = *(int*)((char*)this + 0x20c);
	*(int*)((char*)this + 0x20c) = (int)node;
}

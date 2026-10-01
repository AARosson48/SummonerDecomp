// Leaf functions in bank/004914E0. Each one is a load, a store, or a constant return.

extern "C" {

void fn_00494680()
{
	*(int*)0x243e3dc = 0;
	*(int*)0x243e3e0 = 0;
	*(int*)0x24435f0 = 0;
	*(int*)0x243e3d8 = 0;
}

}

extern "C" {

void __fastcall fn_00493F60(void* self)
{
	*(int*)self = 0x581ae4;
}

int __fastcall fn_004943D0(void* self)
{
	return (int)((char*)self + 0x40);
}

int __fastcall fn_004943E0(void* self)
{
	return (int)((char*)self + 0x4c);
}

int __fastcall fn_00493F70(void* self)
{
	return *(int*)((char*)self + 4);
}

float __fastcall fn_004943F0(float* self)
{
	return self[5] + self[4] + self[3] - self[2];
}

}

struct Obj493F40 {
	void* fn_00493F40(unsigned int flags);
};

extern "C" void vfs_heap_free(void* block);

void* Obj493F40::fn_00493F40(unsigned int flags)
{
	fn_00493F60(this);
	if (flags & 1)
		vfs_heap_free(this);
	return this;
}

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

}

// One-instruction methods in bank/004D7C70. The object is in ecx.

extern "C" {

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

struct ObjD7CC0 {
	void fn_004D7CC0(int* node);
};

void ObjD7CC0::fn_004D7CC0(int* node)
{
	*node = *(int*)((char*)this + 0x20c);
	*(int*)((char*)this + 0x20c) = (int)node;
}

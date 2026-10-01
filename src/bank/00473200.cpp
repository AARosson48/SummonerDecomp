// One-instruction methods in bank/00473200. The object is in ecx.

extern "C" {

int __fastcall fn_00473600(void* self)
{
	return *(int*)self;
}

}

struct Obj73610 {
	void fn_00473610(int value);
};

void Obj73610::fn_00473610(int value)
{
	*(int*)((char*)this + 0x18) = value;
}

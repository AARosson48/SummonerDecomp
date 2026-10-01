// One-instruction methods in bank/00500DE0. The object is in ecx.

extern "C" {

void __fastcall fn_00501870(void* self)
{
	*(int*)self = 0xffffffff;
}

void __fastcall fn_00501900(void* self)
{
	*(int*)self = 0xffffffff;
}

void __fastcall fn_005019F0(void* self)
{
	*(int*)self = 0xffffffff;
}

void* __fastcall fn_00501860(void* self)
{
	void* result;

	result = self;
	*(int*)result = -1;
	return result;
}

}

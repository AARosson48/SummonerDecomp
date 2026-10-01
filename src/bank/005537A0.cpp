// One-instruction methods in bank/005537A0. The object is in ecx.

extern "C" {

unsigned char __fastcall fn_00553EF0(void* self)
{
	return *(unsigned char*)((char*)self + 0x274);
}

}

struct Obj53EE0 {
	void fn_00553EE0(unsigned char value);
};

void Obj53EE0::fn_00553EE0(unsigned char value)
{
	*((unsigned char*)this + 0x274) = value;
}

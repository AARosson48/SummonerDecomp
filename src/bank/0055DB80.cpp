// Leaf functions in bank/0055DB80. Each one is a load, a store, or a constant return.

extern "C" {

void fn_0055DCA0()
{
	*(unsigned char*)0x2f798b4 = 0;
}

void fn_0055EF50()
{
}

}

extern "C" {

int __fastcall fn_0055EB10(void* self)
{
	return *(int*)((char*)self + 0x28c);
}

}

struct Obj5EB30 {
	void fn_0055EB30(int value);
	void fn_0055EB50(int value);
	void fn_0055EB60(int value);
};

void Obj5EB30::fn_0055EB30(int value)
{
	*(int*)((char*)this + 0x28c) = value;
}

void Obj5EB30::fn_0055EB50(int value)
{
	*(int*)((char*)this + 0x290) = value;
}

void Obj5EB30::fn_0055EB60(int value)
{
	*(int*)((char*)this + 0x294) = value;
}

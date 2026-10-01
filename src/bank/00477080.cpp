// Leaf functions in bank/00477080. Each one is a load, a store, or a constant return.

extern "C" {

int fn_00477770()
{
	return *(int*)0xa5a2c8;
}

int fn_00477780(int index)
{
	return (index * 7 << 4) + *(int*)0xa5a2cc;
}

void* __fastcall fn_004775E0(void* self)
{
	void* result;

	result = self;
	*(int*)((char*)result + 0x6c) = -1;
	return result;
}

}

struct Obj776A0 {
	void fn_004776A0(int index, unsigned char value);
};

void Obj776A0::fn_004776A0(int index, unsigned char value)
{
	*((unsigned char*)this + index + 0x6c) = value;
}

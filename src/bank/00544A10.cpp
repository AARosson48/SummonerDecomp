// Leaf functions in bank/00544A10. Each one is a load, a store, or a constant return.

extern "C" {

int fn_00544B30()
{
	return 0x28;
}

}

extern "C" {

int __fastcall fn_00544C10(void* self)
{
	return *(int*)((char*)self + 0xc);
}

}

struct Obj44B20 {
	void* fn_00544B20(int value);
};

void* Obj44B20::fn_00544B20(int value)
{
	void* result;

	result = this;
	*(int*)result = value;
	return result;
}

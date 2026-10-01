// Leaf functions in bank/00503C80. Each one is a load, a store, or a constant return.

extern "C" {

void fn_00503DA0(int value)
{
	*(int*)0x25c6894 = value;
}

}

extern "C" {

void __fastcall fn_00505540(void* self)
{
	*(int*)((char*)self + 8) = 0;
}

}

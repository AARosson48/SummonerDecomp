// Leaf functions in bank/00541700. Each one is a load, a store, or a constant return.

extern "C" {

int fn_00541CD0()
{
	return 1;
}

int fn_00541E75()
{
	return 1;
}

int fn_0054210F()
{
	return 1;
}

}

extern "C" {

unsigned char __fastcall fn_00542FE0(void* self)
{
	return *(unsigned char*)((char*)self + 0x1c);
}

unsigned char __fastcall fn_00543000(void* self)
{
	return *(unsigned char*)((char*)self + 0x1d);
}

int __fastcall fn_00542FF0(void* self)
{
	return *(unsigned char*)((char*)self + 0x1c) == 0;
}

}

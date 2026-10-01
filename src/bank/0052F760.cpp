// Leaf functions in bank/0052F760. Each one is a load, a store, or a constant return.

extern "C" {

int fn_00530C00()
{
	return *(int*)0x2cbff10;
}

unsigned char fn_00530C10()
{
	return *(unsigned char*)0x2cbff1c;
}

int fn_00531C50()
{
	return 0;
}

void fn_00531C60()
{
}

void fn_00531C70()
{
}

void fn_00531C80()
{
}

}

extern "C" {

int __fastcall fn_00530B80(void* self)
{
	return *(int*)((char*)self + 0x4d8);
}

void fn_00530C20(int value)
{
	int index;

	index = *(int*)0x2cbff24;
	*(int*)(index * 4 + 0x2cbfd60) = value;
	*(int*)0x2cbff24 = index + 1;
}

}

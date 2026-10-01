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

}

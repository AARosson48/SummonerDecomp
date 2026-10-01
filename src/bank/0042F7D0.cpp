// Leaf functions in bank/0042F7D0. Each one is a load, a store, or a constant return.

extern "C" {

int fn_004300B0()
{
	return *(int*)0x5f9e70;
}

void fn_004302A0()
{
	*(int*)0x5f9e64 = 0;
}

void fn_00430B10()
{
	*(unsigned char*)0x5f9e74 = 0;
}

void fn_00431DF0()
{
	*(unsigned char*)0x5fc01c = 0;
}

void fn_00432E50(int value)
{
	*(int*)0x5931b4 = value;
}

int fn_00432710(void);
void fn_004119A0(int value);
void fn_00411CD0(void);

void fn_00431E00(void)
{
	*(unsigned char*)0x5fc01c = 1;
	fn_004119A0(fn_00432710());
	fn_00411CD0();
}

}

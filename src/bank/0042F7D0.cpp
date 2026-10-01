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

void fn_00412620(int kind, int flags);
void fn_00401020(int value);

void fn_00430B80()
{
	fn_00412620(0x15, 0);
}

void fn_00431E70()
{
	fn_00401020(4);
}

void fn_00431E00(void)
{
	*(unsigned char*)0x5fc01c = 1;
	fn_004119A0(fn_00432710());
	fn_00411CD0();
}

}

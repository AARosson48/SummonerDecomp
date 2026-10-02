// Leaf functions in bank/00437570. Each one is a load, a store, or a constant return.

extern "C" {

void fn_0043A390()
{
	*(int*)0x60aba4 = 0;
}

void fn_00436620();
void fn_00439BA0(int text, int a, int b, int c, int d);

#pragma optimize("", off)
void fn_00437650()
{
	fn_00436620();
}
#pragma optimize("", on)

void fn_00439D80()
{
	fn_00439BA0(0x593858, 1, 0, 1, -1);
}

void fn_00439DD0()
{
	fn_00439BA0(0x593870, 1, 0, 1, -1);
}

void fn_00439E20()
{
	fn_00439BA0(0x593840, 1, 0, 1, -1);
}

}

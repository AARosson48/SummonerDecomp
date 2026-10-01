// Leaf functions in bank/0050C130. Each one is a load, a store, or a constant return.

extern "C" {

void fn_0050D6E0(int value)
{
	*(int*)0x2682988 = value;
}

void fn_0050D6C0(int first, int second)
{
	*(int*)0x268298c = first;
	*(int*)0x2682990 = second;
}

}

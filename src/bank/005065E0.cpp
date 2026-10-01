// Leaf functions in bank/005065E0. Each one is a load, a store, or a constant return.

extern "C" {

float fn_00506950()
{
	return *(float*)0x25d8d48;
}

void fn_00507D20()
{
}

void fn_00506C50(int first, int second)
{
	*(int*)0x25d8d88 = first;
	*(int*)0x25d8d8c = second;
}

}

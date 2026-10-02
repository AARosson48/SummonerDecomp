// Leaf functions in bank/004102A0. Each one is a load, a store, or a constant return.

extern "C" {

int fn_00410510()
{
	return *(int*)0x5bca84;
}

void fn_004106D0()
{
	*(int*)0x5bd0b4 = 5;
}

void fn_00410750()
{
	*(int*)0x5bd0b4 = 0;
}

void fn_004123F0()
{
	*(int*)0x5bd968 = 0;
}

int fn_00412610()
{
	return *(int*)0x5bd814;
}

float fn_004134A0()
{
	return *(float*)0x58be24;
}

void fn_004138A0()
{
	*(int*)0x5bd77c = 0;
}

void fn_00414020();

void fn_00410730()
{
	if (*(int*)0x5bd0b4 != 6)
		return;
	fn_004138A0();
	fn_00414020();
}

int fn_00413E20()
{
	return (*(int*)0x5bd77c & 0x80000000) != 0;
}

}

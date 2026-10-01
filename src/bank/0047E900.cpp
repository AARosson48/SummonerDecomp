// Leaf functions in bank/0047E900. Each one is a load, a store, or a constant return.

extern "C" {

void fn_00481E40()
{
	*(unsigned char*)0x22fb6f4 = 0;
}

float fn_00481E50()
{
	if (*(unsigned char*)0x22fb6f4 != 0)
		return *(float*)0x22fb6e8;
	return *(float*)0x57eb84;
}

}

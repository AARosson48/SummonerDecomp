// Leaf functions in bank/00544CD0. Each one is a load, a store, or a constant return.

extern "C" {

void fn_00546120()
{
	*(int*)0x2f076c0 = 0;
}

void fn_00546130(void)
{
	int index;
	int slot;

	index = *(int*)0x2f076c0;
	slot = *(int*)(index * 4 + 0x2f075bc);
	*(int*)0x2f076c0 = index + 1;
	*(unsigned char*)(*(int*)slot + 0x19) = 0;
}

}

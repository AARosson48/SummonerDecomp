// Leaf functions in bank/00414270. Each one is a load, a store, or a constant return.

extern "C" {

void fn_00414560()
{
	*(unsigned char*)0x5bd960 = 0;
	*(unsigned char*)0x5bd961 = 0;
	*(unsigned char*)0x5bd7b0 = 0;
	*(unsigned char*)0x5bd50c = 0;
}

void fn_004146A0()
{
	*(unsigned char*)0x5bd962 = 1;
}

void fn_004146B0()
{
	*(unsigned char*)0x5bd962 = 0;
}

void fn_00416674()
{
}

void fn_004326C0(void);
void fn_00412E50(int value);
void fn_00406360(int value);

void fn_004158B0()
{
	if (*(unsigned char*)0x5c100c & 1)
		fn_004326C0();
	if (*(unsigned char*)0x5c100c & 2)
		fn_00412E50(0);
	if (*(unsigned char*)0x5c100c & 4)
		fn_00406360(0);
}

}

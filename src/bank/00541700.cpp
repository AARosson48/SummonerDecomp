// Leaf functions in bank/00541700. Each one is a load, a store, or a constant return.

extern "C" {

int fn_00541CD0()
{
	return 1;
}

int fn_00541E75()
{
	return 1;
}

int fn_0054210F()
{
	return 1;
}

}

extern "C" {

unsigned char __fastcall fn_00542FE0(void* self)
{
	return *(unsigned char*)((char*)self + 0x1c);
}

unsigned char __fastcall fn_00543000(void* self)
{
	return *(unsigned char*)((char*)self + 0x1d);
}

int __fastcall fn_00542FF0(void* self)
{
	return *(unsigned char*)((char*)self + 0x1c) == 0;
}

}

struct Obj543010 {
	void* fn_00543010(unsigned int flags);
};

extern "C" void fn_0055EF50(void);
extern "C" void vfs_heap_free(void* block);

void* Obj543010::fn_00543010(unsigned int flags)
{
	fn_0055EF50();
	if (flags & 1)
		vfs_heap_free(this);
	return this;
}

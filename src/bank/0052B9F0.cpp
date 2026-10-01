// One-instruction methods in bank/0052B9F0. The object is in ecx.

extern "C" {

unsigned char __fastcall fn_0052C4A0(void* self)
{
	return *(unsigned char*)((char*)self + 0x248);
}

int __fastcall fn_0052C5C0(void* self)
{
	return *(int*)((char*)self + 0x25c);
}

int __fastcall fn_0052C5E0(void* self)
{
	return *(int*)((char*)self + 0x26c);
}

int __fastcall fn_0052C5F0(void* self)
{
	return *(int*)((char*)self + 0x270);
}

int __fastcall fn_0052C680(void* self)
{
	return *(int*)((char*)self + 0x254);
}

void __fastcall fn_0052D560(void* self)
{
	*(int*)self = 0x581e30;
}

int __fastcall fn_0052D8A0(void* self)
{
	return *(int*)((char*)self + 0x2b4);
}

void __fastcall fn_0052D9A0(void* self)
{
	*(int*)((char*)self + 0x2b4) = 0;
}

int __fastcall fn_0052F040(void* self)
{
	return *(int*)((char*)self + 0x2a8);
}

int __fastcall fn_0052F050(void* self)
{
	return *(int*)((char*)self + 0x2ac);
}

void* __fastcall fn_0052C930(void* self)
{
	void* result;

	result = self;
	*(int*)result = 0;
	*(int*)((char*)result + 0x14) = (int)result;
	*(int*)((char*)result + 4) = -1;
	return result;
}

}

struct Obj52F400 {
	void* fn_0052F400(unsigned int flags);
};

struct Obj52C690 {
	void fn_0052C690(int value, int index);
};

extern "C" void vfs_heap_free(void* block);

void* Obj52F400::fn_0052F400(unsigned int flags)
{
	fn_0052D560(this);
	if (flags & 1)
		vfs_heap_free(this);
	return this;
}

void Obj52C690::fn_0052C690(int value, int index)
{
	*(int*)((char*)this + index * 4 + 0x24c) = value;
}

struct Obj52C960 {
	void fn_0052C960(void* node);
};

struct Obj52D5A0 {
	void fn_0052D5A0(int value);
};

struct Obj52D5B0 {
	void fn_0052D5B0(int value);
};

void Obj52C960::fn_0052C960(void* node)
{
	void* next;

	next = *(void**)((char*)node + 0x14);
	*(void**)((char*)node + 0x14) = this;
	*(void**)((char*)this + 0x14) = next;
}

void Obj52D5A0::fn_0052D5A0(int value)
{
	*((unsigned char*)this + 4) = 1;
	*(int*)((char*)this + 0x28) = value;
}

void Obj52D5B0::fn_0052D5B0(int value)
{
	*(int*)((char*)this + 0x30) = value;
}

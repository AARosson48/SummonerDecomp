// bank/00512880 holds the search paths and the file openers.
#include "../vsdk/vfile/vfile_paths.cpp"
#include "../vsdk/vfile/vfs_file.cpp"

extern "C" void* __fastcall fn_00513A20(void* self)
{
	void* result;

	result = self;
	*(int*)result = -1;
	*(int*)((char*)result + 8) = 0;
	return result;
}

extern "C" void* __fastcall fn_00514EB0(void* self)
{
	void* result;

	result = self;
	*(int*)result = 0;
	return result;
}

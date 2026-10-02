// Bank 00465D24. _surfaces.tbl lives in this block. The small functions
// store one field, scale a vec3, and play a sound id kept at 0x59a334.

struct Obj681D0 {
	int value;
	void fn_004681D0(int value);
};

void Obj681D0::fn_004681D0(int next)
{
	value = next;
}

extern "C" {

void fn_00520D50(int a, int b, int c, int d, int e);
int fn_00521650(int id);

void fn_00468A90(float* dst, float scale, float* src)
{
	dst[0] = scale * src[0];
	dst[1] = scale * src[1];
	dst[2] = scale * src[2];
}

void fn_00468DC0(int a, int b)
{
	fn_00520D50(a, b, 0, 0, 1);
}

int fn_00469140(void)
{
	int id = *(int*)0x59a334;
	if (id == -1)
		return 0;
	return fn_00521650(id);
}

int fn_00469160(void)
{
	if (fn_00469140())
		return *(int*)0x59a33c;
	return -1;
}

}

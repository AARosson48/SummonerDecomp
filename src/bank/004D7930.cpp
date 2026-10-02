// Bank 004D7930. Page switches through fn_00412620, and one call into
// the object at 0x2545b68.

struct ObjD7800 {
	void fn_004D7800(int value, int kind);
};

extern "C" {

void fn_00412620(int page, void* arg);

int fn_004D7AC0(int value)
{
	((ObjD7800*)0x2545b68)->fn_004D7800(value, 3);
	return 0;
}

void fn_004D7AE0(int value)
{
	fn_00412620(0xd, &value);
}

void fn_004D7B00(void)
{
	fn_00412620(0, 0);
}

}

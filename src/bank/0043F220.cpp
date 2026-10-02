// One pass of bank/0043F220. Strings in this block: **, * , Load a monster layout file, load_mlayout, Save a monster layout file, save_mlayout, j, Toggle whether net rate info for a packet is displayed.

struct ConsoleCmd {
	void fn_0050A180(const char* name, const char* help, void (*handler)(void));
};

extern "C" {

void fn_00440170(void)
{
	((ConsoleCmd*)0x60f928)->fn_0050A180((char*)0x595044, (char*)0x595054, (void (*)(void))0x4401a0);
}

void fn_004402C0(void)
{
	((ConsoleCmd*)0x60f4f0)->fn_0050A180((char*)0x5950c4, (char*)0x5950d4, (void (*)(void))0x4402f0);
}

void fn_00442350(void)
{
	((ConsoleCmd*)0x65e0c0)->fn_0050A180((char*)0x595988, (char*)0x595994, (void (*)(void))0x442380);
}

int strncmp(const char* a, const char* b, unsigned int n);
void fn_00412620(int page, void* arg);
void fn_00440440(void);
void fn_00530BF0(int count);
int fn_00512800(void);
void fn_005127F0(void* path);

// Two-character prefix. neg/sbb/inc is strncmp(...) == 0.
int fn_0043F400(const char* text)
{
	return strncmp(text, (const char*)0x58e0c8, 2) == 0;
}

int fn_0043F420(const char* text)
{
	return strncmp(text, (const char*)0x594ff0, 2) == 0;
}

int fn_0043F440(const char* text)
{
	return text[0] == '<';
}

void fn_0043F9C0(void)
{
	fn_00412620(0x10, 0);
}

void fn_0043FC80(void)
{
	fn_00530BF0(0xa);
}

void fn_00440480(void)
{
	*(int*)0x6513a0 = 0;
	fn_00440440();
	*(int*)0x65139c = 0;
}

// Save the current path, then push the caller's path.
void fn_00440860(void* path)
{
	*(void**)0x64fe30 = (void*)fn_00512800();
	fn_005127F0(path);
}

void fn_00440880(void)
{
	fn_005127F0(*(void**)0x64fe30);
	*(int*)0x64fe30 = 0;
}

}

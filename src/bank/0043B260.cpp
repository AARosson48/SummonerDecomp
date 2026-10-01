// bank/0043B260 holds both the level-archive check and the table
// search-path setup. One object so objdiff can score both symbols.
#include "../Engine/s3d/s3d.cpp"
#include "../vsdk/vfile/vfile_paths.cpp"

extern "C" void fn_00412620(int kind, int flags);

extern "C" void fn_0043ED80()
{
	fn_00412620(0x11, 0);
}

#ifndef SUMMONER_ENGINE_H
#define SUMMONER_ENGINE_H

/* Translation units that still carry their original path are listed in
 * config/modules.txt. The shared layer is vsdk (graphics, packfile, bmpman,
 * character animation, parse, os). Gameplay sits under Engine/, Summoner/,
 * and levelscripts/.
 *
 * Graphics backends named in the binary: gr_direct3d.cpp, gr_opengl.cpp,
 * gr_glide.cpp. glide2x.dll is loaded by name; it is not in the import table.
 * binkw32.dll and EAX.DLL stay external imports and are not decompiled.
 */

#endif

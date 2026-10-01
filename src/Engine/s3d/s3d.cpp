// Original: D:\projects\Summoner\pccode\Engine\s3d\s3d.cpp
// Level archive mount recovered from Sum.exe VA 0x438570 and VA 0x43E940.
// Not byte-matched.

#include "vfs.h"

#include <cstdio>

extern void level_mount_fallback(void);

int level_archives_ready(void) {
    char path[0x100];
    char resolved[0x100];
    vfs_resolve_path(vfs_path_models_levels(), resolved);
    std::sprintf(path, "%s\\eleh.s3d", resolved);
    FILE* file = std::fopen(path, "rb");
    if (!file) {
        return 0;
    }
    std::fclose(file);
    return 1;
}

void level_mount_archives(void) {
    VfsFile file_ref;
    if (level_archives_ready()) {
        return;
    }
    vfs_file_ctor(&file_ref);
    if (!vfs_file_open(&file_ref, "levelm.vpp", vfs_path_root())) {
        level_mount_fallback();
    }
    packfile_add("levelm.vpp");
    packfile_add("levele.vpp");
    packfile_add("levelt.vpp");
    vfs_file_dtor(&file_ref);
}

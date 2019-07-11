#pragma once
#include <string>
#include <functional>
#include "tjs.h"
#include <dirent.h>

#ifndef S_IFMT
#define S_IFDIR  0x4000 // Directory
#define S_IFREG  0x8000 // Regular
#endif

struct tTVPLocalFileInfo {
	const char * NativeName;
	unsigned short Mode; // S_IFMT
	tjs_uint64         Size;
	time_t         AccessTime;
	time_t         ModifyTime;
	time_t         CreationTime;
};


#undef st_atime
#undef st_ctime
#undef st_mtime

struct tTVP_stat {
	uint16_t st_mode;
	uint64_t st_size;
	uint64_t st_atime;
	uint64_t st_mtime;
	uint64_t st_ctime;
};

bool TVP_stat(const tjs_char *name, tTVP_stat &s);
bool TVP_stat(const char *name, tTVP_stat &s);

void TVPGetLocalFileListAt(const ttstr &name, const std::function<void(const ttstr&, tTVPLocalFileInfo*)>& cb);

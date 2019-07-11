#pragma once
#include "tjsCommHead.h"
#include <string>
#include <vector>

void TVPExitApplication(int code);


void TVPRelinquishCPU();



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
void TVP_utime(const char *name, time_t modtime);


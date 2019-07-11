
#include "GetLocalFileListAt.h"
#include "CharacterSet.h"
void TVPGetLocalFileListAt(const ttstr &name, const std::function<void(const ttstr&, tTVPLocalFileInfo*)>& cb) {
	DIR *dirp;
	struct dirent *direntp;
	tTVP_stat stat_buf;
	tjs_string wname(name.AsStdString());
	std::string nname;
	if( TVPUtf16ToUtf8(nname, wname) ) {
		if ((dirp = opendir(nname.c_str())))
		{
			while ((direntp = readdir(dirp)) != NULL)
			{
				if( direntp->d_type == DT_REG ) {
					std::string fullpath = nname + "/" + direntp->d_name;
					if (!TVP_stat(fullpath.c_str(), stat_buf))
						continue;
					ttstr file(direntp->d_name);
					if (file.length() <= 2) {
						if (file == TJS_W(".") || file == TJS_W(".."))
							continue;
					}
					tjs_char *p = file.Independ();
					while (*p)
					{
						// make all characters small
						if (*p >= TJS_W('A') && *p <= TJS_W('Z'))
							*p += TJS_W('a') - TJS_W('A');
						p++;
					}
					tTVPLocalFileInfo info;
					info.NativeName = direntp->d_name;
					info.Mode = stat_buf.st_mode;
					info.Size = stat_buf.st_size;
					info.AccessTime = stat_buf.st_atime;
					info.ModifyTime = stat_buf.st_mtime;
					info.CreationTime = stat_buf.st_ctime;
					cb(file, &info);
				}
			}
			closedir(dirp);
		}
	}
}
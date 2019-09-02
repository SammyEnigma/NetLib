#include "Directory.hpp"
#include <limits>

#if defined(_WIN32) || defined(WIN32) || defined (_WIN64) || defined (WIN64)
    #include <Windows.h>
    #include <direct.h>
    #define createdir(str) _mkdir(str)
    #define getcwd(ptr,n) _getcwd(ptr,n)
    #define chdir(str) _chdir(str)
#else
    #include <dirent.h>
    #include <unistd.h>
    #include <string.h>
    #include <sys/stat.h>
    #define createdir(str) mkdir(str,S_IRWXU)
#endif

namespace CoreLibrary
{
	DirectoryInformation::DirectoryInformation()
	{
	}

	DirectoryInformation::DirectoryInformation(const GenericString &fileName)
	{
		_FullPath = fileName;
		initDirectoryInformation();
	}

	void DirectoryInformation::setPath(const GenericString &fileName)
	{
		_FullPath = fileName;
		initDirectoryInformation();
	}

	GenericString* DirectoryInformation::getPath()
	{
		return &_FullPath;
	}

	GenericString* DirectoryInformation::getName()
	{
		return &_Name;
	}

	GenericString* DirectoryInformation::getParentDirectory()
	{
		return &_ParentDirectory;
	}

	DateTime* DirectoryInformation::getCreationTime()
	{
		return &_CreationTime;
	}

	DateTime* DirectoryInformation::getLastModifiedTime()
	{
		return &_LastModifiedTime;
	}

	void DirectoryInformation::initDirectoryInformation()
	{
		int pos = 0;

		// Name
		for (pos = _FullPath.length() - 1; ; pos--)
		{
			char ch = _FullPath.getAt(pos);

			if (ch == '\\' || ch == '/')
			{
				break;
			}
		}

		_Name.assign(_FullPath.buffer(), pos + 1, _FullPath.length());

		// Parent Directory
		_ParentDirectory.assign(_FullPath.buffer(), 0, pos - 1);


		// File time stamps
#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
		HANDLE hfind = 0;
		WIN32_FIND_DATAA dstruct;
		int err;
		hfind = FindFirstFileA(_FullPath.buffer(), &dstruct);

		if (hfind == INVALID_HANDLE_VALUE)
		{
			err = GetLastError();
		}

		if (dstruct.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
		{
			ULONGLONG ull = 0;

			time_t createtm;
			ull = 0;
			ull = reinterpret_cast<const ULONGLONG&>(dstruct.ftCreationTime);
			ull -= 116444736000000000;
			ull /= 10000000;
			if ((ull > ULONG_MAX))
			{
				ull = ULONG_MAX;
			}
			createtm = static_cast<time_t>(ull);
			_CreationTime.buildFromTime(createtm);

			time_t modifytm;
			ull = 0;
			ull = reinterpret_cast<const ULONGLONG&>(dstruct.ftLastWriteTime);
			ull -= 116444736000000000;
			ull /= 10000000;
			if ((ull > ULONG_MAX))
			{
				ull = ULONG_MAX;
			}
			modifytm = static_cast<time_t>(ull);
			_LastModifiedTime.buildFromTime(modifytm);
		}

#else
		DIR *dir = nullptr;
		struct dirent *dent = nullptr;

		dir = opendir(_ParentDirectory.buffer());
		if (dir != NULL)
		{
			while (true)
			{
				dent = readdir(dir);
				if (dent == NULL)
				{
					break;
				}

				if ((memcmp(dent->d_name, ".", 1) == 0) || (memcmp(dent->d_name, "..", 2) == 0))
				{
					continue;
				}

				if (dent->d_type == DT_DIR)
				{
					GenericString str = dent->d_name;

					if (str == _Name)
					{
						struct stat filestat;
						stat(_FullPath.buffer(), &filestat);
						time_t createtm = filestat.st_mtime;
						time_t modifytm;

						if (filestat.st_ctime > filestat.st_mtime)
						{
							modifytm = filestat.st_mtime;
						}
						else
						{
							modifytm = filestat.st_ctime;
						}

						_CreationTime.buildFromTime(createtm);
						_LastModifiedTime.buildFromTime(modifytm);
					}
				}
			}
		}
#endif
	}

	//////////////////////////////////////////////////////////////////////////////////

	Directory::Directory()
	{

	}

	Directory::Directory(const GenericString &directoryName)
	{
		_DirectoryInfoStruct.setPath(directoryName);
	}


	Directory::~Directory()
	{

	}

	void Directory::setPath(const GenericString &directoryName)
	{
		_DirectoryInfoStruct.setPath(directoryName);
	}


	GenericString* Directory::getParentDirectory()
	{
		return _DirectoryInfoStruct.getParentDirectory();
	}

	GenericString* Directory::getName()
	{
		return _DirectoryInfoStruct.getName();
	}

#if defined(_WIN32) || defined(WIN32)

	void Directory::getDirectoryList(List<DirectoryInformation> &dlist)
	{
		HANDLE hfind = 0;
		WIN32_FIND_DATAA dstruct;
		int err;

		GenericString fullpath;

		GenericString str = _DirectoryInfoStruct.getPath();

		if (str.getAt(str.length() - 1) != '/')
		{
			str += "/";
		}

		str.replace('\\', '/');

		fullpath = str;

		str += "*.*";

		dlist.clear();

		memset((void*)&dstruct, 0, sizeof(dstruct));

		hfind = FindFirstFileA(str.buffer(), &dstruct);

		if (hfind == INVALID_HANDLE_VALUE)
		{
			err = GetLastError();
		}

		while (hfind != INVALID_HANDLE_VALUE)
		{
			if (dstruct.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY || dstruct.dwFileAttributes == 0x30)
			{
				if ((memcmp(dstruct.cFileName, ".", 1) != 0) && (memcmp(dstruct.cFileName, "..", 2) != 0))
				{
					DirectoryInformation dinfo;
					GenericString curdir = fullpath + dstruct.cFileName;
					dinfo.setPath(curdir);
					dlist.append(dinfo);
				}
			}

			memset((void*)&dstruct, 0, sizeof(dstruct));

			if (!FindNextFileA(hfind, &dstruct))
			{
				break;
			}
		}
		FindClose(hfind);
	}

	void Directory::getFileList(List<FileInformation> &flist, const GenericString &extension)
	{
		HANDLE hfind = 0;
		WIN32_FIND_DATAA dstruct;
		int err;

		GenericString fullpath;

		GenericString str = _DirectoryInfoStruct.getPath();

		if (str.getAt(str.length() - 1) != '/')
		{
			str += "/";
		}

		str.replace('\\', '/');

		fullpath = str;

		if (extension.getAt(0) == '.')
		{
			str += "*";
			str += extension;
		}
		else
		{
			str += "*.";
			str += extension;
		}

		flist.clear();

		fullpath = str;

		hfind = FindFirstFileA(fullpath.buffer(), &dstruct);

		if (hfind == INVALID_HANDLE_VALUE)
		{
			err = GetLastError();
		}

		while (hfind != INVALID_HANDLE_VALUE)
		{
			if (memcmp(dstruct.cFileName, ".", 1) != 0 && memcmp(dstruct.cFileName, "..", 2) != 0)
			{
				FileInformation finfo;
				GenericString curfile = fullpath;
				curfile.remove(extension);
				curfile.remove('*');
				curfile += dstruct.cFileName;
				finfo.setPath(curfile);
				flist.append(finfo);
			}

			if (!FindNextFileA(hfind, &dstruct))
			{
				break;
			}
		}

		FindClose(hfind);
	}

#else

	void Directory::getDirectoryList(List<DirectoryInformation> &dlist)
	{
		DIR *dir;
		struct dirent *dent;

		GenericString fullpath = _DirectoryInfoStruct.getPath();

		if (fullpath[fullpath.length() - 1] != '/')
		{
			fullpath += "/";
		}

		dlist.clear();

		dir = opendir(fullpath.buffer());

		if (dir != nullptr)
		{
			while (true)
			{
				dent = readdir(dir);
				if (dent == nullptr)
				{
					break;
				}

				if (dent->d_type == DT_DIR && (strcmp(dent->d_name, ".") != 0 && strcmp(dent->d_name, "..") != 0))
				{
					GenericString str = fullpath;
					str += dent->d_name;

					DirectoryInformation  dinfo;
					dinfo.setPath(str);
					dlist.append(dinfo);
				}
			}

			closedir(dir);
		}
	}

	void Directory::getFileList(List<FileInformation> &flist, const GenericString &extension)
	{
		DIR *dir;
		struct dirent *dent;

		GenericString fullpath = _DirectoryInfoStruct.getPath();

		if (fullpath[fullpath.length() - 1] != '/')
		{
			fullpath += "/";
		}

		flist.clear();

		dir = opendir(fullpath.buffer());

		if (dir != nullptr)
		{
			while (true)
			{
				dent = readdir(dir);
				if (dent == nullptr)
				{
					break;
				}

				if (dent->d_type = !DT_DIR && (strcmp(dent->d_name, ".") != 0 && strcmp(dent->d_name, "..") != 0))
				{
					GenericString str = fullpath;
					str += dent->d_name;

					FileInformation  finfo;
					finfo.setPath(str);
					flist.append(finfo);
				}
			}

			closedir(dir);
		}
	}

#endif

	bool Directory::directoryExists(const char *ptr)
	{
#if defined(_WIN32) || defined(WIN32) || defined (_WIN64) || defined (WIN64)

		HANDLE hfind = 0;
		WIN32_FIND_DATAA dstruct;
		hfind = FindFirstFileA(ptr, &dstruct);

		if (hfind == INVALID_HANDLE_VALUE)
		{
			return false;
		}

		FindClose(hfind);
		return true;

#else

		DIR *dirp;

		dirp = opendir(ptr);
		if (dirp == NULL)
		{
			return false;
		}
		closedir(dirp);
		return true;

#endif
	}

	void Directory::createDirectory(const char *str)
	{
		createdir(str);
	}

	void Directory::getCurrentDirectory(char* str, int bufflen)
	{
		getcwd(str, bufflen);
	}

	void Directory::changeDirectory(const char* str)
	{
		chdir(str);
	}
}

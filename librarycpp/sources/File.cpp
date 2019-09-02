#include "File.hpp"
#include <limits>
#include <stdio.h>

#if defined(_WIN32) || defined(WIN32) || defined (_WIN64) || defined (WIN64)
    #include <Windows.h>
#else
    #include <dirent.h>
    #include <string.h>
    #include <sys/stat.h>
#endif

namespace CoreLibrary
{
	class FileReference
	{
	public:
		FILE* _FilePointer;
	};

	FileInformation::FileInformation()
	{
		_IsValid = false;
	}

	FileInformation::FileInformation(const GenericString &fileName)
	{
		_FullPath = fileName;
		_FullPath.replace('\\', '/');
		initFileInformation();
	}

	bool FileInformation::IsValid()
	{
		return _IsValid;
	}

	void FileInformation::setPath(const GenericString &fileName)
	{
		_FullPath = fileName;
		initFileInformation();
	}

	GenericString* FileInformation::getPath()
	{
		return &_FullPath;
	}

	GenericString* FileInformation::getName()
	{
		return &_Name;
	}

	GenericString* FileInformation::getExtension()
	{
		return &_Extension;
	}

	GenericString* FileInformation::getParentDirectory()
	{
		return &_ParentDirectory;
	}

	MimeType FileInformation::getMimeType()
	{
		return _FileMimeType;
	}

	DateTime* FileInformation::getCreationTime()
	{
		return &_CreationTime;
	}

	DateTime* FileInformation::getLastModifiedTime()
	{
		return &_LastModifiedTime;
	}

	void FileInformation::initFileInformation()
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

		// Extension
		pos = _Name.indexOf('.');

		if (pos > 0)
		{
			_Extension.assign(_Name.buffer(), pos + 1, _Name.length());
		}

		// File time stamps
#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
		HANDLE hfind = 0;
		WIN32_FIND_DATAA dstruct;
		int err;
		hfind = FindFirstFileA(_FullPath.buffer(), &dstruct);

		if (hfind == INVALID_HANDLE_VALUE)
		{
			err = GetLastError();
			_IsValid = false;
		}

		if (dstruct.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY && hfind != INVALID_HANDLE_VALUE)
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
			_IsValid = true;
		}

#else
		DIR *dir = nullptr;
		struct dirent *dent = nullptr;

		dir = opendir(_ParentDirectory.buffer());

		if (dir != nullptr)
		{
			while (true)
			{
				dent = readdir(dir);
				if (dent == nullptr)
				{
					break;
				}

				if ((memcmp(dent->d_name, ".", 1) == 0) || (memcmp(dent->d_name, "..", 2) == 0))
				{
					continue;
				}

				if (dent->d_type != DT_DIR)
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

			closedir(dir);
		}
#endif
	}

	//////////////////////////////////////////////////////////////////////////////////

	File::File()
	{
		_FileReference = new FileReference();
		_FileReference->_FilePointer = nullptr;
		_Open = false;
	}

	File::File(const GenericString &fileName)
	{
		_FileInformationStruct.setPath(fileName);
	}

	void File::setPath(const GenericString &fileName)
	{
		_FileInformationStruct.setPath(fileName);
	}

	bool File::openFile(FileOpenMode fmode, MimeType mimetype)
	{
		_FileReference->_FilePointer = nullptr;

		if (_FileInformationStruct.IsValid())
		{
			_FileReference->_FilePointer = fopen(_FileInformationStruct.getPath()->buffer(), "r");

			if (_FileReference->_FilePointer != nullptr)
			{
				_Open = true;
				return true;
			}
		}

		return false;
	}

	File::~File()
	{
		closeFile();
	}

	bool File::closeFile()
	{
		if (_FileReference->_FilePointer != nullptr)
		{
			fflush(_FileReference->_FilePointer);
			fclose(_FileReference->_FilePointer);
		}

		delete _FileReference;

		return true;
	}

	GenericString* File::getParentDirectory()
	{
		return _FileInformationStruct.getParentDirectory();
	}

	GenericString* File::getExtension()
	{
		return _FileInformationStruct.getExtension();
	}

	bool File::fileExists()
	{
		return _FileInformationStruct.IsValid();
	}

	GenericString* File::getName()
	{
		return _FileInformationStruct.getName();
	}

	FileInformation* File::getFileInformation()
	{
		return &_FileInformationStruct;
	}

	bool File::seekBegin()
	{
		if (fseek(_FileReference->_FilePointer, 0, SEEK_SET) == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool File::seekEnd()
	{
		if (fseek(_FileReference->_FilePointer, 0, SEEK_END) == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool File::seek(long pos)
	{
		if (fseek(_FileReference->_FilePointer, pos, SEEK_SET) == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	long File::getSize()
	{
		int curpos = 0;
		int size = -1;

		if (fseek(_FileReference->_FilePointer, 0, SEEK_CUR) == 0)
		{
			curpos = ftell(_FileReference->_FilePointer);
			fseek(_FileReference->_FilePointer, 0, SEEK_END);
			size = ftell(_FileReference->_FilePointer);
			fseek(_FileReference->_FilePointer, curpos, SEEK_SET);
		}

		return size;
	}

	bool File::isTextFile()
	{
		return true;
	}

	bool File::isOpen()
	{
		return _Open;
	}

	void File::read(Buffer &var)
	{
		int len = 0;

		if (!isOpen())
		{
			return;
		}

		len = getSize();

		char *buffer = new char[len + 1];
		memset(buffer, 0, len + 1);

		fread(buffer, len, 1, _FileReference->_FilePointer);

		var.assign(buffer, len);

		delete[] buffer;

		fseek(_FileReference->_FilePointer, 0, SEEK_SET);
	}

	void File::write(Buffer &var)
	{
		if (!isOpen())
		{
			return;
		}

		fwrite(var.buffer(), var.length(), 1, _FileReference->_FilePointer);

		fflush(_FileReference->_FilePointer);

		fseek(_FileReference->_FilePointer, 0, SEEK_END);
	}

	void File::write(GenericString &str)
	{
		if (!isOpen())
		{
			return;
		}

		fwrite(str.buffer(), str.length(), 1, _FileReference->_FilePointer);

		fflush(_FileReference->_FilePointer);

		fseek(_FileReference->_FilePointer, 0, SEEK_END);
	}


	bool File::fileExists(const char *ptr)
	{
		FILE* tempptr = fopen(ptr, "rb");

		if (tempptr == nullptr)
		{
			return false;
		}

		fclose(tempptr);

		return true;
	}
}

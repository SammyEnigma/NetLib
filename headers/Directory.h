#ifndef _DIRECTORY
#define _DIRECTORY

#include "GenericString.h"
#include "List.h"
#include "DateTime.h"
#include "File.h"

namespace CoreLibrary
{
	class DirectoryInformation
	{
	public:
		DirectoryInformation();
		DirectoryInformation(const GenericString &fileName);
		void setPath(const GenericString &fileName);
		GenericString*	getPath();
		GenericString*	getName();
		GenericString*	getParentDirectory();
		DateTime*		getCreationTime();
		DateTime*		getLastModifiedTime();
	private:
		void initDirectoryInformation();
		GenericString _Name;
		GenericString _FullPath;
		GenericString _ParentDirectory;
		DateTime _CreationTime;
		DateTime _LastModifiedTime;
	};

	class Directory
	{
	public:
		Directory();
		Directory(const GenericString &directoryName);
		~Directory();
		void setPath(const GenericString &directoryName);
		GenericString* getParentDirectory();
		GenericString* getName();
		void getDirectoryList(List<DirectoryInformation> &dlist);
		void getFileList(List<FileInformation> &flist, const GenericString &extension);

		static bool directoryExists(const char *ptr);
		static void createDirectory(const char *str);
		static void getCurrentDirectory(char* str, int bufflen);
		static void changeDirectory(const char* str);
	private:
		DirectoryInformation _DirectoryInfoStruct;
	};
}
#endif
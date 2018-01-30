#ifndef _FILE
#define _FILE

#include "GenericString.hpp"
#include "Buffer.hpp"
#include "List.hpp"
#include "DateTime.hpp"
#include "Mime.hpp"

namespace CoreLibrary
{
	typedef enum FileOpenMode
	{
		ReadOnly = 0,
		Create = 1,
		Append = 2
	}FileOpenMode;

	class FileInformation
	{
	public:
		FileInformation();
		FileInformation(const GenericString &fileName);
		void setPath(const GenericString &fileName);
		bool IsValid();
		GenericString*	getPath();
		GenericString*	getName();
		GenericString*	getExtension();
		GenericString*	getParentDirectory();
		MimeType		getMimeType();
		DateTime*		getCreationTime();
		DateTime*		getLastModifiedTime();
	private:
		void initFileInformation();
		GenericString _Name;
		GenericString _FullPath;
		GenericString _ParentDirectory;
		GenericString _Extension;
		DateTime _CreationTime;
		DateTime _LastModifiedTime;
		MimeType _FileMimeType;
		bool _IsValid;
	};

	class FileReference;

	class File
	{
	public:
		File();
		File(const GenericString &fileName);
		~File();

		void setPath(const GenericString &fileName);
		bool openFile(FileOpenMode fmode, MimeType mimetype);
		bool closeFile();

		GenericString* getParentDirectory();
		GenericString* getExtension();
		GenericString* getName();
		long getSize();
		FileInformation* getFileInformation();
		bool fileExists();

		bool seekBegin();
		bool seekEnd();
		bool seek(long pos);

		bool isTextFile();
		bool isOpen();

		void read(Buffer &var);
		void write(Buffer &var);
		void write(GenericString &str);

		static bool fileExists(const char *ptr);
	private:
		FileReference* _FileReference;
		FileInformation _FileInformationStruct;
		bool _Open;
	};
}
#endif


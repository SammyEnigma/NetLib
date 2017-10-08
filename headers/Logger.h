#ifndef _LOGGER
#define _LOGGER

#include "GenericString.h"
#include "Directory.h"
#include "File.h"
#include "Map.h"
#include "DateTime.h"
#include "Buffer.h"

namespace CoreLibrary
{
	#if defined(_WIN32) || defined(WIN32) || defined (_WIN64) || defined (WIN64)
	#define FUNCTIONNAME __FUNCTION__
	#endif

	#if defined(__gnu_linux__) || defined (__linux__)
	#define FUNCTIONNAME __PRETTY_FUNCTION__
	#endif

	#if defined(__unix__)
	#define FUNCTIONNAME "NoName"
	#endif

	typedef enum LogLevel
	{
		LogInfo = 0,
		LogError = 1,
		LogWarning = 2,
		LogCritical = 3
	}LogLevel;

	typedef enum LogFileMode
	{
		FileAppend = 0,
		FileCreateNew = 1
	}LogFileMode;

	typedef Map<LogLevel, GenericString> LogLevelStrings;

	class Logger
	{
	public:
		Logger();
		~Logger();

		void   startLogging(LogFileMode fmode);
		void   stopLogging();
		void   write(GenericString logEntry, LogLevel llevel, const char* func, const char* file, int line);
		void   writeExtended(LogLevel llevel, const char* func, const char* file, int line, const char* format, ...);
		void   setRemotePort(int remotePort);
		void   setRemoteHost(GenericString remoteHost);
		void   setLogFileSize(int flsz);
		void   setLogDirectory(GenericString dirpath);
		void   setModuleName(GenericString mname);
		static Logger*  GetInstance();
	private:
		void createBackupFileName(GenericString &str);
		int				_RemoteLogPort;
		GenericString	_LogFilename;
		GenericString	_RemoteLogHost;
		GenericString	_LogDirectory;
		GenericString	_LogBackupDirectory;
		int				_LogFileSize;
		GenericString	_ModuleName;
		File			_LogFile;
		LogFileMode		_FileMode;
		LogLevelStrings	_LogLevelMap;
	};

	#define writeLog(str,level) Logger::GetInstance()->write(str,level,FUNCTIONNAME,__FILE__,__LINE__);
	#define writeLogNormal(str) Logger::GetInstance()->write(str,LogInfo,FUNCTIONNAME,__FILE__,__LINE__);
}
#endif


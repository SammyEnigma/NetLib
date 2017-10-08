#include "Logger.h"
#include <stdarg.h>
#include <stdio.h>

namespace CoreLibrary
{
	#if defined(_WIN32) || defined(WIN32) || defined (_WIN64) || defined (WIN64)
	#include <memory.h>
	#include <process.h>
	#include <direct.h>
	#define createdir(str) _mkdir(str)
	#define getcwd(ptr,n) _getcwd(ptr,n)
	#define chdir(str) _chdir(str)
	#define getpid() _getpid()
	#else
	#include <unistd.h>
	#include <string.h>
	#endif

	Logger objLogger;

	Logger*  Logger::GetInstance()
	{
		return &objLogger;
	}

	Logger::Logger()
	{
		_RemoteLogPort = 9090;
		_RemoteLogHost = "127.0.0.1";
		_LogDirectory = "";
		_LogFileSize = 1024;

		char pidstr[16];
		memset((char*)&pidstr[0], 0, 16);
		sprintf(pidstr, "%d", getpid());
		_ModuleName = pidstr;

		_LogLevelMap.clear();

		LogLevel l1 = LogInfo;
		LogLevel l2 = LogWarning;
		LogLevel l3 = LogError;
		LogLevel l4 = LogCritical;

		_LogLevelMap.insert(l1, "Information");
		_LogLevelMap.insert(l2, "Warning    ");
		_LogLevelMap.insert(l3, "Error      ");
		_LogLevelMap.insert(l4, "Critical   ");
	}

	Logger::~Logger()
	{
		stopLogging();
	}

	void Logger::stopLogging()
	{
		if (_LogFile.isOpen())
		{
			_LogFile.closeFile();
		}
		_LogLevelMap.clear();
	}

	void Logger::createBackupFileName(GenericString &str)
	{
		DateTime ts;
		GenericString tstamp = ts.getDateString("yyyy.MM.dd-hh.mm.ss");
		char temp[1024];
		memset((char*)&temp[0], 0, 16);
		sprintf(temp, "%s_%s.log", _ModuleName.buffer(), tstamp.buffer());
		str = temp;
	}

	void Logger::startLogging(LogFileMode fmode)
	{
		_FileMode = fmode;

		if (_LogDirectory.length() < 1)
		{
			char filepathbuffer[1024];
			memset((char*)&filepathbuffer[0], 0, 1024);
			getcwd(&filepathbuffer[0], 1024);

			Directory dir(filepathbuffer);
			GenericString pdir = dir.getParentDirectory();

			pdir += "config/";

			if (!Directory::directoryExists(pdir.buffer()))
			{
				Directory::createDirectory(pdir.buffer());
			}

			_LogDirectory = filepathbuffer;
		}

		_LogFilename = _LogDirectory + _ModuleName + ".log";

		_LogFile.setPath(_LogFilename);

		if (_FileMode == FileAppend)
		{
			_LogFile.openFile(FileOpenMode::Append, MimeType::Text);
		}
		else
		{
			_LogFile.openFile(FileOpenMode::Create, MimeType::Text);
		}
	}

	void Logger::write(GenericString logEntry, LogLevel llevel, const char* func, const char* file, int line)
	{
		if (_LogFile.isOpen())
		{
			int sz = _LogFile.getSize();

			if (sz >= _LogFileSize * 1024)
			{
				GenericString temp;
				createBackupFileName(temp);
				GenericString backupfile = _LogBackupDirectory + temp;
				stopLogging();
				rename(_LogFilename.buffer(), backupfile.buffer());
				startLogging(_FileMode);
			}

			File fl(file);
			GenericString sourcefile = fl.getName();
			GenericString* lvel = _LogLevelMap[llevel];

			DateTime ts;
			GenericString tstamp = ts.getDateString("yyyy.MM.dd-hh.mm.ss");
			char temp[1024];
			memset((char*)&temp[0], 0, 16);

			GenericString fname = func;

			#if !defined(_WIN32) && !defined(WIN32)
			int pos = fname.indexOf('(');
			fname.SetAt(pos, '\0');
			#endif

			GenericString left, right;
			fname.getKeyValuePair(left, right, "::");

			if (right.length() > 1)
			{
				fname = right;
			}

			fname.getKeyValuePair(left, right, " ");

			if (right.length() > 1)
			{
				fname = right;
			}

			sprintf(temp, "%s|%s|%05d|%s|%s| ", tstamp.buffer(), lvel->buffer(), line, fname.buffer(), sourcefile.buffer());

			GenericString tbuff(temp);
			_LogFile.write(tbuff);
			_LogFile.write(logEntry);
		}
	}

	void Logger::setModuleName(GenericString mname)
	{
		int len = mname.length();

		int ctr = 0;

		int pos1 = 0;
		int pos2 = 0;

		_ModuleName = mname;

		pos1 = _ModuleName.indexOf('/');
		pos2 = _ModuleName.indexOf('\\');

		if (pos1 > -1 || pos2 > -1)
		{
			for (ctr = len; ; ctr--)
			{
				if (mname[ctr] == '/' || mname[ctr] == '\\')
				{
					break;
				}
			}
			_ModuleName.assign(mname.buffer(), mname.length() - ctr, ctr + 1);
		}
		else
		{
			_ModuleName = mname;
		}

		_ModuleName.replace(".exe", "");
		_ModuleName.replace(".EXE", "");
	}

	void Logger::setRemotePort(int remotePort)
	{
		_RemoteLogPort = remotePort;
	}

	void Logger::setRemoteHost(GenericString remoteHost)
	{
		_RemoteLogHost = remoteHost;
	}

	void Logger::setLogFileSize(int flsz)
	{
		_LogFileSize = flsz;
	}

	void Logger::setLogDirectory(GenericString dirpath)
	{
		_LogDirectory = dirpath;

		int len = _LogDirectory.length();

		char buffer[2048] = { 0 };

		memcpy(buffer, _LogDirectory.buffer(), len);

		if (buffer[len - 1] == '/' || buffer[len - 1] == '\\')
		{
			buffer[len - 1] = 0;
		}

		memcpy(buffer + len, ".bak/", 5);

		_LogBackupDirectory = buffer;

		if (!Directory::directoryExists(buffer))
		{
			Directory::createDirectory(buffer);
		}
	}

	void Logger::writeExtended(LogLevel llevel, const char *func, const char *file, int line, const char* format, ...)
	{
		char tempbuf[1024];
		memset((char*)&tempbuf[0], 0, 1024);
		va_list args;
		va_start(args, format);
		vsprintf(tempbuf, format, args);
		tempbuf[1023] = 0;
		write(tempbuf, llevel, func, file, line);
	}
}


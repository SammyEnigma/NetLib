#include "ProcessLock.h"
#include <stdlib.h>

#if defined(_WIN32) || defined(WIN32) || defined (_WIN64) || defined (WIN64)
    #include <Windows.h>
    #include <process.h>
    #include <TlHelp32.h>
    #define getpid() _getpid()
#else
    #include <fcntl.h>
    #include <unistd.h>
    #include <errno.h>
    #include <stdio.h>
    #include <string.h>
#endif

namespace CoreLibrary
{
	ProcessLock::ProcessLock()
	{
		_LockFile = 0;
	}

	bool ProcessLock::lockProcess(GenericString &lockfileame)
	{
		getLockFileName(_LockFileName);
		if (_LockFile != 0 && _LockFile != -1)
		{
			//File is already open
			return false;
		}

		#if defined(_WIN32) || defined(WIN32)
		return true;
		#else
		_LockFile = open(_LockFileName.buffer(), O_CREAT | O_RDWR, 0666);
		if (_LockFile != -1)
		{
			off_t sz = 0;
			int rc = lockf(_LockFile, F_TLOCK, sz);
			if (rc == -1)
			{
				close(_LockFile);
				_LockFile = 0;
				if (EAGAIN == errno || EACCES == errno)
				{
				}
				else
				{
				}
				return false;
			}

			// Okay! We got a lock
			lockfileame = _LockFileName;
			return true;
		}
		else
		{
			_LockFile = 0;
			return false;
		}
		#endif

		return false;
	}

	void ProcessLock::getLockFileName(GenericString &lockfileame)
	{
		GenericString procname, uname, tmpdir;
		getProcessName(procname);
		getUserName(uname);
		getTempDir(tmpdir);

		lockfileame = tmpdir;

		lockfileame += "/";
		lockfileame += procname;
		lockfileame += ".";
		lockfileame += uname;
		lockfileame += ".lock";
	}

	ProcessLock::~ProcessLock()
	{
		#if defined(_WIN32) || defined(WIN32)
		#else
		close(_LockFile);
		#endif
	}

	#if defined(_WIN32) || defined(WIN32)
	void ProcessLock::getProcessName(GenericString &processName)
	{
		int ownpid = getpid();

		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hSnapshot)
		{
			PROCESSENTRY32 pe32;
			pe32.dwSize = sizeof(PROCESSENTRY32);
			memset((void*)&pe32.szExeFile[0], 0, sizeof(pe32.szExeFile));
			if (Process32First(hSnapshot, &pe32))
			{
				do
				{
					if (pe32.th32ProcessID == ownpid)
					{
						processName = pe32.szExeFile;
					}
				} while (Process32Next(hSnapshot, &pe32));
			}
			CloseHandle(hSnapshot);
		}
	}

	void ProcessLock::getUserName(GenericString &uName)
	{
		char buffer[256] = { 0 };
		unsigned long sz = 255;
		GetUserNameA((char*)&buffer[0], &sz);
		uName = buffer;
	}

	void ProcessLock::getTempDir(GenericString &dirName)
	{
		dirName = getenv("TEMP");
		dirName.replace('\\', '/');
	}

	# else

	void ProcessLock::getProcessName(GenericString &processName)
	{
		FILE *pipein_fp;
		char readbuf[80] = { 0 };

		int ownpid = getpid();

		char cmdbuffer[256] = { 0 };
		sprintf(cmdbuffer, "ps aux | tr -s ' ' | cut -d ' ' -f2,11 | grep %d", ownpid);

		// Create one way pipe line with call to popen()
		if ((pipein_fp = popen(cmdbuffer, "r")) == NULL)
		{
			return;
		}


		bool found = false;

		// Processing loop
		while (true)
		{
			memset((void*)&readbuf, 0, sizeof(readbuf));
			char *ptr = fgets(readbuf, 80, pipein_fp);
			if (ptr == NULL)
			{
				break;
			}

			for (int idx = 0; idx < 80; idx++)
			{
				if (readbuf[idx] == '\r' || readbuf[idx] == '\n')
				{
					readbuf[idx] = 0;
				}
			}

			if (strlen(readbuf) < 1)
			{
				continue;
			}

			// Check for zombie processes
			if (strstr(readbuf, "<defunct>") != NULL)
			{
				continue;
			}

			List<GenericString> strlist;

			GenericString tempreadbuf(readbuf);

			tempreadbuf.getSubStringList(strlist, ' ');

			if (strlist.count() < 2)
			{
				continue;
			}

			processName = *strlist.getAt(1);
			processName.remove('.');
			processName.remove('&');
			processName.remove('/');

		}
		// Close the pipes
		pclose(pipein_fp);
	}

	void ProcessLock::getUserName(GenericString &uName)
	{
		uName = getenv("USER");
	}

	void ProcessLock::getTempDir(GenericString &dirName)
	{
		dirName = "/tmp";
	}
	#endif
}

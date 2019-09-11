#ifndef _PROCESS_LOCK
#define _PROCESS_LOCK

#include <string>

namespace CoreLib
{
	class ProcessLock
	{
	public:
		ProcessLock();
		virtual ~ProcessLock();
		bool lockProcess(std::string &lockfileame);
		void getLockFileName(std::string &lockfileame);
		void getUserName(std::string &uName);
	private:
		void getProcessName(std::string &processName);
		void getTempDir(std::string &dirName);
		int _LockFile;
		std::string _LockFileName;
	};
}
#endif

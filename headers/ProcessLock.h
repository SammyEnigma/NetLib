#ifndef _PROCESS_LOCK
#define _PROCESS_LOCK

#include "GenericString.h"
#include "List.h"

namespace CoreLibrary
{
	class ProcessLock
	{
	public:
		ProcessLock();
		virtual ~ProcessLock();
		bool lockProcess(GenericString &lockfileame);
		void getLockFileName(GenericString &lockfileame);
		void getUserName(GenericString &uName);
	private:
		void getProcessName(GenericString &processName);
		void getTempDir(GenericString &dirName);
		int _LockFile;
		GenericString _LockFileName;
	};
}
#endif

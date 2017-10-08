#ifndef _MUTEX
#define _MUTEX

namespace CoreLibrary
{
	class MutexReference;

	class Mutex
	{
	public:
		Mutex();
		~Mutex();
		bool create();
		bool destroy();
		bool lock();
		bool tryLock();
		bool unlock();
	private:
		MutexReference* _MutexReference;
		bool _Created;
	};
}
#endif

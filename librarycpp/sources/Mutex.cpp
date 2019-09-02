#include "Mutex.hpp"

#if defined(_WIN32) || defined(WIN32) || defined (_WIN64) || defined (WIN64)
    #include <Windows.h>
#else
    #include <pthread.h>
#endif

namespace CoreLibrary
{
	class MutexReference
	{
	public:
		#if defined(_WIN32) || defined(WIN32) || defined (_WIN64) || defined (WIN64)
		HANDLE _Mutex;
		#else
		pthread_mutex_t _Mutex;
		#endif
	};

	Mutex::Mutex()
	{
		_MutexReference = new MutexReference();
		_Created = false;
	}

	Mutex::~Mutex()
	{
		delete _MutexReference;
	}

	bool Mutex::lock()
	{
		if (!_Created)
		{
			return false;
		}

		#if defined(WIN32) || defined(_WIN32)
		if (WaitForSingleObject(_MutexReference->_Mutex, INFINITE) == WAIT_OBJECT_0)
		{
			return true;
		}
		#else
		int rc = pthread_mutex_lock(&_MutexReference->_Mutex);
		if (rc == 0)
		{
			return true;
		}
		#endif

		return false;
	}

	bool Mutex::tryLock()
	{
		if (!_Created)
		{
			return false;
		}

		#if defined(WIN32) || defined(_WIN32)
		if (WaitForSingleObject(_MutexReference->_Mutex, 100) == WAIT_OBJECT_0)
		{
			return true;
		}
		#else
		int rc = pthread_mutex_trylock(&_MutexReference->_Mutex);
		if (rc == 0)
		{
			return true;
		}
		#endif

		return false;
	}

	bool Mutex::unlock()
	{
		if (!_Created)
		{
			return false;
		}

		#if defined(WIN32) || defined(_WIN32)
		return (bool)ReleaseMutex(_MutexReference->_Mutex);
		#else
		int rc = pthread_mutex_unlock(&_MutexReference->_Mutex);
		if (rc == 0)
		{
			return true;
		}
		#endif

		return false;
	}

	bool Mutex::create()
	{
		#if defined(WIN32) || defined(_WIN32)
		_MutexReference->_Mutex = ::CreateMutex(NULL, FALSE, NULL);

		if (_MutexReference->_Mutex != NULL)
		{
			_Created = true;
			return true;
		}
		#else
		int rc = pthread_mutex_init(&_MutexReference->_Mutex, NULL);

		if (rc == 0)
		{
			_Created = true;
			return true;
		}
		#endif

		return false;
	}

	bool Mutex::destroy()
	{
		if (!_Created)
		{
			return false;
		}

		#if defined(WIN32) || defined(_WIN32)
		if (::CloseHandle(_MutexReference->_Mutex))
		{
			_Created = false;
			return true;
		}
		else
		{
			return false;
		}

		#else
		int rc = pthread_mutex_destroy(&_MutexReference->_Mutex);

		if (rc == 0)
		{
			_Created = false;
			return true;
		}
		#endif

		return false;
	}
}

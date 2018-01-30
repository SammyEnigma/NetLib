#ifndef	_NETWORK
#define	_NETWORK

#include "NetworkDefs.hpp"

namespace CoreLibrary
{
	class Network
	{
	public:
		Network();
		virtual ~Network();
		static bool isIPAddress(char* str);
		static bool isIP6Address(char* str);
		static bool isIP4Address(char* str);
		static void getLocalHostName(char *hostname);
		static void getLocalIPAddress(const int newServerfd, char *ipaddress);
		static void getEndPoint(const char* url, char* host, int &port);
		static bool addToDescriptors(void* sockref);
		static bool removeFromDescriptors(void* sockref);
		static Descriptor* getActiveDescriptor();
	private:
	};
}
#endif


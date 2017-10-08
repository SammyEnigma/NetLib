#ifndef	_TCP_CLIENT
#define	_TCP_CLIENT

#include "GenericString.h"
#include "NetworkDefs.h"

namespace CoreLibrary
{
	class SocketReference;

	class TcpClient : public Descriptor
	{
	public:
		TcpClient();
		TcpClient(int inSocket, bool requireSSL = false);
		TcpClient(const TcpClient& other);
		TcpClient& operator=(const TcpClient& other);

		virtual ~TcpClient();
		bool createSocket(const char* servername, int serverport, bool requireSSL = false);
		bool createSocket(unsigned long inSocket, bool requireSSL = false);
		bool connectSocket(int &returncode);
		bool closeSocket();
		bool isConnected();
		void setPacketDelimeter(char* str);
		void setPacketLength(long len);

		GenericString* certificateName();

		bool sendBuffer(const char* data, int &len);
		bool sendString(const GenericString &str);

		bool receiveBuffer(int len);
		bool receiveString(GenericString &ioStr, const char* delimeter);

		unsigned long getSocket();

		int pendingPreFetchedBufferSize();

		virtual void dataReceived(const char* ptr, int len);

		DescriptorType descriptorType();
	private:
		SocketReference*    _SocketReference;
	};

	typedef TcpClient Client;
}
#endif


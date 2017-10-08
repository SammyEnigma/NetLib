#ifndef	_TCP_SERVER
#define	_TCP_SERVER

#include "TcpClient.h"
#include "NetworkDefs.h"

namespace CoreLibrary
{
	class TcpServer : public Descriptor
	{
	public:
		TcpServer();
		virtual ~TcpServer();
		ServerState run(int port);
		void stop();

		virtual void connectionAccepted(TcpClient* ptr) = 0;
		virtual void connectionClosed(TcpClient* ptr) = 0;
		virtual void dataReceived(TcpClient* ptr) = 0;

		DescriptorType descriptorType();
	};
}
#endif

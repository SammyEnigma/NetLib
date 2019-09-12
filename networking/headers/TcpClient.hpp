#ifndef	_TCP_CLIENT
#define	_TCP_CLIENT

#if defined(_WIN32) || defined(WIN32) || defined (_WIN64) || defined (WIN64)
#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include <Windows.h>
#include <WS2tcpip.h>
#endif

#if defined(__gnu_linux__) || defined (__linux__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <fcntl.h>
#endif

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/x509_vfy.h>

#include <string>
#include "Network.hpp"
#include "StringEx.hpp"

using namespace std;

class SocketReference;

class TcpClient
{
public:
	TcpClient();
	TcpClient(int inSocket, bool requireSSL = false);

	virtual ~TcpClient();

	bool CreateSocket(const char* servername, int serverport, bool reqSSL = false);
	bool CreateSocket(unsigned long inSocket, bool requireSSL = false);
	bool ConnectSocket(int &returncode);
	bool CloseSocket();
	bool IsConnected();
	void SetPacketDelimeter(char* str);
	void SetPacketLength(long len);

	bool SwitchToSecureMode();

	bool SendBuffer(const char* data, int &len);
	bool SendString(const std::string &str);

	bool ReceiveBuffer(int len);
	bool ReceiveString(std::string &ioStr, const char* delimeter);
	bool ReceiveString(std::string& ioStr);

	unsigned long GetSocket();

	int PendingPreFetchedBufferSize();
private:
	bool				connected;
	unsigned long		socketFd;
	sockaddr_in			serverAddress;
	std::string 		serverName;
	int					serverPort;
	size_t				preFetchedBufferSize;
	unsigned char*		preFetchedBuffer;
	int					packetSize;
	unsigned char*		packet;
	char				packetDelimeter[32];
	bool				requireSSL;
	PacketBehaviour		phv;

	const SSL_METHOD*	SSLMethod;
	SSL_CTX*			SSLContext;
	SSL*				SSLSession;
};

#endif


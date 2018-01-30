#if defined(_WIN32) || defined(WIN32) || defined (_WIN64) || defined (WIN64)
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <memory.h>
#endif

#if defined(__gnu_linux__) || defined (__linux__)
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#endif

#if defined(__unix__)
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <fcntl.h>
#endif

#include <stdlib.h>

#if defined(_WIN32) || defined(WIN32) || defined (_WIN64) || defined (WIN64)
#define socklen_t int
#define socketerror  ::WSAGetLastError()
#define socketioctl(socket, flag, var) ioctlsocket(socket, flag, (u_long*)&var)
#ifndef ERESTART
#define ERESTART 999
#endif
#endif

#if defined(__gnu_linux__) || defined (__linux__)
#define socketerror   errno
#define socketioctl(socket, flag, var) ioctlsocket(socket, flag, (char*)&var)
#define closesocket(n) close(n) 
#define SOCKET long
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)
#endif

#if defined(__unix__)
#define socketerror   errno
#define socketioctl(socket, flag, var) ioctl(socket, flag, (char*)&var)
#define closesocket(n) close(n) 
#define SOCKET long
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)
#ifndef ERESTART
#define ERESTART 999
#endif
#endif

#include "TcpServer.hpp"
#include "Map.hpp"

namespace CoreLibrary
{
	SOCKET _ListenerSocket;
	Map<SOCKET, TcpClient*> _ResponderList;

	bool acceptConnections(TcpServer* serverPtr, SOCKET listenerSocket, long &max_sd, struct fd_set *ptr);
	bool receiveData(TcpServer* serverPtr, SOCKET responderSocket, long &max_sd, struct fd_set *ptr);

	TcpServer::TcpServer()
	{
		_ResponderList.clear();
		_ListenerSocket = 0;
	}

	TcpServer::~TcpServer()
	{
		stop();
	}

	ServerState TcpServer::run(int port)
	{
		int    on = 1;
		long   socketIndex = 0;
		int    rc = -1;
		long   max_sd;
		int    desc_ready = -1;

		struct sockaddr_in   addr;
		struct timeval       timeout;
		struct fd_set        master_set;
		struct fd_set        working_set;
		bool end_server = false;

		_ListenerSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (_ListenerSocket < 0)
		{
			return ServerState::CreateFailed;
		}

		rc = setsockopt(_ListenerSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
		if (rc < 0)
		{
			return ServerState::CreateFailed;
		}

		rc = socketioctl(_ListenerSocket, FIONBIO, on);

		if (rc < 0)
		{
			return ServerState::CreateFailed;
		}

		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
		addr.sin_port = htons(port);

		rc = bind(_ListenerSocket, (struct sockaddr *)&addr, sizeof(addr));

		if (rc < 0)
		{
			return ServerState::BindFailed;
		}

		rc = listen(_ListenerSocket, 32);
		if (rc < 0)
		{
			return ServerState::ListenFailed;
		}

		FD_ZERO(&master_set);
		memset(&master_set, 0, sizeof(fd_set));
		max_sd = _ListenerSocket;
		FD_SET(_ListenerSocket, &master_set);

		timeout.tv_sec = 10 * 60;
		timeout.tv_usec = 0;

		while(true)
		{
			FD_ZERO(&working_set);
			memcpy(&working_set, &master_set, sizeof(master_set));

			rc = select(max_sd + 1, &working_set, NULL, NULL, &timeout);

			if (rc < 0)
			{
				int err = SOCKET_ERROR;
				break;
			}

			if (rc == 0)
			{
				break;
			}

			desc_ready = rc;

			for (socketIndex = 0; socketIndex <= max_sd && desc_ready > 0; ++socketIndex)
			{
				if (FD_ISSET(socketIndex, &working_set))
				{
					desc_ready -= 1;

					if (socketIndex == _ListenerSocket)
					{
						end_server = acceptConnections(this, _ListenerSocket, max_sd, &master_set);
					}

					else
					{
						end_server = receiveData(this, socketIndex, max_sd, &master_set);
					}
				}
			}
		}

		for (socketIndex = 0; socketIndex <= max_sd; ++socketIndex)
		{
			if (FD_ISSET(socketIndex, &master_set))
				closesocket(socketIndex);
		}

		return ServerState::NormalExit;
	}

	void TcpServer::stop()
	{
		TcpClient* ptr = *_ResponderList.getFirst();

		while (ptr != nullptr)
		{
			ptr->closeSocket();
			ptr = *_ResponderList.getNext();
		}

		_ResponderList.clear();

		shutdown(_ListenerSocket, 2);
		closesocket(_ListenerSocket);
	}

	DescriptorType TcpServer::descriptorType()
	{
		return FDServer;
	}

	bool acceptConnections(TcpServer *serverPtr, SOCKET listenerSocket, long &max_sd, struct fd_set *ptr)
	{
		bool end_server = false;
		SOCKET responderSocket = -1;

		do
		{
			responderSocket = accept(listenerSocket, NULL, NULL);

			if (responderSocket <= 0)
			{
				int res = socketerror;

				if ((res != EWOULDBLOCK) && (res != ECHILD) && (res != ERESTART) && (res != EINTR))
				{
					end_server = true;
				}
				break;
			}

			if (responderSocket != -1)
			{
				FD_SET(responderSocket, ptr);

				if (responderSocket > max_sd)
				{
					max_sd = responderSocket;
				}

				TcpClient *rp = new TcpClient(responderSocket);

				_ResponderList.insert(responderSocket, rp);
				serverPtr->connectionAccepted(rp);
			}
		} while (responderSocket != -1);

		return end_server;
	}

	bool receiveData(TcpServer* serverPtr, SOCKET responderSocket, long &max_sd, struct fd_set *ptr)
	{
		TcpClient* respptr = *_ResponderList.value(responderSocket);
		bool close_conn = false;



		/*
		int len = 0;
		char   buffer[81];
		buffer[80] = 0;
		int rc = -1;

		if (ptr != nullptr && respptr != nullptr)
		{
			do
			{
				rc = recv(responderSocket, buffer, sizeof(buffer), 0);

				if (rc < 0)
				{
					int res = socketerror;
					if (res != EWOULDBLOCK)
					{
						close_conn = true;
					}
					break;
				}

				if (rc == 0)
				{
					close_conn = true;
					break;
				}

				len = rc;

				if (ptr != nullptr)
				{
					serverPtr->dataReceived(respptr);
					respptr->dataReceived(buffer, len);
				}

			} while (true);
		}
		else
		{
			close_conn = true;
		}
		*/

		if (close_conn)
		{
			closesocket(responderSocket);
			FD_CLR(responderSocket, ptr);
			if (responderSocket == max_sd)
			{
				while (FD_ISSET(max_sd, ptr) == 0)
					max_sd -= 1;
			}

			serverPtr->connectionClosed(respptr);
		}

		return close_conn;
	}
}

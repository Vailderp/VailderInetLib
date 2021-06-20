#pragma once
#include "vildefs.h"

#pragma comment(lib, "ws2_32.lib")
#include <functional>
#include <iostream>
#include <map>
#include <vector>
#include <WinSock2.h>

#include "Packet.h"
#include "Socket.h"
#include "States.h"
#pragma warning( push )
#pragma warning( disable: 4996 )

_VIL_BEGIN
enum class ServerError
{
	SOCKET_CONNECTED,
	SERVER_INIT
};
_VIL_END

_VIL_BEGIN
class Server : public _VIL StatesDynamic
{
private:
	SOCKET* connections_;
	std::map<socket_n_t, Socket> sockets_;
	u_short n_connections_{ 0 };
	const u_short hostshort_;
	const char* inet_addr_;

public:
	explicit VIL_API Server(
		int n_connections = 1 << 8,
		u_short hostshort = 7070, 
		const char* inet_address = "127.0.0.1");

	VIL_API ~Server();

	virtual void on(packetc_t packet, Socket socket) = 0;
	
	virtual void onConnection(Socket socket) = 0;
	
	virtual void onDisconnection(Socket socket) = 0;
	
	virtual void onError(ServerError error) = 0;

	void VIL_API start();
	
	void VIL_API emit(packetc_t packet) const;

	void VIL_API emit(packetc_t packet, const Socket& exception_socket);

	void VIL_API emit(packetc_t packet, DWORD delay) const;

	void VIL_API emit(packetc_t packet, const Socket& exception_socket, DWORD delay);

	void VIL_API disconnectSocket(const Socket& socket);
	
	void VIL_API muteSocket(const Socket& socket);

};
_VIL_END

///////////////////////////////////////////////////////////////////////////////

#pragma warning( pop )
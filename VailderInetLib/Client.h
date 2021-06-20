#pragma once
#include "vildefs.h"

#pragma once
#pragma comment(lib, "ws2_32.lib")
#include <functional>
#include <iostream>
#include <map>
#include <vector>
#include <WinSock2.h>
#include "Error.h"
#include <thread>

#include "Packet.h"
#include "Socket.h"

#pragma warning( push )
#pragma warning( disable: 4996 )

_VIL_BEGIN
enum class SocketError
{
	SOCKET_CONNECTED,
	SERVER_INIT
};
_VIL_END

_VIL_BEGIN
class Client
{
private:
	const u_short hostshort_;
	const char* inet_addr_;
	SOCKET connection_;
	_VIL Socket socket_;

public:
	explicit Client(const u_short hostshort = 7070, const char* inet_address = "127.0.0.1");

	~Client();

	virtual void on(packetc_t packet, Socket socket) = 0;

	virtual void onConnection(Socket socket) = 0;

	virtual void onDisconnection(Socket socket) = 0;

	virtual void onError(SocketError error) = 0;

	void VIL_API start();

	bool VIL_API emit(packetc_t packet) const;

	void VIL_API emit(packetc_t packet, const DWORD delay) const;

};
_VIL_END
#pragma warning( pop )

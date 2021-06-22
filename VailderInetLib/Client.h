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
enum class ClientError
{
	SOCKET_CONNECT,
	SOCKET_INIT
};
_VIL_END

_VIL_BEGIN
class Client : public _VIL StatesDynamic
{
private:
	const u_short hostshort_;
	const char* inet_addr_;
	SOCKET connection_;
	_VIL Socket socket_;

public:
	explicit VIL_API Client(const u_short hostshort = 7070, const char* inet_address = "127.0.0.1");

	VIL_API ~Client();

	virtual void on(packetc_t packet) = 0;

	virtual void onConnection() = 0;

	virtual void onDisconnection() = 0;

	virtual void onError(ClientError error) = 0;

	void VIL_API start();

	bool VIL_API emit(packetc_t packet) const;

	void VIL_API emit(packetc_t packet, const DWORD delay) const;

};
_VIL_END
#pragma warning( pop )

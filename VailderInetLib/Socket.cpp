#include "pch.h"
#include "Socket.h"

_VIL Socket::Socket(SOCKET* connection, const socket_n_t index) :
	connection_(connection),
	index(index)
{
	
}

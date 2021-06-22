#pragma once
#include <WinSock2.h>
#include "vildefs.h"
#include "States.h"

using socket_n_t = int;

_VIL_BEGIN

class Server;

class Socket : public _VIL States
{

	friend class _VIL Server;

private:
	
	SOCKET* connection_;

public:
	
	socket_n_t index;
	
	Socket(SOCKET* connection = nullptr, const socket_n_t index = NULL);
	
};

_VIL_END

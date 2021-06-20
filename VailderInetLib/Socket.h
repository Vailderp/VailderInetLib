#pragma once
#include <WinSock2.h>
#include "vildefs.h"
#include "States.h"

using socket_n_t = int;

_VIL_BEGIN

class Socket : public _VIL States
{
private:

public:
	socket_n_t index;
	
	Socket(const socket_n_t index = NULL);
	
};

_VIL_END

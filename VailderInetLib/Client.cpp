#include "pch.h"
#include "Client.h"

#pragma warning( push )
#pragma warning( disable: 4996 )

_VIL Client::Client(const u_short hostshort, const char* inet_address):
	hostshort_(hostshort),
	inet_addr_(inet_address)
{

}

void vil::Client::start()
{
	WSAData wsa_data{};
	const WORD dll_version = MAKEWORD(2, 1);
	if (WSAStartup(dll_version, &wsa_data) != 0)
	{
		onError(ClientError::SOCKET_INIT);
	}
	else
	{
		onConnection();
		SOCKADDR_IN sockaddr_in;
		sockaddr_in.sin_addr.S_un.S_addr = inet_addr(inet_addr_);
		sockaddr_in.sin_port = htons(hostshort_);
		sockaddr_in.sin_family = AF_INET;

		connection_ = socket(AF_INET, SOCK_STREAM, NULL);

		if (connect(connection_, reinterpret_cast<SOCKADDR*>(&sockaddr_in), sizeof(sockaddr_in)) != 0)
		{
			onError(ClientError::SOCKET_CONNECT);
		}

		std::thread* thread = new std::thread(
			[&]()
			{
				int size_of_packet = 0;
				while (!this->stopped)
				{
					if (!this->paused && !this->muted)
					{
						if (recv(connection_, (char*)&size_of_packet, sizeof(int), NULL) != -1)
						{

							char* packet = new char[size_of_packet + 1];
							if (recv(connection_, packet, size_of_packet, NULL) != -1)
							{
								packet[size_of_packet] = '\0';
								on(packet);
								delete[] packet;
							}
							else
							{
								delete[] packet;
								break;
							}
						}
						else
						{
							break;
						}
					}
				}
				onDisconnection();
			}
		);

		thread->detach();
	}
}


_VIL Client::~Client()
{
	//delete inet_addr_;
}

bool _VIL Client::emit(packetc_t packet) const
{
	if (!this->paused)
	{
		int sizeof_packet = static_cast<int>(strlen(packet));
		send(connection_, reinterpret_cast<char*>(&sizeof_packet), sizeof(int), NULL);
		send(connection_, packet, sizeof_packet, NULL);
		return true;
	}
}

void _VIL Client::emit(packetc_t packet, const DWORD delay) const
{
	emit(packet);
	Sleep(delay);
}

#pragma warning( pop )
#include "pch.h"
#include "Error.h"
#include <thread>
#include "Server.h"

#pragma warning( push )
#pragma warning( disable: 4996 )

_VIL Server::Server(const int n_connections, const u_short hostshort, const char* inet_address) :
	connections_(new SOCKET[n_connections]),
	hostshort_(hostshort),
	inet_addr_(inet_address)
{
}

_VIL Server::~Server()
{
	delete[] connections_;
}


void vil::Server::start()
{
	WSAData wsa_data;
	const WORD dll_version = MAKEWORD(2, 1);
	if (WSAStartup(dll_version, &wsa_data) != 0)
	{
		onError(ServerError::SERVER_INIT);
	}

	SOCKADDR_IN sockaddr_in;
	sockaddr_in.sin_addr.S_un.S_addr = inet_addr(inet_addr_);
	sockaddr_in.sin_port = htons(hostshort_);
	sockaddr_in.sin_family = AF_INET;

	SOCKET socket_listen = socket(AF_INET, SOCK_STREAM, NULL);
	int sizeof_socket_listen = sizeof(sockaddr_in);
	bind(socket_listen, reinterpret_cast<SOCKADDR*>(&sockaddr_in), sizeof_socket_listen);
	listen(socket_listen, SOMAXCONN);

	SOCKET socket_connection;
	for (int l = 0; l < 1 << 8; l++)
	{
		socket_connection = accept(socket_listen, reinterpret_cast<SOCKADDR*>(&sockaddr_in), &sizeof_socket_listen);

		if (socket_connection == 0)
		{
			onError(ServerError::SOCKET_CONNECTED);
		}
		else
		{

			connections_[l] = socket_connection;
			n_connections_++;

			std::thread* thread = new std::thread(
				[&, l]()
				{
					this->onConnection(sockets_[l]);
					int sizeof_packet = 0;
					while (!this->stopped || !sockets_[l].stopped)
					{
						if (!sockets_[l].muted)
						{
							if (recv(connections_[l], enpack(&sizeof_packet), sizeof(int), NULL) != -1)
							{
								packet_t packet = new char[sizeof_packet + 1];
								if (recv(connections_[l], packet, sizeof_packet, NULL) != -1)
								{
									packet[sizeof_packet] = '\0';
									on(packet, sockets_[l]);
									delete[] packet;
								}
								else
								{
									break;
								}
							}
							else
							{
								break;
							}
						}
					}
					sockets_[l].stopped = true;
					onDisconnection(sockets_[l]);
				});
			thread->detach();
		}
	}
}

void _VIL Server::emit(const packetc_t packet) const
{
	if (!this->paused)
	{
		int sizeof_packet = static_cast<int>(strlen(packet));
		for (int i = 0; i < n_connections_; i++)
		{
			send(connections_[i], enpack(&sizeof_packet), sizeof(int), NULL);
			send(connections_[i], packet, sizeof_packet, NULL);
		}
	}
}

void _VIL Server::emit(const packetc_t packet, const Socket& exception_socket)
{
	int sizeof_packet = static_cast<int>(strlen(packet));
	for (int i = 0; i < n_connections_; i++)
	{
		if (sockets_[i].index == exception_socket.index)
		{
			continue;
		}
		send(connections_[i], enpack(&sizeof_packet), sizeof(int), NULL);
		send(connections_[i], packet, sizeof_packet, NULL);
	}
}

void _VIL Server::emit(const packetc_t packet, const DWORD delay) const
{
	emit(packet);
	Sleep(delay);
}

void _VIL Server::emit(const packetc_t packet, const Socket& exception_socket, const DWORD delay) 
{
	emit(packet, exception_socket);
	Sleep(delay);
}

void _VIL Server::disconnectSocket(const Socket& socket)
{
	sockets_[socket.index].stopped = true;
}

void _VIL Server::muteSocket(const Socket& socket)
{
	sockets_[socket.index].muted = true;
}




#pragma warning( pop )
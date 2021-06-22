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
			onError(ServerError::SOCKET_CONNECT);
		}
		else
		{
			connections_[l] = socket_connection;
			n_connections_++;
			sockets_[l] = Socket(&connections_[l], l);
			
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
					sockets_[l].stopped = true;
					onDisconnection(sockets_[l]);
				});
			thread->detach();
		}
	}
}

void _VIL Server::emitTo(const packetc_t packet, const Socket& to_socket) const
{
	int sizeof_packet = static_cast<int>(strlen(packet));
	send(*to_socket.connection_, enpack(&sizeof_packet), sizeof(int), NULL);
	send(*to_socket.connection_, packet, sizeof_packet, NULL);
}

void _VIL Server::emit(const packetc_t packet) const
{
	if (!this->paused)
	{
		for (const auto& socket : sockets_)
		{
			emitTo(packet, socket.second);
		}
	}
}

void _VIL Server::emitExc(const packetc_t packet, const Socket& exception_socket)
{
	for (const auto& socket : sockets_) 
	{
		if (socket.second.index != exception_socket.index)
		{
			emitTo(packet, socket.second);
		}
	}
}

void _VIL Server::emit(const packetc_t packet, const _VIL exception_socket_void exception_socket_void) 
{
	for (const auto& socket : sockets_)
	{
		if (exception_socket_void(socket.second))
		{
			emitTo(packet, socket.second);
		}
	}
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
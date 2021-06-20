#include "pch.h"
#include "Client.h"

#pragma warning( push )
#pragma warning( disable: 4996 )

_VIL Client::Client(const u_short hostshort, const char* inet_address):
	hostshort_(hostshort),
	inet_addr_(inet_address)
{
	WSAData wsa_data{};
	const WORD dll_version = MAKEWORD(2, 1);
	if (WSAStartup(dll_version, &wsa_data) != 0)
	{
		error("WORD & WSAData");
	}

	SOCKADDR_IN sockaddr_in;
	sockaddr_in.sin_addr.S_un.S_addr = inet_addr(inet_addr_);
	sockaddr_in.sin_port = htons(hostshort_);
	sockaddr_in.sin_family = AF_INET;

	connection_ = socket(AF_INET, SOCK_STREAM, NULL);

	if (connect(connection_, reinterpret_cast<SOCKADDR*>(&sockaddr_in), sizeof(sockaddr_in)) != 0)
	{
		error("connection");
	}

	std::thread* thread = new std::thread(
		[&]()
		{
			int size_of_packet = 0;
			while (true)
			{
				if (recv(connection_, (char*)&size_of_packet, sizeof(int), NULL) != -1)
				{
					char* packet = new char[size_of_packet + 1];
					if (recv(connection_, packet, size_of_packet, NULL) != -1)
					{
						packet[size_of_packet] = '\0';
						for (const auto& it : on_functions_)
						{
							if (strcmp(it.first, packet) == 0)
							{
								it.second(packet, connection_);
								break;
							}
						}

						on_any_function_(packet, connection_);
					}
					delete[] packet;
				}
			}
		}
	);
}

_VIL Client::~Client()
{
	//delete inet_addr_;
}

bool _VIL Client::emit(packetc_t packet) const
{
	int sizeof_packet = static_cast<int>(strlen(packet));
	send(connection_, reinterpret_cast<char*>(&sizeof_packet), sizeof(int), NULL);
	send(connection_, packet, sizeof_packet, NULL);
	return true;
}

void _VIL Client::emit(packetc_t packet, const DWORD delay) const
{
	emit(packet);
	Sleep(delay);
}

void _VIL Client::on(packetc_t packet, const std::function<void(const char*, SOCKET socket)>& on_function)
{
	std::cout << packet << std::endl;
	on_functions_[packet] = on_function;
}

void _VIL Client::onAny(const std::function<void(const char*, SOCKET socket)>& on_any_function)
{
	on_any_function_ = on_any_function;
}

#pragma warning( pop )
#include "Server.h"

Server::Server(unsigned short port) : listened_port_(port)
{
	LOG("Server started");
}

void Server::ConnectClients()
{
	while (true)
	{

		Client* new_client = new Client("Vait for name");
		sf::Socket::Status accept_status = listener_.accept(new_client->socket);

		if (accept_status == sf::Socket::Done)
		{
			//new_client->setBlocking(false);
			clients_.push_back(new_client);
			LOG("Added client ");
		}
		else LOG("Coudn not connect");
	}
}

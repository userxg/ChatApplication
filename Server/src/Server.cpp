#include "Server.h"

Server::Server(unsigned short port) : listened_port_(port)
{
	sf::Socket::Status listen_status = listener_.listen(listened_port_);

	if (listen_status != sf::Socket::Done) LOG("Failed to listen");
	else LOG("Server started");
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
			LOG("Added client " << clients_.size());
		}
		else LOG("Coudn not connect");
	}
}

void Server::Run()
{
	std::thread incoming_connection_thread(&Server::ConnectClients, this);

	incoming_connection_thread.join();
}

#include "Server.h"

Server::Server(unsigned short port) : listened_port_(port)
{
	sf::Socket::Status listen_status = listener_.listen(listened_port_);

	if (listen_status != sf::Socket::Done) LOG("Failed to listen");
	else LOG("Server started");
}

bool Server::NameIsTaken(const std::string& checked_name)
{
	for (const auto& client : clients_)
	{
		if (client->name == checked_name)
		{
			return true;
		}
	}

	return false;
}

bool Server::ClientExists(const std::string& checked_name)
{
	for (const auto& client : clients_)
	{
		if (client->name == checked_name)
		{
			return true;
		}
	}

	return false;
}

void Server::ConnectIncomingClients()
{
	while (true)
	{

		Client* new_client = new Client("Vait for name");
		sf::Socket::Status accept_status = listener_.accept(new_client->socket);

		if (accept_status == sf::Socket::Done)
		{
			new_client->socket.setBlocking(false);
			clients_.push_back(new_client);
			LOG("Added client " << clients_.size());
		}
		else LOG("Coudn not connect");
	}
}

void Server::ManageIncomingPackets()
{
	while (true)
	{
		for (size_t i = 0; i < clients_.size(); ++i)
		{
			Client* client = clients_[i];

			sf::Packet received_packet;

			sf::Socket::Status rev_packet_status = client->socket.receive(received_packet);

			switch (rev_packet_status)
			{
			case sf::Socket::Done:
				ProcessReceivedPacket(received_packet, client);
				break;
			case sf::Socket::NotReady:
				break;
			case sf::Socket::Disconnected:
				break;
			case sf::Socket::Error:
				LOG("ERROR on receiving packet");
				break;
			}
		}
	}
}

void Server::ProcessReceivedPacket(sf::Packet& received_packet, Client* client)
{
	MyMessage received_msg;
	received_packet >> received_msg;

	if (received_msg.is_new_client != true)
	{

		SendToClient(received_msg, client);
	}
	else
	{
		sf::Packet validation_packet;
		validation_packet << received_msg;

		if (NameIsTaken(received_msg.name_of_new_client))
		{
			received_msg.is_new_client = true;
			received_msg.name_is_taken = true;
			SendValidationResponse(validation_packet, client);
		}
		else
		{
			received_msg.is_new_client = false;
			received_msg.name_is_taken = false;
			client->name = received_msg.name_of_new_client;
			SendValidationResponse(validation_packet, client);
		}
	}


}

void Server::Run()
{
	std::thread incoming_connection_thread(&Server::ConnectIncomingClients, this);

	incoming_connection_thread.join();
}


Client* Server::FindClient(const std::string& name)
{
	for (auto& client : clients_)
	{
		if (client->name == name)
		{
			return client;
		}
	}
	return nullptr;
}

void Server::SendToClient(MyMessage& send_msg, Client* client)
{
	sf::Packet send_packet;

	//Change on Client online
	Client* client_to = FindClient(send_msg.client_to);
	if (client_to->online)
	{
		
		send_packet << send_msg;
		if (client->socket.send(send_packet) != sf::Socket::Done)
		{
			LOG("Cound't send packet");
		}
	}
	else
	{

		//send_msg << 
		//send_packet << send_msg;

	}
}

void Server::SendValidationResponse(sf::Packet& send_packet, Client* client)
{
	if (client->socket.send(send_packet) != sf::Socket::Done)
	{
		LOG("Cound't send packet");
	}
}

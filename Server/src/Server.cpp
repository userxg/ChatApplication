#include "Server.h"

Server::Server(unsigned short port) : listened_port_(port)
{
	sf::Socket::Status listen_status = listener_.listen(listened_port_);

	if (listen_status != sf::Socket::Done) LOG("Failed to listen");
	else LOG("Server started");
}

bool Server::NameIsTaken(const std::string& checked_name)
{
	LOG("Check if name is taken");
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


			//implement Disconected

			if (received_packet.getDataSize() > 0)
			{
				MyMessage received_msg;
				received_packet >> received_msg;
				ReceivedLog(received_msg);
				ProcessReceivedMessage(received_msg, client);
			}
		}
	}
}



void Server::ReceivedLog(const MyMessage& log_message) const
{
	
	if (log_message.sd.is_new_client)
		LOG("New client tries name: " << log_message.sd.new_client_name);
	else
		LOG("[" << log_message.cd.from << "->" << log_message.cd.to << "]: " << log_message.cd.message);
}

void Server::ProcessReceivedMessage(const MyMessage& received_msg, Client* client)
{
	if (received_msg.sd.is_new_client == true)
	{
		if (NameIsTaken(received_msg.sd.new_client_name))
		{
			MyMessage validation_response(true, true, "");
			LOG("Name is taken");
			SendValidationResponse(validation_response, client);

		}
		else
		{
			MyMessage validation_response(false, false, received_msg.sd.new_client_name);
			client->name = received_msg.sd.new_client_name;
			LOG("Name is available");
			SendValidationResponse(validation_response, client);
		}
		
	}
	else
	{
		SendToClient(received_msg, client);
	}


}

void Server::Run()
{
	std::thread incoming_connection_thread(&Server::ConnectIncomingClients, this);
	std::thread manage_incoming_packets(&Server::ManageIncomingPackets, this);


	incoming_connection_thread.join();
	manage_incoming_packets.join();
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

void Server::SendToClient(const MyMessage& send_msg, Client* client)
{
	sf::Packet send_packet;

	//Change on Client online
	Client* client_to = FindClient(send_msg.cd.to);
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

void Server::SendValidationResponse(const MyMessage& send_msg, Client* client)
{
	LOG("sending Validation response");
	sf::Packet send_packet;
	send_packet << send_msg;
	if (client->socket.send(send_packet) != sf::Socket::Done)
	{
		LOG("Cound't send packet");
	}
}

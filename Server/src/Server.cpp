#include "Server.h"

Server::Server(unsigned short port) : listened_port_(port)
{
	sf::Socket::Status listen_status = listener_.listen(listened_port_);

	if (listen_status != sf::Socket::Done) LOG("Failed to listen");
	else
	{
		std::ofstream data_base_create("D:\\CPP\\CMODULES\\projects\\4_ChatApplication\\DB\\0users.txt");
		if (data_base_create.is_open())
			LOG("DB created");
		data_base_create.close();
		LOG("Server started");
	}
}

void Server::TryRegisterClient(const MyMessage& received_msg, Client* reg_client)
{
	if (NameIsTaken(received_msg.sd.client_name))
	{
		MyMessage validation_response(ServerData::kNameIsTaken, "", "");
		LOG("Name is taken");
		SendToClient(validation_response, reg_client);

	}
	else
	{
		MyMessage validation_response(ServerData::kNewRegisterted, received_msg.sd.client_name, "");
		BroadcastMessage(validation_response);
		AddClientToDB(received_msg);
		LOG("Name is available");
		SendToClient(validation_response, reg_client);
	}

}

bool Server::NameIsTaken(const std::string& checked_name)
{
	LOG("Check if name is taken");
	std::ifstream data_base(std::string("D:\\CPP\\CMODULES\\projects\\4_ChatApplication\\DB\\0users.txt"));
	if (data_base.is_open())
	{
		while (!data_base.eof())
		{
			std::string key;
			std::string value;
			data_base >> key >> value;
			if (key == "name:")
			{
				if (value == checked_name)
				{
					data_base.close();
					return true;
				}
			}
		}
	}
	data_base.close();
	return false;
}

void Server::TryLoginClient(const MyMessage& received_msg, Client* log_client)
{
	if (ValidNamePassword(received_msg.sd.client_name, received_msg.sd.password))
	{
		MyMessage validation_response(ServerData::kNewLogged, received_msg.sd.client_name, "");
		BroadcastMessage(validation_response);
		LoadPenpals(validation_response);
		log_client->name = received_msg.sd.client_name;
		LOG("Name is available");
		SendToClient(validation_response, log_client);
	}
	else
	{
		MyMessage validation_response(ServerData::KWrongData, "", "");
		LOG("Wrong name or pswd");
		SendToClient(validation_response, log_client);
	}

}

bool Server::ValidNamePassword(const std::string& name, const std::string& password)
{
	return false;
}


//make another
void Server::BroadcastMessage(const MyMessage& send_msg)
{
	LOG("Broadcasting");
	MyMessage broad_msg = send_msg;
	for (auto& client : online_clients_)
	{
		if (client->name != "Unlogged")
		{
			SendToClient(broad_msg, client);
		}
	}
}

void Server::ConnectIncomingClients()
{
	while (true)
	{

		Client* new_client = new Client("Unlogged");
		sf::Socket::Status accept_status = listener_.accept(new_client->socket);

		if (accept_status == sf::Socket::Done)
		{
			new_client->socket.setBlocking(false);
			online_clients_.push_back(new_client);
			LOG("Added client " << online_clients_.size());
		}
		else LOG("Coudn not connect");
	}
}

void Server::DisconnectClient(Client* client, size_t position)
{
	LOG(client->name << " was disconnected");
	delete client;
	online_clients_.erase(online_clients_.begin() + position);
}

void Server::ManageIncomingPackets()
{
	while (true)
	{
		for (size_t i = 0; i < online_clients_.size(); ++i)
		{
			Client* client = online_clients_[i];

			sf::Packet received_packet;
			

			sf::Socket::Status rev_packet_status = client->socket.receive(received_packet);


			if (rev_packet_status == sf::Socket::Disconnected)
			{
				//erasing
				DisconnectClient(client, i);
				break;
			}

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




void Server::LoadPenpals(MyMessage& val_rsp_msg)
{
	
	std::ifstream data_base("D:\\CPP\\CMODULES\\projects\\4_ChatApplication\\DB\\0users.txt");

	if (data_base.is_open())
	{
		while (!data_base.eof())
		{
			std::string key;
			std::string value;
			data_base >> key >> value;
			/*if (key == "name:")
			{
				++val_rsp_msg.sd.penpals_cnt;
				LoadChat(val_rsp_msg.sd.client_name, value);
			}*/
		}
	}

	data_base.close();
	
}

void Server::LoadChat(const std::string& l_client_name, const std::string& penpal_name)
{
}


void Server::AddClientToDB(const MyMessage& val_msg)
{
	std::ofstream data_base("D:\\CPP\\CMODULES\\projects\\4_ChatApplication\\DB\\0users.txt", std::ios::app);
	if (data_base.is_open())
		LOG("ADD to DB");
	data_base << "name: " << val_msg.sd.client_name << "\n";
	data_base << "pswd: " << val_msg.sd.password << "\n";
	data_base.close();
}

void Server::ReceivedLog(const MyMessage& log_message) const
{
	switch (log_message.sd.response)
	{
	case ServerData::kRegistration:
		LOG("On Registration: " << log_message.sd.client_name);
	case ServerData::kLogin:
		LOG("On Login: " << log_message.sd.client_name);
	default:
		LOG("[" << log_message.cd.from << "->" << log_message.cd.to << "]: " << log_message.cd.message);
		break;
	}

		
}

void Server::ProcessReceivedMessage(const MyMessage& received_msg, Client* client)
{
	switch (received_msg.sd.response)
	{
	case ServerData::kRegistration:
		TryRegisterClient(received_msg, client);
	case ServerData::kLogin:
		TryLoginClient(received_msg, client);
	default:

		break;
	}
}

void Server::Run()
{
	std::thread incoming_connection_thread(&Server::ConnectIncomingClients, this);
	std::thread manage_incoming_packets(&Server::ManageIncomingPackets, this);


	incoming_connection_thread.join();
	manage_incoming_packets.join();
}


Client* Server::FindOnlineClient(const std::string& name)
{
	for (auto& client : online_clients_)
	{
		if (client->name == name)
		{
			return client;
		}
	}
	return nullptr;
}

void Server::SendToClient(const MyMessage& send_msg, Client* client_to)
{
	sf::Packet send_packet;
	send_packet << send_msg;
	if (client_to->socket.send(send_packet) != sf::Socket::Done)
	{
		LOG("Cound't send packet");
	}
	else LOG("Send to " << client_to->name);
}


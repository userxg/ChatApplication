#pragma once


#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <SFML/Network.hpp>


#define LOG(x) std::cout << x << std::endl


struct Client
{	
	Client(std::string name) : name(name), online(true) {}
	sf::TcpSocket socket;
	std::string name;
	bool online;
};

struct Message
{
	//for new clients
	bool is_new_client = false;
	bool name_is_taken = false;
	std::string name_of_new_client;

	//between existing clients
	std::string client_from;
	std::string client_to;
	std::string message;
};


sf::Packet& operator>>(sf::Packet& inp, Message& msg)
{
	inp >> msg.is_new_client >> msg.name_is_taken >> msg.name_of_new_client >> msg.client_from >> msg.client_to >> msg.message;
	return inp;
}
sf::Packet& operator<<(sf::Packet& out, const Message& msg)
{
	out << msg.is_new_client << msg.name_is_taken << msg.name_of_new_client << msg.client_from << msg.client_to << msg.message;
	return out;
}

class Server
{
private:
	
	sf::TcpListener listener_;
	std::vector<Client*> clients_;

	unsigned short listened_port_;


private:
	void ConnectIncomingClients();
	void ManageIncomingPackets();


	void ProcessReceivedPacket(sf::Packet& received_packet, Client* client);
	void SendValidationResponse(sf::Packet& validation_packet, Client* client);
	void SendToClient(Message& send_packet, Client* client);
	Client* FindClient(const std::string& name);
	
	
	bool ClientExists(const std::string& checked_name);
	bool NameIsTaken(const std::string& checked_name);
	

public:

	Server(unsigned short port);

	void Run();

};


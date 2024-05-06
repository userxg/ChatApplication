#pragma once


#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <SFML/Network.hpp>
#include "MyMessage.h"

#define LOG(x) std::cout << x << std::endl


struct Client
{	
	Client(std::string name) : name(name), online(true) {}
	sf::TcpSocket socket;
	std::string name;
	bool online;
};


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
	void SendToClient(MyMessage& send_packet, Client* client);
	Client* FindClient(const std::string& name);
	
	
	bool ClientExists(const std::string& checked_name);
	bool NameIsTaken(const std::string& checked_name);
	

	//LOG functions
	void ReceivedLog(sf::Packet& received_packet);
public:

	Server(unsigned short port);

	void Run();

};


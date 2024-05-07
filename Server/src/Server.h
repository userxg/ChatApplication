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


	void ProcessReceivedMessage(const MyMessage& received_msg, Client* client);
	
	void SendToClient(const MyMessage& send_msg, Client* client);
	void BroadcastMessage(const MyMessage& send_msg);
	Client* FindClient(const std::string& name);
	
	
	/*------------------Validation---------------------------------------------*/
	bool ClientExists(const std::string& checked_name);
	bool NameIsTaken(const std::string& checked_name);
	void SendValidationResponse(const MyMessage& validation_msg, Client* client);

	//LOG functions
	void ReceivedLog(const MyMessage& received_packet)const;
public:

	Server(unsigned short port);

	void Run();

};


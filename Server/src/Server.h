#pragma once


#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <SFML/Network.hpp>


#define LOG(x) std::cout << x << std::endl


struct Client
{	
	Client(std::string name) : name(name) {}
	sf::TcpSocket socket;
	std::string name;
};

class Server
{
private:
	sf::TcpListener listener_;
	std::vector<Client*> clients_;

	unsigned short listened_port_;


private:
	void ConnectClients();

public:

	Server(unsigned short port);

	void Run();

};


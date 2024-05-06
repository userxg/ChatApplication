#pragma once

#include <SFML/Network.hpp>
#include <iostream>

struct MyMessage
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


sf::Packet& operator>>(sf::Packet& inp, MyMessage& msg);


sf::Packet& operator<<(sf::Packet& out, const MyMessage& msg);


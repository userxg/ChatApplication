#pragma once

#include <SFML/Network.hpp>
#include <iostream>
//#include "Penpal.h"

struct MyMessage
{

	

	//Default
	MyMessage()
		: sd{ false, false, "" },cd{ "", "", ""} {};

	//for Server Exchange
	MyMessage(bool is_new_client, bool name_is_taken, std::string new_client_name)
		: sd{ is_new_client, name_is_taken, new_client_name}, cd{ "", "", "" }
	{}

		
	//for Clients chatting
	MyMessage(std::string name_from, std::string name_to, std::string message)
		: sd{ false, false, "" },cd{ name_from, name_to, message } {}

	struct ServerData
	{
		bool is_new_client = false;
		bool name_is_taken = false;
		std::string new_client_name;
		int penpals_cnt;
		std::vector<std::string> penpals;
	}sd;
	

	//between existing clients
	struct MessageInfo
	{
		std::string from;
		std::string to;
		std::string message;
	}cd;
	
};





sf::Packet& operator>>(sf::Packet& inp, MyMessage& msg);


sf::Packet& operator<<(sf::Packet& out, const MyMessage& msg);



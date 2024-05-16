#pragma once

#include <SFML/Network.hpp>
#include <iostream>
//#include "ChatMessage.h"
//#include "Penpal.h"
#include "ServerData.h"

namespace Net
{

	struct MyMessage
	{

		//Default
		MyMessage()
			: sd{ServerData::kNoResponse, "", "", 0, {} }, cd{ "", "", "" } {};

		//for Registration or Login
		MyMessage(int response, 
			const std::string& new_client_name, const std::string& password)
			: sd{ response, new_client_name, password, 0, {} }, cd{ "", "", "" }
		{}
		//for Clients chatting
		MyMessage(const std::string& name_from, const std::string& name_to, const std::string& message)
			: sd{ ServerData::kNoResponse, "", "", 0, {} }, cd{ name_from, name_to, message } {}


		//For server queries
		ServerData sd;

		//between existing clients
		ChatMessage cd;

	};


	sf::Packet& operator>>(sf::Packet& inp, MyMessage& msg);
	sf::Packet& operator<<(sf::Packet& out, const MyMessage& msg);

}





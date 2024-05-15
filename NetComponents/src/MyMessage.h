#pragma once

#include <SFML/Network.hpp>
#include <iostream>

namespace Net
{
	
	

	struct MyMessage
	{
		enum
		{
			kNoResponse,
			kNameIsTaken,
			kNewRegisterted,
			kRegistration,
			kLogin,
			kNewLogged,
			KWrongData
		};


		//Default
		MyMessage()
			: sd{kNoResponse, "", "", 0, {} }, cd{ "", "", "" } {};

		//for Registration of new client
		MyMessage(int response, 
			const std::string& new_client_name, const std::string& password)
			: sd{ response, new_client_name, password, 0, {} }, cd{ "", "", "" }
		{}


		//for Clients chatting
		MyMessage(const std::string& name_from, const std::string& name_to, const std::string& message)
			: sd{ kNoResponse, "", "", 0, {} }, cd{ name_from, name_to, message } {}

		struct ServerData
		{
			int response;
			std::string client_name;
			std::string password;
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

}





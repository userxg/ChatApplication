#pragma once

#include <SFML/Network.hpp>
#include <iostream>

namespace Net
{
	struct MyMessage
	{



		//Default
		MyMessage()
			: sd{ false, false, "" }, cd{ "", "", "" } {};

		//for Registration of new client
		MyMessage(bool is_new_client, bool name_is_taken,
			const std::string& new_client_name, const std::string& password)
			: sd{ is_new_client, name_is_taken, new_client_name, password }, cd{ "", "", "" }
		{}


		//for Clients chatting
		MyMessage(const std::string& name_from, const std::string& name_to, const std::string& message)
			: sd{ false, false, "" }, cd{ name_from, name_to, message } {}

		struct ServerData
		{
			bool is_new_client = false;
			bool name_is_taken = false;
			std::string client_name;
			std::string password;
			std::string db_file;
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





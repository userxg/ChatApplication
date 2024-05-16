#pragma once
#include <iostream>
#include <SFML/Network.hpp>


namespace Net
{
	struct ChatMessage
	{
		ChatMessage() :
			from(""), to(""), message("") {}
		ChatMessage(const std::string& from, const std::string& to, const std::string& message):
		from(from), to(to), message(message){}
		std::string from;
		std::string to;
		std::string message;
	};

	sf::Packet& operator>>(sf::Packet& inp, ChatMessage& ch_msg);

	sf::Packet& operator<<(sf::Packet& out, const ChatMessage& ch_msg);
}

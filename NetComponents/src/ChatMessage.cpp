#include "ChatMessage.h"


namespace Net
{

	sf::Packet& operator>>(sf::Packet& inp, ChatMessage& ch_msg)
	{
		inp >> ch_msg.from >> ch_msg.to >> ch_msg.message;
		return inp;
	}

	sf::Packet& operator<<(sf::Packet& out, const ChatMessage& ch_msg)
	{
		out << ch_msg.from << ch_msg.to << ch_msg.message;
		return out;
	}

}


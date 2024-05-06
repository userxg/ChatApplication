#include "MyMessage.h"

sf::Packet& operator>>(sf::Packet& inp, MyMessage& msg)
{
	inp >> msg.is_new_client >> msg.name_is_taken >> msg.name_of_new_client >> msg.client_from >> msg.client_to >> msg.message;
	return inp;
}

sf::Packet& operator<<(sf::Packet& out, const MyMessage& msg)
{
	out << msg.is_new_client << msg.name_is_taken << msg.name_of_new_client << msg.client_from << msg.client_to << msg.message;
	return out;
}

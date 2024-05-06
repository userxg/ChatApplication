#include "MyMessage.h"

sf::Packet& operator>>(sf::Packet& inp, MyMessage& msg)
{
	inp >> msg.sd.is_new_client >> msg.sd.name_is_taken >> msg.sd.new_client_name >> msg.cd.from >> msg.cd.to >> msg.cd.message;
	return inp;
}

sf::Packet& operator<<(sf::Packet& out, const MyMessage& msg)
{
	out << msg.sd.is_new_client << msg.sd.name_is_taken << msg.sd.new_client_name << msg.cd.from << msg.cd.to << msg.cd.message;
	return out;
}

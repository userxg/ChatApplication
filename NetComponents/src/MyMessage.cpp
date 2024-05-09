#include "MyMessage.h"


namespace Net
{
	sf::Packet& operator>>(sf::Packet& inp, MyMessage& msg)
	{
		inp >> msg.sd.is_new_client >> msg.sd.name_is_taken >> msg.sd.new_client_name >> msg.sd.penpals_cnt >>
			msg.cd.from >> msg.cd.to >> msg.cd.message;
		msg.sd.penpals.resize(msg.sd.penpals_cnt);
		for (int i = 0; i < msg.sd.penpals_cnt; ++i)
			inp >> msg.sd.penpals[i];
		return inp;
	}

	sf::Packet& operator<<(sf::Packet& out, const MyMessage& msg)
	{
		out << msg.sd.is_new_client << msg.sd.name_is_taken << msg.sd.new_client_name << msg.sd.penpals_cnt
			<< msg.cd.from << msg.cd.to << msg.cd.message;

		for (int i = 0; i < msg.sd.penpals_cnt; ++i)
			out << msg.sd.penpals[i];
		return out;
	}
}


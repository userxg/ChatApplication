#include "MyMessage.h"


namespace Net
{

	sf::Packet& operator>>(sf::Packet& inp, Penpal& penp)
	{
		inp >> penp.name >> penp.chat_size;
		penp.chat.resize(penp.chat_size);
		for (int i = 0; i < penp.chat_size; ++i)
		{
			inp >> penp.chat[i].from;
			inp >> penp.chat[i].to;
			inp >> penp.chat[i].message;
		}
		return inp;
	}

	sf::Packet& operator<<(sf::Packet& inp, const Penpal& penp)
	{
		inp << penp.name << penp.chat_size;
		for (int i = 0; i < penp.chat_size; ++i)
		{
			inp << penp.chat[i].from;
			inp << penp.chat[i].to;
			inp << penp.chat[i].message;
		}
		return inp;
	}


	sf::Packet& operator>>(sf::Packet& inp, MyMessage& msg)
	{
		inp >> msg.sd.response >> msg.sd.client_name >> msg.sd.password;
		inp >> msg.sd.penpals_cnt;
		msg.sd.penpals.resize(msg.sd.penpals_cnt);
		for (int i = 0; i < msg.sd.penpals_cnt; ++i)
		{
			inp >> msg.sd.penpals[i];
		}

		inp >> msg.cd.from >> msg.cd.to >> msg.cd.message;
		return inp;
	}


	sf::Packet& operator<<(sf::Packet& inp,const MyMessage& msg)
	{
		inp << msg.sd.response << msg.sd.client_name << msg.sd.password;
		inp << msg.sd.penpals_cnt;
		for (int i = 0; i < msg.sd.penpals_cnt; ++i)
		{
			inp << msg.sd.penpals[i];
		}

		inp << msg.cd.from << msg.cd.to << msg.cd.message;
		return inp;
	}

}


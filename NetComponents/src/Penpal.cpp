#include "Penpal.h"

namespace Net
{
    void Penpal::AddMessage(const ChatMessage& ch_msg)
    {
        ++chat_size_;
        chat_.push_back(ch_msg);
    }

	const ChatMessage& Penpal::operator[](int index) const
	{
		if (index >= 0 && index < chat_size_)
			return chat_[index];
		else
			throw std::out_of_range("Wrong index");
	}

	ChatMessage& Penpal::operator[](int index)
	{
		if (index >= 0 && index < chat_size_)
			return chat_[index];
		else
			throw std::out_of_range("Wrong index");
	}

    void Penpal::AddMessage(const std::string& from, const std::string& to, const std::string& message)
    {
        ChatMessage msg(from, to , message);
        AddMessage(msg);
    }

	
    
	sf::Packet& operator>>(sf::Packet& inp, Penpal& penp)
	{
		size_t chat_size;
		inp >> penp.setName() >> penp.setOnline()  >> chat_size;
		for (int i = 0; i < chat_size; ++i)
		{
			ChatMessage ch_msg;
			inp >> ch_msg;
			penp.AddMessage(ch_msg);
		}
		return inp;
	}

	sf::Packet& operator<<(sf::Packet& inp, const Penpal& penp)
	{
		inp << penp.getName() << penp.getOnline() << penp.getChatSize();
		for (int i = 0; i < penp.getChatSize(); ++i)
		{
			inp << penp[i];
		}
		return inp;
	}
}
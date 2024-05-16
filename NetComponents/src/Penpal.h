#pragma once

#include <vector>
#include <stdexcept>
#include <SFML/Network.hpp>

#include "ChatMessage.h"


namespace Net
{


	class Penpal
	{
		
		std::string name_;
		bool online_;
		//push message after into vector after sending message
		size_t chat_size_;
		std::vector<ChatMessage> chat_;

	public:

		Penpal() : name_(""), online_(false), chat_size_(0), chat_{} {};
		Penpal(std::string name, bool online) : name_(name), online_(online), chat_size_(0), chat_{} {}

		std::string& setName() { return name_; }
		const std::string& getName() const { return name_; }

		bool& setOnline() { return online_; }
		bool getOnline() const{ return online_; }
		bool IsOnline() const{ return online_ == true; }

		//size_t& setChatSize() { return chat_size_; }
		const size_t getChatSize() const { return chat_size_; }
		void resizeChat(size_t new_size) { chat_.resize(new_size); chat_size_ = chat_.size(); }
		void AddMessage(const std::string& from, const std::string& to, const std::string& message);
		void AddMessage(const ChatMessage& ch_msg);
		const ChatMessage& operator[](int index)const;
		ChatMessage& operator[](int index);

	};

	sf::Packet& operator>>(sf::Packet& inp, Penpal& penp);
	sf::Packet& operator<<(sf::Packet& inp, const Penpal& penp);



}




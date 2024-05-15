#pragma once
#include "ChatMessage.h"
#include <vector>

namespace Net
{


	struct Penpal
	{
		Penpal() : name(""), chat_size(0), chat{} {};
		Penpal(std::string name) : name(name), chat_size(0), chat{} {}
		std::string name;

		//push message after into vector after sending message
		size_t chat_size;
		std::vector<ChatMessage> chat;
	};

}




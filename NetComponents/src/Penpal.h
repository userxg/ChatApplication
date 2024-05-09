#pragma once
#include "MyMessage.h"



namespace Net
{


	struct Penpal
	{
		Penpal(std::string name) : name(name) {}
		std::string name;
		//push message after into vector after sending message
		std::vector<MyMessage> chatting;
	};

}




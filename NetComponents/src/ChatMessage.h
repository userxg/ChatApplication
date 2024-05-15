#pragma once
#include <iostream>


namespace Net
{
	struct ChatMessage
	{
		std::string from;
		std::string to;
		std::string message;
	};
}

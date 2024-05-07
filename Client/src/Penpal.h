#pragma once
#include <iostream>
#include "MyMessage.h"

struct Penpal
{
	Penpal(std::string name) : name(name) {}
	std::string name;
	std::vector<MyMessage> chatting;

};


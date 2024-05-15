#pragma once
#include <iostream>
#include <vector>
#include "Penpal.h"

namespace Net
{
	struct ServerData
	{
		enum
		{
			kNoResponse,
			kNameIsTaken,
			kNewRegisterted,
			kRegistration,
			kLogin,
			kNewLogged,
			KWrongData
		};


		int response;
		std::string client_name;
		std::string password;
		int penpals_cnt;
		std::vector<Penpal> penpals;
	};
}
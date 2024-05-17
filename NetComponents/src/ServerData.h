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
			kDisconnected,
			kRegistration,
			kLogin,
			kNewLogged,
			KWrongData,
			kAlreadyOnline
		};


		int response;
		std::string client_name;
		std::string password;
		int penpals_cnt;
		std::vector<Penpal> penpals;
	};
}
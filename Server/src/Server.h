#pragma once


#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <fstream>
#include "MyMessage.h"

#define LOG(x) std::cout << x << std::endl
using namespace Net;

struct Client
{	
	Client(std::string name) : name(name) {}
	sf::TcpSocket socket;
	std::string name;
};


class Server
{
private:
	
	sf::TcpListener listener_;
	std::vector<Client*> online_clients_;

	unsigned short listened_port_;

	std::string db_file_name_;
	std::string db_dir_path_;


private:


	/*-------------------------Net functions-----------------------------*/
	void ConnectIncomingClients();
	void DisconnectClient(Client* client, size_t position);
	void ManageIncomingPackets();


	void ProcessReceivedMessage(const MyMessage& received_msg, Client* client);
	
	void SendToClient(const MyMessage& send_msg, Client* client_to);
	void BroadcastMessage(const MyMessage& send_msg);
	bool IsOnline(const std::string& name)const;
	
	/*--------------Registration--------------------*/

	void TryRegisterClient(const MyMessage& received_msg, Client* client);
	bool NameIsTaken(const std::string& checked_name);
	
	/*---------------Login--------------------------------------*/
	void TryLoginClient(const MyMessage& received_msg, Client* log_client);
	bool ValidNamePassword(const std::string& name, const std::string& password)const;
	void LoadPenpals(MyMessage& val_resp_msg);
	void LoadChatInPenpal(const std::string& l_client_name, Penpal& penpal);
	void LoadMessagesInPenpal(Penpal& penpal, std::ifstream& chat_file);


	/*---------------------------Data Base functions---------------------*/
	void AddClientToDB(const MyMessage& validation_msg);
	//LOG functions
	void ReceivedLog(const MyMessage& received_packet)const;
public:

	Server(unsigned short port);

	void Run();

};


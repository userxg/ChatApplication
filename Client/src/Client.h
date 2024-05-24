#pragma once

#include "imgui.h"
#include "imgui-SFML.h"
#include <imgui_stdlib.h>

#include <thread>
#include <stdexcept>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include "MyMessage.h"
//#include "Penpal.h"
#include "InvalidInput.h"


#define LOG(x) std::cout << x << std::endl

using namespace Net;

class Client
{

private:
	/*----------------Start up Members----------------------*/
	sf::VideoMode video_mode_;
	sf::RenderWindow* window_;
	sf::Event sfml_evnt_;
	sf::Clock delta_clock_;

	/*------------------------Login--------------------------*/
	std::string name_;
	std::string password_;
	std::string re_password_;
	bool logged_;
	bool opened_log_wind_;
	InputError input_error_;
	bool app_closed_;

	/*------------------------Chat---------------------------*/
	std::vector<Penpal*> penpals_;
	int selected_penpal_ = 0;
	bool opened_chat_window;
	std::string input_msg_buf;
	ImGuiWindowFlags w_flags_;

	/*-------------------Work with sockets------------------*/
	sf::TcpSocket socket_;
	sf::Packet last_packet_;
	


private:
	/*-------------Start up Functions----------------------*/
	void InitSFMLWindow();
	void InitImGui();
	void PollEvents();
	bool Running() const;
	void Update();
	void Render();
	void SFMLGuiRun();
	void InitVariables();
	//For all ImGui windows
	void RenderImGui();


	void SetUpFont();

	/*----------------ImGui Windows-----------------------*/
	void LoginForm();
	void Application();
	void ProfileWindow();
	void ChatWindow();
	void MemberWindow();
	void SendWindow();



	/*--------------------Network-----------------------------------------------*/
	void ConnectToServer(const char * ip_adress, unsigned short port);

	void ReceivePackets();
	void ProcessIncomingMessage(const MyMessage& received_msg);
	void SendToMessage(const std::string msg);
	int FindByName(const std::string sender_name);



	/*---------------------Login system-------------------*/
	void LoginWindow();
	void TryLogin(const std::string& name, const std::string& pswd);
	bool CheckCorrectPassword(const std::string& pswd);
	void SendValidationQuery(const std::string& name, const std::string& pswd);
	MyMessage ValidaionResponse();
	void DownloadPenpals(MyMessage& val_rsp_msg);
	void LogOut();
	void SendLogOutQuery();


	/*----------Registration----------*/
	void RegistrationWindow();
	bool CheckCorrectPassword(const std::string& pswd, const std::string& r_pswd);
	void TryRegister(const std::string& name, const std::string& passoword);
	void SendRegisterQuery(const std::string& name, const std::string& pswd);
	bool CheckCorrectName(const std::string& name);


	/*--------------Test & Debug & Memory-------------------------*/
	void InitTestData();
	void LogReceivedMessage(const MyMessage& received_msg);


	void DeletePenpals();

public:
	Client();
	void Run();
	~Client();
	
};

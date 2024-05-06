#pragma once

#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include "MyMessage.h"

#define LOG(x) std::cout << x << std::endl
class Client
{

private:
	/*-------------Start up Members----------------------*/
	sf::VideoMode video_mode_;
	sf::RenderWindow* window_;
	sf::Event sfml_evnt_;
	sf::Clock delta_clock;

	/*Input window*/
	char name_[100];
	bool chat_window_opened_;

	/*Work with sockets*/
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
	//For all ImGui windows
	void RenderImGui();


	void SetUpFont();

	/*----------------ImGui Windows-----------------------*/
	void InputName();
	void InputButton();
	void ChatWindow();
	void ConnectToServer(const char * ip_adress, unsigned short port);
	void SendPacketToServer(sf::Packet & packet);
	MyMessage ReceivePacketsFromServer(sf::TcpSocket * socket);

public:
	Client();
	void Run();
	~Client();
};


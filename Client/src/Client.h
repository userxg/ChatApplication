#pragma once

#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>


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
	bool press_btn_;


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


public:
	Client();
	void Run();
	~Client();
};


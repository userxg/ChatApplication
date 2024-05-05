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


private:
	/*-------------Start up Functions----------------------*/
	void InitSFMLWindow();
	void PollEvents();
	bool Running() const;
	void Update();
	void Render();
	//For all ImGui windows
	void RenderImGui();


	/*----------------ImGui Windows-----------------------*/
	void Example();

public:
	Client();
	void Run();
	~Client();
};


#include "Client.h"

void Client::InitSFMLWindow()
{
	video_mode_ = sf::VideoMode(1920, 1080);
	window_ = new sf::RenderWindow(video_mode_, "client", sf::Style::Close | sf::Style::Titlebar);
	ImGui::SFML::Init(*window_);

	window_->setFramerateLimit(60);
}

void Client::PollEvents()
{
	while (window_->pollEvent(sfml_evnt_))
	{
		ImGui::SFML::ProcessEvent(sfml_evnt_);
		switch (sfml_evnt_.type)
		{
		case sf::Event::Closed:
			window_->close();
			break;
		}
	}

}

void Client::Example()
{
	if (ImGui::Begin("Example"))
	{
	}ImGui::End();
}

Client::Client()
{
	InitSFMLWindow();
}

void Client::Run()
{
	while (Running()) {

		Update();
		Render();
	}

	ImGui::SFML::Shutdown();
}

Client::~Client()
{

}

bool Client::Running() const
{
	return window_->isOpen();
}

void Client::Update()
{
	//Update PollEvents
	PollEvents();
	ImGui::SFML::Update(*window_, delta_clock.restart());


}

void Client::RenderImGui()
{
	/*Implemen all ImGui windwos here
	* 
		Window1();
		Window2();

	*/
	Example();
}

void Client::Render()
{
	RenderImGui();
	window_->clear();
	ImGui::SFML::Render(*window_);
	window_->display();
}

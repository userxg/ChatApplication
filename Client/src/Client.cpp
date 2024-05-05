#include "Client.h"
#include <iostream>

struct Message
{
	//for new clients
	bool is_new_client = false;
	bool name_is_taken = false;
	std::string name_of_new_client;

	//between existing clients
	std::string client_from;
	std::string client_to;
	std::string message;
};

sf::Packet& operator>>(sf::Packet& inp, Message& msg)
{
	inp >> msg.is_new_client >> msg.name_is_taken >> msg.name_of_new_client >> msg.client_from >> msg.client_to >> msg.message;
	return inp;
}





void Client::InitSFMLWindow()
{
	video_mode_ = sf::VideoMode(1920, 1080);
	window_ = new sf::RenderWindow(video_mode_, "client", sf::Style::Close | sf::Style::Titlebar);
	window_->setFramerateLimit(60);
}

void Client::InitImGui()
{
	//ImGui::SFML::Init(*window_, false); for font
	ImGui::SFML::Init(*window_);
	SetUpFont();
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



Client::Client()
{
	InitSFMLWindow();
	InitImGui();
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

	if (chat_window_opened_ == false)
	{
		InputName();
	}
	else
	{
		ChatWindow();
	}
}

void Client::SetUpFont()
{
	//IO.Fonts->Clear(); // clear fonts if you loaded some before (even if only default one was loaded)
}



void Client::Render()
{
	RenderImGui();
	window_->clear();
	ImGui::SFML::Render(*window_);
	window_->display();
}


void Client::InputName()
{
	chat_window_opened_ = false;
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(1920, 1080));
	if (ImGui::Begin("Input Block", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)) {
		ImGui::SetWindowFontScale(2);
		//auto& style = ImGui::GetStyle();
		//style.Colors[ImGuiCol_WindowBg] = ImColor(25, 123, 255);
		ImGui::Text("User name: ");
		ImGui::InputText("##user name", name_, 100);
		//std::cout << name_;
		InputButton();
	}
	ImGui::End();
}

void Client::InputButton()
{
	if (ImGui::Button("Continue", ImVec2(300, 50)))
	{
		chat_window_opened_ = true;
		//if (SendName())
		//{
		//	chat_window_opened  = true;
		//	//
		//}
		//else
		//{
		//	//clean name
		//}
			


	};

	//auto& style = ImGui::GetStyle();
	//style.Colors[ImGuiCol_Button] = ImColor(35, 33, 181);

}

void Client::ChatWindow()
{
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(1920, 1080));
	if (ImGui::Begin("Test", nullptr, ImGuiWindowFlags_NoTitleBar| ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)) {
		ImGui::SetWindowFontScale(2);
		ImGui::Text("User name:");
		ImGui::Text(name_);
	}
	ImGui::End();
}

void Client::SendNameToServer()
{

}





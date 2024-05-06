#include "Client.h"
#include <iostream>






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
	ConnectToServer("127.0.0.1", 2525);
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
	if (logged_)
	{
		ChatWindow();
	}
	else
	{
		LoginForm();
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


void Client::LoginForm()
{
	//CC - check change
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(1920, 1080));
	if (ImGui::Begin("Input Block", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)) {
		
		//CC
		ImGui::SetWindowFontScale(2);
		ImGui::Text("New name: ");
		ImGui::InputText("##name", &name_);

		if (ImGui::Button("Login"))
		{
			//
			if (IsValidName(name_))
			{
				logged_ = true;
			}
			else
			{

			}
		}

		//if ()
		//ImGui::Text("New name: ");

		
	}
	ImGui::End();
}



void Client::ChatWindow()
{
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(1920, 1080));
	if (ImGui::Begin("Test", nullptr, ImGuiWindowFlags_NoTitleBar| ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)) {
		ImGui::SetWindowFontScale(2);
		ImGui::Text("Chat window");
		
	}
	ImGui::End();
}



void Client::ConnectToServer(const char* ip_adress, unsigned short port)
{
	if (socket_.connect(ip_adress, port) != sf::Socket::Done) {
		LOG("Could not connect to the server");
	}
	else {
		LOG("I am connected");
	}
}

bool Client::IsValidName(const std::string& name) const
{
	SendValidationQuery(name);

	MyMessage val_response = ValidaionResponse();

	if (val_response.sd.name_is_taken)
	{
		LOG("Name: " << name << " is taken");
		return false;
	}
	else
	{
		LOG("welcome to usf");
		return true;
	}

}

void Client::SendValidationQuery(const std::string& name) const
{
	MyMessage validation_msg(true, false, name);
	sf::Packet val_packet;
	val_packet << validation_msg;

	if (socket_.send(val_packet) != sf::Socket::Done)
	{
		LOG("Could not send Validation request");
	}
	else {
		LOG("Validation request");
	}
}

MyMessage Client::ValidaionResponse() const
{
	sf::Packet val_pckt;

	sf::Socket::Status receive_status = socket_.receive(val_pckt);

	if (receive_status == sf::Socket::Done)
	{
		MyMessage validation_msg;
		val_pckt >> validation_msg;
		LOG("getting validation response");
		return validation_msg;
	}
}






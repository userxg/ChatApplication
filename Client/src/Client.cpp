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

		MyMessage validation_msg;
		validation_msg.is_new_client = true;
		validation_msg.name_of_new_client = name_;
		sf::Packet validation_packet;
		validation_packet << validation_msg;

		SendPacketToServer(validation_packet);

		validation_msg = ReceivePacketsFromServer(&socket_);

		if (validation_msg.name_is_taken == false)
		{
			chat_window_opened_ = true;
		}

	
		//if (ReceivePacketsFromServer(socket_))
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



void Client::ConnectToServer(const char* ip_adress, unsigned short port)
{
	if (socket_.connect(ip_adress, port) != sf::Socket::Done) {
		LOG("Could not connect to the server");
	}
	else {
		LOG("I am connected");
	}
}

void Client::SendPacketToServer(sf::Packet & packet)
{
	if (socket_.send(packet) != sf::Socket::Done)
	{
		LOG("Could not send packet");
	}
	else {
		LOG("I sent the packet");
	}
}

MyMessage Client::ReceivePacketsFromServer(sf::TcpSocket* socket)
{

	sf::Socket::Status packet_status = socket->receive(last_packet_);

	if (packet_status == sf::Socket::Done)
	{
		MyMessage message_from_server;
		last_packet_ >> message_from_server;
		LOG("Validation response");
		return message_from_server;
	}

}





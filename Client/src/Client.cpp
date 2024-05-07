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



void Client::InitTestData()
{

	penpals_.resize(3);
	penpals_[0] = new Penpal("Nikolay");
	penpals_[1] = new Penpal("Vasiluy");
	penpals_[2] = new Penpal("Someone");
}

void Client::LogReceivedMessage(const MyMessage& received_msg)
{
	if (received_msg.sd.is_new_client)
	{
		LOG("New client: " << received_msg.sd.new_client_name);
	}
	else
	{
		LOG("[" << received_msg.cd.from << "->" << received_msg.cd.to << "]: " << received_msg.cd.message);
	}
}

Client::Client()
{
	ConnectToServer("127.0.0.1", 2525);
	InitSFMLWindow();
	InitImGui();

	InitTestData();


}

void Client::Run()
{
	std::thread receive_incoming_messages(&Client::ReceivePackets, this);
	//std::thread sfml_run(&Client::SFMLGuiRun, this);
	
	SFMLGuiRun();

	receive_incoming_messages.join();
	//sfml_run.join();
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

void Client::SFMLGuiRun()
{
	while (Running()) {

		Update();
		Render();
	}

	ImGui::SFML::Shutdown();
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
				socket_.setBlocking(false);
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
		ImGui::Text(name_.c_str(), "  Chat window");

		

		if (ImGui::BeginListBox("listbox 1"))
		{
			for (int n = 0; n < penpals_.size(); n++)
			{
				const bool is_selected = (item_current_idx == n);
				if (ImGui::Selectable(penpals_[n]->name.c_str(), is_selected))
					item_current_idx = n;

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndListBox();
		}


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

void Client::ReceivePackets()
{
	LOG("Receive Thread started");
	while (true)
	{
		if (logged_)
		{
			LOG("Receiving....");
			sf::Packet received_pckt;
			sf::Socket::Status rev_packet_status = socket_.receive(received_pckt);

			if (received_pckt.getDataSize() > 0)
			{
				MyMessage received_msg;
				received_pckt >> received_msg;
				LogReceivedMessage(received_msg);
				ProcessIncomingMessage(received_msg);
			}
			/*if (socket_.receive(received_pckt) == sf::Socket::Done)
			{

				MyMessage received_msg;
				received_pckt >> received_msg;
				LogReceivedMessage(received_msg);
				ProcessIncomingMessage(received_msg);

			}*/
			//else LOG("Recive Packet problem");
		}
	}
}

void Client::ProcessIncomingMessage(const MyMessage& received_msg)
{

	if (received_msg.sd.is_new_client)
	{
		Penpal* new_penpal = new Penpal(received_msg.sd.new_client_name);
		penpals_.push_back(new_penpal);
	}
	else
	{
		//----------------
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
		LOG("welcome " << name_);
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






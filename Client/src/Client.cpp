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
	penpals_[0] = new Penpal("Nikolay", true);
	penpals_[1] = new Penpal("Vasiluy", false);
	penpals_[0]->AddMessage(name_, penpals_[0]->getName(), "hello, Nikolay");
	penpals_[0]->AddMessage( penpals_[0]->getName(), name_, "hello, " + name_);
}

void Client::LogReceivedMessage(const MyMessage& received_msg)
{
	switch (received_msg.sd.response)
	{
	case ServerData::kNewRegisterted:
		LOG("New Registered: " << received_msg.sd.client_name);
		break;
	case ServerData::kLogin:
		LOG("get Online: " << received_msg.sd.client_name);
		break;
	default:
		LOG("[" << received_msg.cd.from << "->" << received_msg.cd.to << "]: " << received_msg.cd.message);
		break;
	}

}

Client::Client() : name_(""), logged_(false), opened_chat_window(false), opened_log_wind_(true),
input_error_{InvalidInput::kNoErrors, InvalidInput::kNoErrors}
{
	ConnectToServer("127.0.0.1", 2525);
	InitVariables();
	InitSFMLWindow();
	InitImGui();

	//InitTestData();


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
	ImGui::SFML::Update(*window_, delta_clock_.restart());


}

void Client::RenderImGui()
{
	if (logged_)
	{
		Application();
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

void Client::InitVariables()
{

}


void Client::LoginForm()
{
	if (opened_log_wind_)
	{
		LoginWindow();
	}
	else
	{
		RegistrationWindow();
	}
	
}

void Client::LoginWindow()
{
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(1920, 1080));
	if (ImGui::Begin("Log in", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)) {

		//CC
		ImGui::SetWindowFontScale(2);
		ImGui::Text("username ");
		ImGui::InputText("##name", &name_);
		ImGui::Text("password ");
		ImGui::InputText("##password", &password_, ImGuiInputTextFlags_Password);


		


		if (ImGui::Button("I'm new"))
		{
			input_error_.area = InvalidInput::kNoErrors;
			opened_log_wind_ = false;
			name_ = "";
			password_ = "";
		}
		ImGui::SameLine();

		if (ImGui::Button("Sign in"))
		{
			
			bool valid_name = CheckCorrectName(name_);
			bool valid_password = CheckCorrectPassword(password_);
			if (valid_name && valid_password)
			{
				TryLogin(name_, password_);
			}
			else
			{
				input_error_.area = InvalidInput::kWrongLoginData;
			}
		}

		if (input_error_.area == InvalidInput::kWrongLoginData)
		{
			ImGui::Text("wrong username or password");
		}


	}
	ImGui::End();
}

void Client::RegistrationWindow()
{
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(1920, 1080));
	if (ImGui::Begin("Registration", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)) {

		//CC
		ImGui::SetWindowFontScale(2);
		ImGui::Text("new name: ");
		ImGui::InputText("##name", &name_);
		ImGui::Text("password ");
		ImGui::InputText("##password", &password_, ImGuiInputTextFlags_Password);
		ImGui::Text("re-password ");
		ImGui::InputText("##re-password", &re_password_, ImGuiInputTextFlags_Password);


		if (ImGui::Button("Sign in"))
		{
			opened_log_wind_ = true;
			name_ = "";
			password_ = "";
			re_password_ = "";
		}
		ImGui::SameLine();

		if (ImGui::Button("Register"))
		{
			bool valid_name = CheckCorrectName(name_);
			bool valid_password = CheckCorrectPassword(password_, re_password_);
			if (valid_name && valid_password)
			{
				TryRegister(name_, password_);
			}
		}

		switch (input_error_.area)
		{
		case InvalidInput::kInvalidName:
			switch (input_error_.type)
			{
			case InvalidInput::kEmptyName:
				ImGui::Text("Empty name");
				break;
			case InvalidInput::kLongName:
				ImGui::Text("Name is too long");
				break;
			case InvalidInput::kWrongFirstNameChar:
				ImGui::Text("Name should start with character");
				break;
			case InvalidInput::kWrongCharInsideName:
				ImGui::Text("Invalid name: use only numbers, characters or \"_\"");
				break;
			case InvalidInput::kNameIsTaken:
				ImGui::Text(std::string("Name is taken").c_str());
				break;
			}
		case InvalidInput::kInvalidPassword:
			switch (input_error_.type)
			{
			case InvalidInput::kRePasswordIsNotEqual:
				ImGui::Text("re-password isn't equal");
				break;
			case InvalidInput::kEmptyPassword:
				ImGui::Text("Empty password");
				break;
			case InvalidInput::kLongPassword:
				ImGui::Text("Password is to long");
				break;
			case InvalidInput::kWrongCharInsidePassword:
				ImGui::Text("Inavalid password: use only numbers, characters or  ~`!@#$%^&*()_-+={[}]|\\:;\"'<,>.?/,");
				break;
			}
		default:
			break;
		}

		

		
	}
	ImGui::End();
}






void Client::Application()
{
	MemberWindow();
	if (opened_chat_window)
	{
		ChatWindow();
		SendWindow();
	}
	
}

void Client::MemberWindow()
{
	ImGui::SetNextWindowSize(ImVec2(400, 1080));
	if (ImGui::Begin("Members", NULL, NULL))
	{
		ImGui::SetWindowPos(ImVec2(0, 0));
		ImGui::SetWindowFontScale(2);


		for (int i = 0; i < penpals_.size(); i++)
		{

			std::string penpal =
				penpals_[i]->getName() + (penpals_[i]->IsOnline() ? " : Online" : " : Offline");

			if (ImGui::Selectable(penpal.c_str(), selected_penpal_ == i))
			{
				selected_penpal_ = i;
				opened_chat_window = true;
			}
		}
	}ImGui::End();
}


void Client::ChatWindow()
{
	ImGui::SetNextWindowSize(ImVec2(1520, 800));
	if (ImGui::Begin("messages", &opened_chat_window, NULL))
	{
		ImGui::SetWindowPos(ImVec2(400, 0));
		ImGui::SetWindowFontScale(2);
		
		for (int i = 0; i < penpals_[selected_penpal_]->getChatSize(); ++i)
		{
			std::string msg;

			if ((*penpals_[selected_penpal_])[i].from != name_)
			{
				msg += ((*penpals_[selected_penpal_])[i].from +
					+ " : " + (*penpals_[selected_penpal_])[i].message);
			}
			else
			{
				msg +=  ("me : " + (*penpals_[selected_penpal_])[i].message);
			}
			ImGui::Text(msg.c_str());
		}

	}ImGui::End();
}



void Client::SendWindow()
{
	ImGui::SetNextWindowSize(ImVec2(1520, 280));
	if (ImGui::Begin("##sendwindow", &opened_chat_window, ImGuiWindowFlags_NoTitleBar))
	{
		ImGui::SetWindowPos(ImVec2(400, 800));
		ImGui::SetWindowFontScale(2);
		ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CtrlEnterForNewLine;

		ImGui::SetCursorPos(ImVec2(40, 80));
		bool enter_input = ImGui::InputTextMultiline("##onsend", &input_msg_buf, ImVec2(1200, ImGui::GetTextLineHeight() * 5), input_text_flags);
		ImGui::SameLine();

		ImGui::SetCursorPos(ImVec2(1280, 100));
		bool button_input = ImGui::Button("Send", ImVec2(100, 100));


		if (enter_input || button_input)
		{
			SendToMessage(input_msg_buf);
			input_msg_buf = "";
		}

	}ImGui::End();

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
			//LOG("Receiving....");
			sf::Packet received_pckt;
			sf::Socket::Status rev_packet_status = socket_.receive(received_pckt);

			if (received_pckt.getDataSize() > 0)
			{
				MyMessage received_msg;
				received_pckt >> received_msg;
				LogReceivedMessage(received_msg);
				ProcessIncomingMessage(received_msg);
			}
		}
	}
}

void Client::ProcessIncomingMessage(const MyMessage& received_msg)
{

	switch (received_msg.sd.response)
	{
	case ServerData::kNoResponse:
	{
		int sender = FindByName(received_msg.cd.from);
		penpals_[sender]->AddMessage(received_msg.cd);
		break;
	}
	case ServerData::kNewRegisterted:
	{
		Penpal* new_penpal = new Penpal(received_msg.sd.client_name, false);
		penpals_.push_back(new_penpal);
		break;
	}
	case ServerData::kNewLogged:
	{
		int logged_penpal = FindByName(received_msg.cd.from);
		penpals_[logged_penpal]->setOnline() = true;
		break;
	}
	case ServerData::kDisconnected:
	{
		int logged_penpal = FindByName(received_msg.cd.from);
		penpals_[logged_penpal]->setOnline() = false;
		break;
	}
	default:
		break;
	}


}

void Client::TryRegister(const std::string& name, const std::string& pass)
{
	SendRegisterQuery(name, pass);
	MyMessage val_response = ValidaionResponse();

	if (val_response.sd.response == ServerData::kNameIsTaken)
	{
		LOG("Name: " << name << " is taken");
		input_error_.area = InvalidInput::kInvalidName;
		input_error_.type = InvalidInput::kNameIsTaken;
	}
	else
	{
		LOG("Registered " << name_);
		opened_log_wind_ = true;
		name_ = "";
		password_ = "";
		re_password_ = "";
	}
}

void Client::TryLogin(const std::string& name, const std::string& pswd)
{
	SendValidationQuery(name, pswd);

	MyMessage val_response = ValidaionResponse();

	if (val_response.sd.response == ServerData::KWrongData)
	{
		LOG("Wrong username or password");
		input_error_.area = InvalidInput::kWrongLoginData;
	}
	else
	{
		LOG("welcome " << name_);
		DownloadPenpals(val_response);
		name_ = val_response.sd.client_name;
		logged_ = true;
		socket_.setBlocking(false);
	}
}

void Client::SendRegisterQuery(const std::string& name, const std::string& pswd)
{
	MyMessage validation_msg(ServerData::kRegistration, name, pswd);
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

void Client::SendValidationQuery(const std::string& name, const std::string& pswd)
{
	MyMessage validation_msg(ServerData::kLogin, name, pswd);
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

MyMessage Client::ValidaionResponse() 
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



void Client::DownloadPenpals(MyMessage& val_rsp_msg)
{
	for (int i = 0; i < val_rsp_msg.sd.penpals_cnt; ++i)
	{
		Penpal* new_penpal = new Penpal(val_rsp_msg.sd.penpals[i]);
		penpals_.push_back(new_penpal);
	}
}


bool Client::CheckCorrectName(const std::string& name)
{
	if (name.size() == 0)
	{
		input_error_.area = InvalidInput::kInvalidName;
		input_error_.type = InvalidInput::kEmptyName;
		return false;
	}

	if (name.size() > 20)
	{
		input_error_.area = InvalidInput::kInvalidName;
		input_error_.type = InvalidInput::kLongName;
		return false;
	}

	if (!((name[0] >= 'A' && name[0] <= 'Z') || (name[0] >= 'a' && name[0] <= 'z')))
	{
		input_error_.area = InvalidInput::kInvalidName;
		input_error_.type = InvalidInput::kWrongFirstNameChar;
		return false;
	}

	for (char c : name)
	{
		if (!((c >= '0' && c <= '9') || c == '_' ||
			(name[0] >= 'A' && name[0] <= 'Z') || (name[0] >= 'a' && name[0] <= 'z')))
		{
			input_error_.area = InvalidInput::kInvalidName;
			input_error_.type = InvalidInput::kWrongCharInsideName;
			return false;
		}
	}
	return true;
}

bool Client::CheckCorrectPassword(const std::string& pswd, const std::string& r_pswd)
{
	if (pswd != r_pswd)
	{
		input_error_.area = InvalidInput::kInvalidPassword;
		input_error_.type = InvalidInput::kRePasswordIsNotEqual;
		return false;
	}
	if (pswd.size() == 0)
	{
		input_error_.area = InvalidInput::kInvalidPassword;
		input_error_.type = InvalidInput::kEmptyPassword;
		return false;
	}

	if (pswd.size() > 20)
	{
		input_error_.area = InvalidInput::kInvalidPassword;
		input_error_.type = InvalidInput::kLongPassword;
		return false;
	}
	for (char c : pswd)
	{
		if (!(c >= '!' && c <= '~'))
		{
			input_error_.area = InvalidInput::kInvalidPassword;
			input_error_.type = InvalidInput::kWrongCharInsidePassword;
			return false;
		}
	}
	return true;
}

bool Client::CheckCorrectPassword(const std::string& pswd)
{
	if (pswd.size() == 0)
	{
		input_error_.area = InvalidInput::kInvalidPassword;
		input_error_.type = InvalidInput::kEmptyPassword;
		return false;
	}

	if (pswd.size() > 20)
	{
		input_error_.area = InvalidInput::kInvalidPassword;
		input_error_.type = InvalidInput::kLongPassword;
		return false;
	}
	for (char c : pswd)
	{
		if (!(c >= '!' && c <= '~'))
		{
			input_error_.area = InvalidInput::kInvalidPassword;
			input_error_.type = InvalidInput::kWrongCharInsidePassword;
			return false;
		}
	}
	return true;
}


int Client::FindByName(const std::string name)
{
	for (int i = 0; i < penpals_.size(); ++i)
	{
		if (penpals_[i]->getName() == name)
			return i;
	}
	return -1;
}

void Client::SendToMessage(const std::string msg)
{
	MyMessage send_msg = MyMessage(name_, penpals_[selected_penpal_]->getName(), msg);
	sf::Packet send_pck;
	send_pck << send_msg;
	sf::Socket::Status send_status = socket_.send(send_pck);

	if (send_status == sf::Socket::Done)
	{
		penpals_[selected_penpal_]->AddMessage(send_msg.cd);
	}
	else
	{
		LOG("ERROR: Send message to " << penpals_[selected_penpal_]->getName() << " \n");
	}
}






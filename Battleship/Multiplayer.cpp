#include "Multiplayer.h"
#include <iostream>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/TcpListener.hpp>

Multiplayer::Multiplayer() : port_(55001), hosting_finished_(false), connecting_finished_(false), host_thread_(nullptr), connecting_thread_(nullptr), listening_thread_(nullptr), connected_(false),
	new_data_(false), host_(false), player_ready_(false), enemy_ready_(false), connected_init_(false), downloaded_map_(false), game_over_(false), victory_(false), new_attack_(false),
	stopped_host_(false), connecting_(false)
{
	for (int i = 0; i < 5; i++ )
	{
		this->downloaded_ships_.ship_types.push_back("");
		this->downloaded_ships_.ship_placements.push_back(sf::Vector2u());
		this->downloaded_ships_.ship_rotations.push_back(0);
	}
	
}
void Multiplayer::connect(sf::IpAddress ip)
{
	std::cout << "Attempting to connect at " << ip.toString() << std::endl;
	this->connecting_ = true;
	this->connecting_thread_ = new std::thread(&Multiplayer::thread_connect, this, ip);
}

void Multiplayer::thread_connect(sf::IpAddress ip)
{	
	if (this->connect_socket_.Disconnected)
	{
		sf::Socket::Status status = this->connect_socket_.connect(ip, this->port_, sf::milliseconds(5000));
		if (status == sf::Socket::Done || !this->connecting_)
		{
			// Send validation packet
			sf::Packet packet;
			packet << true;
			this->connect_socket_.send(packet);

			std::cout << "You've connected to " << this->connect_socket_.getRemoteAddress().toString() << std::endl;
			this->connected_init_ = true;
			this->connected_ = true;
			this->listening_thread_ = new std::thread(&Multiplayer::thread_look, this);
		}
		else
			std::cout << "You've failed to connect to " << ip.toString() << std::endl;
			
	}
	else
		std::cout << "Error: Socket is still occupied!\n";
	this->connecting_finished_ = true;
}

void Multiplayer::thread_look()
{
	while (this->connected())
	{
		sf::Packet packet;
		if (this->connect_socket_.receive(packet) != sf::Socket::Done || !this->connected())
			std::cout << "Error: Message receive failure\n";
		else
		{
			this->new_chat_message_ = new ChatMessage{ "Enemy", "", sf::Color() };
			packet >> this->new_chat_message_->message;
			this->new_chat_message_->color = sf::Color(255, 0, 0);
			// Data type codes - $M - Message, $R - Ready, $S - New stage, $T - Attack Info & Turn, $P - New enemy map update
			if (this->new_chat_message_->message.find("$M") != std::string::npos)
			{
				this->new_chat_message_->message.erase(this->new_chat_message_->message.begin(), this->new_chat_message_->message.begin() + 2);
				this->new_data_ = true;
			}
			else if (this->new_chat_message_->message.find("$R") != std::string::npos)
			{
				this->toggle_ready(false);
			}
			else if (this->new_chat_message_->message.find("$S") != std::string::npos)
			{
				this->next_stage_ = true;
			}
			else if (this->new_chat_message_->message.find("$P") != std::string::npos)
			{
				for (int i = 0; i < 5; i ++)
				{
					packet >> this->downloaded_ships_.ship_types[i] >> this->downloaded_ships_.ship_placements[i].x >> this->downloaded_ships_.ship_placements[i].y >> this->downloaded_ships_.ship_rotations[i];
				}

				this->downloaded_map_ = true;
			}
			else if (this->new_chat_message_->message.find("$F") != std::string::npos)
			{
				this->turn_ = true;
			}
			else if (this->new_chat_message_->message.find("$T") != std::string::npos)
			{
				packet >> this->enemy_hit_.hit.x >> this->enemy_hit_.hit.y >> this->enemy_hit_.hit_ship;
				this->turn_ = true;
				this->new_attack_ = true;
			}
			else if (this->new_chat_message_->message.find("$G") != std::string::npos)
			{
				if (this->new_chat_message_->message.find('W', 2) != std::string::npos)
					this->victory_ = true;
				else
					this->victory_ = false;
				this->game_over_ = true;
			}
			else if (this->new_chat_message_->message.find("$Q") != std::string::npos)
			{
				this->enemy_surrender_ = true;
				this->game_over_ = true;
			}

			std::cout << "Received " << packet.getDataSize() << " bytes\t" << "Message: " << this->new_chat_message_->message << std::endl;
		}
			
	}
}

void Multiplayer::host()
{
	this->stopped_host_ = false;
	this->hosting_finished_ = false;
	sf::IpAddress address(sf::IpAddress::getLocalAddress());
	std::cout << "Checking for connection on " << address.toString() << std::endl;
	if (this->host_thread_ == nullptr)
		this->host_thread_ = new std::thread(&Multiplayer::thread_host, this);
}

void Multiplayer::thread_host()
{
	this->listener_socket_.listen(this->port_);
	bool client = false;
	// Endless loop that waits for new connections
	do
	{
		if (this->listener_socket_.accept(this->connect_socket_) == sf::Socket::Done)
		{
			// Check to see if the connection is from a valid client
			sf::Packet packet;
			if (this->connect_socket_.receive(packet) == sf::Socket::Done)
				packet >> client;
		}
	}
	while (!client && !this->stopped_host_);

	if (this->stopped_host_)
		return;
	std::cout << "New connection received from " << this->connect_socket_.getRemoteAddress() << std::endl;
	this->connected_init_ = true;
	this->connected_ = true;
	this->host_ = true;
	this->listening_thread_ = new std::thread(&Multiplayer::thread_look, this);
	this->hosting_finished_ = true;
}

bool Multiplayer::hosting() // not actually hosting, more like a check to see if searching for someone
{
	if (this->host_thread_ == nullptr)
		return false;
	return true;
}

void Multiplayer::update()
{
	if (this->hosting_finished_)
		this->stop_hosting();

	if (this->connecting_finished_)
	{
		this->connecting_thread_->join();
		delete this->connecting_thread_;
		this->connecting_thread_ = nullptr;
		this->connecting_finished_ = false;
	}
}

sf::TcpListener& Multiplayer::get_listener()
{
	return this->listener_socket_;
}

std::thread* Multiplayer::get_connecting()
{
	return this->connecting_thread_;
}

bool Multiplayer::connected()
{
	return this->connected_;
}

sf::TcpSocket& Multiplayer::get_connect_socket_()
{
	return this->connect_socket_;
}

void Multiplayer::send_data(std::string message)
{
	sf::Packet packet;
	packet << message;
	this->connect_socket_.send(packet);
}

void Multiplayer::send_data(sf::Packet& packet)
{
	this->connect_socket_.send(packet);
}

ChatMessage& Multiplayer::get_new_message()
{
	return *this->new_chat_message_;
}

bool Multiplayer::new_data()
{
	return this->new_data_;
}

void Multiplayer::unflag_new_data()
{
	this->new_data_ = false;
}

void Multiplayer::stop_hosting()
{
	if (this->hosting())
	{
		this->stopped_host_ = true;
		this->listener_socket_.close();
		this->host_thread_->join();
		delete this->host_thread_;
		this->host_thread_ = nullptr;
	}
}

bool Multiplayer::get_host()
{
	return this->host_;
}

bool Multiplayer::ready(bool player)
{
	if (player)
		return this->player_ready_;
	return this->enemy_ready_;
}

void Multiplayer::toggle_ready(bool player)
{
	if (player)
	{
		if (this->player_ready_ == true)
			this->player_ready_ = false;
		else
			this->player_ready_ = true;
	}
	else
	{
		if (this->enemy_ready_ == true)
			this->enemy_ready_ = false;
		else
			this->enemy_ready_ = true;
	}
}
bool Multiplayer::connected_init()
{
	return this->connected_init_;
}

void Multiplayer::disable_connected_init()
{
	this->connected_init_ = false;
}

bool Multiplayer::next_stage()
{
	return this->next_stage_;
}

bool Multiplayer::downloaded_map()
{
	bool temp = this->downloaded_map_;
	this->downloaded_map_ = false;
	return temp;
}

bool Multiplayer::get_turn()
{
	bool temp = this->turn_;
	if (this->turn_)
		this->turn_ = false;

	return temp;
}

bool Multiplayer::get_new_attack()
{
	bool temp = this->new_attack_;
	if (this->new_attack_)
		this->new_attack_ = false;
	else
		this->new_attack_ = true;

	return temp;
}

DownloadedShips& Multiplayer::download_ships()
{
	return this->downloaded_ships_;
}

bool& Multiplayer::game_over()
{
	return this->game_over_;
}

Hit& Multiplayer::get_hit()
{
	return this->enemy_hit_;
}

bool& Multiplayer::enemy_surrender()
{
	return this->enemy_surrender_;
}

void Multiplayer::set_connected(bool b)
{
	this->connected_ = b;
}




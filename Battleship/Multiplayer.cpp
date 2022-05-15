#include "Multiplayer.h"
#include <iostream>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/TcpListener.hpp>

Multiplayer::Multiplayer() : port_(55001), hosting_finished_(false), connecting_finished_(false), host_thread_(nullptr), connecting_thread_(nullptr), listening_thread_(nullptr), connected_(false),
	new_data_(false) {}

void Multiplayer::connect(sf::IpAddress ip)
{
	std::cout << "Attempting to connect at " << ip.toString() << std::endl;
	this->connecting_thread_ = new std::thread(&Multiplayer::thread_connect, this, ip);
}

void Multiplayer::thread_connect(sf::IpAddress ip)
{	
	if (this->connect_socket_.Disconnected)
	{
		sf::Socket::Status status = this->connect_socket_.connect(ip, this->port_, sf::milliseconds(5000));
		if (status == sf::Socket::Done)
		{
			std::cout << "You've connected to " << this->connect_socket_.getRemoteAddress().toString() << std::endl;
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
	while (true)
	{
		sf::Packet packet;
		if (this->connect_socket_.receive(packet) != sf::Socket::Done)
			std::cout << "Error: Message receive failure\n";
		else
		{
			this->new_chat_message_ = new ChatMessage{ "Enemy", "", sf::Color() };
			packet >> this->new_chat_message_->message;
			this->new_chat_message_->color = sf::Color(255, 0, 0);

			std::cout << "Received " << packet.getDataSize() << " bytes\t" << "Message: " << this->new_chat_message_->message << std::endl;
			this->new_data_ = true;
		}
			
	}
}

void Multiplayer::host()
{
	sf::IpAddress address(sf::IpAddress::getLocalAddress());
	std::cout << "Checking for connection on " << address.toString() << std::endl;
	if (this->host_thread_ == nullptr)
		this->host_thread_ = new std::thread(&Multiplayer::thread_host, this);
}

void Multiplayer::thread_host()
{
	this->listener_socket_.listen(this->port_);

	// Endless loop that waits for new connections
	if (this->listener_socket_.accept(this->connect_socket_) == sf::Socket::Done)
	{
		std::cout << "New connection received from " << this->connect_socket_.getRemoteAddress() << std::endl;
		this->connected_ = true;
		this->listening_thread_ = new std::thread(&Multiplayer::thread_look, this);
	}
	else
		std::cout << "Connection failed.\n";
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

void Multiplayer::send_data(std::string& message)
{
	sf::Packet packet;
	packet << message;
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
		this->listener_socket_.close();
		this->host_thread_->join();
		delete this->host_thread_;
		this->host_thread_ = nullptr;
		this->hosting_finished_ = false;
	}
}


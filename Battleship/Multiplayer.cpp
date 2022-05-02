#include "Multiplayer.h"
#include <iostream>
#include <SFML/Network/TcpListener.hpp>

Multiplayer::Multiplayer() : port_(55001), thread_finished_(false) { }

void Multiplayer::connect(sf::IpAddress ip)
{	
	if (this->connect_socket_.Disconnected)
	{
		sf::Socket::Status status = this->connect_socket_.connect(ip, this->port_);
		if (status != sf::Socket::Done)
			std::cout << "You've failed to connect to " << ip.toString() << std::endl;
	}
	else
		std::cout << "Error: Socket is still occupied!\n";
}

void Multiplayer::host()
{
	sf::IpAddress address(sf::IpAddress::getLocalAddress());
	std::cout << "Checking for connection on " << address.toString() << std::endl;
	if (this->host_thread_ == nullptr)
		host_thread_ = new std::thread(&Multiplayer::thread_host, this);
}

void Multiplayer::thread_host()
{
	// setup listener & listen at 55001
	sf::TcpListener listener;
	listener.listen(this->port_);

	// Endless loop that waits for new connections
	if (listener.accept(this->connect_socket_) == sf::Socket::Done)
	{
		std::cout << "New connection received from " << this->connect_socket_.getRemoteAddress() << std::endl;
	}
	else
		std::cout << "Connection failed.\n";
	this->thread_finished_ = true;
}

bool Multiplayer::hosting() // not actually hosting, more like a check to see if searching for someone
{
	if (this->host_thread_ == nullptr)
		return false;
	return true;
}

void Multiplayer::update()
{
	if (this->thread_finished_)
	{
		delete this->host_thread_;
		this->host_thread_ = nullptr;
		this->thread_finished_ = false;
	}

	if (this->connect_socket_.Disconnected)
		std::cout << "DC'd\n";
}

sf::TcpListener& Multiplayer::get_listener()
{
	return this->listener_socket_;
}

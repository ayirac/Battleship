#ifndef MULTIPLAYER_H
#define MULTIPLAYER_H
#include <thread>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>

class Multiplayer
{
private:
	sf::TcpSocket connect_socket_;
	sf::TcpListener listener_socket_;
	std::thread* host_thread_;
	void thread_host();
	unsigned port_;
	bool thread_finished_;
public:
	Multiplayer();
	void connect(sf::IpAddress ip);
	void host();
	bool hosting();
	void update();
	sf::TcpListener& get_listener();
};
#endif

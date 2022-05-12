#ifndef MULTIPLAYER_H
#define MULTIPLAYER_H
#include <thread>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>

struct ChatMessage
{
	std::string name;
	std::string message;
	sf::Color color;
};

class Multiplayer
{
private:
	sf::TcpSocket connect_socket_; // user socket for connecting to a host
	sf::TcpListener listener_socket_; // host socket listens for connections
	std::thread* host_thread_; // looks for a connection against a user
	std::thread* connecting_thread_; // looks for a connection against a host
	std::thread* listening_thread_; // looks for incoming data
	void thread_host();
	unsigned port_;
	bool hosting_finished_;	// bools handle setting the threads to null & deleting them in update sequence
	bool connecting_finished_;
	bool connected_;
	bool new_data_;
	ChatMessage* new_chat_message_;
public:
	Multiplayer();
	void thread_look();
	void thread_connect(sf::IpAddress ip);
	void host();
	bool hosting();
	void update();
	sf::TcpListener& get_listener();
	std::thread* get_connecting();
	void connect(sf::IpAddress ip);
	bool connected();
	sf::TcpSocket& get_connect_socket_();
	void send_data(std::string& message);
	ChatMessage& get_new_message();
	bool new_data();
	void unflag_new_data();
};
#endif

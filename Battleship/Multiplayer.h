#ifndef MULTIPLAYER_H
#define MULTIPLAYER_H
#include <thread>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/System/Vector2.hpp>

#include "Table.h"

struct ChatMessage
{
	std::string name;
	std::string message;
	sf::Color color;
};

struct DownloadedShips
{
	std::vector<std::string> ship_types;
	std::vector<sf::Vector2u> ship_placements;
	std::vector<unsigned> ship_rotations;
};

class Multiplayer
{
private:
	sf::TcpSocket connect_socket_; // user socket for connecting to a host
	sf::TcpListener listener_socket_; // host socket listens for connections
	std::thread* host_thread_; // looks for a connection against a user
	std::thread* connecting_thread_; // looks for a connection against a host
	std::thread* listening_thread_; // looks for incoming data
	DownloadedShips downloaded_ships_;
	bool new_attack_;
	bool stopped_host_;
	bool enemy_surrender_;
	void thread_host();
	unsigned port_;
	bool hosting_finished_;	// bools handle setting the threads to null & deleting them in update sequence
	bool connecting_finished_;
	bool connected_;
	bool new_data_;
	bool host_;
	bool player_ready_;
	bool enemy_ready_;
	bool connected_init_;
	bool next_stage_;
	bool downloaded_map_;
	bool turn_;
	bool game_over_;
	bool victory_;
	ChatMessage* new_chat_message_;
	Hit enemy_hit_;
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
	void send_data(std::string message);
	void send_data(sf::Packet& packet);
	ChatMessage& get_new_message();
	bool new_data();
	void unflag_new_data();
	void stop_hosting();
	bool get_host();
	bool ready(bool player);
	void toggle_ready(bool player);
	bool connected_init();
	void disable_connected_init();
	bool next_stage();
	bool downloaded_map();
	bool get_turn();
	bool get_new_attack();
	bool& get_enemy_hit();
	DownloadedShips& download_ships();
	bool& game_over();
	Hit& get_hit();
	bool& enemy_surrender();
};
#endif

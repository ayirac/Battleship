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
	bool connecting_;
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
	// Function executed by a thread to connect to an IP address
	void thread_connect(sf::IpAddress ip);
	// Start hosting & the host_thread_
	void host();
	// Function executed by a thread to look for incoming messages/packets
	void thread_look();
	// Returns true if currently in the process of looking for someone to join your hosted game
	bool hosting();
	// Updates various flags
	void update();
	// Returns a reference to the listener socket
	sf::TcpListener& get_listener();
	// Returns a pointer to the connecting thread
	std::thread* get_connecting();
	// Starts a connection on an IP & calls thread_connect on a thread
	void connect(sf::IpAddress ip);
	// Returns true if connected to another client
	bool connected();
	// Returns a reference to the connection socket
	sf::TcpSocket& get_connect_socket_();
	// Sends message data to the other player
	void send_data(std::string message);
	// Sends packet data to the other player
	void send_data(sf::Packet& packet);
	// Returns a reference to the downloaded ChatMessage
	ChatMessage& get_new_message();
	// Returns true if there is new data that has been downloaded
	bool new_data();
	// Sets new_data to false
	void unflag_new_data();
	// Stops the process of hosting/looking for a client to join
	void stop_hosting();
	// Returns true if the player is the host
	bool get_host();
	// Returns true if the player/enemy is readied up, parameter indicates player and enemy.
	bool ready(bool player);
	// Toggles the ready status the given parameter.
	void toggle_ready(bool player);
	// Toggle connection flag 1
	bool connected_init();
	// Toggle connection flag 2
	void disable_connected_init();
	// Indicates that the other player is on the next_stage
	bool next_stage();
	//
	void toggle_next_stage();
	// Indicates that the other player has sent a map for the player to download
	bool downloaded_map();
	void toggle_downloaded_map();
	// Indicates that it is the player's turn
	bool get_turn();
	// Indicates that there is a new attack from the other player
	bool get_new_attack();
	// Indicates that the enemy's hit has been recorded
	bool& get_enemy_hit();
	// Returns a reference to the downloaded ships from the enenmy player
	DownloadedShips& download_ships();
	// Returns true if it is game over
	bool& game_over();
	void toggle_game_over();
	// Returns a reference to the enemy's hit
	Hit& get_hit();
	// Returns true if the enemy has surrendered
	bool& enemy_surrender();
	void toggle_enemy_surrender();
	// Sets the status of connection
	void set_connected(bool b);
};
#endif

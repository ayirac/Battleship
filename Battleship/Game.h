#ifndef GAME_H
#define GAME_H

#include <thread>
#include <SFML/Graphics.hpp>
#include "Map.h"
#include "Button.h"
#include "ButtonTexture.h"
#include "TextureManager.h"
#include "Figurine.h"
#include "ImageBox.h"
#include "ImageTextBox.h"
#include "TableScrollable.h"
#include <SFML/Network.hpp>
#include "InputBox.h"
#include "Multiplayer.h"
#include "Animation.h"
#include "ChatBox.h"
#include "PopupBox.h"


class Game {
private:
	Map player_map_;
	Map enemy_map_;
	sf::RenderWindow* window_;
	std::vector<ImageBox*> image_boxes_;		  // 0 - Background, 1 - Title, 2 - round, 3 - enemy, 4 - player
	std::vector<Button> buttons_;				  // 0 - SP, 1 - MP, 2 - Credits
	std::vector<ButtonTexture> tex_buttons_;	  // 0 - Randomize, 1 - Exit, 2 - Reset, 3 - Start
	std::vector<sf::Font> fonts_;				  // 0 - itc_kabel
	sf::Text victory_text_;
	std::vector<Figurine> figurines_;
	std::vector<Figurine> discarded_figurines_;
	Figurine* figurine_peg_;
	Figurine* held_figurine_;
	Button* held_button_;
	unsigned state_;							  // 0 - main menu, 1 - ship menu, 2 - singleplayer game, 3 - post-game
	TextureManager texturemanager_;
	// ship ai
	bool ai_ship_found_;
	bool ai_reset_first_hit_;
	int ai_orientation_reset_;
	unsigned ai_ship_found_orientation;			// 0 - no ship found, 1 - horizontal, 2 - vertical
	sf::Vector2i ai_last_hit_;
	sf::Vector2i ai_first_hit_;
	std::vector<sf::Vector2i> ai_hits;
	bool side_checked_[4];						// 0 - left, 1 - top, 2 - right, 3 - bottom
	TableScrollable statistics_;
	HMT_stats player_stats_;
	HMT_stats enemy_stats_;
	Table HMT_stats_;
	InputBox* IP_input_box_;
	ChatBox* chatbox_;
	// multiplayer
	Multiplayer multiplayer_;
	// animations
	std::vector<Animation> animations_;
	std::vector<sf::Sprite> animated_sprites_;
	std::vector<PopupBox*> popup_boxes_;
	bool popup_seen_;

public:
	// Setup of objects/textures for the Game in a given RenderWindow
	Game(sf::RenderWindow* window);
	// A screen that allows for menu navigation to different elements of the game. Called when state_ = 0
	void main_menu();
	// Removes any figurines and randomly places the five ships on a board
	void randomize_ships(bool players_ships);
	// A screen that allows the user to setup their ship layout given Buttons and Figurines. Called when state_ = 1
	void ship_menu();
	// A screen that allows the user to play against an enemy computer player. Called when state_ = 2
	void singleplayer_game_start();
	// Screen displays the post-game stats. Called when state_ = 3
	void post_game();
	// A screen that allows the user to host/join a multiplayer game state_ = 4
	void multiplayer_setup();
	// A screen that allows players to select their ships and ready up for a game. state_ = 5
	void multiplayer_ship_menu();
	// A screen that allows the user to play against an enemy player in multiplayer. Called when state_ = 6
	void multiplayer_game_start();
	// Returns the player_map_
	Map& get_player_map();
	// Returns the enemy_map_
	Map& get_enemy_map();
	// Returns the current state_
	const unsigned& get_state() const;
	//
	void set_state(unsigned state);
	// Remove a figurine given a string&
	void remove_figurine(std::string& type);
	// Processes mouse click events to determine the element that is clicked on
	void process_click(const sf::Vector2f& mouse_pos);
	// Executed when a figurine that is held by a mouse is released. Creates a Ship in player_map_
	// if placed released over the map, else figurine returns to original position in Game
	void release_figurine(const sf::Vector2f& mouse_pos);
	// Releases the current figurine
	void release_figurine();
	void reset_game();
	// Executed when a button that is pressed by a mouse is released. Controls a variety of actions
	// based on the button that is pressed as indicated by mouse position.
	void release_button();
	// Rotates a held figurine using the 'A' and 'D' keys which rotates the Figurine by 90 / -90 degrees
	void rotate_held_figurine(const sf::Keyboard::Key& key) const;
	// Returns true if a figurine is being held
	bool holding_figurine() const;
	// Returns true if a button is being pressed
	bool holding_button() const;
	//
	void add_discarded_figurines();
	//
	void ready_attack();
	// Process the hit. Return false if outside the map, else true.
	bool process_hit(sf::Vector2f& mouse_pos, bool player, sf::Vector2i& attack_pos, bool& hit);
	// Process the AI's hit
	void process_ai_hit(sf::Vector2i& attack_pos, bool& hit);
	// RNG Algorithm for AI
	void random_ai_attack(sf::Vector2i& attack_pos, bool& hit);
	// Sink a ship given coordinates of one a successful hit
	void ai_sink_ship(sf::Vector2i& attack_pos, bool& hit);
	// Returns a reference to the statistics table
	TableScrollable& get_statistics();
	// Returns a reference to the HMT table
	Table& get_hmt();
	// Reset the HMT stat objects to a default state
	void reset_hmt_stats();
	// Disables all buttons that don't belong to the current state of the game
	void disable_buttons();
	// Returns the pointer to the inputbox
	InputBox* get_inputbox();
	// Returns the pointer to the chatbox
	ChatBox* get_chatbox();
	//
	Multiplayer& get_multiplayer();
	//
	Animation& get_animation(unsigned id);
	//
	sf::Sprite* get_sprite(unsigned id);
	void set_chatbox_state(unsigned state);
	void process_popup_box(sf::Vector2f& mouse_pos);
	void check_popup_boxes_exit();
};
#endif

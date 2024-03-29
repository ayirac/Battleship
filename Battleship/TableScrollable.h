﻿#ifndef TABLESCROLLABLE_H
#define TABLESCROLLABLE_H
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include "ButtonTexture.h"
#include "ImageBox.h"
#include "Table.h"

class TableScrollable : public Table
{
private:
	std::vector<Hit> player_hits_;
	std::vector<Hit> enemy_hits_;
	unsigned current_round_;
	unsigned current_row_;
	int front_row_;
	int bottom_row_;
	bool player_added_;
	bool enemy_added_;
	std::vector<ButtonTexture> buttons_; // 0 for scroll up, 1 for scroll down
	std::vector<ImageBox> image_boxes_;
	Button* held_button_;
public:
	TableScrollable();
	// Creates a table with three columns for keeping statistics on the game
	TableScrollable(sf::Vector2f pos, sf::Vector2f size, unsigned row_spacing, sf::Font& font, std::vector<sf::Texture*> textures, int rows);
	// draw & take in mouse_pos for checking embeded buttons
	void draw(sf::RenderWindow& win, sf::Vector2f& mouse_pos);
	// add an entry to the Hits table
	void add_entry(sf::Vector2i player_hit, sf::Vector2i enemy_hit, bool& successful_player_attack, bool& successful_enemy_attack);
	// add an entry to the Hits table multi
	void add_entry(sf::Vector2i hit, bool& successful_attack, bool playe);
	// Checks if the mouse position falls within one of the Table's noteworthy bounds
	void process_click(sf::Vector2f& mouse_pos);
	// Checks if the user is holding one of the TableScrollable buttons
	bool holding_button();
	// Releases the held button
	void release_button();
	// Shifts the table rows down
	void shift_down();
	// Shifts the table rows up
	void shift_up();
	// Clears the table entries
	void reset_table();
	// Returns the current round
	std::string get_current_round();
};

#endif

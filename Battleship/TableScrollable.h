#ifndef TABLESCROLLABLE_H
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
	std::vector<ButtonTexture> buttons_; // 0 for scroll up, 1 for scroll down
	std::vector<ImageBox> image_boxes_;
	Button* held_button_;
public:
	TableScrollable();
	void release_button();
	std::string get_current_round();
	// Creates a table with three columns for keeping statistics on the game
	TableScrollable(sf::Vector2f pos, sf::Vector2f size, unsigned row_spacing, sf::Font& font, std::vector<sf::Texture*> textures, int rows);
	// draw & take in mouse_pos for checking embeded buttons
	void draw(sf::RenderWindow& win, sf::Vector2i& mouse_pos);
	// add an entry to the Hits table
	void add_entry(sf::Vector2i player_hit, sf::Vector2i enemy_hit, bool& successful_player_attack, bool& successful_enemy_attack);
	void process_click(sf::Vector2f& mouse_pos);
	bool holding_button();
	void shift_down();
	void shift_up();
};

#endif

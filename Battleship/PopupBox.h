#ifndef POPUPBOX_H
#define POPUPBOX_H
#include <SFML/Graphics/Text.hpp>
#include "Box.h"
#include "ButtonTexture.h"

class PopupBox : public Box
{
private:
	std::vector<sf::Text> texts_;
	sf::Font* font_;
	std::vector<sf::Texture*> textures_;
	ButtonTexture* exit_button_;
	Button* held_button_;
	unsigned rows_;											// 0, 1, 2,  (header no matter what)
	std::vector<sf::IntRect> text_containers_;				// texts_ are constrained within a rect
public:
	// Creates a PopupBox given size, pos, the header_text, font, header_char_size, amount of rows (0-3). The text content of those rows will be given in rows_texts, where the "|" character will indicate a new row
	PopupBox(sf::Vector2f size, sf::Vector2f pos, std::string header_text, sf::Font* font, unsigned rows, std::string rows_text, std::vector<sf::Texture*>& textures);
	void draw(sf::RenderWindow& win) const;
	void set_text(std::string text);
	void update(sf::RenderWindow& win, sf::Vector2f& mouse_pos);
	void exit_key_pressed();
	void release_button();
	bool process_click(sf::Vector2f& mouse_pos);
	bool holding_button();
	Button* get_button();
};

#endif
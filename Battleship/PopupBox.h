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
	// Draws the PopupBox
	void draw(sf::RenderWindow& win) const;
	// Sets the Header of the PopupBox
	void set_text(std::string text);
	// Updates the PopupBox button to check if it's button has been clicked
	void update(sf::RenderWindow& win, sf::Vector2f& mouse_pos);
	// Sets the state to 0, exiting out of the PopupBox
	void exit_key_pressed();
	// Releases the held button.
	void release_button();
	// Checks if the mouse position falls within one of the PopupBox's noteworthy bounds
	bool process_click(sf::Vector2f& mouse_pos);
	// Checks if the user is holding a button
	bool holding_button();
	// Gets a pointer to the exit button
	Button* get_button();
};

#endif
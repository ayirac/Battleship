#ifndef BUTTONTEXTURE_H
#define BUTTONTEXTURE_H
#include "Button.h"

class ButtonTexture : public Button
{
private:
	sf::RectangleShape shape_;
	std::vector<sf::Texture*> textures_;
	sf::Vector2f original_size_;
public:
	ButtonTexture();
	// Create a textured button with text centered at the middle
	ButtonTexture(const sf::Vector2f& pos, const sf::Vector2f& size, const unsigned& char_size, const std::string& text,
		const sf::Font& font, sf::Texture* normal_tex, sf::Texture* press_tex, sf::Color default_color,
		sf::Color hover_color, sf::Color press_color, unsigned state);
	// Create a texture button with no text
	ButtonTexture(const sf::Vector2f& pos, const sf::Vector2f& size, const std::string& text, sf::Texture* normal_tex, sf::Texture* press_tex, unsigned state);
	// Updates the button state
	void update(const sf::Vector2f& mouse_pos, sf::RenderWindow& window);
	// Changes the size of the button to let the user know they're hovering over the button
	void hover(unsigned x_off, unsigned y_off, unsigned char_off);
	// Change the texture/size of the button to let the user know they've pressed the button
	void press();
	// Checks if the button contains the given coordinates
	bool contains(const sf::Vector2f& mouse_pos) const;
	// Draws the button given a window
	void draw(sf::RenderWindow& win) const;
	// Resets the button state to its original state
	void reset();

};
#endif
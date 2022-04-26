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
	ButtonTexture(const sf::Vector2f& pos, const sf::Vector2f& size, const unsigned& char_size, const std::string& text,
		const sf::Font& font, sf::Texture* normal_tex, sf::Texture* press_tex, sf::Color default_color,
		sf::Color hover_color, sf::Color press_color, unsigned state);
	ButtonTexture(const sf::Vector2f& pos, const sf::Vector2f& size, const std::string& text, sf::Texture* normal_tex, sf::Texture* press_tex, unsigned state);
	void update(const sf::Vector2i& mouse_pos, sf::RenderWindow& window);
	void hover(unsigned x_off, unsigned y_off, unsigned char_off);
	void press();
	void release();
	bool contains(const sf::Vector2f& mouse_pos) const;
	void draw(sf::RenderWindow& win) const;
	void reset();

};
#endif
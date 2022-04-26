#ifndef IMAGETEXTBOX_H
#define IMAGETEXTBOX_H
#include <SFML/Graphics.hpp>
#include "ImageBox.h"

class ImageTextBox : public ImageBox
{
private:
	sf::Text text_;
	const sf::Color colors_[2]; // 0 text fill, 1 text ouline
	sf::Vector2f text_offset_;

public:
	ImageTextBox();
	ImageTextBox(sf::Texture* texture, sf::Vector2f size, sf::Vector2f pos, std::string text, const sf::Font& font, unsigned char_size, sf::Color text_color, sf::Color outline_color, sf::Vector2f text_offset);
	void draw(sf::RenderWindow& win) const;
	void set_text(std::string text);
};

#endif

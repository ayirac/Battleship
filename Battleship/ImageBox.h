#ifndef IMAGEBOX_H
#define IMAGEBOX_H
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Box.h"

class ImageBox : public Box
{
protected:
	sf::Texture* texture_;
public:
	ImageBox();
	// Creates an ImageBox given texture, size, and pos
	ImageBox(sf::Texture* texture, sf::Vector2f size, sf::Vector2f pos);
	// Creates a ImageBox given texture and size, no position is set.
	ImageBox(sf::Texture* texture, sf::Vector2f size);
	// Sets the texture of the ImageBox
	void set_texture(sf::Texture* texture);
	// Draws the Imagebox
	void draw(sf::RenderWindow& win) const;
	// Sets the text of the Imagebox
	void set_text(std::string text);
	// Sets the position of the Imagebox
	void set_position(sf::Vector2f pos);
};

#endif

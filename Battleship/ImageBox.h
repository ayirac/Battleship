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
	ImageBox(sf::Texture* texture, sf::Vector2f size, sf::Vector2f pos);
	ImageBox(sf::Texture* texture, sf::Vector2f size);
	void set_texture(sf::Texture* texture);
	virtual void draw(sf::RenderWindow& win) const;
	virtual void set_text(std::string text);
};

#endif

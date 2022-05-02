#ifndef BOX_H
#define BOX_H
#include <string>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Box
{
protected:
	sf::RectangleShape shape_;
	unsigned state_;
public:
	Box(sf::Vector2f size, sf::Vector2f pos);
	virtual void draw(sf::RenderWindow& win) const = 0;
	virtual void set_text(std::string text) = 0;
	sf::RectangleShape& get_shape();
	unsigned& get_state();
	void set_state(unsigned state);
};

#endif

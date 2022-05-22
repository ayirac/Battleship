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
	// Creates a Box given size and position
	Box(sf::Vector2f size, sf::Vector2f pos);
	// Pure virtual draw function used by derived classes for drawing
	virtual void draw(sf::RenderWindow& win) const = 0;
	// Returns a reference to the shape
	sf::RectangleShape& get_shape();
	// Returns the current state of the box
	unsigned& get_state();
	// Sets the state of the box
	void set_state(unsigned state);
};

#endif

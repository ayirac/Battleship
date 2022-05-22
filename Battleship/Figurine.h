#ifndef FIGURINE_H
#define FIGURINE_H
#include <string>
#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "Map.h"


class Figurine
{
private:
	sf::Sprite sprite_;
	sf::Texture* texture_;
	std::vector<sf::Texture*> ship_textures_;
	unsigned state_;
	sf::Vector2f original_scale_;
	sf::Vector2f original_pos_;
	std::string type_;

public:
	// Creates a drag-able Figurine given a Texture*, position, scale, orientation, and type.
	Figurine(sf::Texture* texture, sf::Vector2f pos, sf::Vector2f scale, bool vertical, std::string type, std::vector<sf::Texture*> ship_textures);
	// Creates a drag-able Figurine given a Texture*, scale, and type.
	Figurine(sf::Texture* texture, sf::Vector2f scale, std::string type);
	// Draws a Figurine to a given RenderWindow
	void draw(sf::RenderWindow& window) const;
	// Checks the state of the Figurine when called and calls press(), hover(), and reset() depending on the state
	void update(const sf::Vector2f& mouse_pos);
	// Changes the size of the Figurine's elements
	void hover(double x_off, double y_off);
	// Sets the position of the Figurine to the Mouse cursor's as it is dragged
	void drag(const sf::Vector2f& mouse_pos);
	// Resets the size and texture of the Figurine to their original states when called
	void reset();
	// Attempts to add a Ship to a Map if the Figurine is released over the map, else it will reset
	bool release(Map& map, sf::FloatRect& bounds, unsigned rotation, sf::Vector2u& ship_pos);
	// Returns the type of the Figurine
	std::string& get_type();
	// Rotates the Figurine by an angle
	void rotate(float angle);
	// Returns true if mouse_pos is located inside of the Figurine
	bool contains(sf::Vector2f mouse_pos) const;
	// Sets the state of the figurine to the value of the parameter
	void set_state(unsigned state);
	// Returns a vector of pointers that point to the Figurine's ship textures
	std::vector <sf::Texture*> get_ship_texture();
	// Returns the absolute angle of the Figurine
	float get_abs_angle() const;
	// Resets the position of the Figurine to its original state
	void reset_position();
	// Returns a FloatRect representing the global bounds of the Figurine
	sf::FloatRect get_global_bounds();
};
#endif

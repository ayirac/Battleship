#ifndef CELL_H
#define CELL_H
#include <SFML/Graphics.hpp>
#include "ImageBox.h"

class Cell
{
private:
	unsigned const size_;
	sf::RectangleShape box_;
	sf::Texture* texture_;
	sf::Texture* empty_texture_;
	std::string type_;
	ImageBox* peg_;
	bool visible_;
public:
	// Creates a cell with the parameter's value as its size
	Cell(unsigned size, sf::Texture* empty_tex);
	// Sets the texture of the cell to NULL if "NULL" is entered
	void set_texture(std::string type);
	// Sets the texture of the cell and its type
	void set_texture(sf::Texture* t, std::string type, bool change);
	void remove_texture();
	// Get the pointer to the texture of the cell
	sf::Texture* get_texture();
	// Draws the cell at the first parameter's location in the specified RenderWindow
	void draw(sf::Vector2f& loc, sf::RenderWindow& win);
	// Rotates a cell by some float angle
	void rotate(float& angle);
	// Absolutely sets the rotation to an unsigned angle
	void abs_rotate(unsigned& angle);
	// Returns the type of the cell
	std::string get_type();
	// Returns true if the cell contains the mouse position, else false
	bool contains(const sf::Vector2f& pos);
	// Adds a peg to the cell
	void add_peg(ImageBox* peg); // false for white, true for read
	// Removes a peg from a cell
	void remove_peg();
	//
	bool has_peg();
	//
	sf::RectangleShape& get_shape();
	bool& visible();
};
#endif

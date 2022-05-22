#ifndef SHIP_H
#define SHIP_H
#include <SFML/Graphics.hpp>
#include "Cell.h"

class Ship
{
private:
	unsigned HP_;	// Represents the amount of Cells the Ship takes up
	std::vector<sf::Texture*> textures_;
	std::vector<Cell* > ships_cells_;
	unsigned next_texture_;
	std::string type_;
public:
	// Sets the ships type, textures, and HP_
	Ship(const std::string& type, std::vector<sf::Texture*> textures);
	// Sets the ships type and HP_
	Ship(const std::string& type);
	// Destructor
	~Ship();
	// Returns the next element from the textures_ vector and increments
	sf::Texture* get_next_texture();
	// Returns the HP of the ship
	unsigned& get_hp();
	// Returns the type of the ship
	std::string& get_type();
	// Resets the next_texture_ to 0
	void reset();
	// Sets the ships_cells_ to the first parameter
	void set_ship_cells(std::vector<Cell*>& ship_cells);
	// Gets the ships_cells_
	std::vector<Cell*>& get_ship_cells();
	// Removes 1 from the ships health points
	void remove_hp();
	// Removes all HP from the ship
	void destroy();
};


#endif

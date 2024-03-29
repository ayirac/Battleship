#ifndef MAP_H
#define MAP_H
#include <SFML/Graphics.hpp>
#include "Cell.h"
#include "Ship.h"
#include "GridMark.h"

class Map {
private:
	std::vector<std::vector<Cell>> cells_;	// where [x][y] represents a Map of cells_
	sf::Vector2f next_location_;	// Next cell's location be drawn
	sf::Vector2f left_point_;	// top-left cell location
	std::vector<Ship*> ships_;
	unsigned cell_size_;
	static sf::Texture* cell_empty_;
	std::string name_;
	std::vector<ImageBox*> pegs_;
	GridMark horizontal_marks_;
	GridMark vertical_marks_;
public:
	// Default constructor
	Map();
	// Sets up a Map grid using the first parameter as the size and the second parameter for starting location
	Map(unsigned size, unsigned cell_size, sf::Vector2f next_loc, sf::Texture* empty_tex, std::string name, std::vector<ImageBox*> pegs, sf::Font& font);
	// Draws the cells_ to screen
	void draw(sf::RenderWindow& win);
	// Adds a ship to the map
	void add_ship(unsigned& x, unsigned& y, unsigned& rotation, std::string& type, std::vector<sf::Texture*> ship_textures, bool change);
	// Add an invisible ship to the map
	//void add_ship(unsigned& x, unsigned& y, unsigned& rotation, std::string& type);
	// Checks if given an x, y, rotation, and the HP of a ship if the ship overlaps an existing ship, returns true if found
	bool check_overlap(unsigned& x, unsigned& y, unsigned& rotation, std::string& type);
	// When a figurine is released this function checks to see if the figurine lands inside the grid and doesn't overflow
	bool check_placement(sf::FloatRect& bounds, std::string& type, std::vector<sf::Texture*> ship_texture, unsigned& rotation, sf::Vector2u& ship_pos);
	// Remove a ship from the map given its type
	bool remove_ship(std::string& type);
	// Remove all ships from the map
	void remove_ships();
	// Return a reference to the ships_ vector
	std::vector<Ship*>& get_ships();
	// Checks the Map's ships for a hit given mouse coordinates
	std::string check_hit(sf::Vector2f& mouse_pos, sf::Vector2i& attack_pos);
	// Checks the Map's ships for a hit given cell coordinates
	std::string check_hit(sf::Vector2i& pos);
	// Debug operator overload for seeing placement of ships
	friend std::ostream& operator<<(std::ostream& output, Map& map);
	// Returns the player's name who owns the Map
	std::string& get_name();
	// Returns the size of the cells vector (or the size of Map)
	unsigned get_size();
	// Returns a reference to a vector of the Map cells
	std::vector<std::vector<Cell>>& get_cells();
	// Returns the coordinates to the first open cell of the Map
	sf::Vector2i& get_first_open_cell();
	// Draws the grid marks given a RenderWindow
	void draw_grid_marks(sf::RenderWindow& win);
	// Resets the Map to its original state
	void reset_map();
	// Returns the left point of the Map
	sf::Vector2f& get_left();
	// Returns the size of a cell belonging to a Map
	unsigned get_cell_size();
	// Destroys all ships on the Map
	void destroy_ships();
};


#endif

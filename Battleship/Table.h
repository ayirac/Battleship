#ifndef TABLE_H
#define TABLE_H
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>


struct HMT_stats
{
	int hits = 0;
	int misses = 0;
	int total = 0;
	double hit_rate = 0.0;
};

struct Hit
{
	sf::Vector2i hit;
	bool hit_ship;
};

class Table
{
protected:
	sf::RectangleShape outline_shape_;
	std::vector<sf::RectangleShape> lines_;
	std::vector<sf::Color> colors_;
	std::vector<sf::Text> header_text_;
	std::vector<std::vector<sf::Text>> texts_;
	sf::Font font_;
	unsigned max_rows_;
	unsigned row_spacing_;
	std::vector<sf::Texture*> textures_;
public:
	Table();
	// Creates a table with three columns for keeping statistics on the game
	Table(sf::Vector2f pos, sf::Vector2f size, unsigned row_spacing, sf::Font& font, int rows);
	// Creates a table with three columns for keeping statistics on the game for dervied tables
	Table(sf::Vector2f pos, sf::Vector2f size, unsigned row_spacing, sf::Font& font, std::vector<sf::Texture*> textures, int rows, bool lazy);
	// draw a table with no buttons accounted for
	void draw(sf::RenderWindow& win);
	// add entry to the HMT table
	void add_entry(HMT_stats& player_stats, HMT_stats& enemy_stats);
	// Clears the table of all entries
	void reset_table();
};

#endif


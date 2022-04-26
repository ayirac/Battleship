#ifndef GRIDMARK_H
#define GRIDMARK_H
#include <SFML/Graphics.hpp>

class GridMark {
private:
	std::vector<sf::Text> mark_text_;
public:
	GridMark();
	// Creates GridMarks for a map. False for horizontal, true for vertical. 
	GridMark(bool orientation, sf::Vector2f& pos, unsigned marks, sf::Font& font);
	void draw(sf::RenderWindow& win);

};


#endif

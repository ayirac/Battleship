#include "GridMark.h"

#include <iostream>

GridMark::GridMark()
{
}

GridMark::GridMark(bool orientation, sf::Vector2f& pos, unsigned marks, sf::Font& font)
{
	sf::Text* p_grid;
	if (orientation == false) // horizontal
	{
		int current_int = 1;
		for (int i = 0; i < marks; i++)
		{
			p_grid = new sf::Text(std::to_string(current_int), font, 30);
			p_grid->setFillColor(sf::Color::White);
			p_grid->setOutlineThickness(2);
			p_grid->setOutlineColor(sf::Color::Black);
			p_grid->setPosition(pos.x - 10 + i*50, pos.y - 65);
			this->mark_text_.push_back(*p_grid);
			current_int++;
		}
	}
	else if (orientation == true) // vertical
	{
		char current_char = 65;
		for (int i = 0; i < marks; i++)
		{
			p_grid = new sf::Text(current_char, font, 30);
			p_grid->setFillColor(sf::Color::White);
			p_grid->setOutlineThickness(2);
			p_grid->setOutlineColor(sf::Color::Black);
			p_grid->setPosition(pos.x - 65, pos.y - 20 + i*50);
			this->mark_text_.push_back(*p_grid);
			current_char++;
		}
	}
}

void GridMark::draw(sf::RenderWindow& win)
{
	for (int i = 0; i < this->mark_text_.size(); i++)
	{
		win.draw(this->mark_text_[i]);
	}
}


/*
 *	  if horizontal, make 10 grid marker texts seperated by cell size on the x axis
 *		if vertical, make 10 grid marks seperated by cell size on the y axis
 *
 */

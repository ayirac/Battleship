#include "GridMark.h"

#include <iostream>

GridMark::GridMark()
{
}

GridMark::GridMark(bool orientation, sf::Vector2f& pos, unsigned marks, sf::Font& font, unsigned cell_size)
{
	sf::Text* p_grid;
	if (orientation == false) // horizontal
	{
		int current_int = 1;
		for (unsigned i = 0; i < marks; i++)
		{
			p_grid = new sf::Text(std::to_string(current_int), font, cell_size * 0.6);
			p_grid->setFillColor(sf::Color::White);
			p_grid->setOutlineThickness(2);
			p_grid->setOutlineColor(sf::Color::Black);
			p_grid->setPosition(pos.x - cell_size / 5 + i * cell_size, pos.y - cell_size * 1.3);
			this->mark_text_.push_back(*p_grid);
			current_int++;
		}
	}
	else if (orientation == true) // vertical
	{
		char current_char = 65;
		for (unsigned i = 0; i < marks; i++)
		{
			p_grid = new sf::Text(current_char, font, cell_size * 0.6);
			p_grid->setFillColor(sf::Color::White);
			p_grid->setOutlineThickness(2);
			p_grid->setOutlineColor(sf::Color::Black);
			p_grid->setPosition(pos.x - cell_size * 1.3, pos.y - cell_size * 0.4  + i * cell_size);
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
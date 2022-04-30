#include "Ship.h"
#include <iostream>

Ship::Ship(const std::string& type, std::vector<sf::Texture*> textures) : next_texture_(0), type_(type), HP_(0)
{
	const std::string ship_types[5] = { "Carrier", "Battleship", "Cruiser", "Submarine", "Destroyer" };
	std::vector<sf::Texture*>::iterator itr;
	for (itr = textures.begin(); itr < textures.end(); itr++)
		this->textures_.push_back(*itr);

	if (type == ship_types[0])
		this->HP_ = 5;
	else if (type == ship_types[1])
		this->HP_ = 4;
	else if (type == ship_types[2])
		this->HP_ = 3;
	else if (type == ship_types[3])
		this->HP_ = 3;
	else if (type == ship_types[4])
		this->HP_ = 2;

}

Ship::Ship(const std::string& type) : type_(type), next_texture_(0), HP_(0)
{
	std::cout << "Ship created\n";
	const std::string ship_types[5] = { "Carrier", "Battleship", "Cruiser", "Submarine", "Destroyer" };

	if (type == ship_types[0])
		this->HP_ = 5;
	else if (type == ship_types[1])
		this->HP_ = 4;
	else if (type == ship_types[2])
		this->HP_ = 3;
	else if (type == ship_types[3])
		this->HP_ = 3;
	else if (type == ship_types[4])
		this->HP_ = 2;
}

Ship::~Ship()
{
	/*
	std::cout << "Destroying the " << this->get_type() << " & setting all cells to visible\n";
 	for (int i = 0; i < this->get_ship_cells().size(); i++)
	{
 		this->get_ship_cells()[i]->set_texture(this->get_ship_cells()[i]->get_texture(), this->get_type(), true);
	}
	*/

}

sf::Texture* Ship::get_next_texture()
{
	sf::Texture* tex = this->textures_.at(this->next_texture_);
	this->next_texture_++;
	return tex;
}

unsigned& Ship::get_hp()
{
	return this->HP_;
}

std::string& Ship::get_type()
{
	return this->type_;
}

void Ship::reset()
{
	this->next_texture_ = 0;
}

void Ship::set_ship_cells(std::vector<Cell*>& ship_cells)
{
	this->ships_cells_ = ship_cells;
}

std::vector<Cell*>& Ship::get_ship_cells()
{
	return this->ships_cells_;
}

void Ship::remove_hp()
{
	this->HP_--;
}

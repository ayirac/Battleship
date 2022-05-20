#include <iostream>
#include "Map.h"

#include <iomanip>

sf::Texture* Map::cell_empty_ = nullptr;

Map::Map() : cell_size_(10)
{
	Cell cell(this->cell_size_, nullptr);
	std::vector<Cell> cells;
	for (unsigned i = 0; i < 10; i++)
	{
		for (unsigned j = 0; j < 10; j++)
		{
			cells.push_back(cell);
		}
		this->cells_.push_back(cells);
	}

	sf::Vector2f cvt_nextloc(0, 0);
	this->next_location_ = cvt_nextloc;
	this->left_point_ = cvt_nextloc;
}

Map::Map(unsigned size, unsigned cell_size, sf::Vector2f next_loc, sf::Texture* empty_tex, std::string name,
         std::vector<ImageBox*> pegs, sf::Font& font) : cell_size_(cell_size), name_(name), pegs_(pegs)
{
	std::vector<Cell> cells;
	Map::cell_empty_ = empty_tex;
	Cell cell(this->cell_size_, Map::cell_empty_);
	for (unsigned i = 0; i < size; i++)
	{
		for (unsigned j = 0; j < size; j++)
		{
			cells.push_back(cell);
		}
		this->cells_.push_back(cells);
	}

	sf::Vector2f cvt_nextloc((next_loc.x - (size * cell_size_ - cell_size_)) / 2, (next_loc.y - (size * cell_size_ - cell_size_)) / 2);
	this->horizontal_marks_ = GridMark(false, cvt_nextloc, size, font), this->vertical_marks_ = GridMark(true, cvt_nextloc, size, font);
	this->next_location_ = cvt_nextloc;
	this->left_point_ = cvt_nextloc;
}

void Map::draw(sf::RenderWindow& win)
{
	// Iterates through the y axis
	for (int i = 0; i < this->cells_.size(); i++)
	{
		// Iterates through the x axis
		for (int j = 0; j < this->cells_.size(); j++)
		{
			this->cells_[j][i].draw(this->next_location_, win);
			this->next_location_.x += this->cell_size_;
		}
		this->next_location_.x = left_point_.x;
		this->next_location_.y += this->cell_size_;
	}
	this->next_location_.y = left_point_.y;
}

void Map::add_ship(unsigned& x, unsigned& y, unsigned& rotation, std::string& type, std::vector<sf::Texture*> ship_textures, bool change)
{
	std::vector<Cell*> temp_ship_cells;
	Cell* p_ship_cell;
	Ship* p_ship = new Ship(type, ship_textures);

	// Set textures for each cell of the ship
	if (rotation == 0)
	{
		for (unsigned i = 0; i < ship_textures.size(); i++)
		{
			unsigned new_y = y + i;
			this->cells_[x][new_y].set_texture(p_ship->get_next_texture(), p_ship->get_type(), change);
			this->cells_[x][new_y].abs_rotate(rotation);
			p_ship_cell = &this->cells_[x][new_y];
			temp_ship_cells.push_back(p_ship_cell);
		}
	}
	else if (rotation == 90)
	{
		for (unsigned i = 0; i < ship_textures.size(); i++)
		{
			unsigned new_x = (x + ship_textures.size()) - i - 1;
			this->cells_[new_x][y].set_texture(p_ship->get_next_texture(), p_ship->get_type(), change);
			this->cells_[new_x][y].abs_rotate(rotation);
			p_ship_cell = &this->cells_[new_x][y];
			temp_ship_cells.push_back(p_ship_cell);
		}
	}
	else if (rotation == 180)
	{
		for (unsigned i = 0; i < ship_textures.size(); i++)
		{
			unsigned new_y = (y + ship_textures.size()) - i - 1;

			this->cells_[x][new_y].set_texture(p_ship->get_next_texture(), p_ship->get_type(), change);
			this->cells_[x][new_y].abs_rotate(rotation);
			p_ship_cell = &this->cells_[x][new_y];
			temp_ship_cells.push_back(p_ship_cell);
		}
	}
	else if (rotation == 270)
	{
		for (unsigned i = 0; i < ship_textures.size(); i++)
		{
			unsigned new_x = x + i;
			this->cells_[new_x][y].set_texture(p_ship->get_next_texture(), p_ship->get_type(), change);
			this->cells_[new_x][y].abs_rotate(rotation);
			p_ship_cell = &this->cells_[new_x][y];
			temp_ship_cells.push_back(p_ship_cell);
		}
	}
	p_ship->set_ship_cells(temp_ship_cells);
	this->ships_.push_back(p_ship);
}

bool Map::check_overlap(unsigned& x, unsigned& y, unsigned& rotation, std::string& type)
{
	unsigned hp = 0;
	if (type == "Carrier")
		hp = 5;
	else if (type == "Battleship")
		hp = 4;
	if (type == "Submarine" || type == "Cruiser")
		hp = 3;
	if (type == "Destroyer")
		hp = 2;

	bool ship_found = false;

	if (rotation == 270 || rotation == 90) // horizontal
	{
		for (unsigned i = 0; i < hp; i++)
		{
			if (this->cells_[i + x][y].get_type() != "Empty")
				ship_found = true;
		}
	}
	else if (rotation == 0 || rotation == 180) // vertical
	{
		for (unsigned i = 0; i < hp; i++)
		{
			if (this->cells_[x][i + y].get_type() != "Empty")
				ship_found = true;
		}
	}
	return ship_found;
}

bool Map::check_placement(sf::FloatRect& bounds, std::string& type, std::vector<sf::Texture*> ship_textures, unsigned& rotation, sf::Vector2u& ship_pos)
{
	float x_offset;
	if (type == "Carrier")
		x_offset = 9;
	else if (type == "Battleship")
		x_offset = 8;
	else if (type == "Submarine" || type == "Cruiser")
		x_offset = 6;
	else
		x_offset = 4;

	sf::Vector2f center_first_cell;
	if (rotation == 90 || rotation == 270)
		center_first_cell = sf::Vector2f(bounds.left + bounds.width / x_offset, bounds.top + bounds.height / 2);
	else if (rotation == 0 || rotation == 180)
		center_first_cell = sf::Vector2f(bounds.left + bounds.width / 2, bounds.top + bounds.height / x_offset); //775, 763

	for (unsigned i = 0; i < this->cells_.size(); i++)
	{
		for (unsigned j = 0; j < this->cells_.size(); j++)
		{
			if (this->cells_[i][j].contains(center_first_cell))
			{

				if (rotation == 0 || rotation == 180) // vertical  cont here
				{
					//if (j != 1)
						//j = j - static_cast<unsigned>(ship_textures.size()) / 2;
					if (j + ship_textures.size() > 10 || j + ship_textures.size() < ship_textures.size())
					{
						std::cout << "OVERFLOW!\n";
						return false;
					}
				}
				else if (rotation == 90 || rotation == 270) // horizontal
				{
					//if (i != 1)
						//i = i - static_cast<unsigned>(ship_textures.size()) / 2;
					if (i + ship_textures.size() > 10 || i + ship_textures.size() < ship_textures.size())
					{
						std::cout << "OVERFLOW!\n";
						return false;
					}
				}

				if (!this->check_overlap(i, j, rotation, type))
				{
					this->add_ship(i, j, rotation, type, ship_textures, true);
					ship_pos.x = i, ship_pos.y = j;
				}
				else
					return false;
				return true;
			}
		}
	}
	return false;
}

bool Map::remove_ship(std::string& type)
{
	const std::string ship_list[5] = { "Carrier", "Battleship", "Cruiser", "Submarine", "Destroyer"};
	int angle = 180;
	for (int i = 0; i < this->ships_.size(); i++)
	{
		if (this->ships_[i]->get_type() == type)
		{
			for (int j = 0; j < this->ships_[i]->get_ship_cells().size(); j++)
			{
				this->ships_[i]->get_ship_cells()[j]->set_texture(Map::cell_empty_, "Empty", true);
				//this->cell_ships_[i][j]->set_texture(NULL, "Empty");
			}
			return true;
		}
	}
	return false;
	
}

void Map::remove_ships()
{
	std::vector<Ship*>::iterator itr;
	for (itr = this->ships_.begin(); itr != this->ships_.end(); itr++)
	{
		for (int j = 0; j < (*itr)->get_ship_cells().size(); j++)
		{

			(*itr)->get_ship_cells()[j]->set_texture(Map::cell_empty_, "Empty", true);
		}
	}
	size_t const size = this->ships_.size();
	for (size_t i = 0; i < size; i++)
	{
		delete this->ships_[size-i-1];
		this->ships_.pop_back();
	}
}

std::vector<Ship*>& Map::get_ships()
{
	return this->ships_;
}

std::string Map::check_hit(sf::Vector2f& mouse_pos, sf::Vector2i& attack_pos)
{
	std::string ship_types[5] = { "Carrier", "Battleship", "Cruiser", "Submarine", "Destroyer" };
	bool inside_map = false;
	unsigned pos_x, pos_y;

	// Iterate through the 2D vector to check if a Cell contains the mouse_pos
	for (unsigned i = 0; i < this->cells_.size() && inside_map == false; i++)
	{
		for (unsigned j = 0; j < this->cells_.size() && inside_map == false; j++)
		{
			// Checks if any of the ships is located in the Cell
			if (this->cells_[i][j].contains(mouse_pos))
			{
				inside_map = true;
				pos_x = i, pos_y = j;
				attack_pos.x = i, attack_pos.y = j;
				// If there isn't already a peg on the Cell then add one
				if (!this->cells_[i][j].has_peg())
				{
					for (unsigned e = 0; e < 5; e++)
					{
						if (this->cells_[i][j].get_type() == ship_types[e])
						{
							this->cells_[i][j].add_peg(this->pegs_[1]);
							// Iterates through the ships_ to locate the ship to remove HP from
							for (unsigned q = 0; q < this->ships_.size(); q++)
							{
								if (this->ships_[q]->get_type() == ship_types[e])
								{
									this->ships_[q]->remove_hp();
								}
							}
							return ship_types[e];
						}
					}
				}

				
			}
		}
	}
	if (inside_map)
	{
		if (!this->cells_[pos_x][pos_y].has_peg())
		{
			this->cells_[pos_x][pos_y].add_peg(this->pegs_[0]);
			return "Empty";
		}
		return "Filled";
	}
	return "Outside";
}

std::string Map::check_hit(sf::Vector2i& pos)
{
	std::string ship_types[5] = { "Carrier", "Battleship", "Cruiser", "Submarine", "Destroyer" };
	if (!this->cells_[pos.x][pos.y].has_peg())
	{
		for (unsigned e = 0; e < 5; e++)
		{
			if (this->cells_[pos.x][pos.y].get_type() == ship_types[e])
			{
				this->cells_[pos.x][pos.y].add_peg(this->pegs_[1]);
				// Iterates through the ships_ to locate the ship to remove HP & add a peg to
				for (unsigned q = 0; q < this->ships_.size(); q++)
				{
					if (this->ships_[q]->get_type() == ship_types[e])
						this->ships_[q]->remove_hp();
				}
				return ship_types[e];
			}
		}
	} else
	{
		return "Filled";
	}
	this->cells_[pos.x][pos.y].add_peg(this->pegs_[0]);
	return "Empty";
}

std::string& Map::get_name()
{
	return this->name_;
}

unsigned Map::get_size()
{
	return this->cells_.size();
}

std::vector<std::vector<Cell>>& Map::get_cells()
{
	return this->cells_;
}

sf::Vector2i& Map::get_first_open_cell()
{
	// Iterates through the y axis
	for (int i = 0; i < this->cells_.size(); i++)
	{
		// Iterates through the x axis
		for (int j = 0; j < this->cells_.size(); j++)
		{
			if (this->cells_[i][j].get_type() != "Filled")
			{
				sf::Vector2i coord = sf::Vector2i(i, j);
				return coord;
			}
		}
	}
}

void Map::draw_grid_marks(sf::RenderWindow& win)
{
	this->horizontal_marks_.draw(win);
	this->vertical_marks_.draw(win);
}

void Map::reset_map()
{
	for (int i = 0; i < this->cells_.size(); i++)
	{
		for (int j = 0; j < this->cells_.size(); j++)
		{
			if (this->cells_[i][j].has_peg())
			{
				this->cells_[i][j].remove_peg();
				this->cells_[i][j].remove_texture();
			}
			else if (this->cells_[i][j].get_type() != "Empty")
				this->cells_[i][j].remove_texture();
		}
	}
}

sf::Vector2f& Map::get_left()
{
	return this->left_point_;
}

unsigned Map::get_cell_size()
{
	return this->cell_size_;
}

void Map::destroy_ships()
{
	for (int i = 0; i < this->ships_.size(); i++)
	{
		this->ships_[i]->destroy();
	}
}


std::ostream& operator<<(std::ostream& output, Map& map)
{
	output << map.get_name() << std::endl << std::left << std::setw(10) << "Ship" << std::setw(10) << "HP" << std::endl;
	for (unsigned i = 0; i < map.ships_.size(); i++)
	{
		output << std::setw(15) << map.ships_[i]->get_type() << std::setw(5) << map.ships_[i]->get_hp() << std::endl;
	}
	return output;
}

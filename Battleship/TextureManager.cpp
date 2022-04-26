#include "TextureManager.h"
#include <iostream>


TextureManager::TextureManager() : ship_textures_(5)
{
	// Load ship textures
	int hp[5] = { 5, 4, 3, 3, 2 };
	std::string ship_types[5] = { "Carrier", "Battleship", "Cruiser", "Submarine", "Destroyer" };
	sf::Texture tex;
	for (int j = 0; j < 5; j++) {
		for (int i = 0; i < hp[j]; i++)
		{
			std::string fp = ".\\Resources\\Textures\\" + ship_types[j] + "\\Ship" + ship_types[j] + "Hull_0" + std::to_string(i + 1) + ".png";
			if (!tex.loadFromFile(fp))
				std::cout << "Failed to load " << fp << std::endl;
			else
				this->ship_textures_[j].push_back(tex);
		}
	}

	// Load main-menu textures
	std::string tex_names[10] = { "background.jpg", "title.png", "sunk_ship.png", "button.png", "button_press.png"};
	for (int i = 0; i < 5; i++)
	{
		if (!tex.loadFromFile(".\\Resources\\Textures\\" + tex_names[i]))
			std::cout << "Failed to load " << tex_names[i] << std::endl;
		else
			this->game_textures_.push_back(tex);
	}

	// Load figurine textures
	for (int i = 0; i < 5; i++)
	{
		if (!tex.loadFromFile(".\\Resources\\Textures\\" + ship_types[i] + "\\Ship" + ship_types[i] + "Hull.png"))
			std::cout << "Failed to load " << ship_types[i] << std::endl;
		else
			this->game_textures_.push_back(tex);
	}

	// Load empty, peg, and misc textures
	std::string misc_textures[5] = { "empty-cell", "peg-white", "peg-red", "sign", "ImageTextBoxBackground1"};
	for (int i = 0; i < 5; i++)
	{
		if (!tex.loadFromFile(".\\Resources\\Textures\\" + misc_textures[i] + ".png"))
			std::cout << "Failed to load " << misc_textures[i] << ".png" << std::endl;
		else
			this->game_textures_.push_back(tex);
	}

	// Load table textures
	std::string table_textures[6] = { "scroll-up", "scroll-up-press", "scroll-down", "scroll-down-press", "scroll-bar", "left-sign"};
	for (int i = 0; i < 6; i++)
	{
		if (!tex.loadFromFile(".\\Resources\\Textures\\Table\\" + table_textures[i] + ".png"))
			std::cout << "Failed to load " << table_textures[i] << ".png" << std::endl;
		else
			this->game_textures_.push_back(tex);
	}
	
}

std::vector<sf::Texture*> TextureManager::get_ship_texture(unsigned id)
{
	std::vector<sf::Texture>::iterator itr;
	std::vector<sf::Texture*> p_textures;
	for (itr = this->ship_textures_[id].begin(); itr < this->ship_textures_[id].end(); itr++)
		p_textures.push_back(&*itr);
	return p_textures;
}

std::vector<sf::Texture*> TextureManager::get_ship_texture(std::string& type)
{
	std::vector<sf::Texture>::iterator itr;
	std::vector<sf::Texture*> p_textures;
	int id;

	if (type == "Carrier")
		id = 0;
	else if (type == "Battleship")
		id = 1;
	else if (type == "Cruiser")
		id = 2;
	else if (type == "Submarine")
		id = 3;
	else if (type == "Destroyer")
		id = 4;

	for (itr = this->ship_textures_[id].begin(); itr < this->ship_textures_[id].end(); itr++)
		p_textures.push_back(&*itr);
	return p_textures;
}

sf::Texture* TextureManager::get_texture(unsigned id) { return &this->game_textures_[id]; }

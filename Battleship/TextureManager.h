#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H
#include <SFML/Graphics.hpp>

class TextureManager
{
private:
	std::vector<std::vector<sf::Texture>> ship_textures_;
	std::vector<sf::Texture> game_textures_;
	std::vector<sf::Texture> sprite_maps_;
public:
	// Loads textures for Ships, Figurines, and various element into their respective vectors
	TextureManager();
	// Returns a vector of pointers to textures given a ships ID
	std::vector<sf::Texture*> get_ship_texture(unsigned id);
	// Returns a vector of pointers to textures given a ships type
	std::vector<sf::Texture*> get_ship_texture(std::string& type);
	// Returns a pointer to a texture given its ID
	sf::Texture* get_texture(unsigned id);
	// Spritemap exclusive
	sf::Texture* get_spritemap(unsigned id);
};
#endif

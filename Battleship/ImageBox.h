#ifndef IMAGEBOX_H
#define IMAGEBOX_H
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class ImageBox
{
protected:
	sf::RectangleShape shape_;
	sf::Texture* texture_{};

public:
	ImageBox();
	ImageBox(sf::Texture* texture, sf::Vector2f size, sf::Vector2f pos);
	ImageBox(sf::Texture* texture, sf::Vector2f size);
	void set_texture(sf::Texture* texture);
	void set_size(sf::Vector2f& size);
	void set_position(sf::Vector2f& pos);
	virtual void draw(sf::RenderWindow& win) const;
	sf::RectangleShape& get_shape();
	virtual void set_text(std::string text);
};

#endif

#include "ImageBox.h"

ImageBox::ImageBox() : Box(sf::Vector2f(0, 0), sf::Vector2f(0, 0)) {}

ImageBox::ImageBox(sf::Texture* texture, sf::Vector2f size, sf::Vector2f pos) : texture_(texture), Box(size, pos)
{
	this->shape_.setTexture(this->texture_);
}

ImageBox::ImageBox(sf::Texture* texture, sf::Vector2f size) : texture_(texture), Box(size, sf::Vector2f(0, 0))
{
	this->shape_.setTexture(this->texture_);
	this->shape_.setSize(size);
}

void ImageBox::set_texture(sf::Texture* texture)
{
	this->texture_ = texture;
	this->shape_.setTexture(this->texture_);
}

void ImageBox::draw(sf::RenderWindow& win) const
{
	win.draw(this->shape_);
}

void ImageBox::set_text(std::string text) {}
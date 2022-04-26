#include "ImageBox.h"

ImageBox::ImageBox() 
= default;

ImageBox::ImageBox(sf::Texture* texture, sf::Vector2f size, sf::Vector2f pos) : texture_(texture)
{
	this->shape_.setTexture(this->texture_);
	this->shape_.setSize(size);
	this->shape_.setPosition(pos);
}

ImageBox::ImageBox(sf::Texture* texture, sf::Vector2f size) : texture_(texture)
{
	this->shape_.setTexture(this->texture_);
	this->shape_.setSize(size);
}

void ImageBox::set_texture(sf::Texture* texture)
{
	this->texture_ = texture;
	this->shape_.setTexture(this->texture_);
}

void ImageBox::set_size(sf::Vector2f& size)
{
	this->shape_.setSize(size);
}

void ImageBox::set_position(sf::Vector2f& pos)
{
	this->shape_.setPosition(pos);
}

void ImageBox::draw(sf::RenderWindow& win) const
{
	win.draw(this->shape_);
}

sf::RectangleShape& const ImageBox::get_shape() { return this->shape_; }

void ImageBox::set_text(std::string text)
{
	
}

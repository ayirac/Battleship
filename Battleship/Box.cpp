#include "Box.h"

Box::Box(sf::Vector2f size, sf::Vector2f pos) : shape_(size)
{
	this->shape_.setPosition(pos);
}

sf::RectangleShape& Box::get_shape()
{
	return this->shape_;
}

unsigned& Box::get_state()
{
	return this->state_;
}

void Box::set_state(unsigned state)
{
	this->state_ = state;
}

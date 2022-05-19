#include "Figurine.h"
#include <iostream>

#include "Game.h"

Figurine::Figurine(sf::Texture* texture, const sf::Vector2f pos, const sf::Vector2f scale, const bool vertical,
                   std::string type, std::vector<sf::Texture*> ship_textures) : texture_(texture), state_(0),
	original_scale_(scale), original_pos_(pos), type_(std::move(type))
{
	// Setup sprite_
	this->sprite_.setTexture(*this->texture_);
	this->sprite_.setScale(scale);
	this->sprite_.setOrigin(static_cast<sf::Vector2f>(this->texture_->getSize()) / 2.f);
	this->sprite_.setPosition(pos);
	if (!vertical)
		this->sprite_.rotate(270);
	// Setup ship textures
	if (ship_textures.empty())
		std::cout << "ERROR: Textures not found!\n";
	else
	{
		std::vector<sf::Texture*>::iterator itr = ship_textures.begin();
		for (itr = ship_textures.begin(); itr < ship_textures.end(); itr++)
			this->ship_textures_.push_back(*itr);
	}
}

Figurine::Figurine(sf::Texture* texture, sf::Vector2f scale, std::string type) : texture_(texture), state_(0),
original_scale_(scale), type_(std::move(type))
{
	// Setup sprite_
	this->sprite_.setTexture(*this->texture_);
	this->sprite_.setScale(scale);
	this->sprite_.setOrigin(static_cast<sf::Vector2f>(this->texture_->getSize()) / 2.f);
}

void Figurine::draw(sf::RenderWindow& window) const
{
	window.draw(this->sprite_);
}

// Sets the state_ to 0 default for no change/reset, 1 for hovering, 2 for being held
void Figurine::update(const sf::Vector2f& mouse_pos)
{
	if (this->state_ == 2)
		this->drag(mouse_pos);
	else if (this->sprite_.getGlobalBounds().contains(sf::Vector2f(mouse_pos.x, mouse_pos.y))) //Button is hovered over
	{
		this->state_ = 1;
		this->hover(.1, .1);
	}
	else if (this->state_ != 0) // resets if not in bounds of the button
	{
		this->state_ = 0;
		this->reset();
	}
}

void Figurine::hover(const double x_off, const double y_off)
{
	this->sprite_.setScale(original_scale_.x + static_cast<float>(x_off), original_scale_.y + static_cast<float>(y_off));
	this->sprite_.setColor(sf::Color(255, 0, 0));
}

void Figurine::drag(const sf::Vector2f& mouse_pos)
{
	this->sprite_.setPosition(mouse_pos.x, mouse_pos.y);
}

void Figurine::reset()
{
	this->sprite_.setScale(original_scale_.x, original_scale_.y);
	this->sprite_.setColor(sf::Color::White);
	this->sprite_.setRotation(270);
}

bool Figurine::release(Map& map, const sf::Vector2f& mouse_pos, unsigned rotation, sf::Vector2u& ship_pos)
{
	if (!map.check_placement(mouse_pos, this->type_, this->ship_textures_, rotation, ship_pos))
	{
		this->reset_position();
		return false;
	}
	return true;
}

std::string& Figurine::get_type()
{
	return this->type_;
}

void Figurine::rotate(const float angle)
{
	this->sprite_.rotate(angle);
}

bool Figurine::contains(const sf::Vector2f mouse_pos) const
{
	if (this->sprite_.getGlobalBounds().contains(mouse_pos.x, mouse_pos.y))
		return true;
	return false;
}

void Figurine::set_state(const unsigned state)
{
	this->state_ = state;
}

std::vector <sf::Texture*> Figurine::get_ship_texture()
{
	return this->ship_textures_;
}

float Figurine::get_abs_angle() const
{
	return this->sprite_.getRotation();
}

void Figurine::reset_position()
{
	this->sprite_.setPosition(this->original_pos_.x, this->original_pos_.y);
}

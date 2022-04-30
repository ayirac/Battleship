#include "Cell.h"

Cell::Cell(unsigned size, sf::Texture* empty_tex) : size_(size), box_(sf::Vector2f(static_cast<float>(this->size_), static_cast<float>(this->size_))), type_("Empty"),
	visible_(false), peg_(nullptr), texture_(nullptr), empty_texture_(empty_tex)
{
	this->box_.setOrigin(this->size_/2, this->size_/2);
	this->box_.setOutlineColor(sf::Color::Red);
	this->box_.setOutlineThickness(1);
	this->box_.setTexture(empty_tex);
}

void Cell::set_texture(std::string type)
{
	if (type == "NULL")
	{
		this->box_.setTexture(NULL);
		this->texture_ = NULL;
		this->type_ = "Empty";
	}
}

void Cell::set_texture(sf::Texture* t, std::string type, bool change)
{
	this->texture_ = t;
	this->type_ = type;
	if (change)
	{
		this->box_.setTexture(this->texture_, true);
		this->visible_ = true;
	}
}

void Cell::remove_texture()
{
	this->type_ = "Empty";
	this->texture_ = nullptr;
	this->box_.setTexture(this->empty_texture_, true);
	this->visible() = false;
}

void Cell::draw(sf::Vector2f& loc, sf::RenderWindow& win)
{
	this->box_.setPosition(loc);
	win.draw(this->box_);
	// draw peg if there is one on it
	if (this->peg_ != nullptr) 
	{
		sf::Vector2f peg_pos(loc.x - this->peg_->get_shape().getSize().x / 2, loc.y - this->peg_->get_shape().getSize().y / 2);
		this->peg_->set_position(peg_pos);
		this->peg_->draw(win);
	}
}

void Cell::rotate(float& angle)
{
	this->box_.rotate(angle);
}

void Cell::abs_rotate(unsigned& angle)
{
	this->box_.setRotation(angle);
}

std::string Cell::get_type()
{
	return this->type_;
}

bool Cell::contains(const sf::Vector2f& pos)
{
	if (this->box_.getGlobalBounds().contains(pos.x, pos.y))
		return true;
	return false;
}

void Cell::add_peg(ImageBox* peg)
{
	this->peg_ = peg;
}

void Cell::remove_peg()
{
	this->peg_ = nullptr;
}

bool Cell::has_peg()
{
	if (this->peg_ == nullptr)
		return false;
	return true;
}

sf::RectangleShape& Cell::get_shape()
{
	return this->box_;
}

bool& Cell::visible()
{
	return this->visible_;
}

sf::Texture* Cell::get_texture()
{
	return this->texture_;
}

#include "Button.h"

Button::Button(sf::Vector2f pos, unsigned char_size, std::string text, const sf::Font& font,
               sf::Color default_color, sf::Color hover_color, sf::Color press_color, unsigned state) :
	text_(text, font, char_size), original_pos_(pos), original_char_size_(char_size), state_(state), colors_{default_color, hover_color, press_color}
{
	this->text_.setFillColor(this->colors_[0]);
	this->text_.setPosition(sf::Vector2f(pos.x - (this->text_.getLocalBounds().width / 2), pos.y - (this->text_.getLocalBounds().height / 2)));
}

Button::Button(sf::Vector2f pos, unsigned state) : original_pos_(pos), state_(state), original_char_size_(0) { }

void Button::draw(sf::RenderWindow& win) const
{
	win.draw(this->text_);
}

void Button::update(const sf::Vector2i& mouse_pos, sf::RenderWindow& window)
{
	if (this->state_ == 2)
	{
		this->press();
	}
	else if (this->text_.getGlobalBounds().contains(sf::Vector2f(static_cast<float>(mouse_pos.x), static_cast<float>(mouse_pos.y))))
	{
		this->state_ = 1;
		this->hover(3, 1, 2);
	}
	else if (this->state_ != 0)
	{
		this->state_ = 0;
		this->reset();
	}
}

void Button::hover(unsigned x_off, unsigned y_off, unsigned char_off)
{
	this->text_.setFillColor(this->colors_[1]);
	this->text_.setCharacterSize(this->original_char_size_ + char_off);
	this->text_.setPosition(sf::Vector2f(this->original_pos_.x - this->text_.getLocalBounds().width / 2, original_pos_.y - this->text_.getLocalBounds().height / 2));
}

void Button::press()
{
	this->text_.setFillColor(this->colors_[2]);
}

void Button::reset()
{
	this->text_.setCharacterSize(this->original_char_size_);
	this->text_.setFillColor(this->colors_[0]);
	this->text_.setPosition(sf::Vector2f(this->original_pos_.x - this->text_.getLocalBounds().width / 2, original_pos_.y - this->text_.getLocalBounds().height / 2));
		
}


bool Button::contains(const sf::Vector2f& mouse_pos) const
{
	if (this->text_.getGlobalBounds().contains(mouse_pos.x, mouse_pos.y))
		return true;
	return false;
}

void Button::set_state(unsigned state)
{
	this->state_ = state;
}

std::string Button::get_text() const
{
	return this->text_.getString();
}

unsigned& Button::get_state()
{
	return this->state_;
}

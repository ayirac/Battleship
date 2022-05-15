#include "ButtonTexture.h"

ButtonTexture::ButtonTexture()
{
}

ButtonTexture::ButtonTexture(sf::Vector2f pos, const sf::Vector2f& size, const unsigned& char_size, const std::string& text,
	const sf::Font& font, sf::Texture* normal_tex, sf::Texture* press_tex, sf::Color default_color,
	sf::Color hover_color, sf::Color press_color, unsigned state, unsigned y_correction) : Button(pos, char_size, text, font, default_color, hover_color, press_color, state), original_size_(size),
	original_pos_(pos), y_correction_(y_correction)
{
	// Setup shape_
	this->shape_.setPosition(pos);
	this->shape_.setSize(size);
	this->textures_.push_back(normal_tex);
	this->textures_.push_back(press_tex);
	this->shape_.setTexture(this->textures_.at(0));
	// Setup text_
	this->text_.setString(text);
	this->text_.setFont(font);
	this->text_.setCharacterSize(char_size);
	this->text_.setFillColor(this->colors_[0]);
	this->text_.setOutlineColor(sf::Color::Black);
	this->text_.setOutlineThickness(1);
	this->text_.setOrigin(this->text_.getLocalBounds().width / 2, this->text_.getLocalBounds().height / 2);
	this->text_.setPosition(sf::Vector2f(this->shape_.getGlobalBounds().left + this->shape_.getGlobalBounds().width/ 2, 
		this->shape_.getGlobalBounds().top + this->shape_.getGlobalBounds().height / 2 - y_correction));
}

ButtonTexture::ButtonTexture(sf::Vector2f pos, const sf::Vector2f& size, const std::string& text, sf::Texture* normal_tex,
	sf::Texture* press_tex, unsigned state) : Button(pos, state), original_size_(size), original_pos_(pos)
{
	this->text_.setString(text);
	this->shape_.setPosition(pos);
	this->shape_.setSize(size);
	this->textures_.push_back(normal_tex);
	this->textures_.push_back(press_tex);
	this->shape_.setTexture(this->textures_.at(0));
}

void ButtonTexture::update(sf::Vector2f& mouse_pos, sf::RenderWindow& window)
{
	if (this->state_ == 2)
		this->press();
	else if (this->shape_.getGlobalBounds().contains(sf::Vector2f(static_cast<float>(mouse_pos.x), static_cast<float>(mouse_pos.y))))
	{
		this->state_ = 1;
		this->hover(3, 2, 2);

	}
	else if (this->state_ != 0)
	{
		this->reset();
	}
}

void ButtonTexture::hover(unsigned x_off, unsigned y_off, unsigned char_off)
{
	this->text_.setFillColor(this->colors_[1]);
	this->text_.setCharacterSize(this->original_char_size_ + char_off);
	this->shape_.setSize(sf::Vector2f(this->original_size_.x + x_off, this->original_size_.y + y_off));
	this->shape_.setPosition(this->original_pos_);
	this->text_.setPosition(sf::Vector2f(this->shape_.getGlobalBounds().left + this->shape_.getGlobalBounds().width / 2.1, 
		this->shape_.getGlobalBounds().top + this->shape_.getGlobalBounds().height / 2.1 - this->y_correction_));
}

void ButtonTexture::press()
{
	this->text_.setFillColor(this->colors_[2]);
	this->shape_.setTexture(this->textures_.at(1));
}

bool ButtonTexture::contains(const sf::Vector2f& mouse_pos) const
{
	if (this->shape_.getGlobalBounds().contains(mouse_pos.x, mouse_pos.y))
		return true;
	return false;
}

void ButtonTexture::draw(sf::RenderWindow& win) const
{
	win.draw(this->shape_);
	win.draw(this->text_);
}

void ButtonTexture::reset()
{
	this->state_ = 0;
	this->shape_.setSize(this->original_size_);
	this->text_.setCharacterSize(this->original_char_size_);
	this->text_.setFillColor(this->colors_[0]);
	this->shape_.setTexture(this->textures_.at(0));
	this->text_.setPosition(sf::Vector2f(this->shape_.getPosition().x + this->shape_.getSize().x / 2, this->shape_.getPosition().y + this->shape_.getSize().y / 2 - this->y_correction_));
}

sf::RectangleShape& ButtonTexture::get_shape()
{
	return this->shape_;
}

float ButtonTexture::difference(ButtonTexture& button) const
{
	return this->shape_.getGlobalBounds().top - button.get_shape().getGlobalBounds().top;
}

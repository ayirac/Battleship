#include "ImageTextBox.h"

#include <iostream>

ImageTextBox::ImageTextBox() : ImageBox() { }


ImageTextBox::ImageTextBox(sf::Texture* texture, sf::Vector2f size, sf::Vector2f pos, std::string text, const sf::Font& font,
    unsigned char_size, sf::Color text_color, sf::Color outline_color, sf::Vector2f text_offset) : ImageBox(texture, size, pos), text_(text, font, char_size), colors_{ text_color, outline_color }, text_offset_(text_offset)
{
    this->text_.setFillColor(this->colors_[0]);
    this->text_.setOutlineThickness(2);
    this->text_.setOutlineColor(this->colors_[1]);
    this->text_.setOrigin(this->text_.getLocalBounds().left + this->text_.getLocalBounds().width / 2.0f, this->text_.getLocalBounds().top + this->text_.getLocalBounds().height / 2.0f);
    this->text_.setPosition(sf::Vector2f((this->shape_.getGlobalBounds().left + this->shape_.getGlobalBounds().width / 2) + text_offset.x,
        (this->shape_.getGlobalBounds().top + this->shape_.getGlobalBounds().height / 2) + text_offset.y));
}

void ImageTextBox::draw(sf::RenderWindow& win) const
{
    win.draw(this->shape_);
    win.draw(this->text_);
}

void ImageTextBox::set_text(std::string text)
{
	this->text_.setString(text);
    this->text_.setOrigin(this->text_.getLocalBounds().left + this->text_.getLocalBounds().width / 2.0f, this->text_.getLocalBounds().top + this->text_.getLocalBounds().height / 2.0f);
    this->text_.setPosition(sf::Vector2f((this->shape_.getGlobalBounds().left + this->shape_.getGlobalBounds().width / 2) + this->text_offset_.x,
        (this->shape_.getGlobalBounds().top + this->shape_.getGlobalBounds().height / 2) + this->text_offset_.y));
}

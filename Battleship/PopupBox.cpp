#include "PopupBox.h"

#include <iostream>

PopupBox::PopupBox(sf::Vector2f size, sf::Vector2f pos, std::string header_text, sf::Font* font, unsigned rows, std::string rows_text, std::vector<sf::Texture*>& textures) :
	Box(size, pos), font_(font), rows_(rows), textures_(textures)
{
	// Set the proportions depending on the amount of rows
	this->shape_.setOrigin(this->shape_.getSize().x / 2, this->shape_.getSize().y / 2);
	double header_proportion = 0, button_proportion = 0, row_proportion = 0;
	if (rows == 0)
	{
		header_proportion = 0.6;
		row_proportion = 0.0;
		button_proportion = 0.4;
	}
	else if (rows == 1)
	{
		header_proportion = 0.4;
		row_proportion = 0.3;
		button_proportion = 0.3;
	}
	else if (rows == 2)
	{
		header_proportion = 0.3;
		row_proportion = 0.21;
		button_proportion = 0.28;
	}
	else if (rows == 3)
	{
		header_proportion = 0.28;
		row_proportion = 0.173;
		button_proportion = 0.20;
	}

	// Create text_containers to constrain the text to ensure consistent spacing
	this->text_containers_.push_back(sf::IntRect(this->shape_.getGlobalBounds().left, this->shape_.getGlobalBounds().top, 
		this->shape_.getGlobalBounds().width, this->shape_.getGlobalBounds().height * header_proportion));				// header
	for (int i = 0; i < rows; i++)																						// rows
	{
		this->text_containers_.push_back(sf::IntRect(this->shape_.getGlobalBounds().left, this->text_containers_.back().top + this->text_containers_.back().height,
			this->shape_.getGlobalBounds().width, this->shape_.getGlobalBounds().height * row_proportion));
	}
	this->text_containers_.push_back(sf::IntRect(this->shape_.getGlobalBounds().left, this->text_containers_.back().top + this->text_containers_.back().height,
		this->shape_.getGlobalBounds().width, this->shape_.getGlobalBounds().height * button_proportion));				// button


	// Setup texts_ vector
	this->texts_.push_back(sf::Text(header_text, *this->font_, this->text_containers_.back().height * 0.6));
	this->texts_.back().setOrigin(this->texts_.back().getGlobalBounds().width / 2, this->texts_.back().getGlobalBounds().height / 2);
	this->texts_.back().setPosition(this->text_containers_[0].left + this->text_containers_[0].width / 2, this->text_containers_[0].top + this->text_containers_[0].height / 2);
	this->texts_.back().setOutlineColor(sf::Color::Black);
	this->texts_.back().setOutlineThickness(2);
	this->texts_.back().setFillColor(sf::Color(245, 163, 53));
	unsigned row_start = 0, row_len = 0;
	for (int i = 1; i < rows + 1; i++)
	{
		row_start = rows_text.find("|", row_start + 1);
		row_len = rows_text.find("|", row_start + 1) - row_start;
		this->texts_.push_back(sf::Text(rows_text.substr(row_start + 1, row_len - 1), *this->font_, this->text_containers_.back().height * 0.45));
		this->texts_.back().setOrigin(this->texts_.back().getLocalBounds().width / 2, this->texts_.back().getLocalBounds().height / 1.1);
		this->texts_.back().setFillColor(sf::Color(245, 163, 53));
		this->texts_.back().setOutlineColor(sf::Color::Black);
		this->texts_.back().setOutlineThickness(1);
		this->texts_.back().setPosition(this->text_containers_[i].left + this->text_containers_[i].width / 2, this->text_containers_[i].top + this->text_containers_[i].height / 2);
	}

	sf::Vector2f button_size(this->text_containers_.back().width * 0.3, this->text_containers_.back().height * 0.8);
	this->exit_button_ = new ButtonTexture{ sf::Vector2f(this->text_containers_.back().left + this->text_containers_.back().width / 2 - button_size.x / 2,
		this->text_containers_.back().top + this->text_containers_.back().height / 2 - button_size.y / 2), button_size, static_cast<unsigned>(button_size.y * 0.7),
		"Exit", *this->font_, this->textures_[0], this->textures_[1], sf::Color(243, 163, 53), sf::Color(251,188,38), sf::Color(213,144,19), 0, 8};
	this->shape_.setFillColor(sf::Color(145, 109, 74));
	this->shape_.setOutlineColor(sf::Color(115, 86, 59));
	this->shape_.setOutlineThickness(4);
}

void PopupBox::draw(sf::RenderWindow& win) const
{
	win.draw(this->shape_);
	for (int i = 0; i < this->texts_.size(); i++)
		win.draw(this->texts_[i]);
	this->exit_button_->draw(win);
}

void PopupBox::update(sf::RenderWindow& win, sf::Vector2f& mouse_pos)
{
	this->exit_button_->update(mouse_pos, win);
}

void PopupBox::exit_key_pressed()
{
	this->state_ = 0;
}

void PopupBox::release_button()
{
	const std::string btn_text = this->held_button_->get_text();
	if (btn_text == "Exit")
	{
		this->exit_key_pressed();
	}

	this->held_button_->reset();
	if (this->held_button_->get_state() != 3)
		this->held_button_->set_state(0);
	this->held_button_ = nullptr;
}

bool PopupBox::process_click(sf::Vector2f& mouse_pos)
{
	if (this->exit_button_->contains(mouse_pos) && this->exit_button_->get_state() != 3)
	{
		this->exit_button_->set_state(2);
		this->held_button_ = this->exit_button_;
		return true;
	}
	return false;
}

bool PopupBox::holding_button()
{
	if (this->held_button_ == nullptr)
		return false;
	return true;
}

Button* PopupBox::get_button()
{
	return this->exit_button_;
}

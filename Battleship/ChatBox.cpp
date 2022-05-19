#include "ChatBox.h"
#include <iostream>

ChatBox::ChatBox(sf::Vector2f& size, sf::Vector2f& pos, std::string& instructions, unsigned char_size,
	std::vector<sf::Texture*> textures, sf::Font& font, Multiplayer* multiplayer, unsigned max_textfield_entry, unsigned max_rows) :
	InputBox(size, pos, instructions, char_size, textures, font, multiplayer, max_textfield_entry), detect_messages_thread_(nullptr), scroll_bar_image_(nullptr),
	CHAT_MESSAGES_BASE_Y_(pos.y + size.y / 1.5), bottom_row_(-1), front_row_(-1), current_row_(-1), MAX_ROWS_(max_rows), row_spacing_(1.25 * char_size)
{
	// Setup textfield
	this->textfield_shape_.setSize(sf::Vector2f(this->textfield_shape_.getSize().x, this->textfield_shape_.getSize().y / 1.6));
	this->textfield_shape_.setPosition(sf::Vector2f(pos.x + size.x / 3.7 - this->textfield_shape_.getSize().x / 3.3, pos.y + size.y / 1.23));
	this->buttons_.push_back(ButtonTexture(sf::Vector2f(this->textfield_shape_.getGlobalBounds().left + this->textfield_shape_.getGlobalBounds().width + 5, 
		this->textfield_shape_.getGlobalBounds().top + this->textfield_shape_.getGlobalBounds().height / 2 - (size.y / 5.1) / 2), sf::Vector2f(size.x / 9, 
		this->textfield_shape_.getGlobalBounds().height/1.05), "Enter", textures[0], textures[1], 0));
	this->textfield_text_.setPosition(this->textfield_shape_.getGlobalBounds().left + 10, this->textfield_shape_.getGlobalBounds().top + this->textfield_shape_.getGlobalBounds().height/2.6);

	// Setup scrolling buttons
	sf::Vector2f btn_size(25, 25);
	this->buttons_.push_back(ButtonTexture(sf::Vector2f((this->shape_.getGlobalBounds().left + this->shape_.getGlobalBounds().width),
		this->shape_.getGlobalBounds().top), btn_size, "Up", this->textures_[2], this->textures_[3], 0));
	this->buttons_.push_back(ButtonTexture(sf::Vector2f((this->shape_.getGlobalBounds().left + this->shape_.getGlobalBounds().width),
		this->shape_.getGlobalBounds().top + this->shape_.getGlobalBounds().height - btn_size.y), btn_size, "Down", this->textures_[4], this->textures_[5], 0));
	// Setup scrollbar
	this->scroll_bar_image_ = new ImageBox(this->textures_[6], sf::Vector2f(btn_size.x, this->buttons_[2].difference(this->buttons_[1])), 
		sf::Vector2f(this->shape_.getGlobalBounds().left + this->shape_.getGlobalBounds().width, this->shape_.getGlobalBounds().top));
}

void ChatBox::draw(sf::RenderWindow& win) const
{
	// Draw chatbox overall shape
	win.draw(this->shape_);
	win.draw(this->textfield_shape_);
	win.draw(this->textfield_text_);
	this->scroll_bar_image_->draw(win);
	for (int i = 0; i < this->buttons_.size(); i++)
		this->buttons_[i].draw(win);

	if (this->edit_mode_ && this->cursor_active_)
		win.draw(this->text_cursor_);

	// Draw chatbox messages
	if (this->front_row_ != -1)
	{
		int e = 0;
		for (int i = this->front_row_; i >= this->bottom_row_ && e < this->MAX_ROWS_ + this->bottom_row_; i--)
		{
			win.draw(this->name_texts_[i]);
			win.draw(this->message_texts_[i]);
			e++;
		}
	}
}

void ChatBox::update(sf::RenderWindow& win, sf::Vector2f& mouse_pos)
{
	for (int i = 0; i < this->buttons_.size(); i++)
		this->buttons_[i].update(mouse_pos, win);
	if (this->edit_mode_ && this->animate_cursor_thread_ == nullptr)
	{
		this->animate_cursor_thread_ = new std::thread(&InputBox::animate_text_cursor, this);
	}
	else if (!this->edit_mode() && this->animate_cursor_thread_ != nullptr)
	{
		this->animate_cursor_thread_->join();
		delete this->animate_cursor_thread_;
		this->animate_cursor_thread_ = nullptr;
	}

	if (this->multiplayer_->connected() && this->detect_messages_thread_ == nullptr)
	{
		this->detect_messages_thread_ = new std::thread(&ChatBox::detect_messages, this);
	}
}

void ChatBox::release_button()
{
	const std::string btn_text = this->held_button_->get_text();
	if (btn_text == "Enter")
	{
		this->enter_key_pressed();
	}
	else if (btn_text == "Up")
	{
		if (this->bottom_row_ > 0)
			this->shift_up();
	}
	else if (btn_text == "Down")
	{
		if (this->chat_messages_.size() > this->MAX_ROWS_ && this->front_row_ < this->chat_messages_.size() - 1)
			this->shift_down();
	}

	this->held_button_->reset();
	if (this->held_button_->get_state() != 3)
		this->held_button_->set_state(0);
	this->held_button_ = nullptr;
}

void ChatBox::add_entry(ChatMessage& chat_message)
{
	std::cout << chat_message.name << ": " << chat_message.message << std::endl;
	if (this->message_texts_.empty()) { // init
		this->bottom_row_++;
		this->front_row_++;
		this->current_row_++;
	}

	if (this->chat_messages_.size() > this->MAX_ROWS_ - 1)
		this->bottom_row_++;
	if (!this->message_texts_.empty())
	{
		while (this->front_row_ + 1 != this->message_texts_.size()) // Move to bottom of chat before adding the new entry
			shift_down();
	}
	if (1 + this->front_row_ <= this->message_texts_.size()) // Doesn't allow movement if at bottom of table
		this->front_row_++;

	// Push messages up if there are any
	for (int i = 0; i < this->message_texts_.size(); i++)
	{
		this->chat_message_containers_[i].left = this->message_texts_[i].getPosition().x;
		this->chat_message_containers_[i].top = this->message_texts_[i].getPosition().y - this->row_spacing_;
		this->name_texts_[i].setPosition(this->name_texts_[i].getPosition().x, this->name_texts_[i].getPosition().y - this->row_spacing_);
		this->message_texts_[i].setPosition(this->message_texts_[i].getPosition().x, this->message_texts_[i].getPosition().y - this->row_spacing_);
	}
	// Setup texts/their container & add them. Note - container is used so a fixed line height is adhered to no matter the char_size
	this->message_texts_.push_back(sf::Text(": " + chat_message.message, *this->font_, this->char_size_));
	this->name_texts_.push_back(sf::Text(chat_message.name, *this->font_, this->char_size_));
	this->chat_message_containers_.push_back(sf::IntRect(this->textfield_shape_.getGlobalBounds().left + 10, this->CHAT_MESSAGES_BASE_Y_ + this->row_spacing_ * this->current_row_,
		this->textfield_shape_.getGlobalBounds().width, this->row_spacing_)); 
	this->name_texts_.back().setPosition(this->chat_message_containers_.back().left, this->CHAT_MESSAGES_BASE_Y_);
	this->name_texts_.back().setFillColor(chat_message.color);
	this->name_texts_.back().setOutlineThickness(1);
	this->name_texts_.back().setOutlineColor(sf::Color::Black);
	this->message_texts_.back().setPosition(this->name_texts_.back().getGlobalBounds().left + this->name_texts_.back().getGlobalBounds().width + 1, this->CHAT_MESSAGES_BASE_Y_);
	this->message_texts_.back().setFillColor(sf::Color(245, 163, 53));
	this->message_texts_.back().setOutlineThickness(1);
	this->message_texts_.back().setOutlineColor(sf::Color::Black);
	this->chat_messages_.push_back(chat_message);

	if (this->current_row_ != this->MAX_ROWS_)
		this->current_row_++;
}

void ChatBox::shift_down()
{
	int e = this->front_row_ + 1;
	for (unsigned i = this->front_row_ + 1; e < (this->front_row_ + 1) + this->MAX_ROWS_; i--)
	{
		this->name_texts_[i].setPosition(this->name_texts_[i - 1].getPosition());
		this->message_texts_[i].setPosition(this->message_texts_[i - 1].getPosition());
		e++;
	}
	this->front_row_++;
	this->bottom_row_++;
}

void ChatBox::shift_up()
{
	for (unsigned i = this->bottom_row_ - 1; i < this->front_row_; i++)
	{
		this->name_texts_[i].setPosition(this->name_texts_[i + 1].getPosition());
		this->message_texts_[i].setPosition(this->message_texts_[i + 1].getPosition());
	}
	this->bottom_row_--;
	this->front_row_--;
}

void ChatBox::enter_key_pressed()
{
	ChatMessage c{ "Player", this->textfield_entry_, sf::Color(54, 148, 244) };
	std::string message = "$M" + this->textfield_entry_;
	this->multiplayer_->send_data(message);
	this->add_entry(c);
	this->textfield_entry_.clear();
	this->textfield_text_.setString("");
	this->edit_mode_ = false;
}

void ChatBox::detect_messages()
{
	while (true)
	{
		if (this->multiplayer_->new_data())
		{
			this->add_entry(this->multiplayer_->get_new_message());
			this->multiplayer_->unflag_new_data();
		}
	}
	
}

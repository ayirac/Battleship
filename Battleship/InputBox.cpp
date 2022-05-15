#include "InputBox.h"
#include <iostream>

InputBox::InputBox(sf::Vector2f size, sf::Vector2f pos, std::string& header, unsigned char_size,
	std::vector<sf::Texture*> textures, sf::Font& font, Multiplayer* multiplayer, unsigned max_textfield_entry) : Box(size, pos), textures_(textures), textfield_text_("Enter a message...", font, char_size), edit_mode_(false), cursor_active_(true),
	text_cursor_(sf::Vector2f(3, char_size)), animate_cursor_thread_(nullptr), held_button_(nullptr), multiplayer_(multiplayer), original_header_(header), MAX_TEXTFIELD_ENTRY(max_textfield_entry),
	original_textfield_color_(sf::Color(151, 117, 86)), font_(&font), char_size_(char_size)
{
	this->shape_.setFillColor(sf::Color(145, 109, 74));
	this->shape_.setOutlineColor(sf::Color(115, 86, 59));
	this->shape_.setOutlineThickness(4);


	this->textfield_shape_ = sf::RectangleShape(sf::Vector2f(size.x / 1.2, size.y / 4));
	this->textfield_shape_.setOutlineColor(sf::Color(115, 86, 59));
	this->textfield_shape_.setOutlineThickness(4);
	this->textfield_shape_.setFillColor(this->original_textfield_color_);

	this->header_text_.setOrigin(this->header_text_.getLocalBounds().width / 2, this->header_text_.getLocalBounds().height / 2); // header text
	this->header_text_.setFillColor(sf::Color(245, 163, 53));
	this->header_text_.setOutlineColor(sf::Color::Black);

	this->textfield_text_.setOrigin(this->textfield_text_.getLocalBounds().left, this->textfield_text_.getLocalBounds().height / 2); // textfield text
	this->textfield_text_.setFillColor(sf::Color(245, 163, 53));
	this->textfield_text_.setOutlineColor(sf::Color::Black);
	this->text_cursor_.setFillColor(sf::Color(245, 163, 53));
}

InputBox::InputBox(sf::Vector2f size, sf::Vector2f pos, std::string& instructions, unsigned char_size, std::vector<sf::Texture*> textures, sf::Font& font, Multiplayer* multiplayer,
                   sf::Sprite* loading_sprite, unsigned max_textfield_entry) : Box(size, pos), textures_(textures), header_text_(instructions, font, char_size), textfield_text_("192.168.254.22", font, char_size), edit_mode_(false), cursor_active_(true),
	text_cursor_(sf::Vector2f(4, char_size)), animate_cursor_thread_(nullptr), held_button_(nullptr), multiplayer_(multiplayer),
	loading_sprite_(loading_sprite), original_header_(instructions), MAX_TEXTFIELD_ENTRY(max_textfield_entry),original_textfield_color_(sf::Color(151, 117, 86)), font_(&font)
{
	this->shape_.setFillColor(sf::Color(145, 109, 74)); 
	this->shape_.setOutlineColor(sf::Color(115, 86, 59));
	this->shape_.setOutlineThickness(4);

	sf::RectangleShape textfield(sf::Vector2f(size.x / 1.2, size.y / 4));
	this->textfield_shape_ = textfield;
	this->textfield_shape_.setPosition(sf::Vector2f(pos.x + size.x / 3.2 - this->textfield_shape_.getSize().x / 3, pos.y + size.y / 1.5));
	this->textfield_shape_.setOutlineColor(sf::Color(115, 86, 59));
	this->textfield_shape_.setOutlineThickness(4);
	this->textfield_shape_.setFillColor(this->original_textfield_color_);

	this->header_text_.setOrigin(this->header_text_.getLocalBounds().width / 2, this->header_text_.getLocalBounds().height / 2); // header text
	this->header_text_.setFillColor(sf::Color(245, 163, 53));
	this->header_text_.setOutlineColor(sf::Color::Black);
	this->header_text_.setPosition(pos.x + size.x/2, pos.y + size.y/4);

	this->textfield_text_.setOrigin(this->textfield_text_.getLocalBounds().left, this->textfield_text_.getLocalBounds().height / 2); // textfield text
	this->textfield_text_.setFillColor(sf::Color(245, 163, 53)); 
	this->textfield_text_.setOutlineColor(sf::Color::Black);
	this->textfield_text_.setPosition(this->textfield_shape_.getGlobalBounds().left + 10, this->textfield_shape_.getGlobalBounds().top + this->textfield_shape_.getGlobalBounds().height/3.9);
	this->text_cursor_.setFillColor(sf::Color(245, 163, 53));

	this->buttons_.push_back(ButtonTexture(sf::Vector2f(this->textfield_shape_.getGlobalBounds().left + this->textfield_shape_.getGlobalBounds().width + 5, 
		this->textfield_shape_.getGlobalBounds().top + this->textfield_shape_.getGlobalBounds().height/2 - (size.y/4)/2), sf::Vector2f(size.x / 9, size.y / 4), "Enter", textures[0], textures[1], 0));
}

InputBox::~InputBox() {}

void InputBox::draw(sf::RenderWindow& win) const
{
	win.draw(this->shape_);
	win.draw(this->header_text_);
	win.draw(this->textfield_shape_);
	win.draw(this->textfield_text_);
	this->buttons_[0].draw(win);

	if (this->edit_mode_ && this->cursor_active_)
		win.draw(this->text_cursor_);
}

void InputBox::update(sf::RenderWindow& win, sf::Vector2f& mouse_pos)
{
	this->buttons_[0].update(mouse_pos, win);
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
	if (this->multiplayer_->get_connecting() != nullptr && this->state_ == 1)
	{
		this->textfield_shape_.setFillColor(sf::Color(122, 122, 119));
		this->header_text_.setString("Connecting");
		this->header_text_.setOrigin(this->header_text_.getLocalBounds().width / 2, this->header_text_.getLocalBounds().height / 2);
		this->header_text_.setPosition(this->shape_.getGlobalBounds().left + this->shape_.getGlobalBounds().width/2, this->shape_.getGlobalBounds().top + this->shape_.getGlobalBounds().height / 4);
		this->loading_sprite_->setOrigin(this->loading_sprite_->getLocalBounds().left, this->loading_sprite_->getLocalBounds().top + this->loading_sprite_->getLocalBounds().height / 2);
		this->loading_sprite_->setPosition(this->header_text_.getGlobalBounds().left + this->header_text_.getGlobalBounds().width + 10, this->header_text_.getGlobalBounds().top + this->header_text_.getGlobalBounds().height/2);
		this->state_ = 2;
	}
	else if (this->multiplayer_->get_connecting() == nullptr && this->state_ == 2)
	{
		this->textfield_shape_.setFillColor(this->original_textfield_color_);
		this->header_text_.setString(this->original_header_);
		this->header_text_.setOrigin(this->header_text_.getLocalBounds().width / 2, this->header_text_.getLocalBounds().height / 2);
		this->header_text_.setPosition(this->shape_.getGlobalBounds().left + this->shape_.getGlobalBounds().width / 2, this->shape_.getGlobalBounds().top + this->shape_.getGlobalBounds().height / 4);
		this->state_ = 1;
	}
	
}
bool& InputBox::edit_mode()
{
	return this->edit_mode_;
}

void InputBox::set_edit_mode(bool b)
{
	this->edit_mode_ = b;
}

void InputBox::process_keyboard(char key)
{
	sf::Text temp_text(this->textfield_entry_ + key, *this->font_, this->char_size_);
	if (temp_text.getGlobalBounds().width * 1.2 < this->textfield_shape_.getGlobalBounds().width)
	{
		if (this->textfield_entry_.size() <= this->MAX_TEXTFIELD_ENTRY)
		{
			this->textfield_entry_ += key;
			this->textfield_text_.setString(this->textfield_entry_);
		}
	}
}

bool InputBox::contains(const sf::Vector2f& mouse_pos)
{
	if (this->textfield_shape_.getGlobalBounds().contains(mouse_pos))
		return true;
	return false;
}

void InputBox::animate_text_cursor()
{
	sf::Clock clock;
	// Duration to control animation speed
	int current_color = 180, original_color = 180, max_color = 220;
	float color_duration = float(), blink_duration = float();
	bool reverse = false;

	//everything below gets copied into the "processSplash" function
	while (this->edit_mode()) 
	{
		// Calculate time since last loop
		sf::Time dt = clock.restart();
		color_duration += dt.asSeconds();
		blink_duration += dt.asSeconds();

		// Blink the cursor
		if (blink_duration < .5)
			this->cursor_active_ = false;
		else if (blink_duration > 1)
			blink_duration = 0;
		else
			this->cursor_active_ = true;

		// Change cursor color
		// Animation duration per frame (0.3f) reached
		if (color_duration > 0.03f) {
			// Restart calculation of the duration
			color_duration = 0;

			// Loop through the animation colors
			if (current_color < max_color && !reverse) {
				current_color += 5;
			}
			else {
				// Start from first frame if last frame reached
				reverse = true;
				current_color -= 5;
				if (current_color < original_color)
					reverse = false;
			}
			//this->text_cursor_.setFillColor(sf::Color(currentColor, 140, 30));
			this->text_cursor_.setPosition(this->textfield_text_.getGlobalBounds().left + this->textfield_text_.getGlobalBounds().width + 5,
				this->textfield_shape_.getGlobalBounds().top + this->textfield_shape_.getGlobalBounds().height / 2 - this->text_cursor_.getGlobalBounds().height / 2);
		}
	}
}

void InputBox::delete_end_textfield()
{
	this->textfield_entry_.pop_back();
	this->textfield_text_.setString(this->textfield_entry_);
}

unsigned InputBox::get_textfield_size()
{
	return this->textfield_entry_.size();
}

void InputBox::release_button()
{
	const std::string btn_text = this->held_button_->get_text();
	if (btn_text == "Enter")
	{
		if (this->multiplayer_->get_connecting() == nullptr)
		{
			this->multiplayer_->connect(this->textfield_entry_);
		}
	}

	this->held_button_->reset();
	if (this->held_button_->get_state() != 3)
		this->held_button_->set_state(0);
	this->held_button_ = nullptr;
}

void InputBox::process_click(sf::Vector2f& mouse_pos)
{
	for (int i = 0; i < this->buttons_.size(); i++)
	{
		if (this->buttons_[i].contains(mouse_pos) && this->buttons_[i].get_state() != 3)
		{
			this->buttons_[i].set_state(2);
			this->held_button_ = &this->buttons_[i];
		}
	}
}

bool InputBox::holding_button()
{
	if (this->held_button_ == nullptr)
		return false;
	return true;
}

std::string& InputBox::get_textfield_entry()
{
	return this->textfield_entry_;
}
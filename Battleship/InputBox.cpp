#include "InputBox.h"
#include <iostream>

InputBox::InputBox() : Box(sf::Vector2f(0,0), sf::Vector2f(0, 0))
{ }

InputBox::InputBox(sf::Vector2f& size, sf::Vector2f& pos, std::string& instructions, unsigned char_size, std::vector<sf::Texture*> textures, const sf::Font& font, Multiplayer* mutliplayer)
	: Box(size, pos), textures_(textures), header_text_(instructions, font, char_size), textfield_text_("", font, char_size), edit_mode_(false), text_cursor_(sf::Vector2f(4, char_size)),
	  animate_cursor_thread_(nullptr), held_button_(nullptr), multiplayer_(mutliplayer)
{
	this->shape_.setFillColor(sf::Color(145, 109, 74)); 
	this->shape_.setOutlineColor(sf::Color(115, 86, 59));
	this->shape_.setOutlineThickness(4);

	sf::RectangleShape textfield(sf::Vector2f(size.x / 1.2, size.y / 4));
	this->textfield_shape_ = textfield;
	this->textfield_shape_.setPosition(sf::Vector2f(pos.x + size.x / 3.2 - this->textfield_shape_.getSize().x / 3, pos.y + size.y / 1.5));
	this->textfield_shape_.setOutlineColor(sf::Color::Black);
	this->textfield_shape_.setOutlineThickness(4);

	this->header_text_.setOrigin(this->header_text_.getLocalBounds().width / 2, this->header_text_.getLocalBounds().height / 2); // header text
	this->header_text_.setFillColor(sf::Color(245, 163, 53));
	this->header_text_.setOutlineColor(sf::Color::Black);
	this->header_text_.setPosition(pos.x + size.x/2, pos.y + size.y/4);

	this->textfield_text_.setOrigin(this->header_text_.getLocalBounds().left, this->header_text_.getLocalBounds().height / 2); // textfield text
	this->textfield_text_.setFillColor(sf::Color(245, 163, 53)); 
	this->textfield_text_.setOutlineColor(sf::Color::Black);
	this->textfield_text_.setPosition(this->textfield_shape_.getGlobalBounds().left + 10, this->textfield_shape_.getGlobalBounds().top + this->textfield_shape_.getGlobalBounds().height/3.9);
	this->text_cursor_.setFillColor(sf::Color(245, 163, 53));

	this->enter_button_ = new ButtonTexture(sf::Vector2f(this->textfield_shape_.getGlobalBounds().left + this->textfield_shape_.getGlobalBounds().width + 5, this->textfield_shape_.getGlobalBounds().top + this->textfield_shape_.getGlobalBounds().height/2 - (size.y/4)/2), sf::Vector2f(size.x / 9, size.y / 4), "Enter", textures[0], textures[1], 0);
}

void InputBox::draw(sf::RenderWindow& win) const
{
	win.draw(this->shape_);
	win.draw(this->header_text_);
	win.draw(this->textfield_shape_);
	win.draw(this->textfield_text_);
	this->enter_button_->draw(win);

	// set the _ to the right of the end of the text... or if no text, position 0
	if (this->edit_mode_)
	{
		win.draw(this->text_cursor_);
	}
}

void InputBox::draw(sf::RenderWindow& win, sf::Vector2f& mouse_pos) const
{
	win.draw(this->shape_);
	win.draw(this->header_text_);
	win.draw(this->textfield_shape_);
	win.draw(this->textfield_text_);
	this->enter_button_->update(mouse_pos, win);
	this->enter_button_->draw(win);

	// set the _ to the right of the end of the text... or if no text, position 0
	if (this->edit_mode_)
	{
		win.draw(this->text_cursor_);
	}

}

void InputBox::set_text(std::string text)
{
	this->header_text_.setString(text);
}

void InputBox::update()
{
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
	this->textfield_entry_ += key;
	this->textfield_text_.setString(this->textfield_entry_);
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
	int currentColor = 190, originalColor = 190;
	float duration = float();
	bool reverse = false;

	//everything below gets copied into the "processSplash" function
	while (this->edit_mode()) 
	{
		// How much time since last loop?
		sf::Time dt = clock.restart();
		duration += dt.asSeconds();

		// Animation duration per frame (0.1f) reached
		if (duration > 0.03f) {
			// Restart calculation of the duration
			duration = 0;

			// Loop through the animation colors
			if (currentColor < 255 && !reverse) {
				currentColor += 5;
			}
			else {
				// Start from first frame if last frame reached
				reverse = true;
				currentColor -= 5;
				if (currentColor < originalColor)
					reverse = false;
			}
			this->text_cursor_.setFillColor(sf::Color(currentColor, 20, 20));
			this->text_cursor_.setPosition(this->textfield_text_.getGlobalBounds().left + this->textfield_text_.getGlobalBounds().width + 5,
				this->textfield_shape_.getGlobalBounds().top + this->textfield_shape_.getGlobalBounds().height/2 - this->text_cursor_.getGlobalBounds().height/2);
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
		std::cout << "Attempting to connecting at " << this->textfield_entry_ << std::endl;
		this->multiplayer_->connect(sf::IpAddress(this->textfield_entry_));
	}

	this->held_button_->reset();
	if (this->held_button_->get_state() != 3)
		this->held_button_->set_state(0);
	this->held_button_ = nullptr;
}

void InputBox::process_click(sf::Vector2f& mouse_pos)
{
	if (this->enter_button_->contains(mouse_pos) && this->enter_button_->get_state() != 3)
	{
		this->enter_button_->set_state(2);
		this->held_button_ = this->enter_button_;
	}
}

bool InputBox::holding_button()
{
	if (this->held_button_ == nullptr)
		return false;
	return true;
}

Multiplayer* InputBox::get_multiplayer()
{
	return this->multiplayer_;
}

std::string& InputBox::get_textfield_entry()
{
	return this->textfield_entry_;
}

/*
 *
 * Input logic
click on box, causes _ to appear, indicating it is ready to type.
on user input, the character is added to string & displayed constrained in the text box
if the users text is about to go over some threshold on the right, the text will shift to the right as the user types.
- if the user hits right arrow, shift right.
- if the user hits left arrow, shift left.
- if the user hits delete, delete last character
utilize a stack?
 */


/*
 *
 *
 * void InputBox::animate_text_cursor()
{
	this->text_cursor_;

	sf::Clock clock;
	// Duration to control animation speed
	int currentColor = 1;
	float duration = float();

	//everything below gets copied into the "processSplash" function
	while (this->edit_mode()) 
	{
		// How much time since last loop?
		sf::Time dt = clock.restart();
		duration += dt.asSeconds();

		// Animation duration per frame (0.1f) reached
		if (duration > 0.01f) {
			// Restart calculation of the duration
			duration = 0;

			// Loop through the animation colors
			if (currentColor < 255) {
				currentColor += 5;
			}
			else {
				// Start from first frame if last frame reached
				currentColor = 0;
			}
			playAgain.setFillColor(sf::Color(0, currentColor, 0));
		}
	}
}
 */
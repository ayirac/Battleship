#ifndef BUTTON_H
#define BUTTON_H
#include <SFML/Graphics.hpp>

class Button
{
protected:
	sf::Text text_;
	sf::Vector2f original_pos_;
	unsigned original_char_size_;
	unsigned state_;			// 0 default, 1 hover, 2 press, 3 invisible
	const sf::Color colors_[3]; // 0 default, 1 hover, 2 press
public:
	// Creates a button given position, size, text size, text content, font, and text colors.
	Button(sf::Vector2f pos, unsigned char_size, std::string text, const sf::Font& font, sf::Color default_color, sf::Color hover_color, sf::Color press_color, unsigned state);
	// Creates a button given position and state.
	Button(sf::Vector2f pos, unsigned state);
	// Draws a button to a given window
	void draw(sf::RenderWindow& win) const;
	// Checks the state of the button when called and calls press(), hover(), and reset() depending on the state
	void update(const sf::Vector2f& mouse_pos, sf::RenderWindow& window);
	// Changes the size of the button's elements when state_ is 1
	void hover(unsigned x_off, unsigned y_off, unsigned char_off);
	// Changes the texture of the button to the pressed texture when state_ is 2
	void press();
	// Resets the size and texture of the button to their original states when called
	virtual void reset();
	// Returns true if the button contains the mouse_pos
	bool contains(const sf::Vector2f& mouse_pos) const;
	// Sets the state_ of the button according to the parameter
	void set_state(unsigned state);
	// Returns the size of the button/shape_
	const sf::Vector2f& get_size() const;
	// Returns the text_ string content as a string
	std::string get_text() const;
	// Return the state of the button
	unsigned& get_state();
};
#endif

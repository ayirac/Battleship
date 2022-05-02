#ifndef INPUTBOX_H
#define INPUTBOX_H
#include <SFML/Graphics/Text.hpp>
#include "Box.h"
#include "ButtonTexture.h"
#include <thread>
#include "Multiplayer.h"


class InputBox : public Box
{
private:
	sf::RectangleShape textfield_shape_;
	sf::RectangleShape text_cursor_;
	std::thread* animate_cursor_thread_;
	sf::Text header_text_;
	sf::Text textfield_text_;
	std::string textfield_entry_;
	ButtonTexture* enter_button_;
	std::vector<sf::Texture*> textures_;
	bool edit_mode_; // Represents if the user is actively editing the text field
	Button* held_button_;
	Multiplayer* multiplayer_;
public:
	InputBox();
	InputBox(sf::Vector2f& size, sf::Vector2f& pos, std::string& instructions, unsigned char_size, std::vector<sf::Texture*> textures, const sf::Font& font, Multiplayer* mutliplayer);
	void draw(sf::RenderWindow& win) const;
	void draw(sf::RenderWindow& win, sf::Vector2f& mouse_pos) const;
	void set_text(std::string text);
	// Updates the textfield animations
	void update(); // remove potentially
	// Returns true if the text field is being edited
	bool& edit_mode();
	void set_edit_mode(bool b);
	void process_keyboard(char key);
	// Checks if the textbox contains the mouse_pos
	bool contains(const sf::Vector2f& mouse_pos);
	void animate_text_cursor();
	void delete_end_textfield();
	unsigned get_textfield_size();
	void release_button();
	void process_click(sf::Vector2f& mouse_pos);
	bool holding_button();
	Multiplayer* get_multiplayer();
	std::string& get_textfield_entry();
};

#endif


// need textfield rectangle, header/top text, enter button to right of textfield, 